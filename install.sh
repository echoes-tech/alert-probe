#!/bin/bash

# 
# THIS SCRIPT IS PROPRIETARY TO ECHOES TECHNOLOGIES SAS
# AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
# WITHOUT COMPANY AUTHORIZATION.
#
# COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
#

########################
# VARIRABLES DEFINITIONS
########################
LOGIN="contact@echoes-tech.com"
PASSWORD="Unknown"

ASSET_ID=1
TOKEN="abcdefghijklmo0123456789"
PROBE_ID=0

API_HOST='alert-api.echoes-tech.com'
API_PORT=443

CURRENT_DIR=$(pwd)
INSTALL_DIR='/opt/echoes-alert/probe'

HOST_DISTRIB="Unknown"
DISTRIB_RELEASE=0
HOST_ARCH="Unknown"

PKG_TYPE="Unknown"

ERR_INSTALL_MSG="Unable to install ECHOES Alert Probe"

#######################
# FUNCTIONS DEFINITIONS
#######################

are_you_root() {
  if [ $(whoami) != 'root' ]
  then
    echo "ECHOES Alert Probe requires root privileges to install."
    echo "Please re-run this script as root or use 'sudo sh $0' if your user is in /etc/sudoers."
    exit 1
  fi
}

check_host_distrib() {
  if [ -x '/usr/bin/lsb_release' ]
  then
    HOST_DISTRIB=$(/usr/bin/lsb_release -is)
  elif [ -f '/etc/redhat-release' ]
  then
    HOST_DISTRIB=$(sed 's/^\(.*\) release.*/\1/' /etc/redhat-release)
  elif [ -f '/etc/issue' ]
  then
    HOST_DISTRIB=$(head -n 1 /etc/issue | cut -d' ' -f1)
  else
    echo "$ERR_INSTALL_MSG: can't find host distrib."
    exit 1
  fi
}

check_distrib_release() {
  if [ -x '/usr/bin/lsb_release' ]
  then
    DISTRIB_RELEASE=$(/usr/bin/lsb_release -rs)
  elif [ -f '/etc/debian_version' ]
  then
    DISTRIB_RELEASE=$(cat /etc/debian_version)
  elif [ -f '/etc/redhat-release' ]
  then
    DISTRIB_RELEASE=$(sed 's/.*release \([0-9][0-9.]*\).*/\1/' /etc/redhat-release)
  else
    echo "$ERR_INSTALL_MSG: can't find distrib release."
  fi
}

check_host_arch() {
  if [ -x '/usr/bin/arch' ]
  then
    HOST_ARCH=$(/usr/bin/arch)
  elif [ -x '/bin/uname' ]
  then
    HOST_ARCH=$(/bin/uname -m)
  else
    echo "$ERR_INSTALL_MSG: can't find host architecture."
    exit 1
  fi
}

enter_password() {
  echo "To download ECHOES Alert Probe, you have to enter your password."
  echo "Note: login is $LOGIN"
  echo -n "Password: "
  stty -echo; read PASSWORD; stty echo
  echo ""
}

encode_url() {
  echo "$1" | sed -e 's/%/%25/g' -e 's/ /%20/g' -e 's/!/%21/g' -e 's/"/%22/g' -e 's/#/%23/g' -e 's/\$/%24/g' -e 's/\&/%26/g' -e "s/'/%27/g" -e 's/(/%28/g' -e 's/)/%29/g' -e 's/\*/%2A/g' -e 's/+/%2B/g' -e 's/,/%2C/g' -e 's/-/%2D/g' -e 's/\./%2E/g' -e 's/\//%2F/g' -e 's/:/%3A/g' -e 's/;/%3B/g' -e 's/</%3C/g' -e 's/=/%3D/g' -e 's/>/%3E/g' -e 's/?/%3F/g' -e 's/@/%40/g' -e 's/\[/%5B/g' -e 's/\\/%5C/g' -e 's/\]/%5D/g' -e 's/\^/%5E/g' -e 's/_/%5F/g' -e 's/`/%60/g' -e 's/{/%7B/g' -e 's/|/%7C/g' -e 's/}/%7D/g' -e 's/~/%7E/g'
}

test_http_return() {
  HTTP_CODE=$(grep "HTTP/" "$2" | sed -e 's/HTTP\/.* \([0-9]*\) .*/\1/' -e 's/ //g')
  if [ "$(echo $HTTP_CODE | grep "^[ [:digit:] ]*$")" ]
  then
    if [ $HTTP_CODE -lt 200 -o $HTTP_CODE -ge 300 ]
    then
      echo "$ERR_INSTALL_MSG:"
      echo "$1"
      cat "$2"
      exit 1
    fi
  else
    echo "$ERR_INSTALL_MSG: No HTTP Code found in header of $1 request."
    exit 1
  fi
}

get() {
  if [ -x '/usr/bin/curl' ] 
  then
    /usr/bin/curl -# -D "${3}_header" -o "$3" -O "https://${API_HOST}:${API_PORT}${1}?${2}"
  elif [ -x '/usr/bin/openssl' ]
  then
    echo -e "HEAD ${1}?${2} HTTP/1.0\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${3}_header
    echo -e "GET ${1}?${2}\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > $3
  else
    echo "$ERR_INSTALL_MSG: can't find HTTP command line client (curl or openssl)."
    exit 1
  fi

  test_http_return $1 ${3}_header
}

