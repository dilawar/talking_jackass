#!/usr/bin/env python
"""extract_notes.py: 

Extract notes from WAV files.
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
import cv2
import math

def find_notes( spec ):
    data = spec.copy( )
    nNotes = 0
    u = data.mean( )
    s = data.std( )
    data[ data <= u + 2*s ] = 0
    # Now walk in time and check if note is there.
    ySum = np.mean( data, axis = 0 )
    timeN = len( ySum ) / 8000
    noteBegin = False
    noteI = [ ]
    allPower = 0
    for i, v in enumerate( ySum ):
        if not noteBegin and v > 0:
            noteBegin = True
            noteI.append( i )
        elif noteBegin and v == 0:
            noteBegin = False
            noteI.append( i )
            power = sum(ySum[noteI[0]:noteI[1]])
            if math.log( power ) > 2.5:
                noteI = [ ]
                continue
            else:
                noteI = [ ]
                allPower += math.log( power )
            nNotes += 1

    #data = np.vstack((data, ySum ))
    # Find edges.
    edges = cv2.Canny( data, u, u + s)
    data[ data > u + 2*s ] = 255
    im2, contours, hierarchy = cv2.findContours( data, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    return  nNotes, allPower, data, contours

def main( infile ):
    spec = cv2.imread( infile, 0 )
    o = 100
    spec = spec[o:-o,o:-o]
    spec = cv2.bilateralFilter( spec, 11, 20, 20 )
    nn, totalP, data, cnts = find_notes( spec )
    img = np.zeros_like( data )
    for c in cnts:
        if len( c ) > 5:
            ellipse = cv2.fitEllipse(c)
            area = cv2.contourArea( c )
            if area > 80 or area < 10:
                pass
            else:
                cv2.ellipse(img, ellipse, 255, 1)

    print( '%d %d' % (nn, totalP) )
    cv2.imwrite( './spectrogram1_processed.png', np.vstack((spec,img)) )

if __name__ == '__main__':
    main( sys.argv[1] )
