#include <pthread.h>    // POSIX threads
#include <time.h>       // Random time delays
#include <unistd.h>     // sleep for specified number of seconds
#include <semaphore.h>  // Semaphores
#include <stdlib.h>
#include <stdio.h>      // Input/output
#include <stdint.h>     // For intptr_t

pthread_t *Students;    // N threads running as students
pthread_t TA;           // Separate thread for TA

int ChairsCount = 0;
int CurrentIndex = 0;
#define MAX_CHAIRS 3

// Semaphores and Mutex Lock
sem_t TA_Sleep;                  // To signal/wait TA's sleep
sem_t Chair_Sem[MAX_CHAIRS];     // To signal/wait for each chair
sem_t Student_Sem;               // To signal/wait for the TA's next student
pthread_mutex_t Chairs_Mutex;    // To lock/unlock ChairsCount
int active_students;             // Tracks number of students still needing help

// Declared Functions
void *TA_Activity(void *arg);           // Modified to accept a void* parameter
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
    int number_of_students; // Variable for number of students (default 5)
    int id;

    srand(time(NULL));

    // Initialize Mutex Lock and Semaphores
    sem_init(&TA_Sleep, 0, 0);
    sem_init(&Student_Sem, 0, 0);
    pthread_mutex_init(&Chairs_Mutex, NULL);
    for (int i = 0; i < MAX_CHAIRS; i++) {
        sem_init(&Chair_Sem[i], 0, 0);
    }

    // Get number of students from command-line or use default
    if (argc < 2) {
        printf("Number of Students not specified. Using default (5) students.\n");
        number_of_students = 5;
    } else {
        printf("Number of Students specified. Creating %d threads.\n", atoi(argv[1]));
        number_of_students = atoi(argv[1]);
    }

    active_students = number_of_students; // Initialize active students count

    // Allocate memory for students
    Students = (pthread_t*) malloc(sizeof(pthread_t) * number_of_students);

    // Create TA thread and student threads
    pthread_create(&TA, NULL, TA_Activity, NULL);
    for (id = 0; id < number_of_students; id++) {
        pthread_create(&Students[id], NULL, Student_Activity, (void*)(intptr_t)id); // Use intptr_t for portability
    }

    // Wait for all student threads to complete
    for (int i = 0; i < number_of_students; i++) {
        pthread_join(Students[i], NULL);
    }

    // Signal TA to exit once all students are done
    sem_post(&TA_Sleep);
    pthread_join(TA, NULL);

    // Free allocated memory and destroy semaphores/mutex
    free(Students);
    pthread_mutex_destroy(&Chairs_Mutex);
    sem_destroy(&TA_Sleep);
    sem_destroy(&Student_Sem);
    for (int i = 0; i < MAX_CHAIRS; i++) {
        sem_destroy(&Chair_Sem[i]);
    }

    return 0;
}

void *TA_Activity(void *arg) {       // Now takes a void* argument
    while (1) {
        // TA is currently sleeping until a student wakes them up
        sem_wait(&TA_Sleep);

        // Check for waiting students
        pthread_mutex_lock(&Chairs_Mutex);
        if (active_students == 0) {  // Exit condition for TA
            pthread_mutex_unlock(&Chairs_Mutex);
            break;                   // Exit the TA loop
        }
        pthread_mutex_unlock(&Chairs_Mutex);

        while (1) {
            pthread_mutex_lock(&Chairs_Mutex);
            if (ChairsCount == 0) {
                pthread_mutex_unlock(&Chairs_Mutex);
                break; // No more students waiting, go back to sleep
            }

            // Get next student
            sem_post(&Chair_Sem[CurrentIndex]);
            ChairsCount--;
            CurrentIndex = (CurrentIndex + 1) % MAX_CHAIRS;
            pthread_mutex_unlock(&Chairs_Mutex);

            // Help the student
            printf("TA is helping a student.\n");
            sleep(rand() % 3 + 1); // Simulate help time
            sem_post(&Student_Sem);
        }
    }
    printf("TA has finished all sessions and is exiting.\n");
    return NULL;
}

void *Student_Activity(void *threadID) {
    intptr_t id = (intptr_t)threadID;  // Use intptr_t for portability

    while (1) {
        printf("Student %ld needs help.\n", id);
        pthread_mutex_lock(&Chairs_Mutex);

        if (ChairsCount < MAX_CHAIRS) {
            // Take a seat and notify TA
            int chair_index = (CurrentIndex + ChairsCount) % MAX_CHAIRS;
            ChairsCount++;
            printf("Student %ld is sitting on chair %d. Waiting for the TA.\n", id, chair_index);
            pthread_mutex_unlock(&Chairs_Mutex);

            // Wake up the TA if they are sleeping
            sem_post(&TA_Sleep);

            // Wait for TA's help
            sem_wait(&Chair_Sem[chair_index]);
            printf("Student %ld is getting help from the TA.\n", id);
            sem_wait(&Student_Sem);

            printf("Student %ld left TA room.\n", id);

            // Decrement active students count after receiving help
            pthread_mutex_lock(&Chairs_Mutex);
            active_students--;
            pthread_mutex_unlock(&Chairs_Mutex);

            break;
        } else {
            // No chairs available; come back later
            printf("Student %ld found no chair available, will come back later.\n", id);
            pthread_mutex_unlock(&Chairs_Mutex);
            sleep(rand() % 3 + 1); // Retry after random delay
        }
    }

    return NULL;
}
