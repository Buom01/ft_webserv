#/bin/sh

../webserv ./siege.conf > /dev/null &
server_pid=$!

sleep 3

siege -b -t 30S http://0.0.0.0:9001 > /dev/null

kill $server_pid
