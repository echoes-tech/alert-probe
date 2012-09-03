#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debian6_64bits_Release
CND_DISTDIR=dist
CND_BUILDDIR=build
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe
OUTPUT_BASENAME=probe
PACKAGE_TOP_DIR=/opt/echoes-alert/probe/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
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
function copyFileToTmpDir
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

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/sbin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}sbin/ea-probe" 0750

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/etc"
copyFileToTmpDir "conf/probe.conf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}etc/probe.conf" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/etc/plugins"
copyFileToTmpDir "plugins/Helios_Debian6.0-System-Test.json" "${NBTMPDIR}/${PACKAGE_TOP_DIR}etc/plugins/Helios_Debian6.0-System-Test.json" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}/DEBIAN"
copyFileToTmpDir "packages/deb/README.Debian" "${NBTMPDIR}/DEBIAN/README.Debian" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/DEBIAN"
copyFileToTmpDir "packages/deb/changelog" "${NBTMPDIR}/DEBIAN/changelog" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/DEBIAN"
copyFileToTmpDir "packages/deb/copyright" "${NBTMPDIR}/DEBIAN/copyright" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/DEBIAN"
copyFileToTmpDir "packages/deb/conffiles" "${NBTMPDIR}/DEBIAN/conffiles" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/DEBIAN"
copyFileToTmpDir "packages/deb/postrm" "${NBTMPDIR}/DEBIAN/postrm" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/DEBIAN"
copyFileToTmpDir "packages/deb/squeeze/postinst" "${NBTMPDIR}/DEBIAN/postinst" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/DEBIAN"
copyFileToTmpDir "packages/deb/prerm" "${NBTMPDIR}/DEBIAN/prerm" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}//etc/init.d"
copyFileToTmpDir "packages/deb/echoes-alert-probe.init" "${NBTMPDIR}//etc/init.d/ea-probe" 0755

cd "${TOP}"
makeDirectory  "${NBTMPDIR}//var/log/echoes-alert/" 0750

cd "${TOP}"
makeDirectory "${NBTMPDIR}//var/log/echoes-alert"
copyFileToTmpDir "/dev/null" "${NBTMPDIR}//var/log/echoes-alert/probe.log" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/doc"
copyFileToTmpDir "AUTHORS" "${NBTMPDIR}/${PACKAGE_TOP_DIR}doc/AUTHORS" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/doc"
copyFileToTmpDir "COPYING" "${NBTMPDIR}/${PACKAGE_TOP_DIR}doc/COPYING" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/doc"
copyFileToTmpDir "README" "${NBTMPDIR}/${PACKAGE_TOP_DIR}doc/README" 0640


# Create control file
cd "${TOP}"
CONTROL_FILE=${NBTMPDIR}/DEBIAN/control
rm -f ${CONTROL_FILE}
mkdir -p ${NBTMPDIR}/DEBIAN

cd "${TOP}"
echo 'Source: echoes-alert-probe' >> ${CONTROL_FILE}
echo 'Version: 0.1.0-alpha-1' >> ${CONTROL_FILE}
echo 'Section: non-free/admin' >> ${CONTROL_FILE}
echo 'Priority: optional' >> ${CONTROL_FILE}
echo 'Maintainer: Florent Poinsaut <florent.poinsaut@echoes-tech.com>' >> ${CONTROL_FILE}
echo 'Homepage: http://alert.echoes-tech.com' >> ${CONTROL_FILE}
echo 'Package: echoes-alert-probe' >> ${CONTROL_FILE}
echo 'Architecture: amd64' >> ${CONTROL_FILE}
echo 'Depends: libssl0.9.8 | libssl1.0.0' >> ${CONTROL_FILE}
echo 'Description: The ECHOES Alert Probe' >> ${CONTROL_FILE}

# Create Debian Package
cd "${TOP}"
cd "${NBTMPDIR}/.."
dpkg-deb  --build ${TMPDIRNAME}
checkReturnCode
cd "${TOP}"
mkdir -p  ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
mv ${NBTMPDIR}.deb ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/echoes-alert-probe_0.1.0-alpha-1_squeeze_amd64.deb
checkReturnCode
echo Debian: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/echoes-alert-probe_0.1.0-alpha-1_squeeze_amd64.deb

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
