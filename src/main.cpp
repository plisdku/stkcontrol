#include <iostream>
#include <cstdint>

#include "stkcontrol.h"

int main()
{
    std::cout << "Hello, world!\n";

    initialize();

    pushOn(0, 0, 440.0, 0.25);
    pushOn(10, 22050, 550.0, 1.0);
    pushOff(0, 44100, 0.5);
    pushOff(10, 66150, 1.0);
    pushStop(200000);

    writeWav("out.wav");
}
