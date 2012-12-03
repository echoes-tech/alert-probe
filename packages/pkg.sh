#!/bin/bash -x

######
# Test
######

# An arguement ?
if test $# -eq 0
then
    echo "Usage: ./packages/[...].sh ARCH"
    exit 0
fi

########
# Macros
########

CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe
OUTPUT_BASENAME=probe
PACKAGE_TOP_DIR=/opt/echoes-alert/probe/

###########
# Functions
###########

checkReturnCode ()
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}

makeDirectory ()
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}

copyFileToTmpDir ()
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

createVarDir ()
# $1 name
# $2 path
# $3 chmod
{
    eval "${1}_dir=$2"
    eval "${1}_dir_chmod=$3"

    dir_list="$dir_list $1"
}

createVarFile ()
# $1 name
# $2 src
# $3 dst
# $4 chmod
{
    eval "${1}_file_src=$2"
    eval "${1}_file_dst=$3"
    eval "${1}_file_chmod=$4"

    file_list="$file_list $1"
}

addLib ()
{
   # Lib
   createVarDir "lib" "${NBTMPDIR}/${PACKAGE_TOP_DIR}/lib" $default_dir_right

   mkdir -p "${NBTMPDIR}/${PACKAGE_TOP_DIR}"

   cd /opt/echoes-alert/probe/
   tar cf ${TOP}/${NBTMPDIR}/${PACKAGE_TOP_DIR}/lib.tar --exclude 'python*' --exclude '*.la' --exclude '*.a' lib

   cd ${TOP}/packages
   tar rf ${TOP}/${NBTMPDIR}/${PACKAGE_TOP_DIR}/lib.tar doc

   cd ${TOP}/${NBTMPDIR}/${PACKAGE_TOP_DIR}
   gzip lib.tar

   cd "${TOP}"
}

#######
# Setup
#######

cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

###########
# Variables
###########

project_url="http://alert.echoes-tech.com/"

pkg_description="The ECHOES Alert Probe"
pkg_maintainer="Florent Poinsaut <florent.poinsaut@echoes-tech.com>"

bin_name="ea-probe"
bin_version="0.1.0.beta"

default_file_right=0640
default_dir_right=0750

# Doc
createVarDir "doc" "${NBTMPDIR}/${PACKAGE_TOP_DIR}/doc" 0755

createVarFile "copyright" "COPYRIGHT" "$doc_dir/COPYRIGHT" $default_file_right
createVarFile "authors" "AUTHORS" "$doc_dir/AUTHORS" $default_file_right
createVarFile "readme" "README" "$doc_dir/README" $default_file_right

# Etc
createVarDir "etc" "${NBTMPDIR}/${PACKAGE_TOP_DIR}/etc" $default_dir_right
createVarDir "plugins" "$etc_dir/plugins" $default_dir_right

createVarFile "conf" "conf/probe.conf" "$etc_dir/probe.conf" $default_file_right

# Sbin
createVarDir "sbin" "${NBTMPDIR}/${PACKAGE_TOP_DIR}/sbin" $default_dir_right

createVarFile "bin" "${OUTPUT_PATH}" "$sbin_dir/$bin_name" $default_dir_right

# Init
createVarDir "init" "${NBTMPDIR}/etc/init.d" 0755

# Var
createVarDir "log" "${NBTMPDIR}/var/log/echoes-alert" 0755

createVarFile "log" "/dev/null" "$log_dir/probe.log" $default_file_right
