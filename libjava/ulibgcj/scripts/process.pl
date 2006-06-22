#!/usr/bin/perl -w

# a preprocessor for Java code, similar to gnu.kawa.util.PreProcess

# The following directives are supported:

#   #ifdef <symbol>  - enables the following code iff <symbol> is defined
#   #ifndef <symbol> - enables the following code iff <symbol> is not defined
#   #else            - enables or disables the following code per the reverse
#                      logic of the corresponding #ifdef or #ifndef
#   #endif           - indicates the end of the area affected by the
#                      corresponding #ifdef, #ifndef, or #else
#   #eoc             - flag to replace '*/' when disabling code containing
#                      comments

# Symbols may be defined via -D<symbol> arguments to this script.

use strict;
use subs 'write';

sub replaceEOC {
    my $text = shift;
    if ($text =~ /^(.*)\*\/(.*)$/) {
        return "$1#eoc$2";
    } else {
        return $text;
    }
}

sub restoreEOC {
    my $text = shift;
    if ($text =~ /^(.*)#eoc(.*)$/) {
        return "$1*/$2";
    } else {
        return $text;
    }
}

sub write {
    my $read = shift;
    
    my $inComment = 0;
    my $positive = 1;
    while (1) {
        (my $line, my $next) = &$read;
        return if ($line == 0);

        if ($line->{isDirective}) {
            $positive = $line->{positive};
        }

        if ($inComment) {
            if ($line->{isDirective}) {
                if ($next && ($next->{isDirective} || !$positive)) {
                    print "$line->{text}\n";
                } else {
                    print "$line->{text}*/\n";
                    $inComment = 0;
                }
            } else {
                my $replaced = replaceEOC $line->{text};
                print "$replaced\n";
            }
        } else {
            if ($line->{isDirective} || !$positive) {
                if ($next && ($next->{isDirective} || !$positive)) {
                    print "/*$line->{text}\n";
                    $inComment = 1;
                } else {
                    print "//$line->{text}\n";
                }
            } else {
                my $restored = restoreEOC $line->{text};
                print "$restored\n";
            }                
        }
    }
}

sub usage {
    die "usage: $0 <-Ddefinition ...>";
}

sub main {
    my %definitions;
    for my $arg (@ARGV) {
        if ($arg =~ /-D(\w+)/) {
            $definitions{$1} = 1;
        } else {
            usage;
        }
    }

    my @stack;
    $stack[++$#stack] = 1;

    my $handle = sub {
        my $text = shift;
        my $line = shift;
        my $positive;

        my $make = sub {
            return { text => $text,
                     isDirective => 1,
                     positive => $positive };
        };

        if ($text =~ /^\s*#ifdef\s*(\w+)\s*$/) {
            $positive = $stack[$#stack] && defined($definitions{$1});
            $stack[++$#stack] = $positive;
            return &$make;
        } elsif ($text =~ /^\s*#ifndef\s*(\w+)\s*$/) {
            $positive = $stack[$#stack] && (! defined($definitions{$1}));
            $stack[++$#stack] = $positive;
            return &$make;
        } elsif ($text =~ /^\s*#else\s*$/) {
            $positive = (! $stack[$#stack]) && $stack[$#stack - 1];
            return &$make;
        } elsif ($text =~ /^\s*#endif\s*$/) {
            $positive = $stack[--$#stack];
            return &$make;
        } else {
            return { text => $line,
                     isDirective => 0,
                     positive => 0 };
        }
    };

    my $inComment = 0;
    my $doRead = sub {
        if ((my $line = <STDIN>)) {
            chomp $line;
            if ($inComment) {
                my $match;
                if ($line =~ /^\s*(.*)\s*\*\/\s*$/) {
                    $inComment = 0;
                    return &$handle($1, $line);
                } else {
                    return &$handle($line, $line);
                }
            } else {
                if ($line =~ /^\s*\/\*\s*(.+)\s*\*\/\s*$/) {
                    return &$handle($1, $line);
                } elsif ($line =~ /^\s*\/\*\s*(.+)\s*$/) {
                    $inComment = 1;
                    return &$handle($1, $line);
                } elsif ($line =~ /^\s*\/\/\s*(.+)\s*$/) {
                    return &$handle($1, $line);
                } else {
                    return { text => $line,
                             isDirective => 0,
                             positive => 0 };
                }
            }
        } else {
            return 0;
        }
    };

    my $last = &$doRead;
    my $read = sub {
        if ($last) {
            my $old = $last;
            $last = &$doRead;
            return ($old, $last);
        } else {
            return (0, 0);
        }
    };

    write $read;
}

main;
