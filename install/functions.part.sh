get() {
  if $API_HTTPS
  then
    if [ -x '/usr/bin/curl' ]
    then
      /usr/bin/curl -# -D "${3}_header" -o "$3" -O "https://${API_HOST}:${API_PORT}${1}?${2}"
    elif [ -x '/usr/bin/openssl' ]
    then
      printf "GET %s?%s HTTP/1.1\nConnection: close\n\n" "$1" "$2" "$API_HOST" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${3}_tmp 2> /dev/null
      sed '/^\r*$/q' ${3}_tmp > ${3}_header
      sed '1,/^\r*$/d' ${3}_tmp > ${3}
    else
      clean_and_exit "can't find HTTPS command line client (curl or openssl)."
    fi
  else
    if [ -x '/usr/bin/curl' ]
    then
      /usr/bin/curl -# -D "${3}_header" -o "$3" -O "http://${API_HOST}:${API_PORT}${1}?${2}"
    elif [ -x '/usr/bin/telnet' ]
    then
      { printf "GET %s?%s HTTP/1.1\nConnection: close\n\n" "$1" "$2"; sleep 1; }  | /usr/bin/telnet ${API_HOST} ${API_PORT} > ${3}_tmp 2> /dev/null
      sed -n '4,8p' ${3}_tmp > ${3}_header
      sed -n '9,$p' ${3}_tmp > $3
    elif [ -x '/bin/nc' ]
    then
      printf "GET %s?%s HTTP/1.1\r\nConnection: close\r\n\r\n" "$1" "$2" | /bin/nc ${API_HOST} ${API_PORT} > ${3}_tmp 2> /dev/null
      sed '/^\r*$/q' ${3}_tmp > ${3}_header
      sed '1,/^\r*$/d' ${3}_tmp > ${3}
    else
      clean_and_exit "can't find HTTP command line client (curl, telnet or netcat)."
    fi
  fi

  test_http_return $1 ${3}_header
}

post() {
  if $API_HTTPS
  then
    if [ -x '/usr/bin/curl' ]
    then
      /usr/bin/curl -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "https://${API_HOST}:${API_PORT}${1}?${2}"
    elif [ -x '/usr/bin/openssl' ]
    then
      printf "POST %s?%s HTTP/1.1\nHost: %s\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\nConnection: close\n\n%s\n" "$1" "$2" "$API_HOST" "$3" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${4}_tmp 2> /dev/null
      sed '/^\r*$/q' ${4}_tmp > ${4}_header
      sed '1,/^\r*$/d' ${4}_tmp > ${4}
    else
      clean_and_exit "can't find HTTP command line client (curl or openssl)."
    fi
  else
    if [ -x '/usr/bin/curl' ]
    then
      /usr/bin/curl -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "http://${API_HOST}:${API_PORT}${1}?${2}"
    elif [ -x '/usr/bin/telnet' ]
    then
      { printf "POST %s?%s HTTP/1.1\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\nConnection: close\n\n%s\n" "$1" "$2" "$3"; sleep 1; }  | /usr/bin/telnet ${API_HOST} ${API_PORT} > ${4}_tmp 2> /dev/null
      sed -n '4,8p' ${4}_tmp > ${4}_header
      sed -n '9,$p' ${4}_tmp > $4
    elif [ -x '/bin/nc' ]
    then
      printf "POST %s?%s HTTP/1.1\r\nContent-Type: application/json; charset=utf-8\r\nContent-length: $(echo $3 | wc -m)\r\nConnection: close\r\n\r\n%s\r\n" "$1" "$2" "$3" | /bin/nc ${API_HOST} ${API_PORT} > ${4}_tmp 2> /dev/null
      sed '/^\r*$/q' ${4}_tmp > ${4}_header
      sed '1,/^\r*$/d' ${4}_tmp > ${4}
    else
      clean_and_exit "can't find HTTP command line client (curl, telnet or netcat)."
    fi
  fi

  test_http_return $1 ${4}_header
}

put() {
  if $API_HTTPS
  then
    if [ -x '/usr/bin/curl' ]
    then
      /usr/bin/curl -X PUT -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "https://${API_HOST}:${API_PORT}${1}?${2}"
    elif [ -x '/usr/bin/openssl' ]
    then
      printf "PUT %s?%s HTTP/1.1\nHost: %s\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\nConnection: close\n\n%s\n" "$1" "$2" "$API_HOST" "$3" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${4}_tmp 2> /dev/null
      sed '/^\r*$/q' ${4}_tmp > ${4}_header
      sed '1,/^\r*$/d' ${4}_tmp > ${4}
    else
      clean_and_exit "can't find HTTP command line client (curl or openssl)."
    fi
  else
    if [ -x '/usr/bin/curl' ]
    then
      /usr/bin/curl -X PUT -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "http://${API_HOST}:${API_PORT}${1}?${2}"
    elif [ -x '/usr/bin/telnet' ]
    then
      { printf "PUT %s?%s HTTP/1.1\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\nConnection: close\n\n%s\n" "$1" "$2" "$3"; sleep 1; }  | /usr/bin/telnet ${API_HOST} ${API_PORT} > ${4}_tmp 2> /dev/null
      sed -n '4,8p' ${4}_tmp > ${4}_header
      sed -n '9,$p' ${4}_tmp > $4
    elif [ -x '/bin/nc' ]
    then
      printf "PUT %s?%s HTTP/1.1\r\nContent-Type: application/json; charset=utf-8\r\nContent-length: $(echo $3 | wc -m)\r\nConnection: close\r\n\r\n%s\r\n" "$1" "$2" "$3" | /bin/nc ${API_HOST} ${API_PORT} > ${4}_tmp 2> /dev/null
      sed '/^\r*$/q' ${4}_tmp > ${4}_header
      sed '1,/^\r*$/d' ${4}_tmp > ${4}
    else
      clean_and_exit "can't find HTTP command line client (curl, telnet or netcat)."
    fi
  fi

  test_http_return $1 ${4}_header
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
              /usr/bin/apt-get -f install
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
            /usr/bin/yum install --nogpgcheck $PKG
        else
            clean_and_exit "can't find yum conmmand."
        fi
    else
        clean_and_exit "bad packet type."
    fi
}
