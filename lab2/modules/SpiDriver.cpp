#include "../modules/SpiDriver.h"
#include "../modules/Cpu.h"

SpiDriver::SpiDriver(CPU & cpu, sc_uint<4> device_number) 
    : m_cpu(cpu), m_device_number(device_number) 
{} 

void SpiDriver::send(const std::vector<unsigned char> & bytes)
{
    sc_uint<32> ADDR(0xFFFF0001);
    ADDR.range(15, 12) = m_device_number;
    sc_uint<32> DATA;
    size_t j = 0;
    ADDR.range(6, 4) = 3;
    for (size_t i = bytes.size(); i >= 4; i -= 4) {
        DATA.range(31, 24) = bytes[j++];
        DATA.range(23, 16) = bytes[j++];
        DATA.range(15, 8)  = bytes[j++];
        DATA.range(7, 0)   = bytes[j++];
        m_cpu.write(ADDR, DATA);
        wait();
    }
    if (bytes.size() > 0) DATA.range(31, 24) = bytes[j++];
    if (bytes.size() > 1) DATA.range(23, 16) = bytes[j++]; 
    if (bytes.size() > 2) DATA.range(15, 8)  = bytes[j++]; 
    auto count = bytes.size() % 4;
    if (count != 0) { 
        ADDR.range(6, 4) = count - 1;
        m_cpu.write(ADDR, DATA);
        wait();
    }
}

void SpiDriver::wait() {
    sc_uint<32> ADDR(0xFFFF0000);
    ADDR.range(15, 12) = m_device_number;
    while(!m_cpu.read(0xFFFFF0008)[0]) m_cpu.next();
}
