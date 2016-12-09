#pragma once

#include <systemc.h>

struct CPU;

class SpiDriver {

    protected:
        CPU & m_cpu;
        sc_uint<4> m_device_number;

    public:    
        SpiDriver(CPU & cpu, sc_uint<4> device_number);
        void send(const std::vector<unsigned char> & bytes);
        void wait();
};
