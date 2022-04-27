#/bin/sh

../webserv ./network.conf > /dev/null &
server_pid=$!

sleep 1

npx mocha network

kill $server_pid &
