#!/bin/sh
# $Id$
## @file
#
# Environment setup script.
#
# Copyright (c) 2005-2008 knut st. osmundsen <bird-kBuild-spam@anduin.net>
#
#
# This file is part of kBuild.
#
# kBuild is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# kBuild is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with kBuild; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#
#set -x

#
# Check if we're in eval mode or not.
#
ERR_REDIR=1
DBG_REDIR=1
EVAL_OPT=
DBG_OPT=
QUIET_OPT=
if test "$1" = "--debug"; then
    DBG_OPT="true"
    shift
fi
if test "$1" = "--quiet"; then
    QUIET_OPT="true"
    shift
fi
if test "$1" = "--eval"; then
    EVAL_OPT="true"
    ERR_REDIR=2
    DBG_REDIR=2
    shift
fi


#
# Determin the kBuild path from the script location.
#
if test -z "$PATH_KBUILD"; then
    PATH_KBUILD=`dirname "$0"`
    PATH_KBUILD=`cd "$PATH_KBUILD" ; /bin/pwd`
fi
if test ! -f "$PATH_KBUILD/footer.kmk" -o ! -f "$PATH_KBUILD/header.kmk" -o ! -f "$PATH_KBUILD/rules.kmk"; then
    echo "$0: error: PATH_KBUILD ($PATH_KBUILD) is not pointing to a popluated kBuild directory." 1>&${ERR_REDIR}
    sleep 1;
    exit 1;
fi
export PATH_KBUILD
test -n "$DBG_OPT" && echo "dbg: PATH_KBUILD=$PATH_KBUILD" 1>&${DBG_REDIR}


#
# Set default build type.
#
if test -z "$BUILD_TYPE"; then
    BUILD_TYPE=release
fi
export BUILD_TYPE
test -n "$DBG_OPT" && echo "dbg: BUILD_TYPE=$BUILD_TYPE" 1>&${DBG_REDIR}

#
# Determin the host platform.
#
# The CPU isn't important, only the other two are.  But, since the cpu,
# arch and platform (and build type) share a common key space, try make
# sure any new additions are unique. (See header.kmk, KBUILD_OSES/ARCHES.)
#
if test -z "$BUILD_PLATFORM"; then
    BUILD_PLATFORM=`uname`
    case "$BUILD_PLATFORM" in
        linux|Linux|GNU/Linux|LINUX)
            BUILD_PLATFORM=linux
            ;;

        os2|OS/2|OS2)
            BUILD_PLATFORM=os2
            ;;

        freebsd|FreeBSD|FREEBSD)
            BUILD_PLATFORM=freebsd
            ;;

        openbsd|OpenBSD|OPENBSD)
            BUILD_PLATFORM=openbsd
            ;;

        netbsd|NetBSD|NETBSD)
            BUILD_PLATFORM=netbsd
            ;;

        Darwin|darwin)
            BUILD_PLATFORM=darwin
            ;;

        SunOS)
            BUILD_PLATFORM=solaris
            ;;

        WindowsNT|CYGWIN_NT-*)
            BUILD_PLATFORM=win
            ;;

        *)
            echo "$0: unknown os $BUILD_PLATFORM" 1>&${ERR_REDIR}
            sleep 1
            exit 1
            ;;
    esac
fi
export BUILD_PLATFORM
test -n "$DBG_OPT" && echo "dbg: BUILD_PLATFORM=$BUILD_PLATFORM" 1>&${DBG_REDIR}

if test -z "$BUILD_PLATFORM_ARCH"; then
    # Try deduce it from the cpu if given.
    if test -s "$BUILD_PLATFORM_CPU"; then
        case "$BUILD_PLATFORM_CPU" in
            i[3456789]86)
                BUILD_PLATFORM_ARCH='x86'
                ;;
            k8|k8l|k9|k10)
                BUILD_PLATFORM_ARCH='amd64'
                ;;
        esac
    fi
