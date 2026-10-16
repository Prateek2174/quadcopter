// CRSF implementation
//get the crsf frame and use a state machine to parse it

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

//Use constants for the CRSF frame structure
#define FRAME_LENGTH_BYTES 26
#define PAYLOAD_LENGTH_BYTES 22
#define CHANNELS 16
#define BITS_PER_CHANNEL 11

#define SYNC 0xC8
#define LEN  0x18
#define TYPE 0x16

#define TYPE_LINK_STATS 0x14
#define TYPE_BATTERY 0x08
#define TYPE_GPS 0x02
#define TYPE_ATTITUDE 0x1E
#define TYPE_PING 0x28
#define TYPE_INFO 0x29

// Define the CRSF frame structure for state machine

typedef enum {

    FSM_IDLE,         //wait till you get the SYNC byte
    FSM_LEN,          //make sure LEN byte = 0x18
    FSM_TYPE,         //check type
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

void crsf_crc_byte(crsf_frame_t *p, uint8_t new_byte) {


    //     crc = crc XOR byte
    //     repeat 8 times:
    //         if the top bit of crc is 1:
    //             crc = (crc shifted left by 1) XOR 0xD5
    //         else:
    //             crc = crc shifted left by 1

    p->crc ^= new_byte;
    uint8_t temp = p->crc;

    //store the byte

    p->payload[p->count++] = new_byte;

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

            if(new_byte == SYNC){

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
            
            if(p->type == TYPE && p->len == LEN){

                p->state = FSM_READ_PAYLOAD;
                p->crc = 0;
                p->count = 0;
                crsf_byte_handler(p, new_byte); //crc covers type


            } else{

                p->state = FSM_IDLE;
            }


            break;

        case FSM_READ_PAYLOAD:

            crsf_byte_handler(p, new_byte);
            
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
            
            break;

    }
    return false;
}

//computer CRC throughout the READ_PAYLOAD state
//make sure to reset the calculated CRC at the end

int main(void) {

    crsf_frame_t p;
    crsf_init(&p);

    while(true){



    }

    return 0;

}