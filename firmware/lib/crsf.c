// CRSF implementation for XR1 Nano
#include "crsf.h"

void crsf_init(crsf_frame_t *p) {

    p->state = FSM_IDLE; //initialize state to idle
    p->crc = 0x00;       //initialize crc to 0

}

void crsf_crc_byte(crsf_frame_t *p, uint8_t new_byte, bool type) {

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

        case CRSF_FSM_IDLE:

            if(new_byte == CRSF_SYNC){

                p->state = FSM_LEN;

            }

            break;

        case CRSF_FSM_LEN: //check length of incoming frame and store in p->len

            //the smallest len is 2 bytes => no payload just type + crc
            if(new_byte >= 2 && new_byte <= 62){

                p->len = new_byte;
                p->crc = 0;
                p->state = FSM_TYPE;

            } else { //len is garbage so we resync

                p->state = FSM_IDLE;

            }

            break;

        case CRSF_FSM_TYPE: //if type == 0x16 then LEN must = 0x18 / 24 or ignore

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

        case CRSF_FSM_SKIP:

            if(p->count < p->len-1){

                p->count++;

            } else {

                p->state = FSM_IDLE;

            }

            break;

        case CRSF_FSM_READ_PAYLOAD:

            crsf_crc_byte(p, new_byte, false);
            
            if(p->count == p->len - 2){

                p->state = FSM_CRC;
            }        

            break;

        case CRSF_FSM_CRC: //compare calculated crc with CRC8

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

void payload_to_channel(crsf_frame_t *p){

    for (size_t i = 0; i < CRSF_CHANNELS; i++)
    {
        
        uint16_t total = 0;

        for (size_t j = 0; j < 11; j++)
        {
            uint8_t pos = i*11 + j;
            uint8_t byte_index = pos / 8;
            uint8_t bit_index = pos % 8;

            uint8_t bit_value = (p->payload[byte_index] >> bit_index) & 1;

            total |= bit_value << j;

        }
        
        p->channels[i] = total;

    }
    
}