fi
if test -z "$BUILD_PLATFORM_ARCH"; then
    # Use uname -m or isainfo (lots of guesses here, please help clean this up...)
    if test "$BUILD_PLATFORM" = "solaris"; then
        BUILD_PLATFORM_ARCH=`isainfo | cut -f 1 -d ' '`
        
    else
        BUILD_PLATFORM_ARCH=`uname -m`
    fi
    case "$BUILD_PLATFORM_ARCH" in
        x86_64|AMD64|amd64|k8|k8l|k9|k10)
            BUILD_PLATFORM_ARCH='amd64'
            ;;
        x86|i86pc|ia32|i[3456789]86)
            BUILD_PLATFORM_ARCH='x86'
            ;;
        sparc32|sparc)
            BUILD_PLATFORM_ARCH='sparc32'
            ;;
        sparc64)
            BUILD_PLATFORM_ARCH='sparc64'
            ;;
        s390)
            BUILD_PLATFORM_ARCH='s390'
            ;;
        s390x)
            BUILD_PLATFORM_ARCH='s390x'
            ;;
        ppc32|ppc|powerpc)
            BUILD_PLATFORM_ARCH='ppc32'
            ;;
        ppc64|powerpc64)
            BUILD_PLATFORM_ARCH='ppc64'
            ;;
        mips32|mips)
            BUILD_PLATFORM_ARCH='mips32'
            ;;
        mips64)
            BUILD_PLATFORM_ARCH='mips64'
            ;;
        ia64)
            BUILD_PLATFORM_ARCH='ia64'
            ;;
        #hppa32|hppa|parisc32|parisc)?
        hppa32|parisc32)
            BUILD_PLATFORM_ARCH='hppa32'
            ;;
        hppa64|parisc64)
            BUILD_PLATFORM_ARCH='hppa64'
            ;;
        arm|armv4l|armv5tel)
            BUILD_PLATFORM_ARCH='arm'
            ;;
        alpha)
            BUILD_PLATFORM_ARCH='alpha'
            ;;

        *)  echo "$0: unknown cpu/arch - $BUILD_PLATFORM_ARCH" 1>&${ERR_REDIR}
            sleep 1
            exit 1
            ;;
    esac

fi
export BUILD_PLATFORM_ARCH
test -n "$DBG_OPT" && echo "dbg: BUILD_PLATFORM_ARCH=$BUILD_PLATFORM_ARCH" 1>&${DBG_REDIR}

if test -z "$BUILD_PLATFORM_CPU"; then
    BUILD_PLATFORM_CPU="blend"
fi
export BUILD_PLATFORM_CPU
test -n "$DBG_OPT" && echo "dbg: BUILD_PLATFORM_CPU=$BUILD_PLATFORM_CPU" 1>&${DBG_REDIR}

#
# The target platform.
# Defaults to the host when not specified.
#
if test -z "$BUILD_TARGET"; then
    BUILD_TARGET="$BUILD_PLATFORM"
fi
export BUILD_TARGET
test -n "$DBG_OPT" && echo "dbg: BUILD_TARGET=$BUILD_TARGET" 1>&${DBG_REDIR}

if test -z "$BUILD_TARGET_ARCH"; then
    BUILD_TARGET_ARCH="$BUILD_PLATFORM_ARCH"
fi
export BUILD_TARGET_ARCH
test -n "$DBG_OPT" && echo "dbg: BUILD_TARGET_ARCH=$BUILD_TARGET_ARCH" 1>&${DBG_REDIR}

if test -z "$BUILD_TARGET_CPU"; then
    if test "$BUILD_TARGET_ARCH" = "$BUILD_PLATFORM_ARCH"; then
        BUILD_TARGET_CPU="$BUILD_PLATFORM_CPU"
    else
        BUILD_TARGET_CPU="blend"
    fi
