#!/usr/bin/env perl
# amalgamate.pl — OpenCSTL single-header amalgamator
# Usage: perl amalgamate.pl [--src-dir ./opencstl] [--out ./opencstl.h] [-c|--compact]
#
# Rules:
#   - #pragma once and include guards (#ifndef / #if !defined + #define ... #endif)
#     are stripped from each individual file and replaced with a single outer guard.
#   - #include <s> at depth-0 (unconditional) are deduplicated and hoisted to
#     the top of the output.
#   - #include <s> inside conditional blocks (#if / #ifdef / ...) are left in
#     place to preserve platform guards (e.g. Windows.h under #ifdef _WIN32).
#   - #include "local" are inlined recursively; already-visited files emit a stub.
#   - --compact strips all C/C++ comments and blank lines for minimum size.

use strict;
use warnings;
use utf8;
use open ':std', ':encoding(UTF-8)';
use Getopt::Long qw(GetOptions);
use File::Spec;
use File::Basename qw(basename dirname);
use File::Copy qw(copy);
use Cwd qw(abs_path);

# ──────────────────────────────────────────────────────────────────────────────
# Regex helpers
# ──────────────────────────────────────────────────────────────────────────────

my $RE_LOCAL_INCLUDE  = qr/^\s*#\s*include\s*"([^"]+)"/;
my $RE_SYSTEM_INCLUDE = qr/^\s*#\s*include\s*<([^>]+)>/;
my $RE_PRAGMA_ONCE    = qr/^\s*#\s*pragma\s+once\s*$/;

my $RE_GUARD_IFNDEF = qr/^\s*#\s*ifndef\s+(\w+)\s*$/;
my $RE_GUARD_IF_DEF = qr/^\s*#\s*if\s+!defined\s*\(\s*(\w+)\s*\)\s*$/;
my $RE_GUARD_DEFINE = qr/^\s*#\s*define\s+(\w+)\s*$/;

my $RE_IF_OPEN  = qr/^\s*#\s*(?:if|ifdef|ifndef)\b/;
my $RE_IF_CLOSE = qr/^\s*#\s*endif\b/;


# ──────────────────────────────────────────────────────────────────────────────
# Include-guard stripping
# ──────────────────────────────────────────────────────────────────────────────

sub strip_include_guard {
    my ($lines_ref) = @_;
    my @lines = grep { $_ !~ $RE_PRAGMA_ONCE } @$lines_ref;
    my $n = scalar @lines;

    my $idx = 0;
    $idx++ while $idx < $n && _strip($lines[$idx]) eq '';
    return \@lines if $idx >= $n;

    my $candidate_guard;
    if ($lines[$idx] =~ $RE_GUARD_IFNDEF) {
        $candidate_guard = $1;
    } elsif ($lines[$idx] =~ $RE_GUARD_IF_DEF) {
        $candidate_guard = $1;
    } else {
        return \@lines;
    }
    my $guard_open_idx = $idx;

    my $next_idx = $idx + 1;
    $next_idx++ while $next_idx < $n && _strip($lines[$next_idx]) eq '';
    return \@lines if $next_idx >= $n;

    return \@lines
        unless $lines[$next_idx] =~ $RE_GUARD_DEFINE && $1 eq $candidate_guard;

    my $guard_define_idx = $next_idx;

    my $depth = 0;
    my $closing_idx;
    for (my $j = $n - 1; $j > $guard_open_idx; $j--) {
        if ($lines[$j] =~ $RE_IF_CLOSE) {
            if ($depth == 0) {
                $closing_idx = $j;
                last;
            }
            $depth--;
        } elsif ($lines[$j] =~ $RE_IF_OPEN) {
            $depth++;
        }
    }

    return \@lines unless defined $closing_idx;

    my %skip = ($guard_open_idx => 1, $guard_define_idx => 1, $closing_idx => 1);
    my @kept;
    for (my $j = 0; $j < $n; $j++) {
        push @kept, $lines[$j] unless $skip{$j};
    }
    return \@kept;
}

