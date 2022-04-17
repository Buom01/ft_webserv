#/bin/sh

../webserv ./tests.conf > /dev/null &
server_pid=$!

sleep 3

yarn mocha http

kill $server_pid