fi
export BUILD_TARGET_CPU
test -n "$DBG_OPT" && echo "dbg: BUILD_TARGET_CPU=$BUILD_TARGET_CPU" 1>&${DBG_REDIR}


# Determin executable extension and path separator.
_SUFF_EXE=
_PATH_SEP=":"
case "$BUILD_PLATFORM" in
    os2|win|nt)
        _SUFF_EXE=".exe"
        _PATH_SEP=";"
        ;;
esac


#
# Calc PATH_KBUILD_BIN (but don't export it).
#
if test -z "$PATH_KBUILD_BIN"; then
    PATH_KBUILD_BIN="${PATH_KBUILD}/bin/${BUILD_PLATFORM}.${BUILD_PLATFORM_ARCH}"
fi
test -n "$DBG_OPT" && echo "dbg: PATH_KBUILD_BIN=${PATH_KBUILD_BIN} (not exported)" 1>&${DBG_REDIR}

# Make shell. OS/2 and DOS only?
if test "$BUILD_PLATFORM" = "os2"; then
    export MAKESHELL="${PATH_KBUILD_BIN}/kmk_ash${_SUFF_EXE}";
fi

#
# Add the bin/x.y/ directory to the PATH.
# NOTE! Once bootstrapped this is the only thing that is actually necessary.
#
PATH="${PATH_KBUILD_BIN}${_PATH_SEP}$PATH"
export PATH
test -n "$DBG_OPT" && echo "dbg: PATH=$PATH" 1>&${DBG_REDIR}

# Sanity and x bits.
if test ! -d "${PATH_KBUILD_BIN}/"; then
    echo "$0: warning: The bin directory for this platform doesn't exists. (${PATH_KBUILD_BIN}/)" 1>&${ERR_REDIR}
else
    for prog in kmk kDepPre kDepIDB kmk_append kmk_ash kmk_cat kmk_cp kmk_echo kmk_install kmk_ln kmk_mkdir kmk_mv kmk_rm kmk_rmdir kmk_sed;
    do
        chmod a+x ${PATH_KBUILD_BIN}/${prog} > /dev/null 2>&1
        if test ! -f "${PATH_KBUILD_BIN}/${prog}${_SUFF_EXE}"; then
            echo "$0: warning: The ${prog} program doesn't exist for this platform. (${PATH_KBUILD_BIN}/${prog}${_SUFF_EXE})" 1>&${ERR_REDIR}
        fi
    done
fi

unset _SUFF_EXE
unset _PATH_SEP

if test -n "$EVAL_OPT"; then
    test -n "$DBG_OPT" && echo "dbg: echoing exported variables" 1>&${DBG_REDIR}
    echo "export BUILD_PLATFORM=${BUILD_PLATFORM}"
    echo "export BUILD_PLATFORM_ARCH=${BUILD_PLATFORM_ARCH}"
    echo "export BUILD_PLATFORM_CPU=${BUILD_PLATFORM_CPU}"
    echo "export BUILD_TARGET=${BUILD_TARGET}"
    echo "export BUILD_TARGET_ARCH=${BUILD_TARGET_ARCH}"
    echo "export BUILD_TARGET_CPU=${BUILD_TARGET_CPU}"
    echo "export BUILD_TYPE=${BUILD_TYPE}"
    echo "export PATH_KBUILD=${PATH_KBUILD}"
    echo "export PATH=${PATH}"
    test -n "$DBG_OPT" && echo "dbg: finished" 1>&${DBG_REDIR}
else
    # Execute command or spawn shell.
    if test $# -eq 0; then
        test -z "${QUIET_OPT}" && echo "$0: info: Spawning work shell..." 1>&${ERR_REDIR}
        if test "$TERM" != 'dumb'  -a  -n "$BASH"; then
            export PS1='\[\033[01;32m\]\u@\h \[\033[01;34m\]\W \$ \[\033[00m\]'
        fi
        $SHELL -i
    else
        echo "$0: info: Executing command: $*"
        $*
    fi
fi

