#include <iostream>
#include <mutex>
#include "pthread.h"

// identify issues of running threads and propose your solution.

using namespace std;

int counter;
mutex counter_mtx;
pthread_rwlock_t rwlock;

void *thd_func1 (void *arg) {
    counter_mtx.lock();
    pthread_rwlock_rdlock(&rwlock);
    counter++;
    pthread_rwlock_unlock(&rwlock);
    counter_mtx.unlock();
    return NULL;
}

void *thd_func2 (void *arg) {
    counter_mtx.lock();
    pthread_rwlock_wrlock(&rwlock);
    counter--;
    pthread_rwlock_unlock(&rwlock);
    counter_mtx.unlock();
    return NULL;
}


int main()
{
    pthread_rwlock_init(&rwlock ,NULL);
    pthread_t thd[2];
    pthread_create(&thd[0], NULL, thd_func1, NULL);
    pthread_create(&thd[1], NULL, thd_func2, NULL);
    for (int i = 0; i < 2; ++i) {
        pthread_join(thd[i], NULL);
    }

    return 0;
}
