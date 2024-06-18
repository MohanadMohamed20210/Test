#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

int data;
std::mutex writeGuard;
std::condition_variable cond;
void Write()
{
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    std::unique_lock<std::mutex> write_guard(writeGuard);
    std::cout<<"Write on the data....."<<std::endl;
    data = 10;
    cond.notify_one();
}
void Read()
{
    std::unique_lock<std::mutex> write_guard(writeGuard);
    std::cout<<"Wait for data to get modified..."<<std::endl;
    cond.wait(write_guard);
    std::cout<<"Data after modification => "<<data<<std::endl;
}
int main()
{
    std::thread write(Write);
    std::thread read(Read);
    write.join();
    read.join();
    return 0;
}