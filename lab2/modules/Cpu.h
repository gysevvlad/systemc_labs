#pragma once

#include <systemc.h>
#include "../modules/PmodOledDriver.h"

SC_MODULE(CPU) {
    SC_HAS_PROCESS(CPU);

    sc_in< bool >         CLK;
    sc_out< bool >        HWRITE;
    sc_out< sc_uint<32> > HADDR;
    sc_out< sc_uint<32> > HWDATA;
    sc_in< sc_uint<32> >  HRDATA;

    bool finish;

    CPU(sc_module_name name);
    
    void run();
    void write(sc_uint<32> ADDR, sc_uint<32> WDATA);
    void next() { WAIT_UNTIL(CLK); }
    sc_uint<32> read(sc_uint<32> ADDR);

    void test1();
    void spi_test();

private:
    PmodOledDriver oled;
};
