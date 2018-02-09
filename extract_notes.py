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

def find_notes( data ):
    nNotes = 0
    u = data.mean( )
    s = data.std( )
    data[ data < u + 2*s ] = 0
    #data = cv2.GaussianBlur( data, (3,3), 0 )
    #edges = cv2.Canny( data, u, u+s )

    # Now walk in time and check if note is there.
    ySum = np.sum( data, axis = 0 )

    noteBegin = False
    for i, v in enumerate( ySum ):
        if not noteBegin and v > 0:
            noteBegin = True
        elif noteBegin and v == 0:
            noteBegin = False
            nNotes += 1

    return  nNotes, np.vstack( (data, ySum) )

def main( ):
    infile = sys.argv[1]
    data = cv2.imread( infile, 0 )
    o = 100
    data = data[o:-o,o:-o]
    nn, data = find_notes( data )
    print( '%d' % nn )

if __name__ == '__main__':
    main()
