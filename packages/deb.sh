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

# Create control file
cd "${TOP}"
CONTROL_FILE=${NBTMPDIR}/DEBIAN/control
rm -f ${CONTROL_FILE}
mkdir -p ${NBTMPDIR}/DEBIAN

cd "${TOP}"
echo "Source: $deb_source" >> ${CONTROL_FILE}
echo "Version: $deb_version" >> ${CONTROL_FILE}
echo "Section: $deb_section" >> ${CONTROL_FILE}
echo "Priority: $deb_priority" >> ${CONTROL_FILE}
echo "Maintainer: $deb_maintainer" >> ${CONTROL_FILE}
echo "Homepage: $deb_homepage" >> ${CONTROL_FILE}
echo "Package: $deb_package" >> ${CONTROL_FILE}
echo "Architecture: $deb_architecture" >> ${CONTROL_FILE}
echo "Depends: $deb_depends" >> ${CONTROL_FILE}
echo "Description: $deb_description" >> ${CONTROL_FILE}

# Create Debian Package
cd "${TOP}"
cd "${NBTMPDIR}/.."
dpkg-deb  --build ${TMPDIRNAME}
checkReturnCode
cd "${TOP}"
mkdir -p  ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
mv ${NBTMPDIR}.deb ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/$deb_pkg_name
checkReturnCode
echo Debian: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/$deb_pkg_name

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
