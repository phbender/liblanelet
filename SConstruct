#!/usr/bin/env python
# Author:         Julius Ziegler <ziegler@fzi.de>

import os
import subprocess
# a change

# in site_scons/
from symlFunc import symlFunc

# this holds subdirectories which should always be build
import distribution_subdirs
subdirs = distribution_subdirs.subdirs

# some pathes default to be relative to the home directory,
# so get it here from the os
home_directory = os.environ["HOME"]
source_tree_root = Dir( "#" ).abspath

# define some things ("configuration variables")
# which are customisable by
# individual users. These are "sticky", since they
# get saved to "variables.cache"
vars = Variables( "variables.cache" )
vars.Add( "CXX", "use this c++ compiler", "/usr/bin/g++" )
vars.Add( "CC", "use this c compiler", "/usr/bin/gcc" )
vars.Add( BoolVariable( "INSTALL_AS_SYML", "If set, installation of files is done by symbolic linking", 1 ) )
vars.Add( BoolVariable( "RELEASE", "Set to build for release", 0 ) )
vars.Add( "OPTIMIZATION_FLAGS", "add these to compile command", "-O3" )
vars.Add( "SPECIAL_LINK_FLAGS", "add these to link commands. Defaults work with g++-4.6, other compilers might need others (try \"-Wl,--no-as-needed\") for g++-4.4", "-Wl,--no-as-needed,--copy-dt-needed-entries" )
vars.Add( BoolVariable( "USE_RPATH", "Hardcode paths to libraries into binaries ($PREFIX/lib). Otherwise, you must rely on your os loader to find them (LD_LIBRARY_PATH, /etc/ld.so.conf or the like)", 1 ) )
vars.Add( PathVariable( "PREFIX", "Installation prefix", os.path.join( source_tree_root, "installs" ), PathVariable.PathAccept ) )
vars.Add( "EXTRA_FLAGS", "Merge these flags into the default build enviroment (quick and dirty)", "" )
vars.Add( "TRACE_LEVEL", 'specify amount of console output as binary disjunction of some symbolic variables, cf. Commons/trace.hpp. The disjunction sign must be escaped! Example: TRACE_LEVEL="warning\|gui", shows debug output up to the "warning" category, plus gui related debug output.', "critical" )

for subdir in subdirs:
    # print "visiting SConscript.vars in", subdir
    subdir_var_sconscript = os.path.join( subdir, "SConscript.conf" )
    if os.path.exists( subdir_var_sconscript ):
        SConscript( subdir_var_sconscript, exports=[ "vars" ] )

env = Environment( variables = vars )

vars.Save( "variables.cache", env )

# this autogenerates help text for config variables, try "scons -h". If
# you want the original scons help, use scons -H
Help( vars.GenerateHelpText( env ) )

# do not try to build anything if help was requested
if env.GetOption('help'):    
    Return()

# change install behaviour if requested
if env["INSTALL_AS_SYML"]:
    # install by symlinks
    env['INSTALL'] = symlFunc

env.MergeFlags( env["EXTRA_FLAGS"] )
env.MergeFlags( env["OPTIMIZATION_FLAGS"] )
  
env.Decider( "MD5-timestamp" )

# I want C++0x by default
# env.MergeFlags( "-std=c++0x" )
env.Append( CCFLAGS="-std=c++0x" )

# this fixes linkage problems on Ubuntu 11.10/gcc-4.6 
env.Append( LINKFLAGS=env["SPECIAL_LINK_FLAGS"] )

# setup RELEASE/DEBUG-stuff
if env["RELEASE"]:
    env.MergeFlags( "-g -DNDEBUG -DBOOST_DISABLE_ASSERTS" ) # -ffast-math
else:
    env.MergeFlags( "-g -DDEBUG" )

# this is where we want compiled libs to be installed
install_libs_dir       = os.path.join( env["PREFIX"], "lib/MRT" )

# to install thirdparty libs in a non-global way (/usr/lib/...), you may pass a prefix.
# choose /path/to/bbf/installs as this prefix. Then, the lib folder should be included in the library
# search path.
install_libs_thirdparty_dir       = os.path.join( env["PREFIX"], "lib" )

# this is where we want compiled executables to be installed
install_bin_dir        = os.path.join( env["PREFIX"], "bin" )

# this is where we want shared data to be installed
install_shared_dir     = os.path.join( env["PREFIX"], "shared", "MRT" )

# this is where headers are installed. This prefix is extended by a "midfix", but see below
install_headers_prefix = os.path.join( env["PREFIX"], "include" )

# this is where we want configuration files to be installed
install_config_dir     = os.path.join( env["PREFIX"], "etc" )

env.Append( LINKFLAGS=env["EXTRA_FLAGS"] )

if env["USE_RPATH"]:
    # env.Append( RPATH = install_libs_dir )
    # the RPATH method above does not work with multiple rpath, do it hackily:
    env.Append( LINKFLAGS="-Wl,-rpath="+install_libs_dir )
    env.Append( LINKFLAGS="-Wl,-rpath="+install_libs_thirdparty_dir )
    # current working directory is good to find libraries which are not installed.
    # They can then be simply linked via their scons node.

    # (This does not work (jz)??)
    # env.Append( LINKFLAGS="-Wl,-rpath="+cwd )

# libs build and installed by us should always be found
env.Append( LIBPATH = [install_libs_dir] );

# ... as well as thirdparty stuff, see above.
env.Append( LIBPATH = env['LIBPATH'] + [install_libs_thirdparty_dir] );

# headers installed by us should always be found
env.Append( CPPPATH = install_headers_prefix );

# pass trace level as a compile time define 
# env.Append( CPPDEFINES = ('TRACE_LEVEL', '\\"' + env["TRACE_LEVEL"] + '\\"') )
env.Append( CPPDEFINES = ('TRACE_LEVEL', env["TRACE_LEVEL"] ) )

# try to compile a trivial program, to see if something is
# completely wrong with the compiler configuration
conf = env.Configure()
conf.Finish()

# Putting all paths
# inside env now (this is different from the former GCDC build system).
env["install_headers_prefix"] = install_headers_prefix
env["install_config_dir"]     = install_config_dir
env["install_libs_dir"]       = install_libs_dir
env["install_libs_thirdparty_dir"]       = install_libs_thirdparty_dir
env["install_bin_dir"]        = install_bin_dir
env["install_shared_dir"]     = install_shared_dir

# as requested by some: even when doing an "install", we want to build non-installable targets
Alias( target = "install", source="." )

for subdir in subdirs:
    # we pass the subdirectory name itself down to
    # the SConscript's, because we will base
    # header installation path on them, like this:
    #
    # /usr/local/include/MRT/MPC/mpc_stuff.hpp
    # /usr/local/include/MRT/Communication/Intercomm.hpp
    # ...
    install_headers_dir = os.path.join( install_headers_prefix, "MRT", subdir )
    # clone an environment for the subdirectory, and add subdir specific header installation path to it
    subdir_env = env.Clone()
    subdir_env["install_headers_dir"] = install_headers_dir
    subdir_env["subdir"] = Dir( subdir )
    
    # Default( subdir )

    variant_dir = os.path.join(subdir, "build")
   
    SConscript( os.path.join( subdir, "SConscript" ), exports=[ "subdir_env",
        "vars" ], variant_dir=variant_dir, duplicate=0 )
    
