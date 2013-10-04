#!/bin/bash -x

rpm_post="cd ${PACKAGE_TOP_DIR}
if [ -f lib.tar.gz ]
then
    tar xzf lib.tar.gz
    rm -f lib.tar.gz
fi
chkconfig --add $bin_name && chkconfig $bin_name on
"

rpm_postun='if [ `find /opt/echoes-alert/probe -type f | wc -l` -eq 0 ]
then
    rm -rf /opt/echoes-alert/probe
    if [ `find /opt/echoes-alert -type f | wc -l` -eq 0 ]
    then
        rm -rf /opt/echoes-alert
    fi
fi'

pm_preun="service $bin_name stop
cd ${PACKAGE_TOP_DIR}
if [ -d lib ]
then
    rm -rf lib
fi
if [ -d doc ]
then
    rm -rf doc
fi
chkconfig $bin_name off
"

rpm_files="\"/${PACKAGE_TOP_DIR}sbin/ea-probe\"
\"/${PACKAGE_TOP_DIR}etc/probe.conf\"
\"//var/log/echoes-alert/probe.log\"
\"//etc/init.d/ea-probe\"
\"//etc/sysconfig/ea-probe\"
\"/${PACKAGE_TOP_DIR}doc/AUTHORS\"
\"/${PACKAGE_TOP_DIR}doc/README\"
\"/${PACKAGE_TOP_DIR}doc/COPYRIGHT\""

rpm_dir="\"//var/log/echoes-alert/\"
\"/${PACKAGE_TOP_DIR}etc/plugins\""

# Sysconfig
createVarDir "rpm_sysconfig" "${NBTMPDIR}/etc/sysconfig/" 0755

createVarFile "rpm_sysconfig" "packages/rpm/echoes-alert-probe.sysconfig" "${NBTMPDIR}/etc/sysconfig/ea-probe" 0755

createVarFile "rpm_init" "packages/rpm/echoes-alert-probe.init" "${NBTMPDIR}/etc/init.d/ea-probe" 0755
