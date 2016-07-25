#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>


// get sockaddr, IPv4 or IPv6: used the following for help.
// https://beej.us/guide/bgnet/output/html/multipage/ipstructsdata.html#ipaddrs2
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// This function actually runs the chat between the client and the server. Takes two
// arguments, the socket number and the user's handle. Message limit is 500 characters
void maxChat(int socket_fd, char* handle){
	char buffer[500];
	int numbytes;
	int quit;
	while(1){
	
		//send
		memset(buffer, 0, 500); //This library function copies the second argument, a number, to the first arg (string) equal to the number of the third arg. Used to prepare the message, empties the struct - http://www.tutorialspoint.com/c_standard_library/c_function_memset.htm
		printf("%s", handle);
		fgets(buffer, 499, stdin); //500 (max char) Less 1 is 499. Grab from standard input
		quit = strncmp(buffer, "\\quit", 4); //strncmp compares the first 4 characters of the strings. Return values described here - http://www.tutorialspoint.com/ansi_c/c_strncmp.htm
		if (quit == 0){
			send(socket_fd, buffer, strlen(buffer), 0);
			close(socket_fd); // If quit was picked, exit.
			exit(0);
		}
		else{
			char * message = malloc(strlen(handle)+strlen(buffer)+1);  //Prepare a new message to contain the handle.
			strcpy(message,handle);  //Add handle to message - http://stackoverflow.com/questions/8465006/how-to-concatenate-2-strings-in-c
			strcat(message, buffer);
			if(send(socket_fd, message, strlen(message), 0) == -1){ //Send the message to the socket.
				perror("send");
				exit(1);
			}
		}
		
		//receive
		if ((numbytes = recv(socket_fd, buffer, 499, 0)) == -1) { //500 (max char) Less 1 is 499 
			perror("recv");
			exit(1);
		}
		
		printf("%s",buffer);
	}
	return;
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buffer[500];
    char handle[64];
	size_t ln;
    
    if (argc != 3) {
        fprintf(stderr,"usage: client hostname\n"); //http://beej.us/guide/bgnet/output/html/multipage/clientserver.html#simpleclient
        exit(1);
    }

	//connect to Server
    struct addrinfo hints, *servinfo, *p;
    int status;  //used for the getaddrinfo function below - https://beej.us/guide/bgnet/output/html/multipage/syscalls.html#getaddrinfo
    char s[INET6_ADDRSTRLEN]; 
    
    memset(&hints, 0, sizeof hints); //This library function copies the second argument, a number, to the first arg (string) equal to the number of the third arg. Used to prepare the message, empties the struct - http://www.tutorialspoint.com/c_standard_library/c_function_memset.htm
    hints.ai_family = AF_UNSPEC;  //This flag indicates if we should use IPv4 or IPv6. Currently unspecified.
    hints.ai_socktype = SOCK_STREAM;  //Flag indicates we want to use a stream rather than a datagram

    if ((status = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {  //Anything other than a zero indicates an error
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // loop through and connect to the server
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
            perror("connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "connection error\n");
        exit(1);
    }

	//Convert the IP to a string and print it, per the beej guide.
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("You are connecting to %s\n", s);

    freeaddrinfo(servinfo); // free the memory

	//get handle and send to Server
	printf("Please enter a handle up to %d characters: ", 64);
	fgets(handle, 64, stdin);
	strtok(handle, "\n"); //Remove ending newline character
	strcat(handle, "> ");
	
	//Start chat
	printf("Welcome to Max Chat! If you'd like to stop chatting type '\\quit' to quit at any time\n");
	maxChat(sockfd, handle);      

    close(sockfd);

    return 0;
}