post() {
  if [ -x '/usr/bin/curl' ] 
  then
    /usr/bin/curl -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "https://${API_HOST}:${API_PORT}${1}?${2}"
  elif [ -x '/usr/bin/openssl' ]
  then
    echo -e "HEAD ${1}?${2} HTTP/1.0\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${4}_header
    echo -e "POST ${1}?${2}\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\n${3}\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > $4
  else
    echo "$ERR_INSTALL_MSG: can't find HTTP command line client (curl or openssl)."
    exit 1
  fi

  test_http_return $1 ${4}_header
}

put() {
  if [ -x '/usr/bin/curl' ] 
  then
    /usr/bin/curl -X PUT -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "https://${API_HOST}:${API_PORT}${1}?${2}"
  elif [ -x '/usr/bin/openssl' ]
  then
    echo -e "HEAD ${1}?${2} HTTP/1.0\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${4}_header
    echo -e "PUT ${1}?${2}\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\n${3}\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > $4
  else
    echo "$ERR_INSTALL_MSG: can't find HTTP command line client (curl or openssl)."
    exit 1
  fi

  test_http_return $1 ${4}_header
}

delete() {
  if [ -x '/usr/bin/curl' ] 
  then
    /usr/bin/curl -X DELETE -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "https://${API_HOST}:${API_PORT}${1}?${2}"
  elif [ -x '/usr/bin/openssl' ]
  then
    echo -e "HEAD ${1}?${2} HTTP/1.0\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${4}_header
    echo -e "DELETE ${1}?${2}\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\n${3}\n" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > $4
  else
    echo "$ERR_INSTALL_MSG: can't find HTTP command line client (curl or openssl)."
    exit 1
  fi

  test_http_return $1 ${4}_header
}

probe_installation() {
  if [ $PKG_TYPE = 'deb' ]
  then
    if [ -x '/usr/bin/dpkg' ]
    then
      if [ -x '/usr/bin/apt-get' ]
      then
        /usr/bin/dpkg -i $PKG
        /usr/bin/apt-get -f install
      else
        echo "$ERR_INSTALL_MSG: can't find apt-get conmmand."
        exit 1
      fi
    else
      echo "$ERR_INSTALL_MSG: can't find dpkg conmmand."
      exit 1
    fi
  elif [ $PKG_TYPE = 'rpm' ]
  then
    if [ -x '/usr/bin/yum' ]
    then
      /usr/bin/yum install --nogpgcheck $PKG
    else
      echo "$ERR_INSTALL_MSG: can't find yum conmmand."
      exit 1
    fi
  else
    echo "$ERR_INSTALL_MSG: bad packet type."
    exit 1
  fi
}


###########
# EXECUTION
###########

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
PKG=$(grep '"filename"' probe_creation_res | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
PKG_TYPE=$(echo $PKG | sed 's/.*\.\(.*\)$/\1/g')

cat probe_creation_res | tr -d '\r\n' | sed 's/.*content": "\(.*\)\",\s\+"version.*/\1/g' | /usr/bin/base64 -d > $PKG
if [ $(ls -s $PKG | cut -d' ' -f 1) -eq 0 ]
then
  echo "$ERR_INSTALL_MSG: this release of your Linux distribution is not yet supported."
  echo "Please open a ticket on https://forge.echoes-tech.com/projects/echoes-alert/issues"
  exit 1
fi

echo "ECHOES Alert Probe downloaded."

probe_installation
echo "ECHOES Alert Probe installed."

sed -i -e "s/\(probe_id=\).*/\1$PROBE_ID/" -e "s/\(token=\).*/\1$TOKEN/" $INSTALL_DIR/etc/probe.conf
echo "ECHOES Alert Probe configured."

get "/assets/$ASSET_ID/plugins" "login=$LOGIN_ENC&password=$PASSWORD_ENC" plugins_list_res

mkdir plugins
cd plugins

plugin_id=""
plugin_name=""

check_plugin_id=true

while read line
do
  if $check_plugin_id
  then
    plugin_id=$(echo $line | grep \"id\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
  fi

  if [ $plugin_id ]
  then
    check_plugin_id=false
    plugin_name=$(echo $line | grep \"name\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/\"//g')
    if [ $plugin_name ]
    then
      check_plugin_id=true
      get "/plugins/$plugin_id" "login=$LOGIN_ENC&password=$PASSWORD_ENC" "$plugin_name"
      sed -i -e "s/\(\"idAsset\": \).*/\1$ASSET_ID,/" "$plugin_name"
      plugin_id=""
      plugin_name=""
    fi
  fi
done < ../plugins_list_res

rm -f *_header
cp * $INSTALL_DIR/etc/plugins/

cd "$CURRENT_DIR"
rm -rf "$TMP_DIR"

unset LOGIN_ENC PASSWORD_ENC

service ea-probe start
