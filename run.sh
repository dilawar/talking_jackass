#!/bin/bash -
#===============================================================================
#
#          FILE: run.sh
#
#         USAGE: ./run.sh
#
#   DESCRIPTION: 
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: Dilawar Singh (), dilawars@ncbs.res.in
#  ORGANIZATION: NCBS Bangalore
#       CREATED: Thursday 08 February 2018 06:21:38  IST
#      REVISION:  ---
#===============================================================================

set -o nounset                                  # Treat unset variables as an error

make run
./read_mic_write_to_arduino.py /dev/ttyACM0

