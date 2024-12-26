#include <cstddef>
#include <queue>
#include <vector>

#include <pthread.h>

typedef void (*task_fn_t)(void*);

struct QueueTask {
    task_fn_t func;
    void* args;
};

class ThreadPool {
  public:
    ThreadPool(size_t);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;  // just create a new thread pool
    ThreadPool(ThreadPool&&) = delete;  // threads store pointers to the pool

    void add_task(task_fn_t task, void* args);
    void wait_all();

  private:
    std::queue<QueueTask> tasks;
    pthread_mutex_t mutex;
    pthread_cond_t
        wake_workers;          // main thread notifies workers to work or stop
    pthread_cond_t wake_main;  // workers notify main that they are finished
                               // working or stopped
    size_t threads_alive;
    size_t threads_working;
    bool stop;

    static void* worker_thread(void*);
};
