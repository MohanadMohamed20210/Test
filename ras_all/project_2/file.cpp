#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <vector>
#include <memory>
#include <condition_variable>
#include "kayn_1.h"
#include "file1.h"

bool flag = true;

class Command
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};
class Audio
{
private:
    int pin0;
    int pin1;
    int pin2;
    int pin3;
    int pin4;
    int pin5;
    int pin6;
    int pin7;
    std::vector<unsigned char> audio;
public:
    Audio(std::vector<unsigned char>& audio)
    {
        pin7 = 8;
        pin6 = 9;
        pin5 = 30;
        pin4 = 21;
        pin3 = 22;
        pin2 = 23;
        pin1 = 24;
        pin0 = 25;
        pinMode(pin0,OUTPUT);
        pinMode(pin1,OUTPUT);
        pinMode(pin2,OUTPUT);
        pinMode(pin3,OUTPUT);
        pinMode(pin4,OUTPUT);
        pinMode(pin5,OUTPUT);
        pinMode(pin6,OUTPUT);
        pinMode(pin7,OUTPUT);
        this->audio = audio;
    }
    void StartAudioSystem()
    {
        for (int i = 0; i < audio.size() ; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int value = ((audio[i]>>j)&1);
                switch(j)
                {
                    case (0) : digitalWrite(pin0,value); break;
                    case (1) : digitalWrite(pin1,value); break;
                    case (2) : digitalWrite(pin2,value); break;
                    case (3) : digitalWrite(pin3,value); break;
                    case (4) : digitalWrite(pin4,value); break;
                    case (5) : digitalWrite(pin5,value); break;
                    case (6) : digitalWrite(pin6,value); break;
                    case (7) : digitalWrite(pin7,value); break;
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(60));
        }
    }
    void StopSystem()
    {
        digitalWrite(pin0,LOW);
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,LOW);
        digitalWrite(pin3,LOW);
        digitalWrite(pin4,LOW);
        digitalWrite(pin5,LOW);
        digitalWrite(pin6,LOW);
        digitalWrite(pin7,LOW);
    }
};
class CommandConcrete : public Command
{
    private:
        std::shared_ptr<Audio> audio;
    public:
        CommandConcrete(std::shared_ptr<Audio> cp_audio):audio(cp_audio){}
        void execute()override
        {
            audio->StartAudioSystem();
        }
        void undo()
        {
            audio->StopSystem();
        }
};
void System_Handle()
{
    std::cout<<"Enter '0' to close the system => ";
    std::cin>>flag;
}
void System(std::shared_ptr<Command> command)
{
    while(flag)
    {
        command->execute();
    }
}
int main()
{
    wiringPiSetup();
    std::shared_ptr<Audio> audio = std::make_shared<Audio>(Kayn_1);
    std::shared_ptr<Command> command = std::make_shared<CommandConcrete>(audio);
    std::thread T1(System,command);
    std::thread T2(System_Handle);

    T1.join();
    T2.join();
    command->undo();
    return 0;
}