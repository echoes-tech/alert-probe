#!/bin/bash -x

########
# Macros
########

TOP=`pwd`

#########
# Include
#########

. ${TOP}/packages/Ubuntu/ubuntu.sh

###########
# Variables
###########

deb_architecture=$1
deb_depends="glib-networking, libjson-glib-1.0-0 (>= 0.12.0), libffi6, libmysqlclient18, libsnmp15"

deb_pkg_name="${deb_pkg_name}_12.04_$deb_architecture.deb"

###########
# Execution
###########

. ${TOP}/packages/deb.sh
