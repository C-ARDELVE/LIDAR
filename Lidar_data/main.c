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

FILE *
read_lidar_pkt(FILE *file, lidar_pkt_t *pkt)
{
    int data;

    do {
        data = fgetc(file);
    } while ((data != EOF) && (data != lidar_start));
    
    if (data == EOF)
        goto exit;
    
    //
    // Line up wit start so we can pick up the whole packet.
    //
    fseek(file, -1, SEEK_CUR);
    
    fread(pkt, sizeof(lidar_pkt_t), 1, file);

exit:
    return (file);
}

void
display_data(FILE *file)
{
    lidar_pkt_t pkt;
    
    while (!feof(file)) {
        file = read_lidar_pkt(file, &pkt);
        printf("start = 0x%02x index = 0x%02x ", pkt.start, pkt.index);
        printf("speed = 0x%04x ", pkt.speed);
        printf("invalid = 0x%01x ", pkt.distance.bits.invalid);
        printf("strength = 0x%01x ", pkt.distance.bits.strength);
        printf("distance = 0x%04x ", pkt.distance.bits.distance);
        printf("signal = 0x%x ", pkt.signal);
        printf("checksum = 0x%x\n", pkt.signal);

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