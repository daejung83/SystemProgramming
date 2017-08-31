/**
 * Chatroom Lab
 * CS 241 - Spring 2016
 */
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "server.h"

void *processClient(void *arg);
void write_to_clients(const char *message, int len);

int endingServer;
int serverSocket;
int clients[MAX_CLIENTS];
int clientsConnected;
static pthread_t threads[MAX_CLIENTS];
static pthread_attr_t attr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void close_server() {
  // Use a signal handler to call this function and close the server
  for(int i = 0; i < MAX_CLIENTS; i++){
    pthread_cancel(threads[i]); 
  }
  pthread_mutex_destroy(&mutex);
  pthread_attr_destroy(&attr);
}

int fd_is_valid(int fd){
	return fcntl(fd, F_GETFD) != -1;
}

int main(int argc, char **argv) {
  signal(SIGINT, close_server);
  /*QUESTION 1*/
  /*QUESTION 2*/
  /*QUESTION 3*/
  if(argc < 2){
    fprintf(stderr, "need arg\n");
    exit(1);
  }
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_fd < 0){
    fprintf(stderr, "socket failed\n");
    exit(1);
  }

  /*QUESTION 4*/
  /*QUESTION 5*/
  /*QUESTION 6*/
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int s = getaddrinfo(NULL, argv[1], &hints, &result);
  /*QUESTION 9*/
  if(s != 0){
    fprintf(stderr, "getaddinfo: %s\n", gai_strerror(s));
    exit(1);
  }
  /*QUESTION 8*/
  int optval = 1;
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &optval, sizeof(optval));
  struct sockaddr_in server;
  int portno = atoi(argv[1]);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(portno);

  if(bind(sock_fd, (struct sockaddr*)&server, sizeof(server)) != 0){
    perror("bind()");
    exit(1);
  }

  /*QUESTION 10*/
  if(listen(sock_fd, MAX_CLIENTS) != 0){
    perror("listen()");
    exit(1);
  }

  // Can now start accepting and processing client connections
  /*QUESTION 11*/
  struct sockaddr_in *result_addr = (struct sockaddr_in *) result->ai_addr;
	printf("Listening on file descriptor %d, port %d\n", sock_fd, ntohs(result_addr->sin_port));
	//for(int i = 0; i < MAX_CLIENTS; i++){
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//}
	printf("Waiting for connection...\n");
  intptr_t tempsock;
	for(int i = 0; i < MAX_CLIENTS; i++){
		//sleep(5);
		//if(pthread_kill(threads[i], 0) == ESRCH){
      //pthread_t threads;
			printf("waiting for new accept\n");
			tempsock = accept(sock_fd, (struct sockaddr*)0, (socklen_t*)0);
      if(tempsock < 0){
        printf("accept failed\n");
      }else if(clientsConnected >= MAX_CLIENTS){
        dprintf(tempsock, REJECT);
        close(tempsock);
      }else{
			  pthread_mutex_lock(&mutex);
        clients[i] = tempsock;
        printf("after accept\n");
			  pthread_create(&threads[i], &attr, processClient, (void*)tempsock);		
        printf("after pthread\n");
			  printf("Connection made: client_fd=%d\n", clients[i]);
			  clientsConnected++;		
			  pthread_mutex_unlock(&mutex);
      }
		//}

		if(i == MAX_CLIENTS-1){
			i = 0;
		}
	}
}

// DO NOT MODIFY THIS FUNCTION
void write_to_clients(const char *message, int len) {
  int i;
  for (i = 0; i < clientsConnected; i++) {
    write(clients[i], message, len);
  }
}

// DO NOT MODIFY THIS FUNCTION (BUT DO USE IT)
void *processClient(void *arg) {
  int client_fd = (intptr_t)arg;
  int client_is_connected = 1;
  printf("client: %d\n", client_fd);
  while (client_is_connected) {

    char buffer[MSG_SIZE];
    int len = 0;
    int num;

    // Read until client sends eof or \n is read
    while (1) {
      num = read(client_fd, buffer + len, MSG_SIZE);
      len += num;
      if (!num) {
        client_is_connected = 0;
        break;
      }
      if (buffer[len - 1] == '\n')
        break;
    }

    // Error or client closed the connection, so time to close this specific
    // client connection
    if (!client_is_connected) {
      printf("User %d left\n", client_fd);
      break;
    }

    write_to_clients(buffer, len);
  }

  close(client_fd);
  pthread_mutex_lock(&mutex);
  clientsConnected--;
  pthread_mutex_unlock(&mutex);
  return NULL;
}
