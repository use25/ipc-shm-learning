#pragma once

// TODO: define the struct that will live IN shared memory.
//       Keep it POD/trivially-copyable - no std::string, no std::vector,
//       nothing with a pointer that's only valid in one process's
//       address space. Think about why that last part matters.

namespace MyConfig
{
struct SharedData
{
    int value;
    bool ready;
};

// TODO: pick names for the shared memory object and the named semaphore.
//       POSIX convention: leading slash, e.g. "/my_shm_demo"
constexpr const char* SHM_NAME = "/my_shm";
constexpr const char* SEM_NAME = "/my_sem";
} // namespace MyConfig
