#!/bin/bash -x

########
# Macros
########

TOP=`pwd`

#########
# Include
#########

source ${TOP}/packages/Debian/ubuntu.sh

###########
# Variables
###########

deb_architecture=$1
deb_depends="libssl1.0.0, libjson-glib-1.0-0 (>= 0.12.0), libffi5, libmysqlclient18"

deb_pkg_name="${deb_pkg_name}_12.04_$deb_architecture.deb"

###########
# Execution
###########

source ${TOP}/packages/deb.sh
