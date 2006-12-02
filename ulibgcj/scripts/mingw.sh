#! /bin/bash

# This script will download, build, and install versions of Mingw's
# binutils, runtime, and w32api packages known to work with GCJ 4.1.1.

# This script is based on http://mingw.org/MinGWiki/index.php/BuildMingwCross

#-----------------------------------------------------
#
# BEGIN USER SETTINGS
#
# You need to review and adjust the macros that follow
#
#-----------------------------------------------------


# What flavor of GCC cross-compiler are we building?

TARGET=mingw32

# What directory will the cross-compiler be built in?
# This is the directory into which source archives will
# be downloaded, expanded, compiled, etc.  You need to
# have write-access to this directory.  If you leave it
# blank, it defaults to the current directory.

BUILDDIR=

# Where does the cross-compiler go?
# This should be the directory into which your cross-compiler
# will be installed.  Remember that if you set this to a directory
# that only root has write access to, you will need to run this
# script as root.

PREFIX=/usr/local/mingw32

# Purge anything and everything already in the $PREFIX
#(also known as the destination or installation) directory?
# Set to "Y" to purge, any other value omits the purge step.

PURGE_DIR="N"


# Set the following to the files from the current MinGW release
# (or whichever MinGW release you wish to build and install)
# You need to set both the URL they will be downloaded from
# and the exact name of the individual component files.

MINGW_URL="http://heanet.dl.sourceforge.net/sourceforge/mingw"

#BINUTILS_ARCHIVE="binutils-2.16.91-20050827-1-src.tar.gz"
BINUTILS_ARCHIVE="binutils-2.16.91-20060119-1-src.tar.gz"

MINGW_ARCHIVE="mingw-runtime-3.7.tar.gz"

W32API_ARCHIVE="w32api-3.7.tar.gz"


#-----------------------------------------------------
#
# END USER SETTINGS
#
# The remainder of the script should not neet any edits
#
#-----------------------------------------------------



# Make sure these are initialized as we want them

BINUTILS=""


# Set our build directory and where our sources will go

if [ "x$BUILDDIR" = "x" ]; then
        # Default to the current directory
        BUILDDIR=$(pwd)
fi
SRCDIR="$BUILDDIR/source"


# Need install directory first on the path so gcc can find binutils

PATH="$PREFIX/bin:$PATH"



#-----------------------------------------------------
#
# Functions that do most of the work
#
#-----------------------------------------------------


function download_files
{
        # Download a file from a given url, only if it is not present
        mkdir -p "$SRCDIR"

        # Make sure wget is installed
        if test "x`which wget`" = "x" ; then
                echo "You need to install wget."
                exit 1
        fi

        download_file "$BINUTILS_ARCHIVE" "$MINGW_URL"
        download_file "$MINGW_ARCHIVE" "$MINGW_URL"
        download_file "$W32API_ARCHIVE" "$MINGW_URL"
}


function download_file
{
        cd "$SRCDIR"
        if test ! -f $1 ; then
                echo "Downloading $1"
                wget "$2/$1"
                if test ! -f $1 ; then
                        echo "Could not download $1"
                        exit 1
                fi
        else
                echo "Found $1 in the srcdir $SRCDIR"
        fi
        cd "$BUILDDIR"
}


function purge_existing_install
{
        echo "Purging the existing files in $PREFIX"
        if cd "$PREFIX"; then
                rm -rf *
        fi
        cd "$BUILDDIR"
}


function install_libs
{
        echo "Installing cross libs and includes"
        mkdir -p "$PREFIX/$TARGET"
        cd "$PREFIX/$TARGET"

        tar -xzf "$SRCDIR/$MINGW_ARCHIVE"
        tar -xzf "$SRCDIR/$W32API_ARCHIVE"

        cd "$BUILDDIR"
}


function extract_binutils
{
        cd "$SRCDIR"
        BINUTILS=`tar -tzf "$SRCDIR/$BINUTILS_ARCHIVE" | head -n 1`
        rm -rf "$BINUTILS"
        echo "Extracting binutils"
        tar -xzf "$SRCDIR/$BINUTILS_ARCHIVE"
        cd "$BUILDDIR"
}


function configure_binutils
{
        cd "$BUILDDIR"
        rm -rf "binutils-$TARGET"
        mkdir "binutils-$TARGET"
        cd "binutils-$TARGET"
        echo "Configuring binutils"
        "$SRCDIR/$BINUTILS/configure" --prefix="$PREFIX" --target=$TARGET --disable-nls \
                --with-gcc --with-gnu-as --with-gnu-ld --disable-shared &> configure.log
        cd "$BUILDDIR"
}


function build_binutils
{
        cd "$BUILDDIR/binutils-$TARGET"
        echo "Building binutils"
        nice -19 make CFLAGS="-O2 -fno-exceptions" LDFLAGS="-s" &> make.log
        if test $? -ne 0; then
                echo "make of binutils failed - log available: binutils-$TARGET/make.log"
                exit 1
        fi
        cd "$BUILDDIR"
}


function install_binutils
{
        cd "$BUILDDIR/binutils-$TARGET"
        echo "Installing binutils"
        make install &> make-install.log
        if test $? -ne 0; then
                echo "install of binutils failed - log available: binutils-$TARGET/make-install.log"
                exit 1
        fi
        cd "$BUILDDIR"
}

#
# Main part of the script
#

download_files

if [ "x$PURGE_DIR" = "xY" ]; then
        purge_existing_install
fi

install_libs

extract_binutils
configure_binutils
build_binutils
install_binutils

echo "Done!"


#
# End
#
