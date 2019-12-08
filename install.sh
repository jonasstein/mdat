#!/bin/bash

# break on first failed command
set -e 

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`

BINPATH=${SCRIPTPATH}/src


(cp ${BINPATH}/mdathistogram /usr/local/bin) && \
(cp ${BINPATH}/mdatdump /usr/local/bin) && \
(cp ${BINPATH}/mdatslice /usr/local/bin) && \
(cp ${BINPATH}/mdataddtof.pl /usr/local/bin) && \
(cp ${BINPATH}/mdatspikefilter.pl /usr/local/bin) && \
(cp ${BINPATH}/mdatshift /usr/local/bin)  

