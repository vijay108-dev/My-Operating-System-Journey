// Producer–Consumer Problem 
// The Producer–Consumer problem is a classic Operating System problem about sharing data safely between two types of processes.
// Producer → creates data (items)
// Consumer → uses that data
// Buffer → a common storage place between them

// Example from real life
// Think of a kitchen 
// Cook (Producer) makes food
// Waiter (Consumer) serves food
// Food counter (Buffer) holds prepared dishes

// Rules:
// Cook cannot put food if the counter is full
// Waiter cannot take food if the counter is empty
// Both must work in coordination, not at the same time on the same dish

// In computer terms
// Producer puts data into a shared buffer
// Consumer takes data from the same buffer

// Problem arises when:
// Producer writes when buffer is full 
// Consumer reads when buffer is empty 
// Both access buffer at same time (data corruption) 
// Why this problem exists
// Because of synchronization issues in multitasking systems.We do not want race condition occurs.

// How OS solves it (conceptually)
// Using:
// Semaphore 
//1. Mutex -> to allow only one access at a time.Binary semphores used to acquire lock on buffer.
//2.empty -> A Counting semaphore intial value is n track empty slots
//3.fill -> tracks filled slots intial =0. 

// Producer–Consumer problem is a synchronization problem where producer and consumer share a common buffer and must coordinate to avoid conflict.


#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in=0;
int out=0;

sem_t emptySlots; // how many empty spaces
sem_t fullSlots;  // how many filled spaces
mutex mtx; // mutex for critical section
void producer(){
    int item=1;
    while (true) {
        sem_wait(&emptySlots);// wait until empty > 0
        mtx.lock();// enter critical section

        buffer[in]=item;
        cout<<"Producer produced: "<<item<<endl;
        in=(in+1)%BUFFER_SIZE;
        item++;

        mtx.unlock(); // exit critical section
        sem_post(&fullSlots);// increment full

        sleep(1);
    }
}

void consumer() {
    while (true) {
        sem_wait(&fullSlots);// wait until full > 0
        mtx.lock();// enter critical section

        int item = buffer[out];
        cout << "Consumer consumed: " << item << endl;
        out = (out + 1) % BUFFER_SIZE;

        mtx.unlock();// exit critical section
        sem_post(&emptySlots);// increment empty

        sleep(2);
    }
}

int main(){
    sem_init(&emptySlots,0,BUFFER_SIZE);
    sem_init(&fullSlots,0,0);
    thread p(producer);
    thread c(consumer);

    p.join();
    c.join();

    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);

    return 0;
}
