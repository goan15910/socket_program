codepath=~/Desktop/socket_program
outputpath=~/Desktop

serverCode=$codepath/server2.c
serverOname=$outputpath/server
clientCode=$codepath/client2.c
clientOname=$outputpath/client

gcc $serverCode -o $serverOname
gcc $clientCode -o $clientOname
