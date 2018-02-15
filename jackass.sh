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

OUTFILE=/tmp/rec.wav
DURATION=20
NOISE_PROFILE=./noise_slc.prof
DATADIR=$HOME/Work/DATA/JACKASS
CACHEDIR=$HOME/.cache/jackass

mkdir -p $CACHEDIR
mkdir -p $CACHEDIR/{POSITIVES_STRONG,POSITIVES}

SERIAL_PORT=/dev/ttyACM0
stty -F $SERIAL_PORT raw speed 38400

function log 
{
    NOW=$(date +"%Y_%m_%d__%H_%M_%S")
    echo "$NOW: $1" | tee -a $HOME/.cache/jackass/jackass.log 
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
    # arecord -f S16_LE -d $DURATION -t wav -c 1 $OUTFILE
    arecord -d $DURATION -t wav -c 1 $OUTFILE
    # now remove noise.
    FILTERED_FILE=$OUTFILE.filtered.wav
    sox -v 0.99 $OUTFILE $FILTERED_FILE noisered $NOISE_PROFILE 0.35

    NOW=$(date +"%Y_%m_%d__%H_%M_%S")
    SPECFILE="$DATADIR/spec_$NOW.png"
    # Create spectogram. We'll use cv2 to extract notes and power from image.
    STATS=$(sox $FILTERED_FILE -n rate 6k spectrogram -o $SPECFILE stats 2>&1)

    OUT=$(./extract_notes.py $SPECFILE)
    nNotes=$(echo $OUT | cut -d' ' -f 1)
    power=$(echo $OUT | cut -d' ' -f 2)

    log "Notes: $nNotes Power: $power"

    # Write to temp file for conky to read.
    echo "$nNotes,$power" > $CACHEDIR/mic

    if [ $nNotes -gt 13 ] && [ $power -gt 12 ]; then
        echo "B" > $SERIAL_PORT
        sleep 4s
    fi

    if [ $nNotes -gt 19 ]; then
        if [ $power -gt 15 ]; then
            log "Noise  ($nNotes) with power ($power)."
            if [ $nNotes -gt 35 ]; then
                notify-send "JackAss  ($nNotes) with power ($power)."
                log "JackAss  ($nNotes) with power ($power)."
                echo "P" > $SERIAL_PORT
                mv $SPECFILE $CACHEDIR/POSITIVES_STRONG/
                sleep 30s
            else
                notify-send "Noise  ($nNotes) with power ($power)."
                echo "A" > $SERIAL_PORT
                mv $SPECFILE $CACHEDIR/POSITIVES/
                sleep 5s
            fi
        fi
    fi
done
