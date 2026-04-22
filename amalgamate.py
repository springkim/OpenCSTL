#!/usr/bin/env python3
"""
amalgamate.py — OpenCSTL single-header amalgamator
Usage: python amalgamate.py [--src-dir ./opencstl] [--out ./opencstl.h] [-c|--compact]

Rules:
  - #pragma once and include guards (#ifndef / #if !defined + #define … #endif)
    are stripped from each individual file and replaced with a single outer guard.
  - #include <s> at depth-0 (unconditional) are deduplicated and hoisted to
    the top of the output.
  - #include <s> inside conditional blocks (#if / #ifdef / …) are left in
    place to preserve platform guards (e.g. Windows.h under #ifdef _WIN32).
  - #include "local" are inlined recursively; already-visited files emit a stub.
  - --compact strips all C/C++ comments and blank lines for minimum size.
"""

import re
import sys
import argparse
from pathlib import Path
import shutil

# ──────────────────────────────────────────────────────────────────────────────
# Regex helpers
# ──────────────────────────────────────────────────────────────────────────────

RE_LOCAL_INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"')
RE_SYSTEM_INCLUDE = re.compile(r'^\s*#\s*include\s*<([^>]+)>')
RE_PRAGMA_ONCE = re.compile(r'^\s*#\s*pragma\s+once\s*$')

RE_GUARD_IFNDEF = re.compile(r'^\s*#\s*ifndef\s+(\w+)\s*$')
RE_GUARD_IF_DEF = re.compile(r'^\s*#\s*if\s+!defined\s*\(\s*(\w+)\s*\)\s*$')
RE_GUARD_DEFINE = re.compile(r'^\s*#\s*define\s+(\w+)\s*$')

RE_IF_OPEN = re.compile(r'^\s*#\s*(if|ifdef|ifndef)\b')
RE_IF_CLOSE = re.compile(r'^\s*#\s*endif\b')


# ──────────────────────────────────────────────────────────────────────────────
# Include-guard stripping
# ──────────────────────────────────────────────────────────────────────────────

def strip_include_guard(lines):
    """Remove outermost include guard and #pragma once lines."""
    lines = [l for l in lines if not RE_PRAGMA_ONCE.match(l)]
    n = len(lines)

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

    next_idx = idx + 1
    while next_idx < n and lines[next_idx].strip() == '':
        next_idx += 1
    if next_idx >= n:
        return lines

    m2 = RE_GUARD_DEFINE.match(lines[next_idx])
    if not m2 or m2.group(1) != candidate_guard:
        return lines

    guard_define_idx = next_idx

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
# C/C++ comment stripping (for --compact mode)
# ──────────────────────────────────────────────────────────────────────────────

def strip_c_comments(source):
    """Strip // and /* */ comments, preserving string and character literals."""
    out = []
    i = 0
    n = len(source)
    while i < n:
        c = source[i]
        nxt = source[i + 1] if i + 1 < n else ''

        # // line comment — skip to end of line (keep the newline)
        if c == '/' and nxt == '/':
            while i < n and source[i] != '\n':
                i += 1
            continue

        # /* block comment */
        if c == '/' and nxt == '*':
            i += 2
            while i < n - 1 and not (source[i] == '*' and source[i + 1] == '/'):
                i += 1
            i += 2
            continue

        # "string literal"
        if c == '"':
            out.append(c)
            i += 1
            while i < n and source[i] != '"':
                if source[i] == '\\' and i + 1 < n:
                    out.append(source[i])
                    out.append(source[i + 1])
                    i += 2
                else:
                    out.append(source[i])
                    i += 1
            if i < n:
                out.append(source[i])
                i += 1
            continue

        # 'character literal'
        if c == "'":
            out.append(c)
            i += 1
            while i < n and source[i] != "'":
                if source[i] == '\\' and i + 1 < n:
                    out.append(source[i])
                    out.append(source[i + 1])
                    i += 2
                else:
                    out.append(source[i])
                    i += 1
            if i < n:
                out.append(source[i])
                i += 1
            continue

        out.append(c)
        i += 1

    return ''.join(out)


def compact_source(source):
    """Strip comments, blank lines, and trailing whitespace."""
    stripped = strip_c_comments(source)
    kept = []
    for line in stripped.splitlines():
        s = line.rstrip()
        if s.strip() == '':
            continue
        kept.append(s)
    return '\n'.join(kept) + '\n'


# ──────────────────────────────────────────────────────────────────────────────
# Core recursive processor
# ──────────────────────────────────────────────────────────────────────────────

