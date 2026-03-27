#!/usr/bin/env python3
"""
amalgamate.py — OpenCSTL single-header amalgamator
Usage: python amalgamate.py [--src-dir ./opencstl] [--out ./opencstl_amalgamated.h]

Rules:
  - #pragma once and include guards (#ifndef / #if !defined + #define … #endif)
    are stripped from each individual file and replaced with a single outer guard.
  - #include <s> at depth-0 (unconditional) are deduplicated and hoisted to
    the top of the output.
  - #include <s> inside conditional blocks (#if / #ifdef / …) are left in
    place to preserve platform guards (e.g. Windows.h under #ifdef _WIN32).
  - #include "local" are inlined recursively; already-visited files emit a comment.
"""

import re
import sys
import argparse
from pathlib import Path

# ──────────────────────────────────────────────────────────────────────────────
# Regex helpers
# ──────────────────────────────────────────────────────────────────────────────

RE_LOCAL_INCLUDE  = re.compile(r'^\s*#\s*include\s*"([^"]+)"')
RE_SYSTEM_INCLUDE = re.compile(r'^\s*#\s*include\s*<([^>]+)>')
RE_PRAGMA_ONCE    = re.compile(r'^\s*#\s*pragma\s+once\s*$')

RE_GUARD_IFNDEF   = re.compile(r'^\s*#\s*ifndef\s+(\w+)\s*$')
RE_GUARD_IF_DEF   = re.compile(r'^\s*#\s*if\s+!defined\s*\(\s*(\w+)\s*\)\s*$')
RE_GUARD_DEFINE   = re.compile(r'^\s*#\s*define\s+(\w+)\s*$')

RE_IF_OPEN        = re.compile(r'^\s*#\s*(if|ifdef|ifndef)\b')
RE_IF_CLOSE       = re.compile(r'^\s*#\s*endif\b')


# ──────────────────────────────────────────────────────────────────────────────
# Include-guard stripping
# ──────────────────────────────────────────────────────────────────────────────

def strip_include_guard(lines):
    """Remove outermost include guard and #pragma once lines."""
    # Pass 1: remove #pragma once
    lines = [l for l in lines if not RE_PRAGMA_ONCE.match(l)]
    n = len(lines)

    # Find first non-blank line
    idx = 0
    while idx < n and lines[idx].strip() == '':
        idx += 1
    if idx >= n:
        return lines

    m = RE_GUARD_IFNDEF.match(lines[idx]) or RE_GUARD_IF_DEF.match(lines[idx])
    if not m:
        return lines

    candidate_guard = m.group(1)
    guard_open_idx = idx

    # Find matching #define GUARD (skip blanks)
    next_idx = idx + 1
    while next_idx < n and lines[next_idx].strip() == '':
        next_idx += 1
    if next_idx >= n:
        return lines

    m2 = RE_GUARD_DEFINE.match(lines[next_idx])
    if not m2 or m2.group(1) != candidate_guard:
        return lines

    guard_define_idx = next_idx

    # Find matching closing #endif (scan backwards, track nesting)
    depth = 0
    closing_idx = None
    for j in range(n - 1, guard_open_idx, -1):
        if RE_IF_CLOSE.match(lines[j]):
            if depth == 0:
                closing_idx = j
                break
            depth -= 1
        elif RE_IF_OPEN.match(lines[j]):
            depth += 1

    if closing_idx is None:
        return lines

    skip = {guard_open_idx, guard_define_idx, closing_idx}
    return [line for j, line in enumerate(lines) if j not in skip]


# ──────────────────────────────────────────────────────────────────────────────
# Core recursive processor
# ──────────────────────────────────────────────────────────────────────────────

