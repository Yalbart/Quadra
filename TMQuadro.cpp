// TMQuadro.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Igra.h"

int main(int argc, char** argv)
{
    //std::cout << "Hello World!\n";
    Igra igra(argc, argv);
    igra.run();
    return 0;
}
