#/bin/sh

../webserv ./http.conf > /dev/null &
server_pid=$!

sleep 1

npx mocha http

kill $server_pid &
