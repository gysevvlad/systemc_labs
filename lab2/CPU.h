#include <systemc.h>
#include "PmodOLEDController.h"


SC_MODULE(CPU) {
    sc_in< bool >         CLK;
    sc_out< bool >        HWRITE;
    sc_out< sc_uint<32> > HADDR;
    sc_out< sc_uint<32> > HWDATA;
    sc_in< sc_uint<32> >  HRDATA;

    sc_uint<32> RDATA;
    bool finish;

    void clock();

    sc_uint<32> read(sc_uint<32> ADDR) {
        HADDR.write(ADDR);
        HWRITE.write(false);
        WAIT_UNTIL(CLK);
        WAIT_UNTIL(!CLK);
        auto temp = HRDATA.read();
        HADDR.write(0x00000000);
        WAIT_UNTIL(CLK);
        return temp;
    };

    void write(sc_uint<32> ADDR, sc_uint<32> WDATA) {
        HWRITE.write(true);
        HADDR.write(ADDR);
        HWDATA.write(WDATA);
        next();
    };

    SC_CTOR(CPU) : finish(false) {
        SC_THREAD(clock);
        dont_initialize();
        sensitive << CLK.pos() << CLK.neg();
    }

    void next() {
        WAIT_UNTIL(CLK);
    }
};

void CPU::clock() {
    
    write(0xFFFF0006, 0x11223344);
    write(0x00000000, 0x00000000);
    write(0x00000000, 0x00000000);
    write(0x00000000, 0x00000000);
    write(0x00000000, 0x00000000);
    write(0x00000000, 0x00000000);
    while( read(0xFFFFF0000)[0] ) next();

    finish = true;
    // ...
}

#undef next
