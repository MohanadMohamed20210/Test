#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <signal.h>
#include <fstream>

#define SEG_A_1 6
#define SEG_B_1 10
#define SEG_C_1 11
#define SEG_D_1 31
#define SEG_E_1 27
#define SEG_F_1 28
#define SEG_G_1 29

#define SEG_A_2 14
#define SEG_B_2 30
#define SEG_C_2 21
#define SEG_D_2 22
#define SEG_E_2 23
#define SEG_F_2 24
#define SEG_G_2 25

class _7Seqment
{
public:
    virtual void Update(double cp_freq) = 0;
};

class _7SeqmentConcrete : public _7Seqment
{
private:
    double Freq;
public:
    _7SeqmentConcrete()
    {
        pinMode(SEG_A_1, OUTPUT);
        pinMode(SEG_B_1, OUTPUT);
        pinMode(SEG_C_1, OUTPUT);
        pinMode(SEG_D_1, OUTPUT);
        pinMode(SEG_E_1, OUTPUT);
        pinMode(SEG_F_1, OUTPUT);
        pinMode(SEG_G_1, OUTPUT);

        pinMode(SEG_A_2, OUTPUT);
        pinMode(SEG_B_2, OUTPUT);
        pinMode(SEG_C_2, OUTPUT);
        pinMode(SEG_D_2, OUTPUT);
        pinMode(SEG_E_2, OUTPUT);
        pinMode(SEG_F_2, OUTPUT);
        pinMode(SEG_G_2, OUTPUT);
    }

    void Parse()
    {
        int cp_freq = static_cast<int>(Freq);
        int i = 0;
        while (i < 2)
        {
            int reminder = cp_freq % 10;
            if (i == 0)
            {
                _7Segment_1(reminder);
            }
            else if (i == 1)
            {
                _7Segment_2(reminder);
            }
            cp_freq /= 10;
            i++;
        }
    }

    void _7Segment_2(int num)
    {
        int digits[10][7] =
        {
            {1, 1, 1, 1, 1, 1, 0}, // 0
            {0, 1, 1, 0, 0, 0, 0}, // 1
            {1, 1, 0, 1, 1, 0, 1}, // 2
            {1, 1, 1, 1, 0, 0, 1}, // 3
            {0, 1, 1, 0, 0, 1, 1}, // 4
            {1, 0, 1, 1, 0, 1, 1}, // 5
            {1, 0, 1, 1, 1, 1, 1}, // 6
            {1, 1, 1, 0, 0, 0, 0}, // 7
            {1, 1, 1, 1, 1, 1, 1}, // 8
            {1, 1, 1, 1, 0, 1, 1}  // 9
        };
        digitalWrite(SEG_A_2, digits[num][0]);
        digitalWrite(SEG_B_2, digits[num][1]);
        digitalWrite(SEG_C_2, digits[num][2]);
        digitalWrite(SEG_D_2, digits[num][3]);
        digitalWrite(SEG_E_2, digits[num][4]);
        digitalWrite(SEG_F_2, digits[num][5]);
        digitalWrite(SEG_G_2, digits[num][6]);
    }

    void _7Segment_1(int num)
    {
        int digits[10][7] =
        {
            {1, 1, 1, 1, 1, 1, 0}, // 0
            {0, 1, 1, 0, 0, 0, 0}, // 1
            {1, 1, 0, 1, 1, 0, 1}, // 2
            {1, 1, 1, 1, 0, 0, 1}, // 3
            {0, 1, 1, 0, 0, 1, 1}, // 4
            {1, 0, 1, 1, 0, 1, 1}, // 5
            {1, 0, 1, 1, 1, 1, 1}, // 6
            {1, 1, 1, 0, 0, 0, 0}, // 7
            {1, 1, 1, 1, 1, 1, 1}, // 8
            {1, 1, 1, 1, 0, 1, 1}  // 9
        };
        digitalWrite(SEG_A_1, digits[num][0]);
        digitalWrite(SEG_B_1, digits[num][1]);
        digitalWrite(SEG_C_1, digits[num][2]);
        digitalWrite(SEG_D_1, digits[num][3]);
        digitalWrite(SEG_E_1, digits[num][4]);
        digitalWrite(SEG_F_1, digits[num][5]);
        digitalWrite(SEG_G_1, digits[num][6]);
    }