class Amalgamator:
    def __init__(self, src_dir):
        self.src_dir = src_dir
        self.visited = set()  # basenames already inlined
        self.top_sys_includes = []  # unconditional system includes (hoisted)
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
            return [f'// [already included: {filename}]\n']
        self.visited.add(canonical)

        if not path.exists():
            return [f'// [WARNING: file not found: {filename}]\n']

        raw = path.read_text(encoding='utf-8').splitlines(keepends=True)
        stripped = strip_include_guard(raw)

        sep = '=' * 78
        out = []
        out.append(f'\n// {sep}\n')
        out.append(f'// BEGIN  {filename:<30} (depth {depth})\n')
        out.append(f'// {sep}\n\n')

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
                    out.append('// [external, kept as-is]\n')
                    out.append(line)
                continue

            out.append(line)

        out.append(f'\n// {sep}\n')
        out.append(f'// END    {filename}\n')
        out.append(f'// {sep}\n')

        return out


# ──────────────────────────────────────────────────────────────────────────────
# Output assembly
# ──────────────────────────────────────────────────────────────────────────────

BANNER = """\
//
// ============================================================================
//  OpenCSTL — Single-Header Amalgamation
//  Auto-generated by amalgamate.py  (do NOT edit manually)
//
//  Copyright (C) 2018-2026, Kim Bomm, all rights reserved.
//  Licensed under the OpenCSTL License Agreement.
// ============================================================================
//
"""

BANNER_COMPACT = """\
// OpenCSTL — Single-Header Amalgamation (compact)
// Copyright (C) 2018-2026, Kim Bomm. Licensed under the OpenCSTL License Agreement.
"""

GUARD = '_OPENCSTL_AMALGAMATED_H'


def write_output(out_path, amalg, body_lines, compact=False):
    # Assemble body into a single string, then optionally compact.
    body_src = ''.join(body_lines)
    if compact:
        body_src = compact_source(body_src)

    with out_path.open('w', encoding='utf-8', newline='\n') as f:
        f.write(BANNER_COMPACT if compact else BANNER)
        f.write('\n')
        f.write('#pragma once\n')
        f.write(f'#ifndef {GUARD}\n')
        f.write(f'#define {GUARD}\n')
        if not compact:
            f.write('\n')

        if amalg.top_sys_includes:
            if not compact:
                f.write('// ── System includes — unconditional, deduplicated ─────────────────────\n')
            for inc in amalg.top_sys_includes:
                f.write(inc + '\n')
            if not compact:
                f.write('\n')

        f.write(body_src)
        if not body_src.endswith('\n'):
            f.write('\n')

        if not compact:
            f.write('\n')
        f.write(f'#endif // {GUARD}\n')


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
    parser.add_argument('-c', '--compact', action='store_true',
                        help='Strip all comments and blank lines for minimum size')
    args = parser.parse_args()

    src_dir = Path(args.src_dir).resolve()
    out_path = Path(args.out).resolve()
    compact = args.compact

    if not src_dir.is_dir():
        print(f'ERROR: source directory not found: {src_dir}', file=sys.stderr)
        sys.exit(1)
    if not (src_dir / args.entry).exists():
        print(f'ERROR: entry file not found: {src_dir / args.entry}', file=sys.stderr)
        sys.exit(1)

    print(f'[amalgamate] src-dir : {src_dir}')
    print(f'[amalgamate] entry   : {args.entry}')
    print(f'[amalgamate] output  : {out_path}')
    print(f'[amalgamate] compact : {compact}')

    amalg = Amalgamator(src_dir)
    body = amalg.process_file(args.entry)
    write_output(out_path, amalg, body, compact=compact)

    # Mirror copies — skip silently if the target directory doesn't exist.
    for target in ('examples/opencstl.h',
                   'assets/opencstl.h',
                   'bench/bench_cstl/3rdparty/include/opencstl.h'):
        tgt = Path(target)
        if tgt.parent.exists():
            shutil.copy(out_path, tgt)
        else:
            print(f'[amalgamate] skip copy: {tgt.parent} does not exist',
                  file=sys.stderr)

    total = out_path.read_text(encoding='utf-8').count('\n')
    size_kb = out_path.stat().st_size / 1024
    print(f'[amalgamate] done    : {len(amalg.visited)} file(s) inlined, '
          f'{total} lines, {size_kb:.1f} KB → {out_path.name}')


if __name__ == '__main__':
    main()
