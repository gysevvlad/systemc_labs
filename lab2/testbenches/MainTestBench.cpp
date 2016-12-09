#include <systemc>

#include "../modules/Cpu.h"
#include "../modules/Bus.h"
#include "../modules/PmodOledController.h"
#include "../modules/SpiStud.h"
#include "../modules/PmodOledStud.h"
#include "../modules/Switch.h"

int sc_main(int argc, char * argv[])
{
    sc_clock CLK("CLK", 10, SC_NS);
    sc_signal< sc_uint<32> > HADDR, HRDATA_S1, HRDATA_S2, HRDATA, HWDATA;
    sc_signal< bool > HSEL_S1, HSEL_S2, HWRITE, SPI_CS, 
        SPI_CLK, SPI_DOUT, DC, SPI_MISO;

    bus_t<2> bus("Bus");
    bus(HADDR, HRDATA_S1, HRDATA_S2, HRDATA, HSEL_S1, HSEL_S2);

    CPU cpu("Cpu");
    cpu(CLK, HWRITE, HADDR, HWDATA, HRDATA);

    PmodOLEDController<2> controller("PmodOledController");
    controller(HSEL_S1, CLK, HWRITE, HWDATA, HADDR, HRDATA_S1,
            SPI_CS, SPI_CLK, SPI_DOUT, DC, SPI_MISO);

    Switch switcher("Switch");
    switcher(HSEL_S2, CLK, HWRITE, HWDATA, HADDR, HRDATA_S2);

    PmodOledStud pmodOledStud("PmodOledStud");
    pmodOledStud(SPI_CS, SPI_CLK, DC, SPI_DOUT);

    sc_trace_file * file_trace = sc_create_vcd_trace_file("system");
    file_trace->set_time_unit(1.0, SC_PS);
#define s(name) sc_trace(file_trace, name, #name) 
    s(HADDR); s(HRDATA_S1); s(HRDATA_S2);
    s(HRDATA); s(HWDATA); s(HSEL_S1);
    s(HSEL_S2); s(HWRITE); s(SPI_CS);
    s(SPI_CLK); s(SPI_DOUT); s(DC);
    s(CLK);
#undef s

    while (!cpu.finish) sc_start(10, SC_NS); 
    
    sc_start(5, SC_NS);
    sc_close_vcd_trace_file(file_trace);
    return 0;
}