# trim helper — Python's str.strip() equivalent
sub _strip {
    my ($s) = @_;
    $s =~ s/\A\s+//;
    $s =~ s/\s+\z//;
    return $s;
}


# ──────────────────────────────────────────────────────────────────────────────
# C/C++ comment stripping (for --compact mode)
# ──────────────────────────────────────────────────────────────────────────────

sub strip_c_comments {
    my ($source) = @_;
    my @out;
    my $i = 0;
    my $n = length $source;

    while ($i < $n) {
        my $c   = substr($source, $i, 1);
        my $nxt = ($i + 1 < $n) ? substr($source, $i + 1, 1) : '';

        # // line comment — skip to end of line (keep the newline)
        if ($c eq '/' && $nxt eq '/') {
            $i++ while $i < $n && substr($source, $i, 1) ne "\n";
            next;
        }

        # /* block comment */
        if ($c eq '/' && $nxt eq '*') {
            $i += 2;
            while ($i < $n - 1
                   && !(substr($source, $i, 1) eq '*'
                        && substr($source, $i + 1, 1) eq '/')) {
                $i++;
            }
            $i += 2;
            next;
        }

        # "string literal"
        if ($c eq '"') {
            push @out, $c;
            $i++;
            while ($i < $n && substr($source, $i, 1) ne '"') {
                if (substr($source, $i, 1) eq '\\' && $i + 1 < $n) {
                    push @out, substr($source, $i, 1);
                    push @out, substr($source, $i + 1, 1);
                    $i += 2;
                } else {
                    push @out, substr($source, $i, 1);
                    $i++;
                }
            }
            if ($i < $n) {
                push @out, substr($source, $i, 1);
                $i++;
            }
            next;
        }

        # 'character literal'
        if ($c eq "'") {
            push @out, $c;
            $i++;
            while ($i < $n && substr($source, $i, 1) ne "'") {
                if (substr($source, $i, 1) eq '\\' && $i + 1 < $n) {
                    push @out, substr($source, $i, 1);
                    push @out, substr($source, $i + 1, 1);
                    $i += 2;
                } else {
                    push @out, substr($source, $i, 1);
                    $i++;
                }
            }
            if ($i < $n) {
                push @out, substr($source, $i, 1);
                $i++;
            }
            next;
        }

        push @out, $c;
        $i++;
    }

    return join('', @out);
}

sub compact_source {
    my ($source) = @_;
    my $stripped = strip_c_comments($source);
    my @kept;
    for my $line (split /\n/, $stripped, -1) {
        (my $s = $line) =~ s/\s+\z//;        # rstrip
        next if _strip($s) eq '';
        push @kept, $s;
    }
    return join("\n", @kept) . "\n";
}


# ──────────────────────────────────────────────────────────────────────────────
# Core recursive processor (Amalgamator)
# ──────────────────────────────────────────────────────────────────────────────

sub new_amalgamator {
    my ($src_dir) = @_;
    return {
        src_dir          => $src_dir,
        visited          => {},   # basenames already inlined
        top_sys_includes => [],   # unconditional system includes (hoisted)
        _top_sys_seen    => {},
    };
}

sub _hoist_system_include {
    my ($amalg, $line) = @_;
    if ($line =~ $RE_SYSTEM_INCLUDE) {
        my $key = $1;
        unless ($amalg->{_top_sys_seen}{$key}) {
            $amalg->{_top_sys_seen}{$key} = 1;
            (my $rstripped = $line) =~ s/\s+\z//;
            push @{ $amalg->{top_sys_includes} }, $rstripped;
        }
    }
}

# Python-style splitlines(keepends=True): keep \n/\r\n on each line
sub _splitlines_keepends {
    my ($text) = @_;
    my @out;
    while ($text =~ /\G(.*?(?:\r\n|\n|\r|\z))/gc) {
        my $chunk = $1;
        last if $chunk eq '';
        push @out, $chunk;
    }
    return @out;
}

