#!/bin/bash -x

# DEBIAN
createVarFile "deb_conffiles" "packages/deb/conffiles" "${deb_debian_dir}/conffiles" 0644

createVarFile "deb_init" "packages/deb/echoes-alert-probe.init" "${NBTMPDIR}/etc/init.d/ea-probe" 0755

createVarFile "deb_postinst" "packages/deb/postinst" "${deb_debian_dir}/postinst" 0755
createVarFile "deb_postrm" "packages/deb/postrm" "${deb_debian_dir}/postrm" 0755
createVarFile "deb_prerm" "packages/deb/prerm" "${deb_debian_dir}/prerm" 0755
