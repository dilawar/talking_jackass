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

# record 5 sec into current directory.

OUTFILE=/tmp/rec.wav
DURATION=10
NOISE_PROFILE=./noise_slc.prof

while true; do
    arecord -d $DURATION -t wav -c 1 $OUTFILE
    # now remove noise.
    FILTERED_FILE=$OUTFILE.filtered.wav
    sox $OUTFILE $FILTERED_FILE noisered $NOISE_PROFILE 0.4
    echo "Done removing noise -> $FILTERED_FILE"
    #./extract_notes.py $FILTERED_FILE
    
    ## spectogram
    STATS=$(sox $FILTERED_FILE -n rate 6k spectrogram stats 2>&1)

    ## #echo -e $STATS 
    ## #echo -e $STATS | grep -oP "RMS\s+\w+\s+\S+\s+\S+" || echo "failed #to grep"
    ## RMS_LEVEL=`echo -e $STATS | grep -oP "RMS lev dB\s+\K\S+"`
    ## ST=`echo "$RMS_LEVEL > -30" | bc`
    ## echo $RMS_LEVEL, $ST
    ## if [ $ST -eq 1 ]; then
    ##     notify-send "Too loud.. Trigger jackass."
    ## fi

    nNotes=$(./extract_notes.py ./spectrogram.png)
    echo $nNotes

    if [ $nNotes -gt 10 ]; then
        notify-send "Too many notes: $nNotes. Trigger?"
    fi
done
