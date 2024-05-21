#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
//#include <cstdlib> 
//#include <ctime>   

// variable declarations
int buffer_length;
int number_of_producers;
int number_of_consumers;
int number_of_items;

char rand_char() {
	// generate a random character from a to z or A to Z.

	// Generate a random number between 0 and 25 (inclusive)
	int random_num = rand() % 26;

	// Return the corresponding uppercase letter
	return 'A' + random_num;
}

// monitor struct definition
typedef struct{
	sem_t mutex;
	sem_t signal; // urgent queue in the zyBook example
	int num_signal; // number of processes/threads in the urgent queue
} monitor_t;

monitor_t mon;
// condition variable struct definition
typedef struct{
	sem_t queue;
	int waiting_threads;
} cv_t;

cv_t empty, full;
int count = 0, tot_count = 0;
char* buf;
void monitor_init() {
  	// sem_init();
  	sem_init(&mon.mutex, 0, 1);
  	sem_init(&mon.signal, 0, 0);
  	mon.num_signal = 0;
  	buf=(char*) malloc(buffer_length*sizeof(char));

}

void cv_init(cv_t *cv) {
	sem_init(&(cv->queue), 0, 0);
   	cv->waiting_threads = 0;
}

void wait(cv_t *cv){
	// give up exclusive access to monitor
	//      and suspend appropriate thread
	// implement either Hoare or Mesa Paradigm
	cv->waiting_threads++; 
	if(mon.num_signal>0)
		sem_post(&mon.signal);
  	else
    		sem_post(&mon.mutex);
  
  	sem_wait(&cv->queue);
  	cv->waiting_threads--;
}

void signal(cv_t *cv){
	// unblock suspended thread at head of queue
	// implement either Hoare or Mesa paradigm
	if (cv->waiting_threads > 0) {
		mon.num_signal++;
		sem_post(&cv->queue);
		sem_wait(&mon.signal);
		mon.num_signal--;
  	}
}

void mon_insert(char item){
	buf[count++] = item;
}

char mon_remove() {
	return buf[--count];
}

void produce(char alpha) {
  	pthread_t tid = pthread_self();
	printf("p:<%lu>, item: %c, at %d\n", (unsigned long)tid, alpha, count);
	mon_insert(alpha);
	tot_count++;
}

void consume() {
	if((count == 0) && tot_count >= number_of_items){
		  printf("1\n");
		  
	}
  
  	char result;
  
  	//printf("Total Count: %d Count: %d\n", tot_count, count);
  	result = mon_remove();
  	pthread_t tid = pthread_self();
	printf("c:<%lu>, item: %c, at %d\n", (unsigned long)tid, result, count);
  
  
}

void *producer() {
	char alpha;
  	while(1){
    		sem_wait(&mon.mutex);
    		//printf("Total Count: %d Count: %d no_of_items: %d\n", tot_count, count, number_of_items);
    		if(tot_count == number_of_items){
			if(mon.num_signal>0){
      				sem_post(&mon.signal);
      			}
      			else{
      				sem_post(&mon.mutex);}
		  		pthread_exit(0);
	  		}
    			while(count == buffer_length)
      			wait(&full);
    			alpha = rand_char();
    
    			if(tot_count<number_of_items && count < buffer_length)
      				produce(alpha);
    			signal(&empty);
    
    			
    			if(mon.num_signal>0) 
      				sem_post(&mon.signal);
    			else
      				sem_post(&mon.mutex);

  	}
}

void *consumer() {
  	char result;
  	while(1){
    		sem_wait(&mon.mutex);
    		if((count == 0) && tot_count >= number_of_items){
			exit(0);
	  	}

    		while(count == 0)
      			wait(&empty);

    		if(count>0)
     			consume();
    
    		signal(&full);
    		if(mon.num_signal>0) 
      			sem_post(&mon.signal);
    		else
      			sem_post(&mon.mutex);
   
  	}
}



int main(int argc, char *argv[]) {
  	// similar to the one in part1_suggested.c
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
		
	monitor_init();
    	cv_init(&empty);
    	cv_init(&full);
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
