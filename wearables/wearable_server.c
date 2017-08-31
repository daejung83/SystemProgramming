/**
 * Machine Problem: Wearables
 * CS 241 - Spring 2016
 */

#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>

#include "queue.h"

const char *TYPE1 = "heart_beat";
const char *TYPE2 = "blood_sugar";
const char *TYPE3 = "body_temp";

// The wearable server socket, which all wearables connect to.
int wearable_server_fd;

// A lock for your queue.
pthread_mutex_t queue_lock_;
// A queue for all received data.
queue_t receieved_data_;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
static int end_program;
static struct addrinfo *result;

typedef struct SampleData {

  char type_[50];
  int data_;

} SampleData;

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

int select1(void *data) {
  return strcmp(((SampleData *)data)->type_, TYPE1) == 0;
}

int select2(void *data) {
  return strcmp(((SampleData *)data)->type_, TYPE2) == 0;
}

int select3(void *data) {
  return strcmp(((SampleData *)data)->type_, TYPE3) == 0;
}

int selectorall(void *__attribute__((unused)) val) { return 1; }

typedef struct {
  pthread_t thread;
  int fd;
  long timestamp;
  // TODO you might want to put more things here
	int done;
} thread_data;

thread_data **wearable_threads;
int wearable_threads_size = 0;

/**
 * Used to write out the statistics of a given results set (of
 * timestamp_entry's).  To generate the result set see queue_gather(). fd is the
 * file descriptor to which the information is sent out. The type is the type of
 * data that is written out (TYPE1, TYPE2, TYPE3). results is the array of
 * timestamp_entrys, and size is the size of that array. NOTE: that you should
 * call method for every type (TYPE1, TYPE2, TYPE3), and then write out the
 * infomration "\r\n" to signify that you have finished sending out the results.
 */
void write_results(int fd, const char *type, timestamp_entry *results,
                   int size) {
  long avg = 0;
  int i;

  char buffer[1024];
  int temp_array[size];
  sprintf(buffer, "Results for %s:\n", type);
  sprintf(buffer + strlen(buffer), "Size:%i\n", size);
  for (i = 0; i < size; i++) {
    temp_array[i] = ((SampleData *)(results[i].data_))->data_;
    avg += ((SampleData *)(results[i].data_))->data_;
  }

  qsort(temp_array, size, sizeof(int), compare);

  if (size != 0) {
    sprintf(buffer + strlen(buffer), "Median:%i\n",
            (size % 2 == 0)
                ? (temp_array[size / 2] + temp_array[size / 2 - 1]) / 2
                : temp_array[size / 2]);
  } else {
    sprintf(buffer + strlen(buffer), "Median:0\n");
  }

  sprintf(buffer + strlen(buffer), "Average:%li\n\n",
          (size == 0 ? 0 : avg / size));
  write(fd, buffer, strlen(buffer));
}

/**
 * Given an input line in the form <timestamp>:<value>:<type>, this method
 * parses the infomration from the string, into the given timestamp, and mallocs
 * space for SampleData, and stores the type and value within
 */
void extract_key(char *line, long *timestamp, SampleData **ret) {
  *ret = malloc(sizeof(SampleData));
  sscanf(line, "%zu:%i:%s\n", timestamp, &((*ret)->data_), (*ret)->type_);
  // eat the trailing ":"
  (*ret)->type_[strlen((*ret)->type_) - 1] = '\0';
}

void *wearable_processor_thread(void *args) {
  thread_data *td = (thread_data *)args;
  int socketfd = td->fd;
  // Use a buffer of length 64!
  // TODO read data from the socket until -1 is returned by read
  // char buffer[64];
  // while (read(socketfd, buffer, 64) > 0) ... // or do you need recv???

  char buffer[64];
  while(recv(socketfd, buffer, 64, MSG_ERRQUEUE) > 0){
		SampleData* sample;
		extract_key(buffer, &(td->timestamp), &sample);

		pthread_mutex_lock(&queue_lock_);
		pthread_cond_signal(&cv);
		queue_insert(&receieved_data_, td->timestamp, (void*)sample);
		pthread_mutex_unlock(&queue_lock_);

    //printf("buffer: %s\n", buffer);
  }

	td->done = 1;
	pthread_cond_broadcast(&cv);

  close(socketfd);
  return NULL;
}