    void Update(double cp_freq) override
    {
        Freq = cp_freq;
        Parse();
    }
};


class PWM
{
private:
    double BaseClock;
    double ClockRange;
    double ClockFreq;
    double Freq;
    int pin;

public:
    PWM(double cp_Freq) : Freq(cp_Freq), ClockRange(1024), BaseClock(19200000), pin(26)
    {
        std::cout << "Set Up PWM......" << std::endl;
        setupPWM();
    }

    void setupPWM()
    {
        ClockFreq = BaseClock / (ClockRange * Freq);
        wiringPiSetup();
        pinMode(pin, PWM_OUTPUT);
        pwmSetMode(PWM_MODE_MS);
        pwmSetRange(ClockRange);
        pwmSetClock(static_cast<int>(ClockFreq));

        pinMode(0, OUTPUT);
        pinMode(2, OUTPUT);
        pinMode(3, OUTPUT);

        digitalWrite(0, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
    }

    void updateFrequency(double newFreq)
    {
        static _7SeqmentConcrete obj;
        obj.Update(newFreq);
        Freq = newFreq;
        ClockFreq = BaseClock / (ClockRange * Freq);
        pwmSetClock(static_cast<int>(ClockFreq));
    }

    void startPWM()
    {
        while (true)
        {
            pwmWrite(pin, ClockRange / 2);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 / Freq)));
        }
    }

    void LED_Color(double freq)
    {
        static double old_freq = 0;
        if (old_freq != freq)
        {
            if (freq < 10)
            {
                digitalWrite(0, HIGH);
                digitalWrite(2, LOW);
                digitalWrite(3, LOW);
            }
            else if (freq < 30)
            {
                digitalWrite(0, LOW);
                digitalWrite(2, HIGH);
                digitalWrite(3, LOW);
            }
            else
            {
                digitalWrite(0, LOW);
                digitalWrite(2, LOW);
                digitalWrite(3, HIGH);
            }
        }
        old_freq = freq;
    }

    int getPin()
    {
        return pin;
    }
};


PWM *pwm = nullptr;

void signal_handle(int sig)
{
    if (pwm)
    {
        std::cout << "\nInterrupt signal has been received....." << std::endl;
        pinMode(pwm->getPin(), OUTPUT);
        digitalWrite(pwm->getPin(), LOW);
        digitalWrite(0, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);

        digitalWrite(SEG_A_1, LOW);
        digitalWrite(SEG_B_1, LOW);
        digitalWrite(SEG_C_1, LOW);
        digitalWrite(SEG_D_1, LOW);
        digitalWrite(SEG_E_1, LOW);
        digitalWrite(SEG_F_1, LOW);
        digitalWrite(SEG_G_1, LOW);
        digitalWrite(SEG_A_2, LOW);
        digitalWrite(SEG_B_2, LOW);
        digitalWrite(SEG_C_2, LOW);
        digitalWrite(SEG_D_2, LOW);
        digitalWrite(SEG_E_2, LOW);
        digitalWrite(SEG_F_2, LOW);
        digitalWrite(SEG_G_2, LOW);

        delete pwm;
        exit(0);
    }
}

int main()
{
    double freq;

    signal(SIGINT, signal_handle);
    pwm = new PWM(10);

    std::thread App(&PWM::startPWM, pwm);
    App.detach();

    while (true)
    {
        std::ifstream input("file.txt");
        input >> freq;
        std::cout << "Current Frequency => " << freq << std::endl;
        pwm->LED_Color(freq);
        input.close();
        pwm->updateFrequency(freq);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    delete pwm;
    return 0;
}
