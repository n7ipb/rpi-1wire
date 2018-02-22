/**
@file	 read_probe.c
@brief   Read a requested 1-wire temperature probe
@author  Ken Koster / N7IPB
@date	 2018/02/21
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
 *    Read the requested temperature probe
 *    Requires a symbolic link at /tmp/temps/<probename>
 *    for "/sys/bus/w1/devices/<deviceid>/w1_slave"
 * 
 *    A sample script to create the links is included with
 *    this program.
 * 
 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

int main( int argc, const char* argv[] )
{
    char devPath[128];
    char tempData[8];
    DIR *dir;
    struct dirent *ent;
    
    strcpy (devPath,"/tmp/temps/");
    
    if (argc != 2) {
        printf ("Usage: %s <probename> \n",argv[0]);
        printf ("Available probe names:\n");
        if ((dir = opendir (devPath)) != NULL) {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) {
                if (ent->d_type != DT_DIR) {
                    printf ("  %s\n", ent->d_name);
                }
            }
            closedir (dir);
        } else {
            /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }
        return -1;
    }
    
    strcat(devPath,argv[1]);
    
    int fd = open(devPath, O_RDONLY);
    if(fd == -1) {
        printf ("No such probe as %s\n",argv[1]);
        return 1;
    }
    
    char buf[256];
    read(fd, buf, 256); 
    strncpy(tempData, strstr(buf, "t=") + 2, 6);
    float tempC = strtof(tempData, NULL);
    printf("%.1f\n", (tempC / 1000) * 9 / 5 + 32);
    close(fd);
    return 0;
}
