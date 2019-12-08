#!/bin/bash
#
# Last modified: 2019-12-08

BASEFOLDER=~/my/prj/cevelop/qmr/tests/conversion
INFOLDER=${BASEFOLDER}/03_cutswitch
OUTFOLDER=${BASEFOLDER}/04_nospikes


function nospikes {
	# mdatspikefilter <ChDet> <ChSync> <ChSemaphore> <ChMonitor> <Period ns> <startSpike> <endSpike> <filename> <outfile>
        mdatspikefilter.pl 0 1 2 3 ${2} 47000 55000 ${INFOLDER}/${1} ${OUTFOLDER}/${1} 
}


nospikes M789.txt 10000000
