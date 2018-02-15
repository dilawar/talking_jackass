#!/bin/bash -
#===============================================================================
#
#          FILE: sox_spectrogram.sh
#
#         USAGE: ./sox_spectrogram.sh
#
#   DESCRIPTION: 
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: Dilawar Singh (), dilawars@ncbs.res.in
#  ORGANIZATION: NCBS Bangalore
#       CREATED: Thursday 15 February 2018 11:00:02  IST
#      REVISION:  ---
#===============================================================================

set -o nounset                                  # Treat unset variables as an error
INPUTFILE="$1"

if [ ! -f $INPUTFILE ]; then
    echo "USAGE: $0 filename";
    exit
fi

sox -n -n rate 8k spectrogram -r -o - | convert png:- txt:- \
    | awk -v FS='[,:() ]+' 'NR > 2    { print $1, $2, ($3+$4+$5)/3}' \
    | awk   'NR > 2 && $2 != prev { printf "\n" } {prev = $2 } 1' \
    > spectrogram.dat
