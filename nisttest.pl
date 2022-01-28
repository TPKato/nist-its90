#!/usr/bin/perl

use strict;

my $tolerance = 0.04;

my %data;
my $type;
my $sign = -1;

# read data from NIST database
while (<>) {
    s/[\r\n]+$//;
    s/^\s+//; s/\s+$//;

    if (/ITS-90 Table for type ([A-Z]+) thermocouple/) {
	if ($type ne $1) {
	    # new table starts
	    # dumpdata($type, \%data) if (%data);	# %data should be undef here
	    undef %data;
	    $type = $1;
	}
    } elsif (/^\*+/) {
	# start of coefficients, i.e. end of table
	compare_data($type, \%data) if (%data);
	undef %data;
	$type = '';
    } elsif (/C\s+0\s+(-?)1\s+/) {
	$sign = ($1 eq "-") ? -1 : 1;
    } elsif (/^-?\d+/) {
	next unless $type;

	my ($base, @data) = split(/\s+/);
	for (my $i = 0; $i < $#data + 1; $i++) {
	    my $temp = $base + $sign * $i;
	    my $emf =  $data[$i];

	    if ($data{$temp}) {
		if ($data{$temp} != $emf) {
		    die "something wrong: $data{$temp} != $emf at T = $temp\n";
		}
	    } else {
		$data{$temp} = $emf;
	    }
	}
    }
}

# read data from text file (which is made by nist-its90-*.c)
sub compare_data {
    my ($type, $aref) = @_;
    my $error = 0;

    print STDERR "  checking data for type $type...";

    open(FILE, "<test/test-$type-calc.dat") || die "test/test-$type-calc.dat: $!";
    while (<FILE>) {
	chomp;
	my ($T, $emf) = split(/\s+/);
	if (! ${$aref}{$T}) {
	    print STDERR "Type $type: $T: no data\n";
	    $error = 1;
	} elsif (abs(${$aref}{$T} - $emf) > $tolerance) {
	    print STDERR "Type $type: $T: U(T) = ${$aref}{$T}, E = $emf: diff > $tolerance\n";
	    $error = 1;
	} else {
	    # print STDERR "$type: $T: " . ${$aref}{$T} . ", $emf: OK\n";
	}
    }
    close(FILE);

    print STDERR "OK\n" unless ($error);
    return $error;
}
