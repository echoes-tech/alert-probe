#!/bin/bash -x

# Sysconfig
createVarDir "rpm_sysconfig" "${NBTMPDIR}/etc/sysconfig/" 0755

createVarFile "rpm_sysconfig" "packages/rpm/echoes-alert-probe.sysconfig" "${NBTMPDIR}/etc/sysconfig/ea-probe" 0755

createVarFile "rpm_init" "packages/rpm/echoes-alert-probe.init" "${NBTMPDIR}/etc/init.d/ea-probe" 0755
