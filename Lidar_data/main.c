//
//  main.c
//  Lidar_data
//
//  Created by Francesco DiMambro on 12/21/15.
//  Copyright © 2015 ardelve. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "lidar_data.h"

/* Flag set by ‘--verbose’. */
static int verbose_flag;

static struct option long_options[] =
{
    /* These options set a flag. */
    {"verbose", no_argument, &verbose_flag, 1},
    {"brief",   no_argument, &verbose_flag, 0},
    /* These options don’t set a flag.
     We distinguish them by their indices. */
    {"file", required_argument, 0, 'f'},
    {0, 0, 0, 0}
};

uint16_t
chk_sum(uint8_t *data, size_t length)
{
    uint32_t sum;
    uint16_t data16;
    int i;
    
    sum = 0;
    if (length & 1) {
        length -= 1;
        sum = (*data << 8);
        data++;
    }
    
    for (i = 0; i < (length / 2); i++) {
        data16 = (data[2 * i] + (data[(2 * i) + 1] << 8));
        sum = (sum << 1) + data16;
    }
    sum = (sum & 0x7FFF) + (sum >> 15);
    sum = (sum & 0x7FFF);
    return (sum);
}

FILE *
read_lidar_pkt(FILE *file, lidar_pkt_t *pkt)
{
    int data;
    uint8_t *pkt_ptr;

    do {
        data = fgetc(file);
    } while ((data != EOF) && (data != lidar_start));
    
    if (data == EOF)
        goto exit;
    
    pkt_ptr = (uint8_t *)pkt;
    
    *pkt_ptr = (uint8_t)data;
    pkt_ptr++;
    
    fread(pkt_ptr, (sizeof(lidar_pkt_t) - sizeof(uint8_t)), 1, file);

exit:
    return (file);
}

void
display_data(FILE *file)
{
    lidar_pkt_t pkt;
    uint16_t checksum;
    int i;
    
    while (!feof(file)) {
        file = read_lidar_pkt(file, &pkt);
        if (feof(file))
            break;
        printf("start = 0x%02x index = 0x%02x ", pkt.start, pkt.index);
        printf("speed = 0x%04x\n", pkt.speed);
        for (i = 0; i < lidar_data_sz; i++) {
            printf("data[%d]\n", i);
            printf("invalid = 0x%01x ", pkt.data[i].distance.bits.invalid);
            printf("strength = 0x%01x ", pkt.data[i].distance.bits.strength);
            printf("distance = 0x%04x ", pkt.data[i].distance.bits.distance);
            printf("signal = 0x%04x ", pkt.data[i].signal);
        }
        printf("checksum = 0x%04x ", pkt.checksum);
        checksum = chk_sum((uint8_t *)&pkt, sizeof(pkt) - sizeof(pkt.checksum));
        if (checksum != pkt.checksum)
            printf("expected 0x%04x", checksum);
        printf("\n");
    }
}

int
main (int argc, char **argv)
{
    int c;
    char *file_name;
    FILE *data_file;
    
    file_name = NULL;
    
    while (1) {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "f:",
                         long_options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        
        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg) {
                    printf (" with arg %s", optarg);
                    file_name = optarg;
                }
                printf ("\n");
                break;
                
            case 'f':
                printf ("option -f with value `%s'\n", optarg);
                file_name = optarg;
                break;
                
            case '?':
                /* getopt_long already printed an error message. */
                break;
                
            default:
                abort ();
        }
    }
    
    /* Instead of reporting ‘--verbose’
     and ‘--brief’ as they are encountered,
     we report the final status resulting from them. */
    if (verbose_flag)
        puts ("verbose flag is set");
    
    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        putchar ('\n');
    }
    
    if (file_name == NULL)
        goto exit;
    
    data_file = fopen(file_name, "r");
    
    if (data_file == NULL)
        goto exit;
    
    display_data(data_file);
    
exit:
    exit (0);
}