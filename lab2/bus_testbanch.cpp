#include "bus.h"

int sc_main(int argc, char * argv[])
{
    sc_clock clk_in("clk", 10, SC_NS);
    bus_t<2> bus("bus"); 

    sc_signal< bool > clk_out,
                      hwrite_in, hwrite_out,
                      hsel_s1,   hsel_s2;
    sc_signal< sc_uint<32> > haddr_in,  haddr_out,
                             hwdata_in, hwdata_out,
                             hrdata_s1, hrdata_s2,
                             hrdata_out;

    bus(clk_in,    clk_out, 
        hwrite_in, hwrite_out,
        haddr_in,  haddr_out,
        hwdata_in, hwdata_out,
        hrdata_out, 
        hrdata_s1, hrdata_s2,
        hsel_s1,   hsel_s2);

    sc_trace_file * file_trace = sc_create_vcd_trace_file("bus_testbanch");
    file_trace->set_time_unit(1.0, SC_PS);

#define s(name) sc_trace(file_trace, name, #name)
    s(clk_in);     s(hwdata_in);
    s(clk_out);    s(hwdata_out);
    s(hwrite_out); s(hrdata_out);
    s(hwrite_in);  s(hrdata_s1);
    s(haddr_in);   s(hrdata_s2);
    s(haddr_out);  s(hsel_s1);
    s(hsel_s2);
#undef s

    haddr_in = 0x00000000;
    hrdata_s1 = 0x11111111;
    hrdata_s2 = 0x22222222;

    sc_start(10, SC_NS);

    hwrite_in = true;
    haddr_in = 0xA5A5A5A5;
    hwdata_in = 0x5A5A5A5A;
    sc_start(10, SC_NS);

    hwrite_in = false;
    haddr_in = 0xFFFF0000;
    sc_start(10, SC_NS);

    hrdata_s1 = 0x01010101;
    sc_start(10, SC_NS);

    haddr_in = 0xFFFF1000;
    sc_start(10, SC_NS);

    haddr_in = 0xFFFF2000;
    sc_start(10, SC_NS);

    sc_start(5, SC_NS);
    sc_close_vcd_trace_file(file_trace);
    return 0;
}
