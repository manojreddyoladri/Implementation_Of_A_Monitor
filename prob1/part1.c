#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>

// variable declarations
int buffer_length;
int number_of_producers;
int number_of_consumers;
int number_of_items;


// buffer struct definition
typedef struct {
	char *items;
	int total_count;
	int current_count;
	sem_t mutex, empty, full;
} buffer_t;

buffer_t buffer;

/*
  empty semaphore value buffer_length, full
  semaphore value 0, mutex semaphore value 1
*/
void init() {
	// *item may can be defined using malloc()
	buffer.items = (char*) malloc(buffer_length*sizeof(char));
	// Initialize semaphores
	sem_init(&buffer.mutex, 0, 1);  // Initialize mutex semaphore with value 1
	sem_init(&buffer.empty, 0, buffer_length); // Initialize empty semaphore with buffer_length
	sem_init(&buffer.full, 0, 0); // Initialize full semaphore with value 0
	buffer.total_count = 0;
	buffer.current_count = 0;
}

/* produce to the buffer */
int produce(buffer_t* buffer) {
	
	if(buffer->total_count == number_of_items){
		//printf("2\n");
		return 0;
	}
	else{
		buffer->items[buffer->current_count] = 'X';
		pthread_t tid = pthread_self();
		printf("p:<%lu>, item: %c, at %d\n", (unsigned long)tid, buffer->items[buffer->current_count], buffer->current_count);
		buffer->current_count++;
		buffer->total_count++;
		//printf("Buffer total count: %d\n", buffer->total_count);
	
		return 1;
	}
}

/* consume from buffer */
int consume(buffer_t* buffer) {
	//printf("%d %d\n", buffer->current_count, buffer->total_count);
	if((buffer->current_count == 0) && buffer->total_count >= number_of_items){
		//printf("1\n");
		return 0;
	}
	else{
		pthread_t tid = pthread_self();
		printf("c:<%lu>, item: %c, at %d\n", (unsigned long)tid, buffer->items[buffer->current_count-1], buffer->current_count-1);
		buffer->items[buffer->current_count-1] = '\0';
	
		buffer->current_count--;
		return 1;
	}
}

void *producer(void *arg) {
		
	while (1) {
		sem_wait(&buffer.empty);
		sem_wait(&buffer.mutex);
		int flag = produce(&buffer);
		sem_post(&buffer.mutex);
		sem_post(&buffer.full);
		if (flag == 0)
			pthread_exit(0);
	}
}

void *consumer(void *arg) {
		
	while (1) {
		sem_wait(&buffer.full);
		sem_wait(&buffer.mutex);
		int flag = consume(&buffer);
		sem_post(&buffer.mutex);
		sem_post(&buffer.empty);

		if (flag == 0)
			exit(0);
				
	}
}



int main(int argc, char *argv[]) {
	// Check argv and assign values to 
        // buffer_length;
        // number_of_producers;
        // number_of_consumers;
        // number_of_items;
        // a useful function getopt()
	int opt;
	// Parse command line options
    	while ((opt = getopt(argc, argv, "b:p:c:i:")) != -1) {
        	switch (opt) {
            		case 'b':
                		buffer_length = atoi(optarg);
                		break;
            		case 'p':
                		number_of_producers = atoi(optarg);
                		break;
            		case 'c':
                		number_of_consumers = atoi(optarg);
                		break;
            		case 'i':
                		number_of_items = atoi(optarg);
                		break;
            		default:
                		fprintf(stderr, "Usage: %s -b <buffer_length> -p <number_of_producers> -c <number_of_consumers> -i <number_of_items>\n", argv[0]);
                		exit(EXIT_FAILURE);
        	}
	}
	//printf("Buffer length: %d\n", buffer_length);
	//printf("Number of producers: %d\n", number_of_producers);
	//printf("Number of consumers: %d\n", number_of_consumers);
	//printf("Number of items: %d\n", number_of_items);
		
	init(&buffer);
     
	//sem_init(&buffer.mutex, buffer.full, buffer.empty);
	pthread_t producers[number_of_producers];
	pthread_t consumers[number_of_consumers];


	for (int i = 0; i < number_of_producers; i++)
		pthread_create(&producers[i], NULL, producer,(void*) &producers[i]);

	for (int i = 0; i < number_of_consumers; i++)
		pthread_create(&consumers[i], NULL, consumer, (void*)&consumers[i]);

	for (int i = 0; i < number_of_producers; i++)
		pthread_join(producers[i], NULL);

	for (int i = 0; i < number_of_consumers; i++)
		pthread_join(consumers[i], NULL);

	exit(EXIT_SUCCESS);
}
