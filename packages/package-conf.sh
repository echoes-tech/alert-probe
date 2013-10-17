#!/bin/bash -x

########
# Macros
########

# Executable path generate by compilation
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

PKG_DESCRIPTION="The ECHOES Alert Probe"

# Bin name
BIN_NAME=ea-probe

# Package name
PKG_NAME=ea-probe

# Package type
package="probe"

# Version and package version
version="0.1.0"
pkg_version="1"

# Complete when updating package (update from jenkins environnement = true)
version_old="0.1.0"
pkg_version_old="1"
