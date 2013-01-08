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
PASSWORD=""

ASSET_ID=1
TOKEN="abcdefghijklmo0123456789"
PROBE_ID=0

API_URI='https://alert-api.echoes-tech.com'

CURRENT_DIR=$(pwd)

HOST_DISTRIB=""
DISTRIB_RELEASE=0
HOST_ARCH=""

PKG_TYPE=""

ERR_INSTALL_MSG="Unable to install ECHOES Alert Probe"

#######################
# FUNCTIONS DEFINITIONS
#######################

are_you_root() {
	if [ $(whoami) != 'root' ]
	then
		echo "ECHOES Alert Probe requires root privileges to install. Please re-run this script as root."
		exit 1
	fi
}

check_host_distrib() {
	if [ -x '/usr/bin/lsb_release' ]
	then
		HOST_DISTRIB=$(/usr/bin/lsb_release -is)
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

get() {
	HTTP_CODE=0
        if [ -x '/usr/bin/wget' ]
	then
		/usr/bin/wget -q -S -O "$2" "$1" > "$2_header" 2>&1
	elif [ -x '/usr/bin/curl' ] 
	then
		/usr/bin/curl -# -D "$2_header" -o "$2" -O "$1"
        else
            echo "$ERR_INSTALL_MSG: can't find HTTP command line client (wget or curl)."
            exit 1
        fi
	HTTP_CODE=$(head -n 1 "$2_header" | sed -e 's/HTTP\/.* \([0-9]*\) .*/\1/' -e 's/ //g')
	if [ "$(echo $HTTP_CODE | grep "^[ [:digit:] ]*$")" ]
	then
		if [ $HTTP_CODE -lt 200 -o $HTTP_CODE -ge 300 ]
		then
			echo "$ERR_INSTALL_MSG:"
			cat "$2_header"
			cat "$2"
			exit 1
		fi
	else
		echo "$ERR_INSTALL_MSG."
		exit 1
	fi
}

post() {
	HTTP_CODE=0
        if [ -x '/usr/bin/wget' ]
	then
		/usr/bin/wget -q --header='Content-Type: application/json; charset=utf-8' -S -O "$3" --post-data="$2" "$1" > "$3_header" 2>&1
	elif [ -x '/usr/bin/curl' ] 
	then
		/usr/bin/curl -# --header 'Content-Type: application/json; charset=utf-8' -D "$3_header" -o "$3" -d "$2" -O "$1"
        else
            echo "$ERR_INSTALL_MSG: can't find HTTP command line client (wget or curl)."
            exit 1
        fi
        HTTP_CODE=$(head -n 1 "$3_header" | sed -e 's/HTTP\/.* \([0-9]*\) .*/\1/' -e 's/ //g')
	if [ "$(echo $HTTP_CODE | grep "^[ [:digit:] ]*$")" ]
	then
		if [ $HTTP_CODE -lt 200 -o $HTTP_CODE -ge 300 ]
		then
			echo "$ERR_INSTALL_MSG:"
			cat "$3_header"
			cat $3
			exit 1
		fi
        else
                echo "$ERR_INSTALL_MSG."
                exit 1
	fi
}

parse_probe_request_res()
{
	PROBE_ID=$(cat probe_request_res | grep \"id\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
	PKG=$(cat probe_request_res | grep \"pakage_name\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g' )
	PKG_TYPE=$(cat probe_request_res | grep \"pakage_type\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
	INSTALL_DIR=$(cat probe_request_res | grep \"install_dir\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/"//g')
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

echo "To download ECHOES Alert Probe, you have to enter your password."
echo "Note: login is $LOGIN"
echo -n "Password: "
stty -echo; read PASSWORD; stty echo
echo ""

# URL Encoding
LOGIN_ENC=$(echo "$LOGIN" | sed -e 's/%/%25/g' -e 's/ /%20/g' -e 's/!/%21/g' -e 's/"/%22/g' -e 's/#/%23/g' -e 's/\$/%24/g' -e 's/\&/%26/g' -e "s/'/%27/g" -e 's/(/%28/g' -e 's/)/%29/g' -e 's/\*/%2A/g' -e 's/+/%2B/g' -e 's/,/%2C/g' -e 's/-/%2D/g' -e 's/\./%2E/g' -e 's/\//%2F/g' -e 's/:/%3A/g' -e 's/;/%3B/g' -e 's/</%3C/g' -e 's/=/%3D/g' -e 's/>/%3E/g' -e 's/?/%3F/g' -e 's/@/%40/g' -e 's/\[/%5B/g' -e 's/\\/%5C/g' -e 's/\]/%5D/g' -e 's/\^/%5E/g' -e 's/_/%5F/g' -e 's/`/%60/g' -e 's/{/%7B/g' -e 's/|/%7C/g' -e 's/}/%7D/g' -e 's/~/%7E/g')
PASSWORD_ENC=$(echo "$PASSWORD" | sed -e 's/%/%25/g' -e 's/ /%20/g' -e 's/!/%21/g' -e 's/"/%22/g' -e 's/#/%23/g' -e 's/\$/%24/g' -e 's/\&/%26/g' -e "s/'/%27/g" -e 's/(/%28/g' -e 's/)/%29/g' -e 's/\*/%2A/g' -e 's/+/%2B/g' -e 's/,/%2C/g' -e 's/-/%2D/g' -e 's/\./%2E/g' -e 's/\//%2F/g' -e 's/:/%3A/g' -e 's/;/%3B/g' -e 's/</%3C/g' -e 's/=/%3D/g' -e 's/>/%3E/g' -e 's/?/%3F/g' -e 's/@/%40/g' -e 's/\[/%5B/g' -e 's/\\/%5C/g' -e 's/\]/%5D/g' -e 's/\^/%5E/g' -e 's/_/%5F/g' -e 's/`/%60/g' -e 's/{/%7B/g' -e 's/|/%7C/g' -e 's/}/%7D/g' -e 's/~/%7E/g')

unset LOGIN PASSWORD

TMP_DIR=$(mktemp -d -t ea-probe-install-XXXXXXXXXX)
cd $TMP_DIR

post "$API_URI/asset/$ASSET_ID/probe?login=$LOGIN_ENC&password=$PASSWORD_ENC" "{\"distrib\":{\"name\":\"$HOST_DISTRIB\",\"release\":\"$DISTRIB_RELEASE\"},\"arch\":\"$HOST_ARCH\"}" probe_request_res

parse_probe_request_res

get "$API_URI/probe/$PROBE_ID?login=$LOGIN_ENC&password=$PASSWORD_ENC" $PKG
echo "ECHOES Alert Probe downloaded."

probe_installation
echo "ECHOES Alert Probe installed."

sed -i -e "s/\(probe_id=\).*/\1$PROBE_ID/" -e "s/\(token=\).*/\1$TOKEN/" $INSTALL_DIR/etc/probe.conf
echo "ECHOES Alert Probe configured."

get "$API_URI/asset/$ASSET_ID/plugin?login=$LOGIN_ENC&password=$PASSWORD_ENC" plugins_list_res

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
			get "$API_URI/asset/$ASSET_ID/plugin/$plugin_id?login=$LOGIN_ENC&password=$PASSWORD_ENC" "$plugin_name"
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
