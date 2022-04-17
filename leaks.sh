#!bin/sh
EXEC=webserv
ARGS=demo_website/main.conf

valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --log-file=valgrin.log ./$EXEC $ARGS
grep -A1 "valgrind" valgrin.log | grep $EXEC
