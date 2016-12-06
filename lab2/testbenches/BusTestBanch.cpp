#include <systemc.h>
#include "bus.h"

int sc_main(int argc, char * argv[])
{
    sc_clock clk_in("clk", 10, SC_NS);
    bus_t<2> bus("bus"); 

    sc_signal< bool > HSEL_S[2];
    sc_signal< sc_uint<32> > HRDATA_S[2], HRDATA, HADDR;
    
    bus(HADDR, HRDATA_S[0], HRDATA_S[1], HRDATA, HSEL_S[0], HSEL_S[1]);

    sc_trace_file * file_trace = sc_create_vcd_trace_file("bus_testbanch");
    file_trace->set_time_unit(1.0, SC_PS);

#define s(name) sc_trace(file_trace, name, #name)
    s(HRDATA); s(HADDR); s(HRDATA_S[0]);
    s(HRDATA_S[1]); s(HSEL_S[0]); s(HSEL_S[1]);
#undef s

#define next() sc_start(10, SC_NS)
    HADDR = 0x00000000;
    HRDATA_S[0] = 0x01010101;
    HRDATA_S[1] = 0x02020202;
    next();

    HADDR = 0xFFFF0000;
    next();

    HADDR = 0xFFFF1000;
    next();

    HRDATA_S[1] = 0x20202020;
    next();

    HADDR = 0x00000000;
    next();
#undef next

    sc_start(5, SC_NS);
    sc_close_vcd_trace_file(file_trace);
    return 0;
}
