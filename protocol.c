#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <mariadb/mysql.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "header.h"

void gpiowrite(int pin, int hor); // opens the value file of the output pin and writs 1 OR 0
static int gpiodir(int pin, int di);//opens the direction file of the pin and wirites out or in
void gpioex(int pin);//exports the given pin
void gpiounex(int pin); //unexports the given pin
static int gpioread(int pin);//reads the value in the value file of the input pin

// int main(){
//     gpioex(21);
//     gpiodir(21,1);
//     gpioex(16);
//     gpiodir(16,1);
    
//     while(1){
//         gpiowrite(20, 0);
//         gpiowrite(21, 0);
//         sleep(1);
//         gpiowrite(16, 1);
//         sleep(1);
//         gpiowrite(21, 1);
//         sleep(1);
//         gpiowrite(16, 0);
//         sleep(1);
//         gpiowrite(20, 1);
//         usleep(100000);
//         gpiowrite(20, 0);
//     // int value2 = gpioread(21);
//     // printf("value1 and value2: %d\n", value2); //prints values of input pins
//     // usleep(100000);

//     }
    
//     return 0;
// }

// Raspberry Pi code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
int main()
{
    int fd;                                       // file descriptor for serial device
    // char buffer[256];                             // buffer for data
    int n;                                        // number of bytes read or written
    int value;
    struct termios options;                       // options for serial communication
    fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY); // open serial device
    if (fd == -1)
    {
        perror("Unable to open serial device");
        exit(1);
    }
    tcgetattr(fd, &options);
    // get current options
    cfsetispeed(&options, B9600);               // set input baud rate
    cfsetospeed(&options, B9600);               // set output baud rate
    options.c_cflag &= ~PARENB;                 // disable parity
    options.c_cflag &= ~CSTOPB;                 // set one stop bit
    options.c_cflag &= ~CSIZE;                  // mask data size
    options.c_cflag |= CS8;                     // set 8 bits per character
    options.c_cflag |= CREAD | CLOCAL;          // enable receiver and ignore modem control lines
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // disable software flow control
    options.c_oflag &= ~OPOST;

    // disable output processing
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // set raw mode
    options.c_cc[VMIN] = 4;          // set minimum number of characters to read
    options.c_cc[VTIME] = 0;          // set timeout in deciseconds
    tcsetattr(fd, TCSANOW, &options); // set new options
    fcntl(fd, F_SETFL, 0);
    while (1)
    {
        // printf("Enter a message: ");
        // fgets(buffer, sizeof(buffer), stdin);
        // // get user input
        // n = write(fd, buffer, strlen(buffer));
        // // write to serial device
        // if (n < 0)
        // {
        //     perror("Write failed");
        //     exit(1);
        // }
        // printf("Sent %d bytes\n", n);


        n = read(fd, &value, sizeof(int));
        // read from serial device
        // value = atoi(buffer);
        if (n < 0)
        {
            perror("Read failed");
            exit(1);
        }
        // buffer[n] = '\0';
        
        // terminate string
        printf("Received %d bytes: %d\n", n, value);


        // if (read(fd, buffer, sizeof(buffer)) > 0)
        // {
        //     char* token = strtok(buffer, "\n");
        //     while (token != NULL)
        //     {
        //         value = atoi(token);
        //         printf("Received value: %d\n", value);
        //         token = strtok(NULL, "\n");
        //     }
        // }

    }
    close(fd);
    // close serial device
    return 0;
}



// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <termios.h>

// int main()
// {
//     int fd;
//     struct termios options;
//     char buf[255];

//     fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
//     if (fd == -1)
//     {
//         perror("open_port: Unable to open /dev/ttyS0 - ");
//         return 1;
//     }

//     fcntl(fd, F_SETFL, 0);

//     tcgetattr(fd, &options);
//     cfsetispeed(&options, B9600);
//     cfsetospeed(&options, B9600);
//     options.c_cflag |= (CLOCAL | CREAD);
//     // options.c_cflag &= ~PARENB;
//     // options.c_cflag &= ~CSTOPB;
//     // options.c_cflag &= ~CSIZE;
//     options.c_cflag |= CS8;
//     // options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//     // options.c_iflag &= ~(IXON | IXOFF | IXANY);
//     // options.c_oflag &= ~OPOST;

//     options.c_cflag &= ~(tcflag_t)PARENB;
//     options.c_cflag &= ~(tcflag_t)CSTOPB;
//     options.c_cflag &= ~(tcflag_t)CSIZE;
//     options.c_lflag &= ~(tcflag_t)(ICANON | ECHO | ECHOE | ISIG);
//     options.c_iflag &= ~(tcflag_t)(IXON | IXOFF | IXANY);
//     options.c_oflag &= ~(tcflag_t)OPOST;

//     tcsetattr(fd, TCSANOW, &options);

//     while (1)
//     {
//         int n = read(fd, buf, sizeof(buf));
//         if (n > 0)
//         {
//             buf[n] = '\0';
//             printf("Received %d bytes: %c\n", n, buf);
//             if (strcmp(buf, "X") == 0)
//             {
//                 printf("Received X!\n");
//                 // system("python /home/thomvdv/relay-test.py");
//             }
//         }
//     }

//     close(fd);
//     return 0;
// }





void gpiowrite(int pin, int hor){

	int fd;
	char path[50];
	sprintf (path, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if(fd == -1){

		fprintf(stderr,"faild to open directory for write.");
	}

	char value[5];
	sprintf(value,"%d",hor);
    write(fd,  value, 1);


	close(fd);
}

void gpioex(int pin){

    int fd;
    char path[56];
    sprintf(path,"/sys/class/gpio/export");
    fd = open(path, O_WRONLY);
    if(fd == -1){

		fprintf(stderr,"faild to open directory for export.");
	}

    char value[16];
    sprintf(value, "%d", pin);
    write(fd, value, 2);
    close(fd);

}

void gpiounex(int pin){


    int fd;
    char path[56];
    sprintf(path,"/sys/class/gpio/unexport");
    fd = open(path, O_WRONLY);
    if(fd == -1){

		fprintf(stderr,"faild to open directory for unexport.");
	}

    char value[16];
    sprintf(value, "%d", pin);
    write(fd, value, 2);
    close(fd);
}


#define IN 0
static int gpiodir(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int gpioread(int pin) 
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str)); //atoi converts char string to intger and return returns the value of the dir
}