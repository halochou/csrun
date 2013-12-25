#!/bin/sh
for i in $(seq 0 11)
do
 csmips 38024112 19890312 202.112.136.131 3333 > /dev/null
 sleep 5
done
																
