#include "serialib.h"

using namespace std;
serialib sl;

bool openSerial()
{
    bool res = false;
    
    if (sl.Open("/dev/ttyUSB0", 9600) < 0) //@check nom du port
        cout << "Failed to open serial port!" << endl;
    else
        cout << "Serial port successfully opened." << endl;
        res = true;
    
    return res;
}

void commandRight()
{
    cout << "A droite toute !" << endl;
    //sl.WriteChar('R');
}

void commandLeft()
{
    cout << "A gauche toute !" << endl;
    //sl.WriteChar('L');  
}

void commandStop()
{
    cout << "Halte !" << endl;
    //sl.WriteChar('S');
}

void commandForward()
{
    cout << "Droit devant !" << endl;
    //sl.WriteChar('F');  
}

void commandBackward()
{
    cout << "En arrière !" << endl;
    //sl.WriteChar('B');
}