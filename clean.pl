#!/usr/bin/env perl
use strict;
use warnings;

sub remove_files {
    my ($pattern) = @_;
    for my $file (glob $pattern) {
        unlink $file if -e $file;
    }
}

remove_files("*.exe");
remove_files("*.out");
remove_files("*.pdb");
remove_files("*.tds");
remove_files("*.obj");
