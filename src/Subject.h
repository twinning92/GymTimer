#pragma once
#include "Observer.h"
#include <list>

class Subject
{
public:
    virtual ~Subject() = default;
    void add_observer(Observer *observer) { observers.push_back(observer); }
    void remove_observer(Observer *observer) { observers.remove(observer); }
    virtual void notify()
    {
        for (Observer *o : observers)
        {
            o->on_notify();
        }
    }

private:
    std::list<Observer *> observers;
};