sub process_file {
    my ($amalg, $filename, $depth) = @_;
    $depth //= 0;

    my $path      = File::Spec->catfile($amalg->{src_dir}, $filename);
    my $canonical = basename($path);

    if ($amalg->{visited}{$canonical}) {
        return [ "// [already included: $filename]\n" ];
    }
    $amalg->{visited}{$canonical} = 1;

    unless (-e $path) {
        return [ "// [WARNING: file not found: $filename]\n" ];
    }

    open(my $fh, '<:encoding(UTF-8)', $path)
        or die "ERROR: cannot open $path: $!\n";
    my $raw_text = do { local $/; <$fh> };
    close $fh;

    my @raw = _splitlines_keepends($raw_text);
    my $stripped_ref = strip_include_guard(\@raw);

    my $sep = '=' x 78;
    my @out;
    push @out, "\n// $sep\n";
    push @out, sprintf("// BEGIN  %-30s (depth %d)\n", $filename, $depth);
    push @out, "// $sep\n\n";

    my $cond_depth = 0;   # nesting depth of conditional blocks

    for my $line (@$stripped_ref) {
        # track conditional nesting BEFORE processing the line
        if ($line =~ $RE_IF_OPEN) {
            $cond_depth++;
        } elsif ($line =~ $RE_IF_CLOSE) {
            $cond_depth--;
        }

        if ($line =~ $RE_SYSTEM_INCLUDE) {
            if ($cond_depth == 0) {
                # unconditional → hoist to top, don't emit in place
                _hoist_system_include($amalg, $line);
            } else {
                # inside #ifdef / #if → keep in place (e.g. Windows.h)
                push @out, $line;
            }
            next;
        }

        if ($line =~ $RE_LOCAL_INCLUDE) {
            my $dep = $1;
            if (-e File::Spec->catfile($amalg->{src_dir}, $dep)) {
                push @out, @{ process_file($amalg, $dep, $depth + 1) };
            } else {
                push @out, "// [external, kept as-is]\n";
                push @out, $line;
            }
            next;
        }

        push @out, $line;
    }

    push @out, "\n// $sep\n";
    push @out, "// END    $filename\n";
    push @out, "// $sep\n";

    return \@out;
}


# ──────────────────────────────────────────────────────────────────────────────
# Output assembly
# ──────────────────────────────────────────────────────────────────────────────

my $BANNER = <<'EOT';
//
// ============================================================================
//  OpenCSTL — Single-Header Amalgamation
//  Auto-generated by amalgamate.py  (do NOT edit manually)
//
//  Copyright (C) 2018-2026, Kim Bomm, all rights reserved.
//  Licensed under the OpenCSTL License Agreement.
// ============================================================================
//
EOT

my $BANNER_COMPACT = <<'EOT';
// OpenCSTL — Single-Header Amalgamation (compact)
// Copyright (C) 2018-2026, Kim Bomm. Licensed under the OpenCSTL License Agreement.
EOT

my $GUARD = 'OPENCSTL_AMALGAMATED_H';


sub write_output {
    my ($out_path, $amalg, $body_lines_ref, %opts) = @_;
    my $compact       = $opts{compact}       // 0;
    my $encoding_ansi = $opts{encoding_ansi} // 0;

    my $body_src = join('', @$body_lines_ref);
    $body_src = compact_source($body_src) if $compact;

    open(my $fh, '>:raw:encoding(UTF-8)', $out_path)
        or die "ERROR: cannot write $out_path: $!\n";

    print $fh ($compact ? $BANNER_COMPACT : $BANNER);
    print $fh "\n";
    print $fh "#pragma once\n";
    print $fh "#ifndef $GUARD\n";
    print $fh "#define $GUARD\n";
    print $fh "\n" unless $compact;

    if (@{ $amalg->{top_sys_includes} }) {
        print $fh "// ── System includes — unconditional, deduplicated ─────────────────────\n"
            unless $compact;
        for my $inc (@{ $amalg->{top_sys_includes} }) {
            print $fh $inc . "\n";
        }
        print $fh "\n" unless $compact;
    }

    print $fh $body_src;
    print $fh "\n" unless $body_src =~ /\n\z/;

    print $fh "\n" unless $compact;
    print $fh "#endif // $GUARD\n";
    close $fh;

    if ($encoding_ansi) {
        # Re-read as UTF-8 and rewrite as ASCII (errors replaced).
        open(my $rfh, '<:raw:encoding(UTF-8)', $out_path)
            or die "ERROR: cannot reopen $out_path: $!\n";
        my $text = do { local $/; <$rfh> };
        close $rfh;

        open(my $wfh, '>:raw:encoding(ASCII)', $out_path)
            or die "ERROR: cannot rewrite $out_path: $!\n";
        # Replace non-ASCII characters with '?'
        $text =~ s/([^\x00-\x7F])/?/g;
        print $wfh $text;
        close $wfh;
    }
}


