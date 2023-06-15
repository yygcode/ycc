#include <pthread.h>
#include <stdint.h>

int v;

void spin_lock(int *lock)
{
    __atomic_thread_fence(__ATOMIC_SEQ_CST);

    while (!__sync_bool_compare_and_swap(&v, 0, 1));
}

void spin_unlock(int *lock)
{
    (void)__sync_sub_and_fetch(&v, 1);
}

const uint64_t LC = 10000 * 1000UL * 10;
uint64_t i = 0;

void *thread_func()
{
    for (uint32_t i = 0; i < LC; ++i)
    {
        spin_lock(&v);
        ++i;
        spin_unlock(&v);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    for (uint32_t i = 0; i < LC; ++i)
    {
        spin_lock(&v);
        ++i;
        spin_unlock(&v);
    }
    if (i != LC * 2)
    {
        return 1;
    }
    return 0;
}
