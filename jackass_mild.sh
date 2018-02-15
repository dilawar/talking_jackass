#!/bin/bash -
#===============================================================================
#
#          FILE: jackass.sh
#
#         USAGE: ./jackass.sh
#
#   DESCRIPTION: 
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: Dilawar Singh (), dilawars@ncbs.res.in
#  ORGANIZATION: NCBS Bangalore
#       CREATED: Friday 09 February 2018 10:47:03  IST
#      REVISION:  ---
#===============================================================================

set -o nounset                                  # Treat unset variables as an error
set -e

OUTFILE=/tmp/jackass.wav
DURATION=15
NOISE_PROFILE=./noise_slc.prof

function log 
{
    NOW=$(date +"%Y_%m_%d__%H_%M_%S")
    echo "$NOW: $1" | tee -a $HOME/.cache/jackass/jackass_mild.log 
}

while true; do

    # Dont record in non-working hours.
    HOUR=$(date +"%H")
    # What a complicated way to doing arithmatic in bash.
    if [ $((10#"$HOUR")) -lt 9 ] || [ $((10#"$HOUR")) -gt 18 ]; then 
        log "Non-working hours."
        sleep 10m
        continue
    fi

    IDLE_FOR=$(sudo -u dilawars xprintidle)
    if [ "$IDLE_FOR" -gt 120000 ]; then
        log "Been idle for more than 2 minutes. Doing nothing." 
        sleep 60s
    fi

 
    # Everything breaks down with signed data.
    arecord -f S16_LE -d $DURATION $OUTFILE
    #arecord -d $DURATION -t wav -c 1 $OUTFILE

    # now remove noise.
    FILTERED_FILE=$OUTFILE.filtered.wav
    sox  $OUTFILE $FILTERED_FILE noisered $NOISE_PROFILE 0.21

    NOW=$(date +"%Y_%m_%d__%H_%M_%S")
    SPECFILE="./spectrogram1.png"

    # Create spectrogram.
    # -m : create monochromatic spectrogram.
    sox $FILTERED_FILE -n spectrogram -m -o $SPECFILE 

    OUT=$(python ./is_noisy.py $SPECFILE)
    echo $OUT
done