# ──────────────────────────────────────────────────────────────────────────────
# Entry point
# ──────────────────────────────────────────────────────────────────────────────

sub main {
    my $src_dir_arg = './opencstl';
    my $out_arg     = './opencstl.h';
    my $entry       = 'opencstl.h';
    my $compact     = 0;
    my $ansi        = 0;
    my $help        = 0;

    GetOptions(
        'src-dir=s' => \$src_dir_arg,
        'out=s'     => \$out_arg,
        'entry=s'   => \$entry,
        'c|compact' => \$compact,
        'a|ansi'    => \$ansi,
        'h|help'    => \$help,
    ) or die "Invalid options\n";

    if ($help) {
        print <<'USAGE';
Amalgamate OpenCSTL headers into a single file.

Usage: perl amalgamate.pl [options]

Options:
  --src-dir DIR    Directory containing the headers (default: ./opencstl)
  --out FILE       Output path (default: ./opencstl.h)
  --entry FILE     Entry-point header (default: opencstl.h)
  -c, --compact    Strip all comments and blank lines for minimum size
  -a, --ansi       Use ANSI encoding
  -h, --help       Show this help message
USAGE
        return 0;
    }

    my $src_dir  = abs_path($src_dir_arg);
    my $out_path = abs_path(dirname($out_arg));
    $out_path = defined $out_path
        ? File::Spec->catfile($out_path, basename($out_arg))
        : $out_arg;

    unless (defined $src_dir && -d $src_dir) {
        print STDERR "ERROR: source directory not found: $src_dir_arg\n";
        exit 1;
    }
    unless (-e File::Spec->catfile($src_dir, $entry)) {
        print STDERR "ERROR: entry file not found: "
            . File::Spec->catfile($src_dir, $entry) . "\n";
        exit 1;
    }

    print "[amalgamate] src-dir : $src_dir\n";
    print "[amalgamate] entry   : $entry\n";
    print "[amalgamate] output  : $out_path\n";
    print "[amalgamate] compact : " . ($compact ? 'True' : 'False') . "\n";

    my $amalg = new_amalgamator($src_dir);
    my $body  = process_file($amalg, $entry);
    write_output($out_path, $amalg, $body,
                 compact => $compact, encoding_ansi => $ansi);

    # Mirror copies — skip silently if the target directory doesn't exist.
    for my $target ('examples/opencstl.h',
                    'assets/opencstl.h',
                    'bench/bench_cstl/3rdparty/include/opencstl.h',
                    'usage/opencstl.h') {
        my $tgt_dir = dirname($target);
        if (-d $tgt_dir) {
            copy($out_path, $target)
                or print STDERR "[amalgamate] copy failed: $target: $!\n";
        } else {
            print STDERR "[amalgamate] skip copy: $tgt_dir does not exist\n";
        }
    }

    open(my $rfh, '<:raw:encoding(UTF-8)', $out_path)
        or die "ERROR: cannot reopen $out_path: $!\n";
    my $final_text = do { local $/; <$rfh> };
    close $rfh;

    my $total   = () = $final_text =~ /\n/g;
    my $size_kb = (-s $out_path) / 1024;
    my $count   = scalar keys %{ $amalg->{visited} };
    printf "[amalgamate] done    : %d file(s) inlined, %d lines, %.1f KB → %s\n",
        $count, $total, $size_kb, basename($out_path);

    return 0;
}

exit main();
