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
put "/assets/$ASSET_ID" "login=$LOGIN_ENC&password=$PASSWORD_ENC" "{\"architecture\":\"$HOST_ARCH\",\"distribution\":{\"name\":\"$HOST_DISTRIB\",\"release\":\"$DISTRIB_RELEASE\"}}" asset_update_res


# Create a Probe for this Asset
post "/assets/$ASSET_ID/probes" "login=$LOGIN_ENC&password=$PASSWORD_ENC" "" probe_creation_res

PROBE_ID=$(grep '"id"' probe_creation_res | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
PKG_CONTENT_B64=$(cat probe_creation_res | tr -d '\r\n' | sed 's/.*content": "\(.*\)\",\s\+"version.*/\1/g')
if [ -z $PKG_CONTENT_B64 ]
then
  echo "$ERR_INSTALL_MSG: this release of your Linux distribution is not yet supported."
  echo "Please open a ticket on https://forge.echoes-tech.com/projects/echoes-alert/issues"
  cd "$CURRENT_DIR"
  rm -rf "$TMP_DIR"
  exit 1
fi

echo "ECHOES Alert Probe downloaded."


# Get and install addon common library
get "/probes/$PROBE_ID/addons/common" "login=$LOGIN_ENC&password=$PASSWORD_ENC" addon_common_res

PKG=$(grep '"filename"' addon_common_res | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
PKG_TYPE=$(echo $PKG | sed 's/.*\.\(.*\)$/\1/g')

PKG_CONTENT_B64=$(cat addon_common_res | tr -d '\r\n' | sed 's/.*content": "\(.*\)\",\s\+"version.*/\1/g')
if [ -z $PKG_CONTENT_B64 ]
then
  echo "$ERR_INSTALL_MSG: this release of your Linux distribution is not yet supported."
  echo "Please open a ticket on https://forge.echoes-tech.com/projects/echoes-alert/issues"
  cd "$CURRENT_DIR"
  rm -rf "$TMP_DIR"
  exit 1
fi
echo $PKG_CONTENT_B64 | /usr/bin/base64 -d > $PKG

echo "ECHOES Alert add-on common library downloaded."

package_installation

echo "ECHOES Alert add-on common library installed."


# Install probe
PKG=$(grep '"filename"' probe_creation_res | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
PKG_TYPE=$(echo $PKG | sed 's/.*\.\(.*\)$/\1/g')
PKG_CONTENT_B64=$(cat probe_creation_res | tr -d '\r\n' | sed 's/.*content": "\(.*\)\",\s\+"version.*/\1/g')
echo $PKG_CONTENT_B64 | /usr/bin/base64 -d > $PKG

package_installation
echo "ECHOES Alert Probe installed."

sed -i -e "s/\(probe_id=\).*/\1$PROBE_ID/" -e "s/\(token=\).*/\1$TOKEN/" $INSTALL_DIR/etc/probe.conf
echo "ECHOES Alert Probe configured."


# Get and install addon libraries
EXIT_ON_ERROR=0
ERR_INSTALL_MSG="Unable to install ECHOES Alert addons"

get "/probes/$ASSET_ID/addons" "login=$LOGIN_ENC&password=$PASSWORD_ENC" addons_list_res

addon_id=""
addon_name=""
list_addons=""

check_addon_id=true

while read line
do
  if $check_addon_id
  then
    addon_id=$(echo $line | grep \"id\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
  fi

  if [ $addon_id ]
  then
    check_addon_id=false
    addon_name=$(echo $line | grep \"name\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/\"//g')
    if [ $addon_name ]
    then
      check_addon_id=true
      
      list_addons="$list_addons $addon_id:$addon_id"      
      
      addon_id=""
      addon_name=""      
    fi
  fi
done < addons_list_res

for current_addon in $list_addons
do
	addon_name=$(echo $current_addon | cut -d ':' -f 2)
	addon_id=$(echo $current_addon | cut -d ':' -f 1)

	ERR_INSTALL_MSG="Unable to install ECHOES Alert addon $addon_name"
    
      	get "/probes/$ASSET_ID/addons/$addon_id" "login=$LOGIN_ENC&password=$PASSWORD_ENC" "$addon_name"
	      
      	PKG=$(grep '"filename"' $addon_name | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
      	PKG_TYPE=$(echo $PKG | sed 's/.*\.\(.*\)$/\1/g')

      	PKG_CONTENT_B64=$(cat $addon_name | tr -d '\r\n' | sed 's/.*content": "\(.*\)\",\s\+"version.*/\1/g')
      	if [ -z $PKG_CONTENT_B64 ]
      	then
		echo "$ERR_INSTALL_MSG: this release of your Linux distribution is not yet supported."
		echo "Please open a ticket on https://forge.echoes-tech.com/projects/echoes-alert/issues"
      	else
	      	echo $PKG_CONTENT_B64 | /usr/bin/base64 -d > $PKG
	      	echo "ECHOES Alert add-on $addon_name library downloaded."
	      	package_installation
	      	echo "ECHOES Alert add-on $addon_name library installed."
      	fi
done		

# Get and copy informations file
get "/probes/$PROBE_ID/informations" "login=$LOGIN_ENC&password=$PASSWORD_ENC" informations.json

cp informations.json $INSTALL_DIR/etc/

echo "ECHOES Alert informations downloaded."

cd "$CURRENT_DIR"
rm -rf "$TMP_DIR"

service ea-probe start
