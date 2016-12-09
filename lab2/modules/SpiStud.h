#pragma once

#include <systemc.h>
#include <queue>

SC_MODULE(SpiStud) {
    sc_in< bool > CS;
    sc_in< bool > CLK;
    sc_in< bool > DC;
    sc_in< bool > MOIS;
    sc_out< bool > MISO;

    std::queue< sc_uint<8> > data;

    void run() {
        wait();

        while(true) {
            if (CS.read()) wait(); 

            while(!CS.read()) {
                MISO.write(data.front()[7]);
                for( int i = 0; i < 8; i++) {
                    WAIT_UNTIL(CLK);
                    data.front() <<= 1;
                    data.front()[0] = MOIS.read();
                    WAIT_UNTIL(!CLK);
                    MISO.write(data.front()[7]);
                }
                std::cout << "SpiSlaveStud: recieved "
                          <<  std::hex << data.front() << std::endl;
                data.push(data.front());
                data.pop();
            }
        }
    }

    SC_CTOR(SpiStud) : data({0x00}) {
        SC_THREAD(run)
            sensitive << CS << CLK.pos() << CLK.neg();
    }
};
