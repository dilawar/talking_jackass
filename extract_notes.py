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
import matplotlib.pyplot as plt
import numpy as np
import scipy.io.wavfile as wavfile
import scipy.signal as sig

def main( ):
    infile = sys.argv[1]
    print( 'Processing %s' % infile )
    sample_rate, samples = wavfile.read( infile )
    frequencies, times, spectogram = sig.spectrogram(samples, sample_rate)
    plt.imshow(spectogram , aspect = 'auto' )
    plt.savefig( 'spec.png' )

if __name__ == '__main__':
    main()
