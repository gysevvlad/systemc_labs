#pragma once

#include "../modules/SpiDriver.h"

class PmodOledDriver : public SpiDriver
{
    public:
        PmodOledDriver(CPU & cpu, size_t device_number)
            : SpiDriver(cpu, device_number) {}
        
        typedef unsigned char bitmap_t[32 * 16];

        void init();
        void sent_picture(bitmap_t & map);
        void send_command(const std::vector<unsigned char> & bytes);
        void send_data(const std::vector<unsigned char> & bytes);

    private:
        void setD();
        void setC();
};

extern std::vector<unsigned char> image1;
extern std::vector<unsigned char> image2;
