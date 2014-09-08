import os
from glob import glob

subdirs = [os.path.dirname( filename ) for filename in glob( "*/SConscript" ) ]

