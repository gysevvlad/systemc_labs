#pragma once

#include <systemc.h>

/*
 *  HWRITE HADDR[0:4] COMMAND
 *       0       0000 GET_STATE   
 *       0       0001 RESET
 *       1       0XX0 SEND_DATA
 *       1       0XX1 SEND_COMMAND
 *
 *  INIT[0]    0 READY
 *  INIT[0]    1 TRANSACT
 *  INIT[1]    1 DATA
 *  INIT[1]    0 COMMAND
 *  INIT[2:3] 00 1 byte
 *  INIT[2:3] 01 2 byte
 *  INIT[2:3] 10 3 byte
 *  INIT[2:3] 11 4 byte
 */
template< unsigned int divider >
SC_MODULE(PmodOLEDController) {
    sc_in< bool >         CS;
    sc_in< bool >         CLK;
    sc_in< bool >         HWRITE;
    sc_in< sc_uint<32> >  HWDATA;
    sc_in< sc_uint<32> >  HADDR;
    sc_out< sc_uint<32> > HRDATA;

    sc_out< bool > SPI_CS;
    sc_out< bool > SPI_CLK;
    sc_out< bool > SPI_DOUT;
    sc_out< bool > DC;

    sc_uint<32> data;
    sc_uint<32> init;

    enum class State {
        TRANSACTION = 00,
        READY       = 01
    } state;

    void clk() {
        SPI_CS.write(true);
        state = State::READY;
        init = 0x00000000;
        int sclk, clk;
        bool spi_clk;
        int byte_count;
        sc_uint<32> bytes;
        wait();
        while (true) {
            std::cout << "CLK";
            if (state == State::READY && HWRITE.read() && !CS.read()) {
                std::cout << " 1";
                //SEND_DATA or SEND_COMMAND
                state = State::TRANSACTION;
                bytes = HADDR.read().range(3,1); 
                data = HWDATA.read();
                clk = (bytes + 1) * divider * 8;
                spi_clk = false;

                //INIT PORTS
                SPI_CS = false;

                //INIT UPDATE
                init[0] = true;
                init[1] = HADDR.read()[0];
                init[2] = HADDR.read()[1];
                init[3] = HADDR.read()[2];
                DC.write(init[1]);
            } 

            if (state == State::TRANSACTION) {
                std::cout << " 2";
                if (clk % divider == 0) {
                    std::cout << " a";
                    SPI_DOUT.write(data[0]);
                    data = data >> 1;
                }
                if (clk % (divider >> 1) == 0) {
                    std::cout << " b";
                    SPI_CLK.write(spi_clk);
                    spi_clk = !spi_clk;
                }
                if (clk == 0) {
                    std::cout << " c";
                    SPI_CS.write(true);
                    SPI_CLK.write(false);
                    SPI_DOUT.write(false);
                    DC.write(false);
                    state = State::READY;
                    init[0] = (state == State::TRANSACTION);
                    init = 0;
                }
                else {
                    std::cout << " d";
                    clk--;
                    init.range(16, 8) = sc_uint<8>(clk);
                }
            }

            if (!HWRITE.read() && !HADDR.read()[0] && !CS.read()) {
                std::cout << " 3";
                //GET_STATE
                HRDATA.write(init);
            }

            std::cout << std::endl;
            wait();
        }
    }

    SC_CTOR(PmodOLEDController) {
        SC_CTHREAD(clk, CLK.pos());
    }
};
std::string decrypt_init(const sc_uint<32> & init) {
    std::string str("[ ");
    if (init[0]) {
        str += "TRANSACT |";
        str += (init[1] ? " COMMAND |" : " DATA |");
        sc_uint<3> temp;
        temp[0] = init[2];
        temp[1] = init[3];
        str += " " + std::to_string(temp + 1) + " |";
        sc_uint<8> clk(init.range(16,8));
        str += " " + std::to_string((int)clk) + " ]";
    }
    else {
        str += "READY ]";
    }
    return str;
}


