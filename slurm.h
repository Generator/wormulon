/******************************************************************************
 *
 * slurm.h - structs known from slurm and used in src/
 *
 ******************************************************************************
 * $Id: slurm.h,v 1.2 2003/07/16 16:52:56 hscholz Exp $
 *****************************************************************************/

#ifndef _SLURM_H_

typedef struct IfData{
    char if_name[10];       /* The device name given as start parameter*/
    int  if_speed;          /* The Interface speed */
    char if_speedstring[12]; /* the measuring unit like Mbit, kbit */
    int  if_id;             /* The ID which the interface inside the OS has */
    int  if_amount;         /* The amount of all interfaces available */
    int  if_valid;          /* 1 = selected interface exists
                             * 0 = interfaces does not exists */
} IfData;
IfData ifdata;

/* This structure stays the INFO variables */
typedef struct DataStats {
    unsigned long rx_packets;
    unsigned long rx_errors;
    int rx_over;
    unsigned long tx_packets;
    unsigned long tx_errors;
    int tx_over;
    double rx_bytes;
    double tx_bytes;
    double rx_bytes_comp;
    double tx_bytes_comp;
    double rx_packets_led;
    double tx_packets_led;
    unsigned long connect_time;
    unsigned long current_time;
    float top_speed;
    int online_days;
    int online_hour;
    int online_min;
    int online_sec;
    unsigned long rx_packets_off;
    unsigned long rx_errors_off;
    int rx_over_off;
    unsigned long tx_packets_off;
    unsigned long tx_errors_off;
    int tx_over_off;
    double rx_bytes_off;
    double tx_bytes_off;
    double rx_bytes_comp_off;
    double tx_bytes_comp_off;
} DataStats;
DataStats stats;

#endif
