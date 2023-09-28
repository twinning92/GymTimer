#pragma once

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void on_notify() = 0;
};