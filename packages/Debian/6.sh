#!/bin/bash -x

########
# Macros
########

TOP=`pwd`

#########
# Include
#########

source ${TOP}/packages/Debian/debian.sh

###########
# Variables
###########

deb_architecture=$1
deb_depends="libssl0.9.8, libffi5, libmysqlclient16"

deb_pkg_name="${deb_pkg_name}_Squeeze_$deb_architecture.deb"

###########
# Execution
###########

addLib
source ${TOP}/packages/deb.sh
