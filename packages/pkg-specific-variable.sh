#!/bin/bash -x

###########
# Variables
###########

# Doc
createVarDir "doc" "${NBTMPDIR}/${PACKAGE_TOP_DIR}/doc" 0755

createVarFile "copyright" "COPYRIGHT" "$doc_dir/COPYRIGHT" $default_file_right
createVarFile "authors" "AUTHORS" "$doc_dir/AUTHORS" $default_file_right
createVarFile "readme" "README" "$doc_dir/README" $default_file_right

# Etc
createVarDir "etc" "${NBTMPDIR}/${PACKAGE_TOP_DIR}/etc" $default_dir_right
createVarDir "plugins" "$etc_dir/plugins" $default_dir_right

createVarFile "conf" "conf/probe.conf" "$etc_dir/probe.conf" $default_file_right

# Sbin
createVarDir "sbin" "${NBTMPDIR}/${PACKAGE_TOP_DIR}/sbin" $default_dir_right

createVarFile "bin" "${OUTPUT_PATH}" "$sbin_dir/${bin_name}" $default_dir_right

# Init
createVarDir "init" "${NBTMPDIR}/etc/init.d" 0755

# Var
createVarDir "log" "${NBTMPDIR}/var/log/echoes-alert" 0755

createVarFile "log" "/dev/null" "$log_dir/probe.log" $default_file_right
