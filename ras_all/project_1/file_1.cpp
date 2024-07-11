#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mux;
std::condition_variable cond;
bool flag = true;

class LedControl
{
    private:
        int Led_Pin;
    public:
        LedControl(int cp_pin)
        {
            Led_Pin = cp_pin;
            wiringPiSetup();
            pinMode(Led_Pin,OUTPUT);
            digitalWrite(Led_Pin,LOW);
        }
        void LedOn()
        {
            digitalWrite(Led_Pin,HIGH);
        }
        void LedOff()
        {
            digitalWrite(Led_Pin,LOW);
        }
        void Plink(int time,int stay_time)
        {
            int start = millis();
            while(true)
            {
                digitalWrite(Led_Pin,HIGH);
                std::this_thread::sleep_for(std::chrono::seconds(time));
                digitalWrite(Led_Pin,LOW);
                std::this_thread::sleep_for(std::chrono::seconds(time));
                int end = millis();
                int elapsed = end - start;
                if(elapsed >= (stay_time*1000))
                {
                    break;
                }
            }
        }
};
void System(LedControl& Led)
{
    std::string choice;
    int plink,stay_time;
    while(flag)
    {
        std::cout<<"---------------------------\n"
                "Enter 'on' to turn on led\n"
                "Enter 'off' to tuen off led\n"
                "Enter 'plink' to plink led\n";
                "==> ";
        std::cin>>choice;
        if(choice == "on")
        {
            Led.LedOn();
        }
        else if(choice == "off")
        {
            Led.LedOff();
        }
        else if(choice == "plink")
        {
            std::cout<<"Enter plink time in seconds => ";
            std::cin>>plink;
            std::cout<<"Enter plink time stayed in seconds => ";
            std::cin>>stay_time;
            Led.Plink(plink,stay_time);
        }
        else if(choice == "exit")
        {
            Led.LedOff();
            flag = false;
            cond.notify_all();
        }
        else
        {
            std::cout<<"Sorry kid >_<.... Invalid choice T_T..."<<std::endl;
        }
    }
}
int main()
{
    int pin_number;
    std::cout<<"Enter the pin number => ";
    std::cin>>pin_number;
    LedControl led(pin_number);
    std::thread T1(System,std::ref(led));

    std::unique_lock<std::mutex> guard(mux);
    cond.wait(guard,[]{return !flag;});

    T1.join();
    std::cout<<"Thread has been terminated successfully....."<<std::endl;
    return 0;
}