get() {
  if [ -x '/usr/bin/curl' ] 
  then
    /usr/bin/curl -# -D "${3}_header" -o "$3" -O "http://${API_HOST}:${API_PORT}${1}?${2}"
  elif [ -x '/usr/bin/openssl' ]
  then
    printf "GET %s?%s HTTP/1.0\nHost: %s\n\n" "$1" "$2" "$API_HOST" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${3}_tmp 2> /dev/null
    sed '/^\r*$/q' ${3}_tmp > ${3}_header
    sed '1,/^\r*$/d' ${3}_tmp > ${3}
  else
    clean_and_exit "can't find HTTP command line client (curl or openssl)."
  fi

  test_http_return $1 ${3}_header
}

post() {
  if [ -x '/usr/bin/curl' ] 
  then
    /usr/bin/curl -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "http://${API_HOST}:${API_PORT}${1}?${2}"
  elif [ -x '/usr/bin/openssl' ]
  then
    printf "POST %s?%s HTTP/1.0\nHost: %s\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\n\n%s\n" "$1" "$2" "$API_HOST" "$3" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${4}_tmp 2> /dev/null
    sed '/^\r*$/q' ${4}_tmp > ${4}_header
    sed '1,/^\r*$/d' ${4}_tmp > ${4}
  else
    clean_and_exit "can't find HTTP command line client (curl or openssl)."
  fi

  test_http_return $1 ${4}_header
}

put() {
  if [ -x '/usr/bin/curl' ] 
  then
    /usr/bin/curl -X PUT -# -H 'Content-Type: application/json; charset=utf-8' -D "${4}_header" -o "$4" -d "$3" -O "http://${API_HOST}:${API_PORT}${1}?${2}"
  elif [ -x '/usr/bin/openssl' ]
  then
    printf "PUT %s?%s HTTP/1.0\nHost: %s\nContent-Type: application/json; charset=utf-8\nContent-length: $(echo $3 | wc -m)\n\n%s\n" "$1" "$2" "$API_HOST" "$3" | /usr/bin/openssl s_client -quiet -connect ${API_HOST}:${API_PORT} > ${4}_tmp 2> /dev/null
    sed '/^\r*$/q' ${4}_tmp > ${4}_header
    sed '1,/^\r*$/d' ${4}_tmp > ${4}
  else
    clean_and_exit "can't find HTTP command line client (curl or openssl)."
  fi

  test_http_return $1 ${4}_header
}
