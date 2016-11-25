#pragma once

#include <systemc.h>

template <unsigned int device_number>
SC_MODULE(bus_t)         
{
    sc_in< bool >         clk_in;
    sc_out< bool >        clk_out;

    sc_in< bool >         hwrite_in;
    sc_out< bool >        hwrite_out;    

    sc_in< sc_uint<32> >  haddr_in;
    sc_out< sc_uint<32> > haddr_out;

    sc_in< sc_uint<32> >  hwdata_in;
    sc_out< sc_uint<32> > hwdata_out;

    sc_out< sc_uint<32> > hrdata_out;
    sc_in< sc_uint<32> >  hrdata_s[device_number];

    sc_out< bool >        hsel_s[device_number];

    void clk_assign()    { clk_out.write(clk_in.read()); }
    void hwrite_assign() { hwrite_out.write(hwrite_in.read()); }
    void haddr_assign()  { haddr_out.write(haddr_in.read()); }
    void hwdata_assign() { hwdata_out.write(hwdata_in.read()); }

    void select_device() {
        // [0x00000000, 0x0000FFFF]     
        sc_uint<32> addr(haddr_in.read());
        int selected_device = -1;
        if (addr >= 0xFFFF0000) 
            selected_device = (addr & 0x0000FFFF) >> 12;
        else 
            selected_device = -1;

        //std::cout << "selected device: " << selected_device + 1 << std::endl;

        for ( unsigned int i = 0; i < device_number; ++i)
            hsel_s[i].write(i != selected_device);

        if (selected_device < device_number && selected_device != -1)
            hrdata_out.write(hrdata_s[selected_device].read());
        else 
            hrdata_out.write(sc_uint<32>(0));
    }

    SC_CTOR(bus_t) {
        SC_METHOD(clk_assign);    sensitive << clk_in;
        SC_METHOD(hwrite_assign); sensitive << hwrite_in;
        SC_METHOD(haddr_assign);  sensitive << haddr_in;
        SC_METHOD(hwdata_assign); sensitive << hwdata_in;

        SC_METHOD(select_device); 
        sensitive << haddr_in;
        for (unsigned int i = 0; i < device_number; ++i)
            sensitive << hrdata_s[i];
    }
};
