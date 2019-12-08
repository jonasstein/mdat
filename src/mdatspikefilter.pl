#!/usr/bin/perl

use strict;
use warnings;

my $lasttime_ns=0;
my $duration_s = 0;
my $firsttime_ns = 0;
my $SumOfAll=0;
my $countSyncs=0;
my $grepline="";

my $numArgs = @ARGV;

if ($numArgs!=9)
{
  printf("Expected 9 arguments, got %d\n Usage: \n 
	  mdatspikefilter <ChDet> <ChSync> <ChSemaphore> <ChMonitor> <Period ns> <startSpike> <endSpike> <filename> <outfile>\n",$numArgs);
  exit(1);
}

my ($ChDet, $ChSync, $ChSemaphore, $ChMonitor, $Period_ns, $ArgTstartfilter_ns, $ArgTendfilter_ns, $ArgFileIn, $ArgFileOut) = @ARGV;

my $Tstartfilter_ns = $ArgTstartfilter_ns; # was 47000;
my $Tendfilter_ns = $ArgTendfilter_ns; # was 55000;


open my $fh_in, '<', $ArgFileIn or die $!;

open my $fh_filtered, '>', $ArgFileOut ."_filtered.txt" or die $!;
open my $fh_spikesA,  '>', $ArgFileOut ."_spikesA.txt"  or die $!;
open my $fh_spikesB,  '>', $ArgFileOut ."_spikesB.txt"  or die $!;
open my $fh_log,      '>', $ArgFileOut .".log" or die $!;


my $lastSyncTime_ns=0;
my $relativeTime_ns=0;

while (<$fh_in>) {

  my ($time_ns, $type, $ID, $counter) = split;

  my $IsSpikeA=0;
  my $IsSpikeB=0;
  my $IsSync=0;
  my $IsSemaphore=0;

  $IsSync= ($ID==$ChSync);
  $IsSemaphore=($ID==$ChSemaphore);

  if ($IsSync){ $lastSyncTime_ns = $time_ns; }
  $relativeTime_ns = $time_ns - $lastSyncTime_ns;

  $IsSpikeA = (($relativeTime_ns > $Tstartfilter_ns) & ($relativeTime_ns < $Tendfilter_ns));
  $IsSpikeB = (($relativeTime_ns > ($Tstartfilter_ns + 0.5 * $Period_ns)) & ($relativeTime_ns < ($Tendfilter_ns + 0.5 * $Period_ns)));

  if ($IsSpikeA) { print $fh_spikesA "$relativeTime_ns $type $ID $counter\n"; } 
  if ($IsSpikeB) { print $fh_spikesB "$relativeTime_ns $type $ID $counter\n"; }

  if ($IsSync | $IsSemaphore | (not($IsSpikeA) & (not($IsSpikeB)))) { print $fh_filtered "$time_ns $type $ID $counter\n"; }
  }

  # write logfile
print $fh_log "Date: " . localtime . "\n";
print $fh_log "Arguments: @ARGV";


close $fh_in;
close $fh_filtered; 
close $fh_spikesA; 
close $fh_spikesB;  
close $fh_log;
