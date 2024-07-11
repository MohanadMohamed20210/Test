#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <vector>
#include <csignal> // for signal handling

std::mutex mux;
std::condition_variable cond;
bool flag = false;
int button_pin, led_pin;

void signal_handler(int sig) {
    std::cout << "Signal " << sig << " received. Cleaning up and exiting." << std::endl;
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);
    exit(sig);
}

class LedControlAbstract {
public:
    virtual void Update(int pin_value) = 0;
    virtual void operations() = 0;
};

class LedControl : public LedControlAbstract {
private:
    int Led_Pin;
    int pin_value;

public:
    LedControl(int cp_pin) {
        Led_Pin = cp_pin;
        pinMode(Led_Pin, OUTPUT);
        digitalWrite(Led_Pin, LOW);
    }

    void Update(int pin_value) override {
        this->pin_value = pin_value;
    }

    void operations() override {
        if (pin_value == 1) {
            digitalWrite(Led_Pin, LOW);
        } else if (pin_value == 0) {
            digitalWrite(Led_Pin, HIGH);
        }
    }
};

class ButtonAbstract {
public:
    virtual void Subscribe(std::shared_ptr<LedControlAbstract> LedObject) = 0;
    virtual void notify() = 0;
    virtual void setValue(int pin_value) = 0;
    virtual void getValue() = 0;
};

class Button : public ButtonAbstract {
private:
    int pin_value;
    std::vector<std::shared_ptr<LedControlAbstract>> Leds;
    int Button_Pin;

public:
    Button(int cp_Button) : Button_Pin(cp_Button) {
        pinMode(Button_Pin, INPUT);
        pullUpDnControl(Button_Pin, PUD_UP); // Ensure pull-up resistor is enabled
    }

    void Subscribe(std::shared_ptr<LedControlAbstract> LedObject) override {
        Leds.push_back(LedObject);
    }

    void notify() override {
        for (auto& Led : Leds) {
            Led->Update(pin_value);
        }
    }

    void setValue(int pin_value) override {
        this->pin_value = pin_value;
        notify();
    }

    void getValue() override {
        int pin = digitalRead(Button_Pin);
        setValue(pin);
    }
};

void observable(std::shared_ptr<ButtonAbstract> button) {
    while (true) {
        std::unique_lock<std::mutex> guard(mux);
        button->getValue();
        flag = true;
        cond.notify_all();
        mux.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Debounce delay
    }
}

void observer(std::shared_ptr<LedControlAbstract> led) {
    while (true) {
        std::unique_lock<std::mutex> guard(mux);
        cond.wait(guard, [] { return flag; });
        flag = false;
        led->operations();
        mux.unlock();
    }
}

int main() {
    wiringPiSetup();

    std::cout << "Enter Button pin => ";
    std::cin >> button_pin;

    std::cout << "Enter Led pin => ";
    std::cin >> led_pin;

    signal(SIGINT, signal_handler);
    std::shared_ptr<ButtonAbstract> button = std::make_shared<Button>(button_pin);
    std::shared_ptr<LedControlAbstract> led = std::make_shared<LedControl>(led_pin);

    button->Subscribe(led);

    std::thread Observable(observable, button);
    std::thread Observer(observer, led);

    std::cout << "Press Ctrl+C to exit." << std::endl;

    Observable.join();
    Observer.join();
    return 0;
}
