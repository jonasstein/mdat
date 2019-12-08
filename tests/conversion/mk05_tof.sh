#!/bin/bash
#
# Last modified: 2019-12-08

BASEFOLDER=~/my/prj/cevelop/qmr/tests/conversion
INFOLDER=${BASEFOLDER}/03_cutswitch
OUTFOLDER=${BASEFOLDER}/05_tof

. ./fileselection.sh

for MyF in ${ACTIVEFILES}; do

for filename in ${INFOLDER}/${MyF}*.txt; do 
    BASENAME=`basename ${filename} .txt`
    echo Convert ${BASENAME}
    mdataddtof.pl 0 1 3 2 ${filename} 1477000 | sort --numeric-sort --parallel=9  > ${OUTFOLDER}/${BASENAME}.txt
done
done
