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
mock_addons="1:filesystem:0.1.1 2:file:0.1.0 3:log:0.1.0 5:snmp:0.1.0 6:odbc:0.1.0"

mock_pwd=$(pwd)

get() {
	## Probes
	if mock_testURL $1 /probes/ID/addons
	then
		echo "["  > $3		
		for mock_addon in $mock_addons
		do
			mock_last_addon_id=$(echo $mock_addon | cut -d ':' -f 1)
		done
		for mock_addon in $mock_addons
		do
			echo "	{" >> $3
			echo "		\"id\": $(echo $mock_addon | cut -d ':' -f 1)," >> $3
			echo "		\"name\": \"$(echo $mock_addon | cut -d ':' -f 2)\"" >> $3
			echo -n "	}" >> $3
			if [ $mock_last_addon_id -ne $(echo $mock_addon | cut -d ':' -f 1) ]
			then
				echo "," >> $3
			else
				echo "" >> $3
			fi
		done			
		echo "]" >> $3
		mock_return 200	
	elif mock_testURL $1 /probes/ID/addons/common
	then
		mock_package=$(mock_get_package common)
		echo "{" > $3
		mock_json_content $3 $mock_package $mock_common
		echo "}" >> $3
		mock_return 200	
	elif mock_testURL $1 /probes/ID/addons/ID
	then
		for mock_addon in $mock_addons
		do
			if [ $(echo $1 | cut -d '/' -f 5) -eq $(echo $mock_addon | cut -d ':' -f 1) ]
			then
				mock_addon_info=$mock_addon
			fi
		done
		mock_package=$(mock_get_package addons $(echo $1 | cut -d '/' -f 5))
		echo "{" > $3
		echo "	\"id\": $(echo $mock_addon | cut -d ':' -f 1)," >> $3
		mock_json_content $3 $mock_package $mock_addon_info
		echo "}" >> $3
		mock_return 200
	elif mock_testURL $1 /probes/ID/informations
	then
		cp $mock_pwd/install/mock/informations/informations.json $3
		mock_return 200
	else
		mock_json_bad_request $3
		mock_return 400	
	fi
}

post() {	
	## Assets
	if mock_testURL $1 /assets/ID/probes
	then
		mock_package=$(mock_get_package probe)
		echo "{" > $4
		echo "	\"id\": $ASSET_ID," >> $4
		mock_json_content $4 $mock_package $mock_probe
		echo "}" >> $4
		mock_return 200
	else
		mock_json_bad_request $4
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

mock_get_release_name() {
	if [ $HOST_DISTRIB = CentOS ]
	then	
		if [ $DISTRIB_RELEASE = 5.* ]
		then
			echo 5
		elif [ $DISTRIB_RELEASE = 6.* ]
		then
			echo 6
		fi
	elif [ $HOST_DISTRIB = Debian ]
	then
		if [ $DISTRIB_RELEASE = 6.0.* ]
		then
			echo Squeeze
		fi
	elif [ $HOST_DISTRIB = Ubuntu ]
	then
		if [ $DISTRIB_RELEASE = 12.04 ] || [ $DISTRIB_RELEASE = 12.04.* ]
		then
			echo Precise
		fi
	fi
	
}

mock_get_architecture_name() {
	if [ $HOST_ARCH = i*86 ]
	then
		echo i386
	elif [ $HOST_ARCH = x86_64 ]
	then
		echo amd64
	fi
}

mock_get_package() {
	if [ $1 = addons ]
	then
		for mock_addon in $mock_addons
		do
			if [ $2 -eq $(echo $mock_addon | cut -d ':' -f 1) ]
			then				
				addon_name=$(echo $mock_addon | cut -d ':' -f 2)
			fi
		done
	else
		addon_name=""
	fi
	ls $mock_pwd/install/mock/packages/$1/ea-probe*$addon_name*$mock_VERSION*$HOST_DISTRIB*$(mock_get_release_name)*$(mock_get_architecture_name)*
}

mock_testURL() {
	mock_testURL_regex=$(echo $2 | sed -r "s/\//\\\\\//g" | sed -r "s/ID/(([1-9])|([0-9]{2,}))/g")
	[ -z $(echo $1 | sed -r "s/$mock_testURL_regex//") ]
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
	echo "		\"content\": \"$(cat $(echo $2) | base64 )\"," >> $1
	echo "		\"version\": \"1\"" >> $1
	echo "	}" >> $1
}
