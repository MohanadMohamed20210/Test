#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

class Observer
{
    public:
        virtual void Update(double temp,double pressure,double humidity) = 0;
        virtual void Display() = 0;
};
class ConcreteObserver : public Observer
{
    private:
        double temp;
        double pressure;
        double humidity;
        std::string name;
    public:
        ConcreteObserver(std::string cp_name):name(cp_name){}
        void Update(double temp,double pressure,double humidity)override
        {
            this ->temp = temp;
            this ->pressure = pressure;
            this ->humidity = humidity;
        }
        void Display()override
        {
            std::cout<<"----------------- Weather Data ------------------\n";
            std::cout<<"Name => "<<name<<std::endl;
            std::cout<<"Temperature => "<<temp<<std::endl;
            std::cout<<"Pressure => "<<pressure<<std::endl;
            std::cout<<"Humidity => "<<humidity<<std::endl;
        }
};
class Subject
{
    public:
        virtual void RegisterObserver(std::shared_ptr<Observer> observer) = 0;
        virtual void RemoveObserver(std::shared_ptr<Observer> observer) = 0;
        virtual void Notifiy() = 0;
        virtual void setData(double temp,double pressure,double humidity) = 0;
};
class ConcreteSubject : public Subject
{
    private:
        double temp;
        double pressure;
        double humidity;
        std::vector<std::shared_ptr<Observer>> observers;
    public:
        void RegisterObserver(std::shared_ptr<Observer> observer)override
        {
            observers.push_back(observer);
        }
        void RemoveObserver(std::shared_ptr<Observer> observer)override
        {
            observers.erase(std::remove(observers.begin(),observers.end(),observer),observers.end());
        }
        void Notifiy()override
        {
            for(auto observer : observers)
            {
                observer->Update(temp,pressure,humidity);
            }
        }
        void setData(double temp,double pressure,double humidity)
        {
            this ->temp = temp;
            this ->pressure = pressure;
            this ->humidity = humidity;

            Notifiy();
        }
};
int main()
{
    std::shared_ptr<Subject> sub = std::make_shared<ConcreteSubject>();

    std::shared_ptr<Observer> o_1 = std::make_shared<ConcreteObserver>("Observer_1");
    std::shared_ptr<Observer> o_2 = std::make_shared<ConcreteObserver>("Observer_2");
    std::shared_ptr<Observer> o_3 = std::make_shared<ConcreteObserver>("Observer_3");

    sub->RegisterObserver(o_1);
    sub->RegisterObserver(o_2);
    sub->RegisterObserver(o_3);

    sub->setData(10,20,30);
    o_1->Display();
    o_2->Display();
    o_3->Display();

    sub->RemoveObserver(o_2);
    sub->setData(100,200,300);
    o_1->Display();
    o_2->Display();
    o_3->Display();
    return 0;
}