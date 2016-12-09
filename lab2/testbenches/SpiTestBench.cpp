#include "../modules/PmodType2A.h"

int sc_main(int argc, char * argv[])
{
    sc_signal< bool > SS, MOSI, MISO, SCK, INT, RST;
    Pmod::Type2A::Master master("master");
    master(SS, MOSI, MISO, SCK, INT, RST);
    return 0;
}
