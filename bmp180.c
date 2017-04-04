//
//  bmp180.c
//  BMP180
//
//  Created by Atakan. on 18/08/16.
//  Copyright © 2016 Atakan. All rights reserved.
//

#include "bmp180.h"

bmp180 bmp_180;

const uint16_t bmp180_coeff_addr[11] = {
    0xAAAB, 0xACAD, 0xAEAF, 0xB0B1, 0xB2B3,
    0xB4B5, 0xB6B7, 0xB8B9, 0xBABB, 0xBCBD,
    0xBEBF
};

void init_sensor(short mode) {
    init_twi();
    read_coeffs();
    bmp_180.mode = mode;
}

void calculate_temperature() {
    long UT, X1, X2, B5;
    
    UT = read_temperature();
    X1 = (UT - bmp_180.calib_coeffs.AC6) * bmp_180.calib_coeffs.AC5 / pow(2, 15);
    X2 = bmp_180.calib_coeffs.MC * pow(2, 11) / (X1 + bmp_180.calib_coeffs.MD);
    B5 = X1 + X2;
    bmp_180.calib_coeffs.B5 = B5;
    bmp_180.temperature = (B5 + 8) / pow(2, 4);
}

void calculate_pressure() {
    long UP, X1, X2, X3, B3, B6, _pressure;
    unsigned long B4, B7;
    
    UP = read_pressure();
    B6 = bmp_180.calib_coeffs.B5 - 4000;
    X1 = (bmp_180.calib_coeffs.B2 * (B6 * B6 >> 12)) >> 11;
    X2 = bmp_180.calib_coeffs.AC2 * B6 >> 11;
    X3 = X1 + X2;
    B3 = ((((long)bmp_180.calib_coeffs.AC1 * 4 + X3) << bmp_180.mode) + 2) / 4;
    X1 = bmp_180.calib_coeffs.AC3 * B6 >> 13;
    X2 = (bmp_180.calib_coeffs.B1 * (B6 * B6 >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = bmp_180.calib_coeffs.AC4 * (unsigned long) (X3 + 32768) >> 15;
    B7 = ((unsigned long) UP - B3) * (50000UL >> bmp_180.mode);
    if (B7 < 0x80000000UL)
        _pressure = (B7 * 2) / B4;
    else
        _pressure = (B7 / B4) * 2;
    X1 = (_pressure >> 8) * (_pressure >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * _pressure) >> 16;
    bmp_180.pressure = _pressure + ((X1 + X2 + 3791) >> 4);
}

void calculate_altitude() {
    bmp_180.altitude = (float) 44330 * (1 - pow(((float) bmp_180.pressure / pressure_sea_level), 0.1903));
}

long read_temperature() {
    short _temperature;
    
    start_transmission();
    send_addr(bmp180_write_addr);
    send_data(control_reg_addr);
    send_data(temperature_sel);
    end_transmission();
    
    _delay_ms(wait_temperature);
    
    start_transmission();
    send_addr(bmp180_write_addr);
    send_data(out_msb);
    send_data(out_lsb);
    restart();
    send_addr(bmp180_read_addr);
    _temperature = read_data(1) << 8;
    _temperature |= read_data(0);
    end_transmission();
    return (long) _temperature;
}

long read_pressure() {
    long UP;
    uint8_t msb, lsb, xlsb;
    
    start_transmission();
    send_addr(bmp180_write_addr);
    send_data(control_reg_addr);
    
    switch (bmp_180.mode) {
        default:
        case bmp180_mode_0:
            send_data(pressure_0_sel);
            end_transmission();
            _delay_ms(wait_pressure_mode_0);
            break;
        case bmp180_mode_1:
            send_data(pressure_1_sel);
            end_transmission();
            _delay_ms(wait_pressure_mode_1);
            break;
        case bmp180_mode_2:
            send_data(pressure_2_sel);
            end_transmission();
            _delay_ms(wait_pressure_mode_2);
            break;
        case bmp180_mode_3:
            send_data(pressure_3_sel);
            end_transmission();
            _delay_ms(wait_pressure_mode_3);
            break;
    }
    
    _delay_ms(1);
    start_transmission();
    send_addr(bmp180_write_addr);
    send_data(out_msb);
    send_data(out_lsb);
    restart();
    send_addr(bmp180_read_addr);
    msb = read_data(1);
    lsb = read_data(0);
    end_transmission();
    _delay_ms(1);
    start_transmission();
    send_addr(bmp180_write_addr);
    send_data(out_xlsb);
    restart();
    send_addr(bmp180_read_addr);
    xlsb = read_data(0);
    end_transmission();
    
    UP = ((unsigned long) msb << 16 | (unsigned long) lsb << 8 | (unsigned long) xlsb) >> (8 - bmp_180.mode);
    return (long) UP;
}

void read_coeffs() {
    
    uint16_t coeffs[11];
    
    for (short i = 0; i < 11; i++) {
        start_transmission();
        send_addr(bmp180_write_addr);
        send_data((bmp180_coeff_addr[i] & 0xFF00) >> 8);
        send_data(bmp180_coeff_addr[i] & 0x00FF);
        restart();
        send_addr(bmp180_read_addr);
        coeffs[i] = read_data(1) << 8;
        coeffs[i] |= read_data(0);
        end_transmission();
        _delay_ms(1);
    }
    
    bmp_180.calib_coeffs.AC1 = (short) coeffs[0];
    bmp_180.calib_coeffs.AC2 = (short) coeffs[1];
    bmp_180.calib_coeffs.AC3 = (short) coeffs[2];
    bmp_180.calib_coeffs.AC4 = (uint16_t) coeffs[3];
    bmp_180.calib_coeffs.AC5 = (uint16_t) coeffs[4];
    bmp_180.calib_coeffs.AC6 = (uint16_t) coeffs[5];
    bmp_180.calib_coeffs.B1 = (short) coeffs[6];
    bmp_180.calib_coeffs.B2 = (short) coeffs[7];
    bmp_180.calib_coeffs.MB = (short) coeffs[8];
    bmp_180.calib_coeffs.MC = (short) coeffs[9];
    bmp_180.calib_coeffs.MD = (short) coeffs[10];
    
}

void calculate() {
    calculate_temperature();
    calculate_pressure();
    calculate_altitude();
}

float get_temperature() {
    return (float) bmp_180.temperature / 10;
}
