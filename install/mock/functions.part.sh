####################
## MOCK FUNCTIONS ##
####################

# Mock functions for integration test:
# put()
# post()
# get()

# ID : name : version
mock_probe=":probe:0.1.0"
mock_common=":common:0.1.0"
mock_addons="1:filesystem:0.1.0 2:file:0.1.0 3:log:0.1.0 5:snmp:0.1.0 6:odbc:0.1.0"

mock_pwd=$(pwd)

get() {
	## Probes
	if mock_testURL $1 /probes/ID/packages	
	then	
		echo "[" > $3
		
		mock_package=$(mock_get_package common)
		echo "{" >> $3
		mock_json_content $3 $mock_package $mock_common
		echo "}," >> $3
		
		mock_package=$(mock_get_package probe)
		echo "{" >> $3
		echo "	\"id\": $ASSET_ID," >> $3
		mock_json_content $3 $mock_package $mock_probe
		echo "}," >> $3
		
		for mock_addon in $mock_addons
		do
			mock_package=$(mock_get_package addons $(echo $mock_addon | cut -d ':' -f 1))
			echo "{" >> $3
			echo "	\"id\": $(echo $mock_addon | cut -d ':' -f 1)," >> $3
			mock_json_content $3 $mock_package $mock_addon
			echo "}" >> $3			
		done
		
		echo "]" >> $3
		mock_return 200	
	elif mock_testURL $1 /probes/ID/json
	then
		cp $mock_pwd/install/mock/informations/informations.json $3
		mock_return 200
	else
		mock_json_bad_request $3
		mock_return 400	
	fi
}

put() {
	## Assets
	if mock_testURL $1 /assets/ID
	then
		mock_return 200
	else
		mock_json_bad_request $4
		mock_return 400	
	fi
}

package_installation() {
  if [ $PKG_TYPE = 'deb' ]
  then
    if [ -x '/usr/bin/dpkg' ]
    then
      if [ -x '/usr/bin/apt-get' ]
      then
        /usr/bin/dpkg -i $PKG
        /usr/bin/apt-get update
        /usr/bin/apt-get -y -f install
      else
        clean_and_exit "can't find apt-get conmmand."
      fi
    else
      clean_and_exit "can't find dpkg conmmand."
    fi
  elif [ $PKG_TYPE = 'rpm' ]
  then
    if [ -x '/usr/bin/yum' ]
    then
      /usr/bin/yum install -y --nogpgcheck $PKG
    else
      clean_and_exit "can't find yum conmmand."
    fi
  else
    clean_and_exit "bad packet type."
  fi
}

mock_get_release_name() {
	if [ $HOST_DISTRIB = CentOS ]
	then	
		if mock_testRegex $DISTRIB_RELEASE "5\.[0-9]+"
		then
			echo 5
		elif mock_testRegex $DISTRIB_RELEASE "6\.[0-9]+"
		then
			echo 6
		fi
	elif [ $HOST_DISTRIB = Debian ]
	then
		if mock_testRegex $DISTRIB_RELEASE "6\.0\.[0-9]+"
		then
			echo Squeeze
		fi
	elif [ $HOST_DISTRIB = Ubuntu ]
	then
		if mock_testRegex $DISTRIB_RELEASE "12\.04" || mock_testRegex $DISTRIB_RELEASE "12\.04\.[0-9]+"
		then
			echo Precise
		fi
	fi
	
}

mock_get_architecture_name() {
	if mock_testRegex $HOST_ARCH "i.86"
        then
                echo i386
        elif [ $HOST_ARCH = x86_64 ]
        then
                if [ $HOST_DISTRIB = CentOS ]
                then
                        echo $HOST_ARCH
                else
                        echo amd64
                fi
        fi
}

mock_get_package() {
	mock_addon_name=""
	if [ $1 = addons ]
	then
		for mock_addon in $mock_addons
		do
			if [ $2 -eq $(echo $mock_addon | cut -d ':' -f 1) ]
			then				
				mock_addon_name=$(echo $mock_addon | cut -d ':' -f 2)
				mock_VERSION=$(echo $mock_addon | cut -d ':' -f 3)
			fi
		done
	elif [ $1 = probe ]
	then
		mock_VERSION=$(echo $mock_probe | cut -d ':' -f 3)
	elif [ $1 = common ]
	then
		mock_VERSION=$(echo $mock_common | cut -d ':' -f 3)
	fi

	ls $mock_pwd/install/mock/packages/$1/ea-probe*$mock_addon_name?$mock_VERSION*$HOST_DISTRIB?$(mock_get_release_name)?$(mock_get_architecture_name)* | head -n 1
}

mock_testURL() {
	mock_testURL_regex=$(echo $2 | sed -r "s/\//\\\\\//g" | sed -r "s/ID/(([1-9])|([0-9]{2,}))/g")
	[ -z $(echo $1 | sed -r "s/$mock_testURL_regex//") ]
}

mock_testRegex() {
	[ -z $(echo $1 | sed -r "s/$2//") ]
}

mock_return() {
	echo "######################################################################## 100,0%
HTTP_CODE: $1"
}

mock_json_bad_request() {
	echo "
	{
		\"message\":\"Bad Request\"
	}" > $1
}

mock_json_content() {
	echo "	\"name\": \"$(echo $3 | cut -d ':' -f 2)\"," >> $1
	echo "	\"version\": \"$(echo $3 | cut -d ':' -f 3)\"," >> $1
	echo "	\"package\": {" >> $1
	echo "		\"filename\": \"$(echo $2 | awk -F/ '{print $NF}')\"," >> $1
	echo "		\"content\": \"$(cat $(echo $2) | base64 -w 0)\"," >> $1
	echo "		\"version\": \"1\"" >> $1
	echo "	}" >> $1
}
