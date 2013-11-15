#!/bin/dash

# Usage:
# to generate script for user: sh concat.sh
# to generate script for test: sh concat.sh mock


echo "#!/bin/dash

# 
# THIS SCRIPT IS PROPRIETARY TO ECHOES TECHNOLOGIES SAS
# AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
# WITHOUT COMPANY AUTHORIZATION.
#
# COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
#

########################
# VARIRABLES DEFINITIONS
########################" > install.sh
echo >> install.sh

cat variables.part.sh >> install.sh

echo >> install.sh
echo >> install.sh
echo "#######################
# FUNCTIONS DEFINITIONS
#######################" >> install.sh
echo >> install.sh

cat functions_generic.part.sh >> install.sh
echo >> install.sh

if [ "$1" = "mock" ]
then
	cat mock/functions.part.sh >> install.sh
else
	cat functions.part.sh >> install.sh	
fi

echo >> install.sh
echo >> install.sh
echo "###########
# EXECUTION
###########" >> install.sh
echo >> install.sh

cat execution.part.sh >> install.sh
