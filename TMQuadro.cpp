#include <iostream>
#include <Windows.h>
#include "Igra.h"

int main(int argc, char** argv)
{
    //std::cout << "Hello World!\n";
    SetProcessDPIAware(); //dodato 22.06. govori windowsu da ja hendlujem DPI(dots per inch)(u sustini podesavanje za razlicite screen size-ove, moze se dodati i u manifest fajlu al meni to ne treba)
    Igra igra(argc, argv);
    igra.run();
    return 0;
}
