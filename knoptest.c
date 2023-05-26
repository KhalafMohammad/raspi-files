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

#define POUT 1 //dir out
#define PIN 0 //dir in
#define OFF 1 //led off
#define ON 0 //led on
#define button1 16 //button 1
#define button2 23 //button 2

struct stoplight{ //struct of stoplights

    int red;
    int yellow;
    int green;
};


int main(){

    struct stoplight stoplight1[] = {21,20,26}; //stoplight 1
    struct stoplight stoplight2[] = {22,27,17}; //stoplight 2

    //exports all required pins
    gpioex(button1);
    gpioex(button2);
    gpioex(stoplight1->red);
    gpioex(stoplight1->green);
    gpioex(stoplight1->yellow);
    gpioex(stoplight2->green);
    gpioex(stoplight2->yellow);
    gpioex(stoplight2->red);
    
    //sets all required pin to either output or input
    gpiodir(stoplight1->green, POUT);
    gpiodir(stoplight1->yellow, POUT);
    gpiodir(stoplight1->red, POUT);
    gpiodir(stoplight2->green, POUT);
    gpiodir(stoplight2->yellow, POUT);
    gpiodir(stoplight2->red, POUT);
    gpiodir(button1,PIN);
    gpiodir(button2,PIN);
   
    while(1){
        //reads value off of the input pins files
        int value1 = gpioread(16);
        int value2 = gpioread(23);
        printf("value1 and value2 :%d | %d\n", value1, value2); //prints values of input pins

    
        if (value1 == 1){
                // if button 1 is pressed then stoplight 1 goes ON 
            gpiowrite(stoplight1->green,ON);
            gpiowrite(stoplight1->yellow,OFF);
            gpiowrite(stoplight1->red,OFF);
            sleep(2);
            gpiowrite(stoplight1->green,OFF);
            gpiowrite(stoplight1->yellow,ON);
            gpiowrite(stoplight1->red,OFF);
            sleep(2);
            gpiowrite(stoplight1->green,OFF);
            gpiowrite(stoplight1->yellow,OFF);
            gpiowrite(stoplight1->red,ON);
            sleep(1);
        }else if(value2 == 1){
                // if button 2 is pressed then stoplight 2 goes ON 
            gpiowrite(stoplight2->green, ON);
            gpiowrite(stoplight2->yellow, OFF);
            gpiowrite(stoplight2->red, OFF);
            sleep(2);
            gpiowrite(stoplight2->green, OFF);
            gpiowrite(stoplight2->yellow, ON);
            gpiowrite(22, OFF);
            sleep(2);
            gpiowrite(stoplight2->green, OFF);
            gpiowrite(stoplight2->yellow, OFF);
            gpiowrite(stoplight2->red, ON);
            sleep(2);
        }
        else{
            //if no button is pressed then trun all off except red leds.
            gpiowrite(stoplight1->green, OFF);
            gpiowrite(stoplight1->yellow, OFF);
            gpiowrite(stoplight1->red, ON);

            gpiowrite(stoplight2->green, OFF);
            gpiowrite(stoplight2->yellow, OFF);
            gpiowrite(stoplight2->red, ON);
        }
        
        usleep(100000);
    }


    return 0;
}

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