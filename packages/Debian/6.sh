#!/bin/bash -x

########
# Macros
########

TOP=`pwd`

#########
# Include
#########

. ${TOP}/packages/Debian/debian.sh

###########
# Variables
###########

deb_architecture=$1
deb_depends="libffi5, libmysqlclient16"

deb_pkg_name="${deb_pkg_name}_Squeeze_$deb_architecture.deb"

###########
# Execution
###########

addLib
. ${TOP}/packages/deb.sh
