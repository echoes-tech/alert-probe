#!/bin/bash -x

#########
# Include
#########

. ${TOP}/packages/pkg.sh

###########
# Variables
###########

deb_source="$bin_name"
deb_version="$bin_version-1"
deb_section="non-free/admin"
deb_priority="optional"
deb_maintainer="$pkg_maintainer"
deb_homepage="$project_url"
deb_package="$bin_name"
deb_architecture=""
deb_depends=""
deb_description="$pkg_description"

deb_pkg_name="${deb_package}_${deb_version}"

# DEBIAN
createVarDir "deb_debian" "${NBTMPDIR}/DEBIAN" 0755

createVarFile "deb_changelog" "packages/deb/changelog" "$deb_debian_dir/changelog" 0644
createVarFile "deb_conffiles" "packages/deb/conffiles" "$deb_debian_dir/conffiles" 0644
createVarFile "deb_copyright" "packages/deb/copyright" "$deb_debian_dir/copyright" 0644
createVarFile "deb_postinst" "packages/deb/postinst" "$deb_debian_dir/postinst" 0755
createVarFile "deb_postrm" "packages/deb/postrm" "$deb_debian_dir/postrm" 0755
createVarFile "deb_prerm" "packages/deb/prerm" "$deb_debian_dir/prerm" 0755
createVarFile "deb_readme" "packages/deb/README.Debian" "$deb_debian_dir/README.Debian" 0644

createVarFile "deb_init" "packages/deb/echoes-alert-probe.init" "${NBTMPDIR}/etc/init.d/ea-probe" 0755
