//
//  i2c.h
//  BMP180
//
//  Created by Atakan on 19/08/16.
//  Copyright © 2016 Atakan. All rights reserved.
//

#ifndef i2c_h
#define i2c_h
#include <avr/io.h>

// Master twi transmitter status codes

#define TWI_START_SENT      0x08
#define TWI_R_START_SENT    0x10
#define TWI_SLAVE_W_ACK     0x18
#define TWI_SLAVE_W_NACK    0x20
#define TWI_DATA_SENT_ACK   0x28
#define TWI_DATA_SENT_NACK  0x30
#define TWI_ARBIT_LOST      0x38

// Master twi receiver status codes

#define TWI_SLAVE_R_ACK     0x40
#define TWI_SLAVE_R_NACK    0x48
#define TWI_DATA_RECV_ACK   0x50
#define TWI_DATA_RECV_NACK  0x58

extern void init_twi(void);
extern void start_transmission(void);
extern void end_transmission(void);
extern void terminate(void);
extern void send_addr(uint8_t addr);
extern void send_data(uint8_t data);
extern uint8_t read_data(short);

#endif /* i2c_h */
