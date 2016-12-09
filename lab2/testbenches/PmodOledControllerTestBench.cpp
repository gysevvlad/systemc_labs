#include "../modules/PmodOledController.h"
#include "../modules/SpiStud.h"

int sc_main(int argc, char * argv[]) 
{
    sc_clock CLK("clk", 10, SC_NS);
    sc_signal< bool > HWRITE, CS;
    sc_signal< sc_uint<32> > HWDATA, HADDR, HRDATA;

    sc_signal< bool > SPI_CS, SPI_CLK, SPI_MOSI, DC, SPI_MISO;

    PmodOLEDController<2> oled("PmodOledController");
    oled(CS, CLK, HWRITE, HWDATA, HADDR,    
            HRDATA, SPI_CS, SPI_CLK, SPI_MOSI, DC, SPI_MISO);

    SpiStud stud("SpiStud");
    stud(SPI_CS, SPI_CLK, DC, SPI_MOSI, SPI_MISO);
    stud.data.push(0x11);
    stud.data.push(0x22);
    stud.data.push(0x33);

    sc_trace_file * file_trace = sc_create_vcd_trace_file("spi");
    file_trace->set_time_unit(1.0, SC_PS);
#define s(name) sc_trace(file_trace, name, #name) 
    s(HWRITE);  s(HWDATA);
    s(HADDR);   s(HRDATA);
    s(CLK);     s(SPI_CS);
    s(SPI_CLK); s(SPI_MOSI);
    s(CS);      s(DC);
    s(SPI_MISO);
#undef s

    CS = true;
    sc_start(10, SC_NS);

    /* reset */
    CS = false;
    HWRITE = true;
    HADDR = 0xFFFF0002;
    sc_start(10, SC_NS);

    /* set data */
    CS = false;
    HWRITE = true;
    HADDR = 0x5;
    sc_start(10, SC_NS);

    /* transact */
    CS = false;
    HWRITE = true;
    HWDATA = 0x12000000;
    HADDR = 0x01;
    sc_start(10, SC_NS);

    /* get state */
    CS = false;
    HWRITE = false;
    HADDR = 0x0;
    do {
        sc_start(10, SC_NS);
    } while (!HRDATA.read()[0]);

    /* get data */
    CS = false;
    HWRITE = false;
    HADDR = 0x4;
    sc_start(10, SC_NS);

    /* set data */
    CS = false;
    HWRITE = true;
    HADDR = 0x3;
    sc_start(10, SC_NS);
   
    /* transact */
    CS = false;
    HWRITE = true;
    HWDATA = 0x34000000;
    HADDR = 0x01;
    sc_start(10, SC_NS);

    /* get state */
    CS = false;
    HWRITE = false;
    HADDR = 0x0;
    do {
        sc_start(10, SC_NS);
    } while (!HRDATA.read()[0]);

    /* get data */
    CS = false;
    HWRITE = false;
    HADDR = 0x4;
    sc_start(10, SC_NS);

    sc_start(5, SC_NS);
    sc_close_vcd_trace_file(file_trace);
    return 0;
}
