/**************************************************************************
 * LS3766R_Lib
 *
 * Copyright (C) 2024 Natalie Fredericks
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "LS7366R_Lib.h"

//Constructor
LS7366R_Lib::LS7366R_Lib(int cs){
    spiSpeed = 4000000;
    chipSelect = cs;
}

void LS7366R_Lib::begin()
{
    pinMode(chipSelect, OUTPUT);
    SPISettings comSettings(spiSpeed, bitOrder, spiMode);
    SPI.begin();
    SPI.beginTransaction(comSettings);
    digitalWrite(chipSelect, HIGH);
}

void LS7366R_Lib::clear_MDR0()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B00001000);
    digitalWrite(chipSelect, HIGH);
}

void LS7366R_Lib::clear_MDR1()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B00010000);
    digitalWrite(chipSelect, HIGH);
    
}

void LS7366R_Lib::clear_CNTR()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B00100000);
    digitalWrite(chipSelect, HIGH);
}

void LS7366R_Lib::clear_STR()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B00110000);
    digitalWrite(chipSelect, HIGH);
}

uint8_t LS7366R_Lib::get_MDR0()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B01001000);
    uint8_t recieved = SPI.transfer(0x00);
    return recieved;
}

uint8_t LS7366R_Lib::get_MDR1()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B01010000);
    uint8_t recieved = SPI.transfer(0x00);
    return recieved;
}

uint32_t LS7366R_Lib::get_CNTR()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B01100000);
    uint32_t recieved = recieve_bytes();
    digitalWrite(chipSelect, HIGH);
    return recieved;
}

uint32_t LS7366R_Lib::get_OTR()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B01101000);
    uint32_t recieved = recieve_bytes();
    digitalWrite(chipSelect, HIGH);
    return recieved;
}

uint8_t LS7366R_Lib::get_STR()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B01110000);
    uint8_t recieved = SPI.transfer(0x00);
    return recieved;
}

void LS7366R_Lib::set_MDR0(byte quad_mode, byte count_mode, byte index_mode, byte sync_mode, byte filter_mode)
{
    byte message = quad_mode + count_mode + index_mode + sync_mode + filter_mode;
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B10001000);
    SPI.transfer(message);
    digitalWrite(chipSelect, HIGH);
}

void LS7366R_Lib::set_MDR1(byte byte_mode, byte ena_count, byte idx, byte cmp, byte bw, byte cy)
{
    byte message = byte_mode + ena_count + idx + cmp + bw + cy;
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B10010000);
    SPI.transfer(message);
    digitalWrite(chipSelect, HIGH);
    addressSize = byte_mode;
}

void LS7366R_Lib::set_DTR(uint32_t value)
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B10011000);

    switch(addressSize){
        case byte_4:
            SPI.transfer((value >> 24) & 0xFF);
            SPI.transfer((value >> 16) & 0xFF);
            SPI.transfer((value >> 8) & 0xFF);
            SPI.transfer(value & 0xFF);
            break;
        case byte_3:
            SPI.transfer((value >> 16) & 0xFF);
            SPI.transfer((value >> 8) & 0xFF);
            SPI.transfer(value & 0xFF);
            break;
        case byte_2:
            SPI.transfer((value >> 8) & 0xFF);
            SPI.transfer(value & 0xFF);
            break;
        case byte_1:
            SPI.transfer(value & 0xFF);
            break;
        default:
            SPI.transfer((value >> 24) & 0xFF);
            SPI.transfer((value >> 16) & 0xFF);
            SPI.transfer((value >> 8) & 0xFF);
            SPI.transfer(value & 0xFF);
            break;
    }
    digitalWrite(chipSelect, HIGH);
}


void LS7366R_Lib::load_CNTR()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B11100000);
    digitalWrite(chipSelect, HIGH);
}

void LS7366R_Lib::load_OTR()
{
    digitalWrite(chipSelect, LOW);
    SPI.transfer(B11101000);
    digitalWrite(chipSelect, HIGH);
}

void LS7366R_Lib::set_Speed(int spd)
{
    spiSpeed = spd;
}


uint32_t LS7366R_Lib::recieve_bytes()
{
    uint32_t recieved = 0;

    byte n0 = 0x00;
    byte n1 = 0x00;
    byte n2 = 0x00;
    byte n3 = 0x00;

    switch(addressSize){
    case byte_4:
            n0 = SPI.transfer(0x00);
            n1 = SPI.transfer(0x00);
            n2 = SPI.transfer(0x00);
            n3 = SPI.transfer(0x00);
            recieved = ((uint32_t)n0 << 24) | ((uint32_t)n1 << 16) | ((uint32_t)n2 << 8) | (uint32_t)n3;
        break;
    case byte_3:
            n0 = SPI.transfer(0x00);
            n1 = SPI.transfer(0x00);
            n2 = SPI.transfer(0x00);
            recieved = ((uint32_t)n0 << 16) | ((uint32_t)n1 << 8) | (uint32_t)n2;
        break;
    case byte_2:
            n0 = SPI.transfer(0x00);
            n1 = SPI.transfer(0x00);
            recieved = ((uint32_t)n0 << 8) | (uint32_t)n1;
        break;
    case byte_1:
            n0 = SPI.transfer(0x00);
            recieved = (uint32_t)n0;
        break;
    default:
            n0 = SPI.transfer(0x00);
            n1 = SPI.transfer(0x00);
            n2 = SPI.transfer(0x00);
            n3 = SPI.transfer(0x00);
            recieved = ((uint32_t)n0 << 24) | ((uint32_t)n1 << 16) | ((uint32_t)n2 << 8) | (uint32_t)n3;
        break;
    }
    return recieved;
}