void *user_request_thread(void *args) {
  int socketfd = *((int *)args);

  // TODO read data from the socket until -1 is returned by read
  // Requests will be in the form
  //<timestamp1>:<timestamp2>, then write out statistics for data between
  // those timestamp ranges

	char buffer[64];
	int start, end;
	int gather_size1;
  int gather_size2;
  int gather_size3;

	while(read(socketfd, buffer, 64) > 0){
		sscanf(buffer, "%d:%d", &start, &end);
		
		pthread_mutex_lock(&m);
		for(int i = 0; i < wearable_threads_size; i++){
			while(wearable_threads[i]->done == 0 && wearable_threads[i]->timestamp < end){
				pthread_cond_wait(&cv, &m);
        printf("thread waiting...\n");
			}
		}
		pthread_mutex_unlock(&m);

		timestamp_entry* queue_result1 = queue_gather(&receieved_data_, start, end, select1, &gather_size1);
		timestamp_entry* queue_result2 = queue_gather(&receieved_data_, start, end, select2, &gather_size2);
		timestamp_entry* queue_result3 = queue_gather(&receieved_data_, start, end, select3, &gather_size3);

		write_results(socketfd, TYPE1, queue_result1, gather_size1);
		write_results(socketfd, TYPE2, queue_result2, gather_size2);
		write_results(socketfd, TYPE3, queue_result3, gather_size3);

		free(queue_result1);
		free(queue_result2);
		free(queue_result3);

		write(socketfd, "\r\n", 2);
    strcpy(buffer, "");
	}
  close(socketfd);
  return NULL;
}

// IMPLEMENT!
// given a string with the port value, set up a
// serversocket file descriptor and return it
int open_server_socket(const char *port) {
  // TODO
 	struct addrinfo hints;
 	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0){
		fprintf(stderr, "socket failed\n");
		exit(1);
	}
	int optval = 1;
  int optval2 = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval2, sizeof(int));

	int error = getaddrinfo(NULL, port, &hints, &result);
  //printf("error: %d\n", error);
	if(error != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
		exit(1);
	}

	error = bind(sock_fd, result->ai_addr, result->ai_addrlen);
	if(error != 0){
		perror("bind()");
		exit(1);
	}

	error = listen(sock_fd, 100);
	if(error != 0){
		perror("listen()");
		exit(1);
	}

	struct sockaddr_in *result_addr = (struct sockaddr_in *) result->ai_addr;
	printf("Listening on file descriptor %d, port %d\n", sock_fd, ntohs(result_addr->sin_port));
  //printf("sock_fd: %d\n", sock_fd);
  return sock_fd;
}

void signal_received(int sig) {
  // TODO close server socket, free anything you don't free in main
	end_program = 1;
	close(wearable_server_fd);
  freeaddrinfo(result);
}

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    printf("Invalid input size\n");
    exit(EXIT_FAILURE);
  }
  // TODO setup sig handler for SIGINT
  signal(SIGINT, signal_received); 

  int request_server_fd = open_server_socket(argv[2]);
  wearable_server_fd = open_server_socket(argv[1]);

  pthread_t request_thread;
  int request_socket = accept(request_server_fd, NULL, NULL);
  pthread_create(&request_thread, NULL, user_request_thread, &request_socket);
  close(request_server_fd);

  queue_init(&receieved_data_);
  pthread_mutex_init(&queue_lock_, NULL);
  // TODO accept continous requests
  // TODO join all threads we spawned from the wearables
	wearable_threads = malloc(sizeof(void*)*100);
	int max_size = 100;
	wearable_threads_size = 0;
  end_program = 0;
  while(!end_program){
    //printf("before accept wearable_server_fd: %d\n", wearable_server_fd);
		int fd = accept(wearable_server_fd, NULL, NULL);
		//should i check for failed accpet??
    //printf("passing accept\n");

    //resize if needed
		if(max_size <= wearable_threads_size){
			max_size = max_size * 2;
			wearable_threads = realloc(wearable_threads, sizeof(void*)* max_size);	
		}
		wearable_threads[wearable_threads_size] = malloc(sizeof(thread_data));
		wearable_threads[wearable_threads_size]->fd = fd;
		wearable_threads[wearable_threads_size]->done = 0;
		pthread_create(&wearable_threads[wearable_threads_size]->thread, NULL, wearable_processor_thread, (void*)wearable_threads[wearable_threads_size]);
    pthread_mutex_lock(&m);
		wearable_threads_size++;	
    pthread_mutex_unlock(&m);
	}
  //join all threads and free mem
	for(int i = 0; i < wearable_threads_size; i++){
		pthread_join(wearable_threads[i]->thread, NULL);
		free(wearable_threads[i]);
	}
	free(wearable_threads);
  pthread_join(request_thread, NULL);
  queue_destroy(&receieved_data_, 1);

  return 0;
}
