#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=RPM_Release
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
copyFileToTmpDir "plugins/Linux-System.json" "${NBTMPDIR}/${PACKAGE_TOP_DIR}etc/plugins/Linux-System.json" 0640

cd "${TOP}"
makeDirectory  "${NBTMPDIR}//var/log/echoes-alert/" 0750

cd "${TOP}"
makeDirectory "${NBTMPDIR}//var/log/echoes-alert"
copyFileToTmpDir "/dev/null" "${NBTMPDIR}//var/log/echoes-alert/probe.log" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe"
copyFileToTmpDir "packages/lib/amd64/glib_echoes-alert-probe_squeeze_amd64.tar.gz" "${NBTMPDIR}/${PACKAGE_TOP_DIR}glib_echoes-alert-probe_squeeze_amd64.tar.gz" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe"
copyFileToTmpDir "packages/lib/amd64/json-glib_echoes-alert-probe_squeeze_amd64.tar.gz" "${NBTMPDIR}/${PACKAGE_TOP_DIR}json-glib_echoes-alert-probe_squeeze_amd64.tar.gz" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//etc/init.d"
copyFileToTmpDir "packages/rpm/echoes-alert-probe.init" "${NBTMPDIR}//etc/init.d/ea-probe" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}//etc/sysconfig"
copyFileToTmpDir "packages/rpm/echoes-alert-probe.sysconfig" "${NBTMPDIR}//etc/sysconfig/ea-probe" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/doc"
copyFileToTmpDir "AUTHORS" "${NBTMPDIR}/${PACKAGE_TOP_DIR}doc/AUTHORS" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/doc"
copyFileToTmpDir "README" "${NBTMPDIR}/${PACKAGE_TOP_DIR}doc/README" 0640

cd "${TOP}"
makeDirectory "${NBTMPDIR}//opt/echoes-alert/probe/doc"
copyFileToTmpDir "COPYRIGHT" "${NBTMPDIR}/${PACKAGE_TOP_DIR}doc/COPYRIGHT" 0640


# Ensure proper rpm build environment
RPMMACROS=~/.rpmmacros
NBTOPDIR=/tmp/cnd/rpms

if [ ! -f ${RPMMACROS} ]
then
    touch ${RPMMACROS}
fi

TOPDIR=`grep _topdir ${RPMMACROS}`
if [ "$TOPDIR" == "" ]
then
    echo "**********************************************************************************************************"
    echo Warning: rpm build environment updated:
    echo \"%_topdir ${NBTOPDIR}\" added to ${RPMMACROS}
    echo "**********************************************************************************************************"
    echo %_topdir ${NBTOPDIR} >> ${RPMMACROS}
fi  
mkdir -p ${NBTOPDIR}/RPMS

# Create spec file
cd "${TOP}"
SPEC_FILE=${NBTMPDIR}/../${OUTPUT_BASENAME}.spec
rm -f ${SPEC_FILE}

