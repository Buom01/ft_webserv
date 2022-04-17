#/bin/sh

../webserv ./tests.conf &
server_pid=$!

sleep 3

yarn mocha http

kill $server_pid
