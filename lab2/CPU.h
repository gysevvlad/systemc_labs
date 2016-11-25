#include <systemc.h>
#include "PmodOLEDController.h"

SC_MODULE(CPU) {
    sc_in< bool >         CLK;
    sc_out< bool >        HWRITE;
    sc_out< sc_uint<32> > HADDR;
    sc_out< sc_uint<32> > HWDATA;
    sc_in< sc_uint<32> >  HRDATA;

    void clock();

    bool finish;

    SC_CTOR(CPU) : finish(false) {
        SC_CTHREAD(clock, CLK.pos());
    }
};

void CPU::clock() {
    //init
    wait();

    HWRITE = 1;
    HADDR = 0xFFFF0006;
    HWDATA = 0x44332211;
    wait();

    HWRITE = 0;
    HADDR = 0xFFFF0000;
    wait();
    do {
        wait();
        //std::cout << decrypt_init(HRDATA.read()) << std::endl;
    } while (HRDATA.read()[0]);

    finish = true;

    // ...
}
