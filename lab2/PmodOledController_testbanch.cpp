#include "PmodOLEDController.h"

int sc_main(int argc, char * argv[]) 
{
    sc_clock CLK("clk", 10, SC_NS);
    sc_signal< bool > HWRITE, CS;
    sc_signal< sc_uint<32> > HWDATA, HADDR, HRDATA;

    sc_signal< bool > SPI_CS, SPI_CLK, SPI_MOSI, DC;

    PmodOLEDController<2> oled("PmodOLED");
    oled(CS, CLK, HWRITE, HWDATA, HADDR,    
            HRDATA, SPI_CS, SPI_CLK, SPI_MOSI, DC);

    sc_trace_file * file_trace = sc_create_vcd_trace_file("spi");
    file_trace->set_time_unit(1.0, SC_PS);
#define s(name) sc_trace(file_trace, name, #name) 
    s(CLK);     s(SPI_CS);
    s(SPI_CLK); s(SPI_MOSI);
    s(CS);      s(DC);
#undef s

    CS = true;
    sc_start(10, SC_NS);

    //SEND DATA (1)
    CS = false;
    HWRITE = true;
    HADDR = 0x00000000;
    HWDATA = 0xD5D5D5D5;
    sc_start(10, SC_NS);

    //WAIT READY
    HWRITE = false;
    HADDR = 0x00000000;
    sc_start(10, SC_NS);
    do {
        sc_start(10, SC_NS);
        std::cout << decrypt_init(HRDATA.read()) << std::endl;
    } while(HRDATA.read()[0]);

    CS = true;
    sc_start(10, SC_NS);

    //SEND DATA (2)
    CS = false;
    HWRITE = true;
    HADDR = 0x00000003;
    HWDATA = 0xD5D5D5D5;
    sc_start(10, SC_NS);

    HWRITE = false;
    HADDR = 0x00000000;
    do {
        sc_start(10, SC_NS);
        std::cout << decrypt_init(HRDATA.read()) << std::endl;
    } while (HRDATA.read()[0]);

    CS = true;
    sc_start(10, SC_NS);

    //SEND DATA (3)
    CS = false;
    HWRITE = true;
    HADDR = 0x00000004;
    HWDATA = 0xD5D5D5D5;
    sc_start(10, SC_NS);

    //WAIT READY
    HWRITE = false;
    HADDR = 0x00000000;
    do {
        sc_start(10, SC_NS);
        std::cout << decrypt_init(HRDATA.read()) << std::endl;
    } while(HRDATA.read()[0]);

    CS = true;
    sc_start(10, SC_NS);

    //SEND DATA (4 bytes)
    CS = false;
    HWRITE = true;
    HADDR = 0x00000007;
    HWDATA = 0xD5D5D5D5;
    sc_start(10, SC_NS);

    //WAIT READY
    HWRITE = false;
    HADDR = 0x00000000;
    do {
        sc_start(10, SC_NS);
        std::cout << decrypt_init(HRDATA.read()) << std::endl;
    } while(HRDATA.read()[0]);

    CS = true;
    sc_start(10, SC_NS);

    sc_start(5, SC_NS);
    sc_close_vcd_trace_file(file_trace);
    return 0;
}
