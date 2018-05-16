#!/bin/bash
# 
# File:   mobility-trace.bash
# Author: fabrice
#
# Created on Apr 19, 2018, 2:13:04 PM
#




./bm -f mobility4adaptation RandomWaypoint -x 1500 -y 500 -p 30 -s 2.0 2.0 -R 113 -n 50 -d 25800 -i 3600
./bm NSFile -f mobility4adaptation