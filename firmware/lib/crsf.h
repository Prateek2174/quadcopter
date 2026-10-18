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