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

PROBE_ID=0
ASSET_ID=1

API_URI='https://alert-api.echoes-tech.com'

CURRENT_DIR=$(pwd)

HOST_DISTRIB=""
HOST_ARCH=""

PKG_TYPE=""

ERR_INSTALL_MSG="Unable to install ECHOES Alert Probe"

i=0;
j=0;

plugins_id_list[$i]=1
plugins_name_list[$i]='Linux-System.json'

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
        if [ -x '/usr/bin/wget' ]
	then
		/usr/bin/wget -q --save-headers -O $2 "$1"
	elif [ -x '/usr/bin/curl' ] 
	then
		/usr/bin/curl -# -i -o $2 -O "$1"
        else
            echo "$ERR_INSTALL_MSG: can't find HTTP command line client (wget or curl)."
            exit 1
        fi
}

post() {
        if [ -x '/usr/bin/wget' ]
	then
		/usr/bin/wget -q --save-headers -O $3 --post-data=$2 "$1" 
	elif [ -x '/usr/bin/curl' ] 
	then
		/usr/bin/curl -# -i -o $3 -d $2 -O "$1"
        else
            echo "$ERR_INSTALL_MSG: can't find HTTP command line client (wget or curl)."
            exit 1
        fi
}

parse_probe_request_res()
{
	PROBE_ID=$(cat probe_request_res | grep \"id\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
	PKG=$(cat probe_request_res | grep \"pakage_name\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
	PKG_TYPE=$(cat probe_request_res | grep \"pakage_type\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
	INSTALL_DIR=$(cat probe_request_res | grep \"install_dir\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
}

probe_installation() {
	if [ $PKG_TYPE = 'deb' ]
	then
		if [ -x '/usr/bin/dpkg' ]
		then
			/usr/bin/dpkg -i $PKG
		else
			echo "$ERR_INSTALL_MSG: can't find dpkg conmmand."
			exit 1
		fi
	elif [ $PKG_TYPE = 'rpm' ]
	then
		if [ -x '/bin/rpm' ]
		then
			/bin/rpm -Uvh $PKG
		els
			echo "$ERR_INSTALL_MSG: can't find rpm conmmand."
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
check_host_arch

echo "To download ECHOES Alert Probe, you have to enter your password."
echo "Note: login is $LOGIN"
echo -n "Password: "
read PASSWORD


TMP_DIR=$(mktemp -d -t ea-probe-install-XXXXXXXXXX)
cd $TMP_DIR

#post "$API_URI/asset/$ASSET_ID/probe?login=$LOGIN&password=$PASSWORD" "'{\"distrib\" : \"$HOST_DISTRIB\", \"arch\" : \"$HOST_ARCH\"}'" probe_request_res

echo 'HTTP/1.1 200 OK
Date: Wed, 19 Sep 2012 11:36:58 GMT
Server: Apache
Keep-Alive: timeout=15, max=100
Connection: Keep-Alive
Transfer-Encoding: chunked
Content-Type: text/plain

{
    "id" : 1,
    "pakage_name" : "echoes-alert-probe_0.1.0.alpha-1_i386.deb",
    "pakage_type" : "deb",
    "install_dir" : "/opt/echoes-alert/probe"
}' > probe_request_res

parse_probe_request_res

# Téléchargement de la sonde
#get "$API_URI/probe/$PROBE_ID?login=$LOGIN&password=$PASSWORD" $PKG
echo "ECHOES Alert Probe downloaded."

#probe_installation
echo "ECHOES Alert Probe installed."

#sed -i -e "s/\(probe_id=\).*/\1$PROBE_ID/" $INSTALL_DIR/etc/probe.conf
echo "ECHOES Alert Probe configured."

# Liste des plugins attribués à l'asset
#get "$API_URI/asset/$ASSET_ID/plugin?login=$LOGIN&password=$PASSWORD" plugins_list_res

echo 'HTTP/1.1 200 OK
Date: Wed, 19 Sep 2012 11:36:58 GMT
Server: Apache
Keep-Alive: timeout=15, max=100
Connection: Keep-Alive
Transfer-Encoding: chunked
Content-Type: text/plain

[
    {
         "id" : 1,
         "name" : "Linux-System.json"
    },
    {
         "id" : 2,
         "name" : "Linux-Syslog.json"
    }
]' > plugins_list_res

while read line
do
	tmp_id=$(echo $line | grep \"id\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1)
	tmp_name=$(echo $line | grep \"name\" | sed -e 's/ //g' | cut -d':' -f 2 | cut -d',' -f 1 | sed -e 's/\"//g')
	if [ $tmp_id ]
	then
		plugins_id_list[$i]=$tmp_id
		i=$(($i + 1))
	elif [ $tmp_name ]
	then
		plugins_name_list[$i]=$tmp_name
	fi
done < plugins_list_res

mkdir plugins
cd plugins

for j in "${plugins_id_list[@]}"
do
	#get "$API_URI/asset/$ASSET_ID/plugin/${plugins_id_list[$i]}?login=$LOGIN&password=$PASSWORD" ${plugins_name_list[$i]}
done

#cp * $INSTALL_DIR/etc/plugins/

cd "$CURRENT_DIR"
rm -rf "$TMP_DIR"
