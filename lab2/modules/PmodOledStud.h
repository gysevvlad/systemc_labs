#pragma once

#include <systemc.h>
#include <queue>

SC_MODULE(PmodOledStud) {
    sc_in< bool > CS;
    sc_in< bool > CLK;
    sc_in< bool > DC;
    sc_in< bool > MOIS;

    size_t address;
    size_t bit;

    sc_uint<8> data;
    unsigned char first_byte;
    sc_uint<8> memory[32*16];

    void run() {
        wait();

        while(true) {
            if (CS.read()) wait(); 

            /*
            while(!CS.read()) {
                for( int i = 0; i < 8; i++) {
                    data <<= 1;
                    data[0] = MOIS.read();
                }
                std::cout << "PmodOledStud: recieved " 
                    << (DC.read()? "DATA " : "COMMAND ")
                    <<  std::hex << data << std::endl;
                data = 0x00;
            }
            */
            while(!CS.read()) {
                for( int i = 0; i < 8; i++) {
                    WAIT_UNTIL(CLK);
                    if (DC.read()) {
                        memory[address] <<= 1;
                        memory[address][0] = MOIS.read();
                    }
                    else { 
                        data <<= 1; 
                        data[0] = MOIS.read(); 
                    }
                }

                if (DC.read()) address++;

                if (address == 32*16) {
                    address = 0;
                    print_pict();
                }
                if (!DC.read()) {
                    std::cout << "PmodOledStud: command " <<  std::hex 
                        << data << std::endl;
                }
            }
        }
    }

    void print_pict() {
        std::cout << std::endl;
        for (int i = 0; i < 32; i++) {
            for( int j = 0; j < 16; j++) {
                for (int k = 0; k < 8; k++) {
                    std::cout << (memory[i*16+j][k] ? '#' : '.');
                }
            }
            std::cout << std::endl;
        }
        for ( int i = 0; i < 128; i++) { std::cout << '='; }
        std::cout << std::endl;
    }

    SC_CTOR(PmodOledStud) : address(0) {
        SC_THREAD(run)
            sensitive << CLK.pos() << CS;
    }

    ~PmodOledStud() {
    }
};
