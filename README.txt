#start server
./sever 

#start client
./client [ip address of a server]  [folder/filename]

EXAMPLE TO START A CLIENT: 

./client 127.0.0.1 file.txt OR ./client 127.0.0.1 myfolder

Preferably use folders or files in a current working directory on a SERVER.

NOTE : please provide FULL PATH for directories and files. 
 
Can also use like this for the folder one step outside current working directory:

./client 127.0.0.1 ../myfolder

If the program correctly locates the folder or the files you will see “COPYING..” message on the screen”. It will continue to say that until finishes copying. Writing is a little slow for folders because I was using sleep in several places to achieve better file transfer without a lose. 

