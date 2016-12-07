#pragma once

#include <systemc.h>

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

    sc_in< bool > SPI_MISO;

    sc_uint<32> m_data;
    sc_uint<32> m_state;

    enum class State {
        TRANSACTION = 0,
        READY       = 1
    };

    void setState( State state ) { m_state[0] = state == State::READY; }
    void setD() { m_state[1] = true; }
    void setC() { m_state[1] = false; }

    void clk() {
        State state = State::READY;
        SPI_CS.write(true);
        bool spi_clk;
        int clk;

        while( true ) {
            wait();

            if (state == State::TRANSACTION) {
                SPI_CS.write(false);
                DC.write(m_state[1]);
                if (clk % (divider >> 1) == 0) {
                    SPI_CLK.write(spi_clk);
                    spi_clk = !spi_clk;
                }
                if (clk != 0) {
                    if (clk % divider == 0) {
                        SPI_DOUT.write(m_data[31]);
                        m_data <<= 1;
                    }
                    if (clk % divider == divider >> 1) {
                        m_data[0] = SPI_MISO.read();
                    }
                    clk--;
                }
                else {
                    state = State::READY;
                    setState(State::READY);
                    SPI_CS.write(true);
                }
            }
            
            if (CS.read()) continue;

            if (HWRITE.read() && (HADDR.read() & 0x00000007) == 2) {
                /* reset */
                state = State::READY;
                spi_clk =  false;
                setState(state);
                m_data = 0;
                clk = 0;
                setC();
            }
 
            if (HWRITE.read() && state == State::READY) {
                if ((HADDR.read() & 0x00000007) == 1) {
                    /* transact */
                    state = State::TRANSACTION;
                    setState(state);
                    auto bytes = HADDR.read().range(6, 4) + 1;
                    clk = bytes * divider * 8;
                    m_data = HWDATA.read();
                    spi_clk = false;
                }  
                if ((HADDR.read() & 0x00000007) == 5) {
                    setD();
                }
                if ((HADDR.read() & 0x00000007) == 3) {
                    setC();
                }
            }

            if (!HWRITE.read()) {
                if ((HADDR.read() & 0x0000000F) == 4) {
                    HRDATA.write( m_data );
                }
                if ((HADDR.read() & 0x0000000F) == 0) {
                    HRDATA.write( m_state );
                }
            }
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
