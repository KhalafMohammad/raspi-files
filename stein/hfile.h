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


void gpiowrite(int pin, int hor){ 

 
int fd; 
 
 char path[50]; 
    sprintf(path, "/sys/class/gpio/gpio%d/value",pin);
    fd = open(path, O_WRONLY); 
    if(fd == -1){ 
    fprintf(stderr,"faild to open directory for write."); 
 } 

    char value[5]; 
    sprintf(value,"%d",hor); 
    write(fd, value, 1); 
 

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
    static const char s_directions_str[] = "in\0out"; 
 

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