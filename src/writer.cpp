#include "shm_layout.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>

// TODO: includes: <fcntl.h> for O_* flags, <sys/mman.h> for shm_open/mmap,
//       <semaphore.h> for sem_open, <unistd.h> for ftruncate/close

int main()
{
    // TODO: shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)
    int fd = shm_open(MyConfig::SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0)
    {
        perror("shm_open");
        return 1;
    }
    // TODO: ftruncate() the fd to sizeof(SharedData) - shm_open alone
    //       gives you a 0-byte object, think about why you'd forget this
    //       and what happens if you do
    ftruncate(fd, sizeof(MyConfig::SharedData));
    // TODO: mmap() the fd into this process's address space
    MyConfig::SharedData* mySharedData = static_cast<MyConfig::SharedData*>(
        mmap(nullptr, sizeof(MyConfig::SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x0)
    );

    if (mySharedData == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }
    // TODO: sem_open(SEM_NAME, O_CREAT, 0666, 0) - note initial value 0.
    //       Think about why 0 and not 1 here, given writer goes first
    //       and reader should wait for data to actually exist
    sem_t* mySem = sem_open(MyConfig::SEM_NAME, O_CREAT, 0666, 0);
    if (mySem == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }

    // TODO: write something into the mapped SharedData
    mySharedData->value = 123;
    mySharedData->ready = true;

    // TODO: sem_post() to signal the reader that data is ready
    sem_post(mySem);

    // TODO: cleanup - munmap, close, and *one* of the two processes
    //       (decide which, and why) should sem_unlink/shm_unlink so
    //       the OS doesn't leak these persistent objects across runs
    munmap(mySharedData, sizeof(MyConfig::SharedData));
    sem_close(mySem);
    close(fd);

    return 0;
}