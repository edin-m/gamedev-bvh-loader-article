#ifndef THREAD_H
#define THREAD_H

#include <vector>

#include <pthread.h>
#include <unistd.h>

#include "logger.h"


class Thread
{
    typedef void* (*callback)(void*);

    callback function;
    pthread_t thread;


public:
    Thread(callback _f) : function(_f) {}

    void start()
    {
        pthread_create(&thread, NULL, function, NULL);
    }

};

#endif // THREAD_H
