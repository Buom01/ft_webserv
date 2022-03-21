#!bin/sh
EXEC=webserv
ARGS=website_test/init.conf

make re -j
valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --log-file=valgrin.log ./$EXEC $ARGS
grep -A1 "valgrind" valgrin.log | grep $EXEC
