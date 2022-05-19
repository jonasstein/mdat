#!/usr/bin/perl

# ChDet - TOF can result in negative times, requires signed integers and needs more calculations
# better: add TOF to ChSync and ChSemaphore.
# Ignore ChMonitor

use strict;
use warnings;

my $numArgs = @ARGV;

if ($numArgs!=6)
{
  printf("Expected 6 arguments, got %d\n Usage: \n 
	  mdataddtof <ChDet> <ChSync> <ChSemaphore> <ChMonitor> <filename> <TOF sample..det (ns)>\n",$numArgs);
  printf("Sort events afterwards.");
  exit(1);
}

my ($ChDet, $ChSync, $ChSemaphore, $ChMonitor, $ArgFileIn, $tof_ns) = @ARGV;

open my $fh_in, '<', $ArgFileIn or die $!;

while (<$fh_in>) {
  my ($time_ns, $type, $ID, $counter) = split;
  if (($ID==$ChSync) | ($ID==$ChSemaphore)) { $time_ns += $tof_ns; }
  printf("%d %d %d %d\n",$time_ns, $type, $ID, $counter);
  }

close $fh_in;
