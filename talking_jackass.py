#!/usr/bin/env python

"""test.py: 

"""
    
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2017-, Dilawar Singh"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"

import sys
import os
import numpy as np
import serial
import time
import sounddevice as sd 
import datetime

fs = 44100
sd.default.samplerate = fs
sd.default.channels = 1

def compute_power( vec ):
    w = np.sqrt( np.sum( vec ** 2 ) ) / len(vec)
    return w

def record( duration ):
    vec = sd.rec( int( duration * fs ) )
    sd.wait( )
    return vec

def main( ):
    ser = serial.Serial( sys.argv[1], 38400, timeout = 0.5 )
    frameT = 10
    try:
        while True:
            h = datetime.datetime.now( )
            val = record( frameT )
            power = compute_power( val )
            if h.hour > 8 and h.hour < 19:
                output = ' P' if power > 50 else '.'
                print( power )
                ser.write( output.encode( ) )
                time.sleep( frameT )
            else:
                print( 'not during working hours' )
                output = '.'
                ser.write( output.encode( ) )
                time.sleep( frameT )

            with open( '/tmp/mic_power', 'w' ) as f:
                f.write( '%.3f' % power )

            with open( '/var/run/log/jackass.csv', 'a' ) as f:
                stamp = datetime.datetime.now().isoformat( )
                f.write( '%s,%g\n' % (stamp, power ) )


    except KeyboardInterrupt as e:
        print( "keyboad interrupt" )
        ser.close()

    ser.close()

if __name__ == '__main__':
    main()
