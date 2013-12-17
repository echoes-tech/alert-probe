#!/bin/dash

# 
# THIS SCRIPT IS PROPRIETARY TO ECHOES TECHNOLOGIES SAS
# AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
# WITHOUT COMPANY AUTHORIZATION.
#
# COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
#


#######################
# FUNCTIONS DEFINITIONS
#######################

are_you_root() {
  if [ $(whoami) != 'root' ]
  then
    echo "ECHOES Alert Probe requires root privileges to uninstall."
    echo "Please re-run this script as root or use 'sudo sh $0' if your user is in /etc/sudoers."
    exit 1
  fi
}

package_remove() {
	if [ -x '/usr/bin/apt-get' ]
      	then
        	/usr/bin/apt-get remove --purge $1
        elif [ -x '/usr/bin/yum' ]
        then
      		/usr/bin/yum remove $1        	
        else
        	clean_and_exit "can't find apt-get or yum."
        	exit 1        
        fi
}


###########
# EXECUTION
###########

are_you_root

package_remove "ea-probe-addon"
package_remove "ea-probe"
package_remove "ea-probe-addon*"

echo "ECHOES Alert Probe uninstalled."

exit 0
