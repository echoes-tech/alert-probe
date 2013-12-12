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

clean_and_exit() {
    echo "$ERR_INSTALL_MSG: $1"
    cd "$CURRENT_DIR"
    rm -rf "$TMP_DIR"
    exit 1
}

test_http_return() {
  HTTP_CODE=$(grep "HTTP/" "$2" | sed -e 's/HTTP\/.* \([0-9]*\) .*/\1/' -e 's/ //g')
  echo "HTTP_CODE: ${HTTP_CODE}" 
  if [ "$(echo $HTTP_CODE | grep "^[ [:digit:] ]*$")" ]
  then
    if [ $HTTP_CODE -lt 200 -o $HTTP_CODE -ge 300 ]
    then
      echo "$ERR_INSTALL_MSG:"
      echo "$1"
      cat "$2"
      if [ $EXIT_ON_ERROR ]
      then
      	exit 1
      fi
    fi
  else
    echo "$ERR_INSTALL_MSG: No HTTP Code found in header of ${1} request."
    echo ''
    echo 'ECHOES Alert API is not reachable.'
    echo ''
    echo 'ECHOES Alert Probe may not install if you are behind a HTTP(s) proxy server'
    echo 'ECHOES Alert Probes may not run  correctly in this case'
    echo 'If you are running behind a HTTP proxy please run the following command'
    echo 'before re run the install script'
    echo ' $ export https_proxy=MyProxyAdress:MyProxyPort'
    echo 'or if authentification is needed with your proxy'
    echo ' $ export https_proxy=Login:Password@MyProxyAdress:MyProxyPort'

    cd "$CURRENT_DIR"
    rm -rf "$TMP_DIR"
    if [ $EXIT_ON_ERROR ]
    then
      exit 1
    fi
  fi
}
