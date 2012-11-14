#!/bin/bash -x

########
# Macros
########

TOP=`pwd`

###########
# Execution
###########

# Copy files and create directories and links

for dirname in $dir_list
do
	tmp_dir=${dirname}_dir
	tmp_chmod=${dirname}_dir_chmod

	cd "${TOP}"
	eval makeDirectory "\$${tmp_dir}" \$${tmp_chmod}
done

for filename in $file_list
do
	tmp_src=${filename}_file_src
	tmp_dst=${filename}_file_dst
	tmp_chmod=${filename}_file_chmod

	cd "${TOP}"
	eval copyFileToTmpDir "\$${tmp_src}" "\$${tmp_dst}" \$${tmp_chmod}
done

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
echo "Summary: $rpm_summary" >> ${SPEC_FILE}
echo "Name: $rpm_name" >> ${SPEC_FILE}
echo "Version: $rpm_version" >> ${SPEC_FILE}
echo "Release: $rpm_release" >> ${SPEC_FILE}
echo "License: $rpm_license" >> ${SPEC_FILE}
echo "Group: $rpm_group" >> ${SPEC_FILE}
echo "URL: $rpm_url" >> ${SPEC_FILE}
echo "Vendor: $rpm_vendor" >> ${SPEC_FILE}
echo "Packager: $rpm_packager" >> ${SPEC_FILE}
echo "Requires: $rpm_requires" >> ${SPEC_FILE}
echo "AutoReqProv: $rpm_autoreqprov" >> ${SPEC_FILE}
echo '%description' >> ${SPEC_FILE}
echo "$rpm_description" >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%post' >> ${SPEC_FILE}
echo "$rpm_post" >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%preun' >> ${SPEC_FILE}
echo "$rpm_preun" >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%postun' >> ${SPEC_FILE}
echo "$rpm_postun" >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%changelog' >> ${SPEC_FILE}
echo "$rpm_changelog" >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%files' >> ${SPEC_FILE}
echo "$rpm_files" >> ${SPEC_FILE}
echo '%dir' >> ${SPEC_FILE}
echo "$rpm_dir" >> ${SPEC_FILE}

# Create RPM Package
cd "${TOP}"
LOG_FILE=${NBTMPDIR}/../${OUTPUT_BASENAME}.log
rpmbuild --buildroot ${TOP}/${NBTMPDIR} --target $rpm_target -bb ${SPEC_FILE} > ${LOG_FILE}
makeDirectory "${NBTMPDIR}"
checkReturnCode
cat ${LOG_FILE}
RPM_PATH=`cat $LOG_FILE | grep '\.rpm' | tail -1 |awk -F: '{ print $2 }'`
RPM_NAME=`basename ${RPM_PATH}`
mv ${RPM_PATH} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/$rpm_pkg_name
checkReturnCode
echo RPM: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/$rpm_pkg_name

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
