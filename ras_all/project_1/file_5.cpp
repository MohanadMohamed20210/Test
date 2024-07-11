#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <csignal>

int pin;

void signal_handle(int SIG)
{
    pinMode(pin,OUTPUT);
    digitalWrite(pin,LOW);
    exit(0);
}
class PWM
{
    private:
        int baseClock;
        int clockRange;
        int freq;
        int PWM_Clock;
        double duty_cycle;

    public:
        PWM()
        {
            baseClock = 19200000;
            clockRange = 1024;
            pinMode(pin,PWM_OUTPUT);
            pwmSetMode(PWM_MODE_MS);
            pwmSetRange(clockRange);
        }
        void CalcPwmClock(int cp_freq)
        {
            freq = cp_freq;
            PWM_Clock = baseClock / (freq * clockRange);
            pwmSetClock(PWM_Clock);
        }

        void CalcDutyCycle(double duty)
        {
            duty_cycle = (int)(clockRange * duty);
        }

        void Start_PWM()
        {
            double delay = (double)(1/freq);
            while(true)
            {
                pwmWrite(pin,duty_cycle);
                std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 * delay)));
            }
        }
};
int main()
{
    wiringPiSetup();
    signal(SIGINT,signal_handle);
    int freq;
    double duty;

    std::cout<<"Enter pin => ";
    std::cin>>pin;

    std::cout<<"Enter frequency => ";
    std::cin>>freq;

    std::cout<<"Enter duty cycle => ";
    std::cin>>duty;

    PWM* pwm = new PWM();
    pwm->CalcPwmClock(freq);
    pwm->CalcDutyCycle(duty);

    std::thread T1(&PWM::Start_PWM,pwm);
    T1.join();
    return 0;
}