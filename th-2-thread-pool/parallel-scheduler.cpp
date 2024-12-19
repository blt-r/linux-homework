#include <cstddef>

#include <pthread.h>

#include "parallel-scehduler.h"

void *ThreadPool::worker_thread(void *untyped_args) {

  ThreadPool *pool = (ThreadPool *)untyped_args;

  pthread_mutex_lock(&pool->mutex);

  while (true) {

    // wait for main to tell us to either do a task or stop
    while (pool->tasks.empty() && !pool->stop) {
      pthread_cond_wait(&pool->wake_workers, &pool->mutex);
    }

    if (pool->tasks.empty() && pool->stop) {
      break;
    }

    QueueTask task = pool->tasks.front();
    pool->tasks.pop();

    pool->threads_working += 1;
    pthread_mutex_unlock(&pool->mutex);
    task.func(task.args);
    pthread_mutex_lock(&pool->mutex);
    pool->threads_working -= 1;

    // tell main we are done with the task
    pthread_cond_signal(&pool->wake_main);
  }

  pool->threads_alive -= 1;
  pthread_cond_signal(&pool->wake_main);
  pthread_mutex_unlock(&pool->mutex);

  return nullptr;
}

ThreadPool::ThreadPool(size_t thread_count)
    : threads_alive(thread_count), threads_working(0), stop(false) {
  pthread_mutex_init(&mutex, nullptr);
  pthread_cond_init(&wake_workers, nullptr);
  pthread_cond_init(&wake_main, nullptr);

  for (size_t i = 0; i < thread_count; ++i) {
    pthread_t thread;
    pthread_create(&thread, nullptr, ThreadPool::worker_thread, this);
    pthread_detach(thread); // threads will never be joined
  }
}

ThreadPool::~ThreadPool() {
  pthread_mutex_lock(&mutex);

  // wait for all tasks to complete
  while (!tasks.empty() || threads_working != 0) {
    pthread_cond_wait(&wake_main, &mutex);
  }

  stop = true;

  // wake all threads so they know they need to stop
  pthread_cond_broadcast(&wake_workers);

  // wait for all threads to stop
  while (threads_alive != 0) {
    pthread_cond_wait(&wake_main, &mutex);
  }

  pthread_cond_destroy(&wake_workers);
  pthread_cond_destroy(&wake_main);

  pthread_mutex_destroy(&mutex);
}

void ThreadPool::add_task(task_fn_t task, void *args) {
  pthread_mutex_lock(&mutex);
  tasks.push(QueueTask(task, args));
  pthread_cond_signal(&wake_workers);
  pthread_mutex_unlock(&mutex);
}

void ThreadPool::wait_all() {
  pthread_mutex_lock(&mutex);
  while (!tasks.empty() || threads_working != 0) {
    pthread_cond_wait(&wake_main, &mutex);
  }
  pthread_mutex_unlock(&mutex);
}
