/*
	Read the requested temperature probe
	Requires a symbolic link at /tmp/temps/<probename>
	for "/sys/bus/w1/devices/<deviceid>/w1_slave"

	A sample script to create the links is included with
	this program.

	N7IPB - 2/21/2018
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
	printf ("Available probes:\n");
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
    if(fd == -1)
	{
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
