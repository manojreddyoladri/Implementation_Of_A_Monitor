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
  
  

