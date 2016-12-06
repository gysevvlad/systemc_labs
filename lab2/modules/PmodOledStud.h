#pragma once

#include <systemc.h>

SC_MODULE(PmodOledStud) {
    sc_in< bool > CS;
    sc_in< bool > CLK;
    sc_in< bool > DC;
    sc_in< bool > MOIS;

    sc_uint<8> data;

    void run() {
        wait();

        while(true) {
            if (CS.read()) wait(); 

            while(!CS.read()) {
                for( int i = 0; i < 8; i++) {
                    WAIT_UNTIL(CLK);
                    data >>= 1;
                    data[7] = MOIS.read();
                }
                std::cout << "PmodOledStud: recieved " 
                          << (DC.read()? "DATA " : "COMMAND ")
                          <<  std::hex << data << std::endl;
                data = 0x00;
            }
        }
    }

    SC_CTOR(PmodOledStud) {
        SC_THREAD(run)
            sensitive << CS << CLK.pos();
    }
};
