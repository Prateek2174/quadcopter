// CRSF implementation for XR1 Nano
//get the crsf frame and use a state machine to parse it

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

//Use constants for the CRSF frame structure
#define FRAME_LENGTH_BYTES 26
#define PAYLOAD_LENGTH_BYTES 22
#define CHANNELS 16
#define BITS_PER_CHANNEL 11

#define CRSF_SYNC 0xC8
#define CRSF_LEN  0x18
#define CRSF_TYPE 0x16

#define CRSF_TYPE_LINK_STATS 0x14
#define CRSF_TYPE_BATTERY    0x08
#define CRSF_TYPE_GPS        0x02
#define CRSF_TYPE_ATTITUDE   0x1E
#define CRSF_TYPE_PING       0x28
#define CRSF_TYPE_INFO       0x29

// Define the CRSF frame structure for state machine

typedef enum {

    FSM_IDLE,         //wait till you get the SYNC byte
    FSM_LEN,          //make sure LEN byte = 0x18
    FSM_TYPE,         //check type
    FSM_SKIP,
    FSM_READ_PAYLOAD, //go through and read the payload by channel
    FSM_CRC

} crsf_state_t;

//struct to hold the CRSF frame data

typedef struct {

    crsf_state_t state;
    uint8_t crc;
    uint8_t len; //length = type + payload + crc
    uint8_t count;
    uint8_t type;
    uint8_t payload[PAYLOAD_LENGTH_BYTES];

    //need a 16 element array w/ length 11 bits


} crsf_frame_t;

void crsf_init(crsf_frame_t *p) {

    p->state = FSM_IDLE; //initialize state to idle
    p->crc = 0x00;       //initialize crc to 0

}

void crsf_crc_byte(crsf_frame_t *p, uint8_t new_byte, bool type) {


    //     crc = crc XOR byte
    //     repeat 8 times:
    //         if the top bit of crc is 1:
    //             crc = (crc shifted left by 1) XOR 0xD5
    //         else:
    //             crc = crc shifted left by 1

    p->crc ^= new_byte;
    uint8_t temp = p->crc;

    //store the byte

    if(!type){

        p->payload[p->count++] = new_byte;

    }


    for (size_t i = 0; i < 8; i++) {


        if (temp >> 7 & 1) {

            temp = (temp << 1) ^ 0xD5;

        } else {

            temp = temp << 1;

        }
        
    }
    
    p->crc = temp;
}

bool crsf_byte_handler(crsf_frame_t *p, uint8_t new_byte) {

    //given the next byte handle what should happen based on current state
    switch (p -> state){

        case FSM_IDLE:

            if(new_byte == CRSF_SYNC){

                p->state = FSM_LEN;

            }

            break;

        case FSM_LEN: //check length of incoming frame and store in p->len

            //the smallest len is 2 bytes => no payload just type + crc
            if(new_byte >= 2 && new_byte <= 62){


                p->len = new_byte;
                p->crc = 0;
                p->state = FSM_TYPE;

            } else { //len is garbage so we resync

                p->state = FSM_IDLE;

            }



            break;

        case FSM_TYPE: //if type == 0x16 then LEN must = 0x18 / 24 or ignore

            p->type = new_byte;
            
            if(p->type == CRSF_TYPE && p->len == CRSF_LEN){

                p->state = FSM_READ_PAYLOAD;
                p->crc = 0;
                p->count = 0;
                crsf_crc_byte(p, new_byte, true); //crc covers type


            } else{

                p->count = 1;
                p->state = FSM_SKIP;
            }


            break;

        case FSM_SKIP:

            if(p->count < p->len-1){

                p->count++;

            } else {

                p->state = FSM_IDLE;

            }

            break;

        case FSM_READ_PAYLOAD:

            crsf_crc_byte(p, new_byte, false);
            
            if(p->count == p->len - 2){

                p->state = FSM_CRC;
            }
            

            break;

        case FSM_CRC: //compare calculated crc with CRC8

            if (p->crc == new_byte)
            {
                p->state = FSM_IDLE;
                return true;
            }

            p->state = FSM_IDLE;
            
            break;

    }
    return false;
}

//computer CRC throughout the READ_PAYLOAD state
//make sure to reset the calculated CRC at the end

int main(void) {

    crsf_frame_t p;
    crsf_init(&p);

    //test frame
    const uint8_t frame[26] = {
        0xC8, 0x18, 0x16, 0xAC, 0x98, 0x38, 0x77, 0xE9, 0x33, 0x71,
        0xF0, 0x81, 0x0F, 0x7C, 0xE0, 0x03, 0x1F, 0xF8, 0xC0, 0x07,
        0x3E, 0xF0, 0x81, 0x0F, 0x7C, 0x7F
    };

    const uint8_t bad_frame[26] = {
        0xC8, 0x18, 0x16, 0xAC, 0x98, 0x38, 0x77, 0xE9, 0x33, 0x71,
        0xF1, 0x81, 0x0F, 0x7C, 0xE0, 0x03, 0x1F, 0xF8, 0xC0, 0x07,
        0x3E, 0xF0, 0x81, 0x0F, 0x7C, 0x7F
    };

    const uint8_t stream[67] = {
        0x00, 0xFF, 0xC8, 0x40, 0x12,
        0xC8, 0x18, 0x16, 0xAC, 0x98, 0x38, 0x77, 0xE9, 0x33, 0x71,
        0xF0, 0x81, 0x0F, 0x7C, 0xE0, 0x03, 0x1F, 0xF8, 0xC0, 0x07,
        0x3E, 0xF0, 0x81, 0x0F, 0x7C, 0x7F,
        0xC8, 0x18, 0x16, 0xAC, 0x98, 0x38, 0x77, 0xE9, 0x33, 0x71,
        0xC8, 0x18, 0x16, 0xAC, 0x98, 0x38, 0x77, 0xE9, 0x33, 0x71,
        0xF0, 0x81, 0x0F, 0x7C, 0xE0, 0x03, 0x1F, 0xF8, 0xC0, 0x07,
        0x3E, 0xF0, 0x81, 0x0F, 0x7C, 0x7F
    };

    int count = 0;

    for (size_t i = 0; i < sizeof(stream); i++) 
    {
        if(crsf_byte_handler(&p, stream[i]))
        {
            count++;
            printf("%zu \n", i);
        }
        
    }

    printf("number of good frames = %d", count);
    

    return 0;

}