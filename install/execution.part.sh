are_you_root
check_host_distrib
check_distrib_release
check_host_arch
enter_password

# URL Encoding
LOGIN_ENC=$(encode_url "$LOGIN")
PASSWORD_ENC=$(encode_url "$PASSWORD")
unset LOGIN PASSWORD

TMP_DIR=$(mktemp -d -t ea-probe-install-XXXXXXXXXX)
cd $TMP_DIR

# Update Asset
put "/assets/$ASSET_ID" "login=$LOGIN_ENC&password=$PASSWORD_ENC" "{\"architecture\":\"$HOST_ARCH\",\"distribution\":\"$HOST_DISTRIB\",\"release\":\"$DISTRIB_RELEASE\"}" asset_update_res

# Get all packages
get "/probes/$PROBE_ID/packages" "login=$LOGIN_ENC&password=$PASSWORD_ENC" json_packages

echo "ECHOES Alert Probe downloaded."

exec 3<json_packages

echo > json_package
count=0

while read line 0<&3
do
  occur_open=$(echo $line | grep -o '\[' | wc -l)
  occur_close=$(echo $line | grep -o '\]' | wc -l)
  if [ $count = 0 ] && [ $((occur_open+occur_close)) != 0 ]
  then
    continue
  fi

  echo $line >> json_package
  occur_open=$(echo $line | grep -o '{' | wc -l)
  occur_close=$(echo $line | grep -o '}' | wc -l)
  count=$((count+occur_open-occur_close))

  if [ $count = 0 ]
  then
    PKG=$(grep '"filename"' json_package | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
    PKG_TYPE=$(echo $PKG | sed 's/.*\.\(.*\)$/\1/g')

    PKG_CONTENT_B64=$(grep '"content"' json_package | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
    if [ -z $PKG_CONTENT_B64 ]
    then
      echo "$ERR_INSTALL_MSG: this release of your Linux distribution is not yet supported."
      echo "Please open a ticket on https://forge.echoes-tech.com/projects/echoes-alert/issues"
      cd "$CURRENT_DIR"
      rm -rf "$TMP_DIR"
      exit 1
    fi
    echo $PKG_CONTENT_B64 | /usr/bin/base64 -d > $PKG

    package_installation

    echo > json_package
  fi
done

echo "Alert Probe installed."

sed -i -e "s/\(probe_id=\).*/\1$PROBE_ID/" -e "s/\(token=\).*/\1$TOKEN/" $INSTALL_DIR/etc/probe.conf
echo "ECHOES Alert Probe configured."

# Get and copy informations file
get "/probes/$PROBE_ID/json" "login=$LOGIN_ENC&password=$PASSWORD_ENC" informations.json

# Unescape Json
sed -i -e 's:\\\\:\\:g' -e 's:\\/:/:g' informations.json

cp informations.json $INSTALL_DIR/etc/

echo "Alert informations downloaded."

unset LOGIN_ENC PASSWORD_ENC

cd "$CURRENT_DIR"
rm -rf "$TMP_DIR"

service ea-probe start
