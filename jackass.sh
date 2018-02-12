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
DATADIR=$HOME/Work/DATA/JACKASS

while true; do
    arecord -d $DURATION -t wav -c 1 $OUTFILE
    # now remove noise.
    FILTERED_FILE=$OUTFILE.filtered.wav
    sox $OUTFILE $FILTERED_FILE noisered $NOISE_PROFILE 0.4
    echo "Done removing noise -> $FILTERED_FILE"

    NOW=$(date +"%Y_%m_%d__%H_%M_%S")
    SPECFILE="$DATADIR/spec_$NOW.png"
    # Create spectogram. We'll use cv2 to extract notes and power from image.
    STATS=$(sox $FILTERED_FILE -n rate 6k spectrogram -o $SPECFILE stats 2>&1)

    ## #echo -e $STATS 
    ## #echo -e $STATS | grep -oP "RMS\s+\w+\s+\S+\s+\S+" || echo "failed #to grep"
    ## RMS_LEVEL=`echo -e $STATS | grep -oP "RMS lev dB\s+\K\S+"`
    ## ST=`echo "$RMS_LEVEL > -30" | bc`
    ## echo $RMS_LEVEL, $ST
    ## if [ $ST -eq 1 ]; then
    ##     notify-send "Too loud.. Trigger jackass."
    ## fi

    OUT=$(./extract_notes.py ./spectrogram.png)
    nNotes=$(echo $OUT | cut -d' ' -f 1)
    power=$(echo $OUT | cut -d' ' -f 2)
    echo "Notes: $nNotes Power: $power"

    if [ $nNotes -gt 10 ]; then
        notify-send "Too many notes ($nNotes) with power ($power). Trigger?"
    fi
done
