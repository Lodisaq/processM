#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define BUFF_SIZE 10
char buffer[BUFF_SIZE];
char count;        // 缓冲池里的信息数目
sem_t sem_mutex;   // 生产者和消费者的互斥锁
sem_t p_sem_mutex; // 空的时候，对消费者不可进
sem_t c_sem_mutex; // 满的时候，对生产者不可进
void *Producer()
{
    while (1)
    {
        sem_wait(&p_sem_mutex); //当缓冲池未满时
        sem_wait(&sem_mutex);   //等待缓冲池空闲
        printf("P");
        count++;
        sem_post(&sem_mutex);
        if (count < BUFF_SIZE) //缓冲池未满
            sem_post(&p_sem_mutex);
        if (count > 0) //缓冲池不为空
            sem_post(&c_sem_mutex);
    }
}
void *Consumer()
{
    while (1)
    {
        sem_wait(&c_sem_mutex); //缓冲池未空时
        sem_wait(&sem_mutex);   //等待缓冲池空闲
        printf("C");
        count--;
        sem_post(&sem_mutex);
        if (count > 0)
            sem_post(&c_sem_mutex);
    }
}
int main()
{

    printf("hellow\n");

    pthread_t ptid, ctid;
    // initialize the semaphores
    sem_init(&p_sem_mutex, 0, 1);
    sem_init(&c_sem_mutex, 0, 0);
    // creating producer and consumer threads
    if (pthread_create(&ptid, NULL, Producer, NULL))
    {

        printf("\n ERROR creating thread 1");
    }
    if (pthread_create(&ctid, NULL, Consumer, NULL))
    {

        printf("\n ERROR creating thread 2");
    }
    if (pthread_join(ptid, NULL)) // wait for the producer to finish /
    {

        printf("\n ERROR joining thread");
    }
    if (pthread_join(ctid, NULL)) // wait for consumer to finish /
    {

        printf("\n ERROR joining thread");
    }
    sem_destroy(&p_sem_mutex);
    sem_destroy(&c_sem_mutex);
    // exit the main thread
    pthread_exit(NULL);
    return 0;
}