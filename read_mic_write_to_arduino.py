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

fs = 44100
sd.default.samplerate = fs
sd.default.channels = 1

def compute_power( vec ):
    w = np.sqrt( np.sum( vec ** 2 ) )
    return w

def record( duration ):
    vec = sd.rec( int( duration * fs ) )
    sd.wait( )
    return vec

def main( ):
    frameT = 5
    ser = serial.Serial( sys.argv[1], 38400, timeout = 0.5 )
    try:
        while True:
            val = record( frameT )
            power = compute_power( val )
            output = 'P' if power > 60 else '.'
            print( power, output )
            ser.write( output.encode( ) )
            time.sleep( 0.1 )
            # Write this power value to arduino.
    except KeyboardInterrupt as e:
        print( "keyboad interrupt" )
        ser.close()

    ser.close()

if __name__ == '__main__':
    main()
