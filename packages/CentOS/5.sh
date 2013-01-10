#!/bin/bash -x

########
# Macros
########

TOP=`pwd`

#########
# Include
#########

. ${TOP}/packages/CentOS/centos.sh

###########
# Variables
###########

rpm_target=$1
rpm_requires="mysql, net-snmp-libs"

rpm_files="$rpm_files
\"/${PACKAGE_TOP_DIR}lib.tar.gz\""

rpm_pkg_name="${rpm_pkg_name}-5.$rpm_target.rpm"

###########
# Execution
###########

addLib
. ${TOP}/packages/rpm.sh
