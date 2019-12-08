#!/bin/bash

BASEFOLDER=~/my/prj/cevelop/qmr/tests/conversion
INFOLDER=${BASEFOLDER}/02_cutsemaphore
OUTFOLDER=${BASEFOLDER}/03_cutswitch

. ./fileselection.sh

for MyF in ${ACTIVEFILES}; do

rm -f ${OUTFOLDER}/${MyF}_cutswitch.txt # delete old

  for filename in $(ls ${INFOLDER}/${MyF}/*.txt); do
	BASENAME=`basename $filename .txt`
        # now join all files to one
	echo "delete data before first sync and after last sync."
	mdatslice -f $filename --trigid 7 --dataid 1 --keep 2 --start 1 --autoend >> ${OUTFOLDER}/${MyF}_cutswitch.txt
  done
done

