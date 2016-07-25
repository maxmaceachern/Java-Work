Max MacEachern
CS 372
Summer 2016
Project 1: Chat Server and Client
-----------------------------------------------------------------------
To compile the two files, you'll need to run the following commands:

For chatserve.java - javac chatserve.java

For chatclient.c - gcc -o chatclient chatclient.c
-----------------------------------------------------------------------
To use, enter the following into your shell

server - chatserve.class [port]
client - chatclient [host] [port]

-----------------------------------------------------------------------
The chat program takes requires that messages are sent in turn and therefore 
messages can only be sent one at a time. Additionally, messages of up to 500
haracters and handles of up to 64 characters. Both of the programs were tested
on flip1.engr.oregonstate.edu and flip2.engr.oregonstate.edu

-----------------------------------------------------------------------
The following were used to help develop the program:
https://beej.us/guide/bgnet/
http://docs.oracle.com/javase/tutorial/networking/sockets/index.html

Among many other sources referenced in the code
