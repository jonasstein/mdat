#!/bin/bash

# break on first failed command
set -e 

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`

BINPATH=${SCRIPTPATH}/src


(cp ${BINPATH}/mkhistogram /usr/local/bin) && \
(cp ${BINPATH}/dumpmdat /usr/local/bin) && \
(cp ${BINPATH}/timeshift /usr/local/bin)  

