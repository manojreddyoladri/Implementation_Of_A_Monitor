# Implementation_Of_A_Monitor

-**Overview of the project**
The goal of this project is to familiarize myself with the synchronization constructs - mutexes, semaphores, and monitors. The goal is to ensure no deadlocks, no race conditions, and that data is shared properly between threads.

-**Part 1**
Part 1 will solve the producer/consumer problem by utilizing POSIX semaphores to solve concurrency problems.

-**Part 2**
Part 2 will solve the producer/consumer problem by developing a custom monitor.

-**Program Specification**

- Implement additional switches to specify the buffer length, number of producers and the number of consumers as indicated below to make the program easier to evaluate.
  - `-b` Buffer length in bytes
  - `-p` Number of producer threads
  - `-c` Number of consumer threads
  - `-i` Number of items to insert

- As an example `./part1 -b 1000 -p 10 -c 10 -i 100000` should create a buffer of length 1,000 bytes, 10 producer threads, 10 consumer threads, and insert 10,000 items.
- **Output Convention** Whenever an item is produced (i.e., put in the buffer), a message needs to be printed to the screen with the following (printf()) convention:

  `"p:<%u>, item: %c, at %d", threadid, item, index`

 This indicates a producer with threadid inserted item at index in the buffer. A similar message is required whenever an item is produced:

 `"c:<%u>, item: %c, at %d", threadid, item, index` indicating a consumer with `threadid` has consumed `item` at `index` in the buffer.

- **Part 1: Producer/Consumer with Semaphores**
The goal of Part 1 is to solve the producer/consumer problem using semaphores. Used the pthread library to create producer threads and consumer threads. Each producer thread inserts a single ‘X’ character into a buffer. Each consumer thread removes the most recently inserted ‘X’ from the buffer. Each thread then repeats the process. Use POSIX semaphore (`sem_init`, `sem_wait`, `sem_post`, `sem_destroy`).

- **Part 2: Producer/Consumer with Monitor**
The goal of Part 2 is to create our own monitor to provide synchronization support for the producer/consumer problem. Used the `pthread` library again to create producer threads and consumer threads. Each producer thread inserts a randomly generated character from the alphabet (upper and lower cases) into the first available slot in a buffer.
  
  

