#pragma once
#include <Arduino.h>
#include <SPI.h>


//MDR0 options
#define non_quad 0x0
#define x1 0x1
#define x2 0x2
#define x4 0x3
#define free_run 0x0
#define single_cycle 0x4
#define range_limit 0x8
#define modulo_n 0xC
#define disable_index 0x00
#define CNTR_load 0x10
#define CNTR_reset 0x20
#define OTR_load 0x30
#define async 0x00
#define sync 0x40
#define filter_1 0x00
#define filter_2 0x80


//MDR1 options
#define byte_4 0x0
#define byte_3 0x1
#define byte_2 0x2
#define byte_1 0x3
#define count_ena 0x0
#define count_dis 0x4
#define nop 0x00
#define flag_IDX 0x10
#define flag_CMP 0x20
#define flag_BW 0x40
#define flag_CY 0x80


//Constant SPI settings
#define bitOrder MSBFIRST
#define spiMode SPI_MODE0


//A library for interfacing with the LS7366R quadrature encoder IC for Arudino.

class LS7366R_Lib{
    public:

        LS7366R_Lib(int chip);
        void begin();  //Prepares the LS7366R for communication with the previously set user parameters. (set_MDR0, set_MDR1, set_Speed)

        void clear_MDR0();
        void clear_MDR1();
        void clear_CNTR();
        void clear_STR();

        uint8_t get_MDR0();
        uint8_t get_MDR1();
        uint32_t get_CNTR();
        uint32_t get_OTR();
        uint8_t get_STR();

        void set_MDR0(byte quad_mode, byte count_mode, byte index_mode, byte sync_mode, byte filter_mode);
        void set_MDR1(byte byte_mode, byte ena_count, byte idx, byte cmp, byte bw, byte cy);
        void set_DTR(uint32_t value);

        void load_CNTR();
        void load_OTR();

        void set_Speed(int spd);

    private:

        int spiSpeed;  //SPI communication speed in hz.
        int chipSelect;

};