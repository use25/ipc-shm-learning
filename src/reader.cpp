#include "shm_layout.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    // TODO: shm_open(SHM_NAME, O_RDONLY, 0) - no O_CREAT, this process
    //       doesn't own creation
    int fd = shm_open(MyConfig::SHM_NAME, O_RDONLY, 0);
    if (fd < 0)
    {
        perror("shm_open");
        return 1;
    }
    // TODO: mmap() it - note the protection flags should differ from
    //       writer's (read-only here), think about what happens if you
    //       just copy-paste writer's flags
    MyConfig::SharedData* mySharedDataFromWriter = static_cast<MyConfig::SharedData*>(
        mmap(nullptr, sizeof(MyConfig::SharedData), PROT_READ, MAP_SHARED, fd, 0)
    );
    if (mySharedDataFromWriter == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }
    // TODO: sem_open(SEM_NAME, 0) - again no O_CREAT
    sem_t* mySemData = sem_open(MyConfig::SEM_NAME, 0);
    if (mySemData == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }
    // TODO: sem_wait() - this is the actual IPC synchronization moment,
    //       compare mentally to your Project 2 counting_semaphore.acquire()
    sem_wait(mySemData);
    // TODO: read from the mapped SharedData, print it
    printf("My SharedData: %d, %d\n", mySharedDataFromWriter->ready, mySharedDataFromWriter->value);

    // TODO: munmap, close (no unlink here - see writer's note above)
    munmap(mySharedDataFromWriter, sizeof(MyConfig::SharedData));
    shm_unlink(MyConfig::SHM_NAME);
    sem_close(mySemData);
    sem_unlink(MyConfig::SEM_NAME);
    close(fd);

    return 0;
}