cd "${TOP}"
echo BuildRoot: ${TOP}/${NBTMPDIR} >> ${SPEC_FILE}
echo 'Summary: The ECHOES Alert Probe' >> ${SPEC_FILE}
echo Name: ea-probe >> ${SPEC_FILE}
echo Version: 0.1.0.beta >> ${SPEC_FILE}
echo Release: 1 >> ${SPEC_FILE}
echo License: Proprietary >> ${SPEC_FILE}
echo Group: Applications/System >> ${SPEC_FILE}
echo URL: http://alert.echoes-tech.com >> ${SPEC_FILE}
echo Vendor: ECHOES Technologies SAS >> ${SPEC_FILE}
echo 'Packager: Florent Poinsaut <florent.poinsaut@echoes-tech.com>' >> ${SPEC_FILE}
echo Requires: openssl, mysql-libs >> ${SPEC_FILE}
echo AutoReqProv: no >> ${SPEC_FILE}
echo '%description' >> ${SPEC_FILE}
echo 'The ECHOES Alert Probe' >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%post' >> ${SPEC_FILE}
echo 'cd /opt/echoes-alert/probe' >> ${SPEC_FILE}
echo 'tar xzf glib_echoes-alert-probe_squeeze_amd64.tar.gz' >> ${SPEC_FILE}
echo 'tar xzf json-glib_echoes-alert-probe_squeeze_amd64.tar.gz' >> ${SPEC_FILE}
echo 'rm -f  glib_echoes-alert-probe_squeeze_amd64.tar.gz json-glib_echoes-alert-probe_squeeze_amd64.tar.gz' >> ${SPEC_FILE}
echo 'chkconfig --add ea-probe && chkconfig ea-probe on' >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%preun' >> ${SPEC_FILE}
echo 'service ea-probe stop' >> ${SPEC_FILE}
echo 'cd /opt/echoes-alert/probe' >> ${SPEC_FILE}
echo ' if [ -d lib ]' >> ${SPEC_FILE}
echo ' then' >> ${SPEC_FILE}
echo ' rm -rf lib' >> ${SPEC_FILE}
echo ' fi' >> ${SPEC_FILE}
echo ' if [ -d doc ]' >> ${SPEC_FILE}
echo ' then' >> ${SPEC_FILE}
echo ' rm -rf doc' >> ${SPEC_FILE}
echo ' fi' >> ${SPEC_FILE}
echo 'chkconfig ea-probe off' >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%postun' >> ${SPEC_FILE}
echo 'if [ `find /opt/echoes-alert/probe -type f | wc -l` -eq 0 ]' >> ${SPEC_FILE}
echo 'then' >> ${SPEC_FILE}
echo 'rm -rf /opt/echoes-alert/probe' >> ${SPEC_FILE}
echo 'if [ `find /opt/echoes-alert -type f | wc -l` -eq 0 ]' >> ${SPEC_FILE}
echo 'then' >> ${SPEC_FILE}
echo 'rm -rf /opt/echoes-alert' >> ${SPEC_FILE}
echo 'fi' >> ${SPEC_FILE}
echo 'fi' >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%changelog' >> ${SPEC_FILE}
echo ' * Thu Aug 30 2012 Florent Poinsaut <florent.poinsaut@echoes-tech.com> 0.1.0.beta-1' >> ${SPEC_FILE}
echo '- Initial release' >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%files' >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}sbin/ea-probe\" >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}etc/probe.conf\" >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}etc/plugins/Linux-System.json\" >> ${SPEC_FILE}
echo \"//var/log/echoes-alert/probe.log\" >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}glib_echoes-alert-probe_squeeze_amd64.tar.gz\" >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}json-glib_echoes-alert-probe_squeeze_amd64.tar.gz\" >> ${SPEC_FILE}
echo \"//etc/init.d/ea-probe\" >> ${SPEC_FILE}
echo \"//etc/sysconfig/ea-probe\" >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}doc/AUTHORS\" >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}doc/README\" >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}doc/COPYRIGHT\" >> ${SPEC_FILE}
echo '%dir' >> ${SPEC_FILE}
echo \"//var/log/echoes-alert/\" >> ${SPEC_FILE}

# Create RPM Package
cd "${TOP}"
LOG_FILE=${NBTMPDIR}/../${OUTPUT_BASENAME}.log
rpmbuild --buildroot ${TOP}/${NBTMPDIR} --target x86_64 -bb ${SPEC_FILE} > ${LOG_FILE}
makeDirectory "${NBTMPDIR}"
checkReturnCode
cat ${LOG_FILE}
RPM_PATH=`cat $LOG_FILE | grep '\.rpm' | tail -1 |awk -F: '{ print $2 }'`
RPM_NAME=`basename ${RPM_PATH}`
mv ${RPM_PATH} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
checkReturnCode
echo RPM: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/${RPM_NAME}

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
