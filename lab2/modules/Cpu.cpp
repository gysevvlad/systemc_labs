#include "../modules/Cpu.h"
#include <cassert>

sc_uint<32> CPU::read(sc_uint<32> ADDR) {
    HADDR.write(ADDR);
    HWRITE.write(false);
    WAIT_UNTIL(CLK);
    ADDR.range(11, 0) = 0;
    HADDR.write(ADDR);
    WAIT_UNTIL(!CLK);
    auto temp = HRDATA.read();
    WAIT_UNTIL(CLK);
    return temp;
};

void CPU::write(sc_uint<32> ADDR, sc_uint<32> WDATA) {
    HWRITE.write(true);
    HADDR.write(ADDR);
    HWDATA.write(WDATA);
    next();
};

CPU::CPU(sc_module_name name) 
    : sc_module(name), finish(false), oled(*this, 0)
{
    SC_THREAD(run);
    dont_initialize();
    sensitive << CLK.pos() << CLK.neg();
}

void CPU::test1() {
    oled.init();
    bool old_choice = false;
    int counter = 0;
    while (counter != 4) {
        bool choice = read(0xFFFF1008)[7];
        if (old_choice != choice) {
            counter++;
            oled.send_data( choice ? image1 : image2 );
        }
        old_choice = choice;
    }
}

void CPU::spi_test() {
    std::vector<unsigned char> data;
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 16; j += 2) {
            data.push_back(0xFF);
            data.push_back(0x00);
        } 
    }
    assert(data.size() == 32 * 16);
    oled.send_data(image2);
    oled.send_data(image1);
}

void CPU::run() {
    next();

    test1();
    finish = true;
}
