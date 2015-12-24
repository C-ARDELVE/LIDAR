//
//  lidar_data.h
//  Lidar_data
//
//  Created by Francesco DiMambro on 12/21/15.
//  Copyright © 2015 ardelve. All rights reserved.
//

#ifndef lidar_data_h
#define lidar_data_h

enum {
    lidar_start = 0xfa
};

#pragma pack(1)

//
// Using bit fields make sure stiff lines up correctly, lo to hi, hi to low....etc.
//
typedef struct distance_s {
    uint16_t value;
    struct {
        uint16_t distance:14;
        uint16_t strength:1;
        uint16_t invalid:1;
    } bits;
} distance_t;

typedef struct lidar_pkt_s {
    uint8_t start;
    uint8_t index;
    uint16_t speed;      // Note this is little endian read, needs swap for big endian host.
    distance_t distance;
    uint16_t signal;
    uint16_t checksum;
} lidar_pkt_t;

#pragma pack()

#endif /* lidar_data_h */
