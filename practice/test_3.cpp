#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

int Data;
std::mutex writeGuard;
bool write_complete = false;
void Write()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::unique_lock<std::mutex> write_guard(writeGuard);
    std::cout<<"write on the data..."<<std::endl;
    Data = 10;
    write_complete = true;
}
void Read()
{
    std::unique_lock<std::mutex> write_guard(writeGuard);
    std::cout<<"Hold for the write to be completed...."<<std::endl;
    while(!write_complete)
    {
        write_guard.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        write_guard.lock();
    }
    write_guard.unlock();
    std::cout<<"The data after read => "<<Data<<std::endl;
}
int main()
{
    std::thread write(Write);
    std::thread read(Read);

    write.join();
    read.join();
    return 0;
}