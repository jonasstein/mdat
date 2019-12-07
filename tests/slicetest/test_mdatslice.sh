#!/bin/bash


./mdatslice_before_2019-12-07 02__M416_TRT5500mK_4000Vp_15000ms.txt 1 2 1 23 > goodresult.txt
../../src/mdatslice -f 02__M416_TRT5500mK_4000Vp_15000ms.txt -t 7 -d 1 --keep 2 -m 1 -n 23 > newresult.txt


diff goodresult.txt newresult.txt && echo "INFO: new mdatslice has same result" || echo "ERROR: new mdatslice has different result"
