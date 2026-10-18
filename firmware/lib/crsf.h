#ifndef CRSF_H
#define CRSF_H

#include <stdbool.h>
#include <stdint.h>

#define CRSF_PAYLOAD_LENGTH_BYTES 22
#define CRSF_CHANNELS 16

#define CRSF_SYNC 0xC8
#define CRSF_LEN  0x18
#define CRSF_TYPE 0x16

#define CRSF_TYPE_LINK_STATS 0x14
#define CRSF_TYPE_BATTERY    0x08
#define CRSF_TYPE_GPS        0x02
#define CRSF_TYPE_ATTITUDE   0x1E
#define CRSF_TYPE_PING       0x28
#define CRSF_TYPE_INFO       0x29

//channel sum check
#define CRSF_CH_0 172
#define CRSF_CH_1 1811
#define CRSF_CH_2 1500
#define CRSF_CH_3 500
#define CRSF_CH_4 1811
#define CRSF_CH_5 992

typedef enum {

    CRSF_FSM_IDLE,         //wait till you get the SYNC byte
    CRSF_FSM_LEN,          //make sure LEN byte = 0x18
    CRSF_FSM_TYPE,         //check type
    CRSF_FSM_SKIP,
    CRSF_FSM_READ_PAYLOAD, //go through and read the payload by channel
    CRSF_FSM_CRC

} crsf_state_t;

//struct to hold the CRSF frame data

typedef struct {

    crsf_state_t state;
    uint8_t crc;
    uint8_t len; //length = type + payload + crc
    uint8_t count;
    uint8_t type;
    uint8_t payload[CRSF_PAYLOAD_LENGTH_BYTES];

    //need a 16 element array w/ length 11 bits
    uint16_t channels[CRSF_CHANNELS];

} crsf_frame_t;

void crsf_init(crsf_frame_t *p);
void crsf_crc_byte(crsf_frame_t *p, uint8_t new_byte, bool type);
bool crsf_byte_handler(crsf_frame_t *p, uint8_t new_byte);
void crsf_payload_to_channel(crsf_frame_t *p);

#endif