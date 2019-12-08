#!/bin/bash

BASEFOLDER="~/my/prj/cevelop/qmr/tests/conversion"
INFOLDER=${BASEFOLDER}/01_text
OUTFOLDER=${BASEFOLDER}/02_cutsemaphore

for filename in ${INFOLDER}/*.txt; do
	BASENAME=`basename $filename .txt`
	mkdir ${OUTFOLDER}/${BASENAME}
	csplit $filename "/^.* 7 2 .*$/" {*} --prefix=${OUTFOLDER}/${BASENAME}/${BASENAME} --suffix=_cut%03d.txt
done

