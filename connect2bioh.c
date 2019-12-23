#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h> 
int main() 
{
int i = 0; 
int fd; 
const char *device="/dev/rfcomm0"; 
fd=open(device,O_RDWR | O_NOCTTY | O_NDELAY); //opening serial port 
if(fd==-1) 
{ 
	printf("Failed to open port\n");
} 
else 
{ 
	write(fd,"\x02\x0B\x00\x00\x03",5); //asking for serial number 
	fcntl(fd,F_SETFL,0); //settings for reading 
	int buf [100]; 
	int n = read (fd,buf,sizeof(buf)); //reading the port 
	printf("%d \n", n);
	for(i=0; i< 100; i++)
	{
		printf("value %d",buf[i]); //printing the response message 
	}
}
close(fd); //closing the port 
}

