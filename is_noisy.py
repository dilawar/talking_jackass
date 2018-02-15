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

def find_notes( data ):
    nNotes = 0
    u = data.mean( )
    s = data.std( )
    data[ data < u + 2*s ] = 0

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

    data = np.vstack((data, ySum ))
    return  nNotes, allPower, data

def main( infile ):
    data = cv2.imread( infile, 0 )
    o = 100
    data = data[o:-o,o:-o]
    nn, totalP, data = find_notes( data )
    print( '%d %d' % (nn, totalP) )
    cv2.imwrite( './processed.png', data )

if __name__ == '__main__':
    main( sys.argv[1] )
