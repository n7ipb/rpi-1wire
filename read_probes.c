/**
@file    read_probes.c
@brief   List all attached DS18B20 temperature probes and their current values
@author  Unknown - Heavily modified by Ken Koster / N7IPB
@date    2018/02/21
This is used by SvxLink and snmpd to read DS18B20 temperature probes.

\verbatim
Copyright (C) 2018 Ken Koster / N7IPB
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/
/*
 *  Origin of original code lost - N7IPB 2/21/2018
 * 
 */
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// struct to hold ds18b20 data for linked list
struct ds18b20 {
    char devPath[128];
    char devID[16];
    char tempData[6];
    struct ds18b20 *next;
};

// Find connected 1-wire devices. 1-wire driver creates entries for each device 
int8_t findDevices(struct ds18b20 *d) {
    DIR *dir;
    struct dirent *dirent;  
    struct ds18b20 *newDev;
    char path[] = "/sys/bus/w1/devices";
    int8_t i = 0;
    dir = opendir(path);
    if (dir != NULL) {
        while ((dirent = readdir(dir))) {
            // 1-wire devices are links beginning with 28-
            if (dirent->d_type == DT_LNK &&
                strstr(dirent->d_name, "28-") != NULL) {
                newDev = malloc( sizeof(struct ds18b20) );
            strcpy(newDev->devID, dirent->d_name);
            // Assemble path to OneWire device
            sprintf(newDev->devPath, "%s/%s/w1_slave", path, newDev->devID);
            i++;
            newDev->next = 0;
            d->next = newDev;
            d = d->next;
                }
        }
        (void) closedir(dir);
    }
    else
    {
        perror ("Couldn't open the w1 devices directory");
        return 1;
    }
    return i;
}

int8_t readTemp(struct ds18b20 *d) {
    while(d->next != NULL){
        d = d->next;
        int fd = open(d->devPath, O_RDONLY);
        if(fd == -1) {
            perror ("Couldn't open the w1 device.");
            return 1;
        }
        char buf[256];
        ssize_t numRead;
        while((numRead = read(fd, buf, 256)) > 0) {
            strncpy(d->tempData, strstr(buf, "t=") + 2, 5);
            float tempC = strtof(d->tempData, NULL);
            printf("Device: %s  - ", d->devID);
            printf("Temp: %.3f C  ", tempC / 1000);
            printf("%.3f F\n", (tempC / 1000) * 9 / 5 + 32);
        }
        close(fd);
    }
    return 0;
}

int main (void) {
    struct ds18b20 *rootNode;
    struct ds18b20 *devNode;
    rootNode = malloc( sizeof(struct ds18b20) );
    devNode = rootNode;
    int8_t devCnt = findDevices(devNode);
    printf("\nFound %d devices\n\n", devCnt);
    free(rootNode);
    while(1) {
        rootNode = malloc( sizeof(struct ds18b20) );
        devNode = rootNode;
        devCnt = findDevices(devNode);
        readTemp(rootNode);
        while(rootNode) {
            devNode = rootNode;
            rootNode = devNode->next;
            free(devNode);
        }
        free(rootNode); 
        printf("\n");
    }
    return 0;
}
