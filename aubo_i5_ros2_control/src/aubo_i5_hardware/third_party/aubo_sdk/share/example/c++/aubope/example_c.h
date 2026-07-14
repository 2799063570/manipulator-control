#ifndef EXAMPLE_C_H
#define EXAMPLE_C_H

#include "AuboRobotMetaType.h"
#include "rsdef.h"

class Example_c
{
public:
    void demo_connectrobot();
    void demo_disconnect();
    void demo_circle();
    void demo_relative();
    void demo_computer();
    void demo_teach();
    void demo_rotate();
    void demo_io();

private:
    RSHD rshd_;
};

#endif // EXAMPLE_C_H
