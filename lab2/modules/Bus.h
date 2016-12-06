#pragma once

#include <systemc.h>

template <unsigned int device_number>
SC_MODULE(bus_t)         
{
    sc_in< sc_uint<32> >  HADDR;
    sc_in< sc_uint<32> >  HRDATA_S[device_number];
    sc_out< sc_uint<32> > HRDATA; 
    sc_out< bool >        HSEL_S[device_number];

    void select() {
        sc_uint<32> addr(HADDR.read());
        int selected_device = -1;
        if (addr >= 0xFFFF0000) 
            selected_device = (addr & 0x0000FFFF) >> 12;
        else 
            selected_device = -1;

        for ( unsigned int i = 0; i < device_number; ++i)
            HSEL_S[i].write(i != selected_device);

        if (selected_device < device_number && selected_device != -1)
            HRDATA.write(HRDATA_S[selected_device].read());
        else 
            HRDATA.write(sc_uint<32>(0));
    }

    SC_CTOR(bus_t) {
        SC_METHOD(select); 
        sensitive << HADDR.value_changed();
        for (unsigned int i = 0; i < device_number; ++i)
            sensitive << HRDATA_S[i].value_changed();
    }
};
