#include <iostream>
#include <pthread.h>
#include <vector>
#include <queue>
#include "semaphore.h"
#include <unistd.h>

/**
 * My func to get random number.
 * @return random number.
 */
unsigned int myRandom() {
    srand(time(nullptr));
    static unsigned int value = rand();
    value = 8253729 * value + 2396403;
    return value % 32768;
}

/**
 * Mutex to make uninterrupted output.
 */
pthread_mutex_t open_write;

/**
 * Struct of clients.
 */
struct Customer {
    int id = 0;
    unsigned int buying = myRandom() % 30;
};

// Queue, semaphore, mutex for first worker.
std::queue<sem_t*> first_queue;
sem_t first_cashier_work;
pthread_mutex_t open_first_queue;

// Queue, semaphore, mutex for second worker.
std::queue<sem_t*> second_queue;
sem_t second_cashier_work;
pthread_mutex_t open_second_queue;

/**
 * Struct to send client in thread.
 */
typedef struct oneArgs_tag {
    Customer client;
} oneArgs_t;

/**
 * Function to complete client way in shop.
 * @param args Form to take client.
 * @return nullptr (to no warning).
 */
void* clientWay(void* args) {
    auto *arg = (oneArgs_t*) args;
    auto* sem_cur = new sem_t;
    sem_init(sem_cur, 0, 0);
    usleep(arg->client.buying * 2000); // Time when client choose.
    pthread_mutex_lock(&open_write); // For uninterrupted.
    std::cout << "Customer " << arg->client.id << " finish choose and go to pay." << "\n";
    pthread_mutex_unlock(&open_write);
    // After buying send client into queue.
    unsigned int queue_num = myRandom() % 2;
    if (queue_num == 0) {
        pthread_mutex_lock(&open_first_queue); // For uninterrupted.
        first_queue.push(sem_cur);
        pthread_mutex_unlock(&open_first_queue);
        sem_post(&first_cashier_work);
    } else {
        pthread_mutex_lock(&open_second_queue); // For uninterrupted.
        second_queue.push(sem_cur);
        pthread_mutex_unlock(&open_second_queue);
        sem_post(&second_cashier_work);
    }
    sem_wait(sem_cur); // Wait finish worker.
    // After served give finish information.
    pthread_mutex_lock(&open_write); // For uninterrupted.
    std::cout << "Cashier " << queue_num + 1 << " has served the Customer " << arg->client.id << "." << "\n";
    pthread_mutex_unlock(&open_write);
    sem_destroy(sem_cur);
    return nullptr;
}

/**
 * Function to work first worker.
 * @param args nothing.
 * @return nullptr (to no warning).
 */
void* serveFirst([[maybe_unused]] void* args) {
    sem_wait(&first_cashier_work);
    while (!first_queue.empty()) {
        pthread_mutex_lock(&open_first_queue); // For uninterrupted.
        auto sem = first_queue.front();
        first_queue.pop();
        pthread_mutex_unlock(&open_first_queue);
        usleep(myRandom());
        sem_post(sem); // Free client.
        sem_wait(&first_cashier_work); // Wait next client.
    }
    return nullptr;
}

/**
 * Function to work second worker.
 * @param args nothing.
 * @return nullptr (to no warning).
 */
void* serveSecond([[maybe_unused]] void* args) {
    sem_wait(&second_cashier_work);
    while (!second_queue.empty()) {
        pthread_mutex_lock(&open_second_queue); // For uninterrupted.
        auto sem = second_queue.front();
        second_queue.pop();
        pthread_mutex_unlock(&open_second_queue);
        usleep(myRandom());
        sem_post(sem); // Free client.
        sem_wait(&second_cashier_work); // Wait next client.
    }
    return nullptr;
}

/**
 * main -\/-.
 * @return 0.
 */
int main() {
    // Initialization mutex and semaphores.
    pthread_mutex_init(&open_write, nullptr);
    pthread_mutex_init(&open_first_queue, nullptr);
    pthread_mutex_init(&open_second_queue, nullptr);
    sem_init(&second_cashier_work, 0, 0);
    sem_init(&first_cashier_work, 0, 0);
    // Get clients count.
    int clients_num;
    do {
        std::cout << "Please, input the number of customers:\n";
        std::cin >> clients_num;
    } while (clients_num <= 0);
    // Create threads.
    std::vector<oneArgs_t> args(clients_num);
    pthread_t threads[clients_num];
    pthread_t thread_cushier_1;
    pthread_create(&thread_cushier_1, nullptr, serveFirst, nullptr);
    pthread_t thread_cushier_2;
    pthread_create(&thread_cushier_2, nullptr, serveSecond, nullptr);
    // Send clients into the shop.
    for (int i = 0; i < clients_num; ++i) {
        Customer client;
        client.id = i + 1;
        args[i].client = client;
        pthread_create(&threads[i], nullptr, clientWay, (void *) &args[i]);
    }
    // Say finish to work.
    sleep(2);
    sem_post(&first_cashier_work);
    sem_post(&second_cashier_work);
    pthread_join(thread_cushier_1, nullptr);
    pthread_join(thread_cushier_2, nullptr);
    // Destroy mutex and semaphores.
    sem_destroy(&first_cashier_work);
    sem_destroy(&second_cashier_work);
    pthread_mutex_destroy(&open_write);
    pthread_mutex_destroy(&open_first_queue);
    pthread_mutex_destroy(&open_second_queue);
    std::cout << "Shop is closed!";
    return 0;
}