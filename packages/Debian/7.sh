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
deb_depends="glib-networking, libjson-glib-1.0-0 (>= 0.12.0), libffi5, libmysqlclient18, libsnmp15"

deb_pkg_name="${deb_pkg_name}_Wheezy_$deb_architecture.deb"

###########
# Execution
###########

. ${TOP}/packages/deb.sh
