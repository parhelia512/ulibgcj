#!/usr/bin/perl -w

# a preprocessor for Java code, similar to gnu.kawa.util.PreProcess

# The following directives are supported:

#   #if <expression> - enables the following code iff <expression> is true
#   #else            - enables or disables the following code per the reverse
#                      logic of the corresponding #if
#   #endif           - indicates the end of the area affected by the
#                      corresponding #if or #else
#   #eoc             - flag to replace '*/' when disabling code containing
#                      comments

# "if" expressions have the following grammar:

#     expression ::= symbol | negation | combination
#         symbol ::= '[a-zA-Z_]+'
#       negation ::= 'not' expression
#    combination ::= and | or
#            and ::= expression 'and' expression
#             or ::= expression 'or' expression

# Symbols may be defined via -D<symbol> arguments to this script.

use strict;
use subs 'write';

use constant {
  SymbolType => 0,
  NegationType => 1,
  AndType => 2,
  OrType => 3,
};

sub parseExpression;

sub parseSymbol {
    my $text = shift;

    if ($text =~ /^\s*(\w+)\s*$/) {
        if ($1 ne "not" && $1 ne "or" && $1 ne "and") {
            return { type => SymbolType, value => $1 };
        }
    }

    return 0;
}

sub parseNegation {
    my $text = shift;

    if ($text =~ /^\s*not\s*(.+)\s*$/) {
        my $exp = $1;
        my $result = parseExpression $exp;
        if ($result) {
            return { type => NegationType, value => $result };
        }
    }
    
    return 0;
}

sub parseAnd {
    my $text = shift;

    if ($text =~ /^\s*(.+)\s+and\s+(.+)\s*$/) {
        my $a_exp = $1;
        my $b_exp = $2;
        my $a = parseExpression $a_exp;
        if ($a) {
            my $b = parseExpression $b_exp;
            if ($b) {
                return { type => AndType, first => $a, second => $b };
            }
        }
    }
    
    return 0;
}

sub parseOr {
    my $text = shift;

    if ($text =~ /^\s*(.+)\s+or\s+(.+)\s*$/) {
        my $a_exp = $1;
        my $b_exp = $2;
        my $a = parseExpression $a_exp;
        if ($a) {
            my $b = parseExpression $b_exp;
            if ($b) {
                return { type => OrType, first => $a, second => $b };
            }
        }
    }
    
    return 0;
}

sub parseCombination {
    my $text = shift;

    my $result = parseAnd $text;
    return $result if $result;

    $result = parseOr $text;
    return $result if $result;
}

sub parseExpression {
    my $text = shift;

    my $result = parseCombination $text;
    return $result if $result;

    $result = parseNegation $text;
    return $result if $result;

    $result = parseSymbol $text;
    return $result if $result;
}

sub replaceEOC {
    my $text = shift;
    $text =~ s:\*/:#eoc:g;
    return $text;
}

sub restoreEOC {
    my $text = shift;
    $text =~ s:#eoc:*/:g;
    return $text;
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

    my $evaluate;
    $evaluate = sub {
        my $exp = shift;
        
        if ($exp->{type} == SymbolType) {
            return defined($definitions{$exp->{value}});
        } elsif ($exp->{type} == NegationType) {
            return (! &$evaluate($exp->{value}));
        } elsif ($exp->{type} == AndType) {
            return &$evaluate($exp->{first}) && &$evaluate($exp->{second});
        } elsif ($exp->{type} == OrType) {
            return &$evaluate($exp->{first}) || &$evaluate($exp->{second});
        }
    };

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

        if ($text =~ /^\s*#if\s*(.+)\s*$/) {
            my $expression = $1;
            my $exp = parseExpression $expression;
            die "invalid expression: $expression" unless $exp;

            $positive = $stack[$#stack] && &$evaluate($exp);
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