class Amalgamator:
    def __init__(self, src_dir):
        self.src_dir = src_dir
        self.visited = set()          # basenames already inlined
        self.top_sys_includes = []    # unconditional system includes (hoisted)
        self._top_sys_seen = set()

    def _hoist_system_include(self, line):
        m = RE_SYSTEM_INCLUDE.match(line)
        if m:
            key = m.group(1)
            if key not in self._top_sys_seen:
                self._top_sys_seen.add(key)
                self.top_sys_includes.append(line.rstrip())

    def process_file(self, filename, depth=0):
        """Recursively inline filename, return list of output lines."""
        path = self.src_dir / filename
        canonical = path.name

        if canonical in self.visited:
            return [f'/* [already included: {filename}] */\n']
        self.visited.add(canonical)

        if not path.exists():
            return [f'/* [WARNING: file not found: {filename}] */\n']

        raw = path.read_text(encoding='utf-8').splitlines(keepends=True)
        stripped = strip_include_guard(raw)

        sep = '/' * 78
        out = []
        out.append(f'\n/* {sep} */\n')
        out.append(f'/* BEGIN  {filename:<30} (depth {depth}) */\n')
        out.append(f'/* {sep} */\n\n')

        cond_depth = 0  # nesting depth of conditional blocks

        for line in stripped:
            # track conditional nesting BEFORE processing the line
            if RE_IF_OPEN.match(line):
                cond_depth += 1
            elif RE_IF_CLOSE.match(line):
                cond_depth -= 1

            m_sys = RE_SYSTEM_INCLUDE.match(line)
            if m_sys:
                if cond_depth == 0:
                    # unconditional → hoist to top, don't emit in place
                    self._hoist_system_include(line)
                else:
                    # inside #ifdef / #if → keep in place (e.g. Windows.h)
                    out.append(line)
                continue

            m_local = RE_LOCAL_INCLUDE.match(line)
            if m_local:
                dep = m_local.group(1)
                if (self.src_dir / dep).exists():
                    out.extend(self.process_file(dep, depth + 1))
                else:
                    out.append(f'/* [external, kept as-is] */ {line}')
                continue

            out.append(line)

        out.append(f'\n/* {sep} */\n')
        out.append(f'/* END    {filename} */\n')
        out.append(f'/* {sep} */\n')

        return out


# ──────────────────────────────────────────────────────────────────────────────
# Output assembly
# ──────────────────────────────────────────────────────────────────────────────

BANNER = """\
/*
 * ============================================================================
 *  OpenCSTL — Single-Header Amalgamation
 *  Auto-generated by amalgamate.py  (do NOT edit manually)
 *
 *  Copyright (C) 2018-2026, Kim Bomm, all rights reserved.
 *  Licensed under the OpenCSTL License Agreement.
 * ============================================================================
 */
"""

GUARD = '_OPENCSTL_AMALGAMATED_H'


def write_output(out_path, amalg, body_lines):
    with out_path.open('w', encoding='utf-8', newline='\n') as f:
        f.write(BANNER)
        f.write('\n')
        f.write(f'#pragma once\n')
        f.write(f'#ifndef {GUARD}\n')
        f.write(f'#define {GUARD}\n')
        f.write('\n')

        if amalg.top_sys_includes:
            f.write('/* ── System includes — unconditional, deduplicated ───────────────────── */\n')
            for inc in amalg.top_sys_includes:
                f.write(inc + '\n')
            f.write('\n')

        for line in body_lines:
            f.write(line)

        f.write('\n')
        f.write(f'#endif /* {GUARD} */\n')


# ──────────────────────────────────────────────────────────────────────────────
# Entry point
# ──────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description='Amalgamate OpenCSTL headers into a single file.')
    parser.add_argument('--src-dir', default='./opencstl',
                        help='Directory containing the headers (default: ./opencstl)')
    parser.add_argument('--out', default='./opencstl.h',
                        help='Output path (default: ./opencstl.h)')
    parser.add_argument('--entry', default='opencstl.h',
                        help='Entry-point header (default: opencstl.h)')
    args = parser.parse_args()

    src_dir  = Path(args.src_dir).resolve()
    out_path = Path(args.out).resolve()

    if not src_dir.is_dir():
        print(f'ERROR: source directory not found: {src_dir}', file=sys.stderr)
        sys.exit(1)
    if not (src_dir / args.entry).exists():
        print(f'ERROR: entry file not found: {src_dir / args.entry}', file=sys.stderr)
        sys.exit(1)

    print(f'[amalgamate] src-dir : {src_dir}')
    print(f'[amalgamate] entry   : {args.entry}')
    print(f'[amalgamate] output  : {out_path}')

    amalg = Amalgamator(src_dir)
    body  = amalg.process_file(args.entry)
    write_output(out_path, amalg, body)

    total = out_path.read_text(encoding='utf-8').count('\n')
    print(f'[amalgamate] done    : {len(amalg.visited)} file(s) inlined, '
          f'{total} lines → {out_path.name}')


if __name__ == '__main__':
    main()