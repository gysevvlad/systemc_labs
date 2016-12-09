#pragma once

#include <systemc.h>
#include <cstdio>

SC_MODULE(Switch) {
    sc_in< bool >         CS;
    sc_in< bool >         CLK;
    sc_in< bool >         HWRITE;
    sc_in< sc_uint<32> >  HWDATA;
    sc_in< sc_uint<32> >  HADDR;
    sc_out< sc_uint<32> > HRDATA;

    sc_uint<32> switches;

    void run() {
        switches = 0x00000000;
        while(true) {
            wait();
            if (CS.read()) continue;

            if (HWRITE.read() && (HADDR.read() & 0x0000000F) == 4) {
                std::cout << "Switcher: set Led : " << std::hex 
                    << HWDATA.read() << std::endl;
            }

            if (!HWRITE.read() && (HADDR.read() & 0x0000000F) == 8) {
                int rc = check_switch();
                if (rc >= 0 && rc < 16) {
                    switches[rc] = !switches[rc];
                    std::cout << "Switch: " << std::hex 
                        << switches << std::endl; 
                }
                HRDATA.write(switches);
            }
        }
    }

    int check_switch() {
        int number;
        printf("Print selected switcher: ");
        scanf("%d", &number);
        return number;
    }

    SC_CTOR(Switch) {
        SC_THREAD(run);
        sensitive << CLK.pos();
    }
};
