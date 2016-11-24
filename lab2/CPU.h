#include <systemc.h>

SC_MODULE(CPU) {
    sc_in< bool >         CLK;
    sc_out< bool >        HWRITE;
    sc_out< sc_uint<32> > HADDR;
    sc_out< sc_uint<32> > HWDATA;
    sc_in< sc_uint<32> >  HRDATA;

    void clock();

    SC_CTOR(CPU) {
        SC_CTHREAD(clock, CLK.pos());
    }
};

void CPU::clock() {
    //init
    wait();

    // ...
}
