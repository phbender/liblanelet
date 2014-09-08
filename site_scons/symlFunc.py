#
# Function doing the actual work of the Install Builder.
#
# This was adapted from "copyFunc" which is in the standard
# scons system (see /usr/lib/scons/SCons/Tool/install.py)
#
# Julius Ziegler, KIT
#

import os

def symlFunc(dest, source, env):
    """Install a source file or directory into a destination by symlinking
    """

    absolute_source_path = os.path.join( os.getcwd(), source )

    if os.path.isdir(source):
        if os.path.exists(dest):
            if not os.path.isdir(dest):
                raise SCons.Errors.UserError("cannot overwrite non-directory `%s' with a directory `%s'" % (str(dest), str(source)))
        else:
            parent = os.path.split(dest)[0]
            if not os.path.exists(parent):
                os.makedirs(parent)
        os.symlink(absolute_source_path, dest)
    else:
        os.symlink(absolute_source_path, dest)

    return 0
