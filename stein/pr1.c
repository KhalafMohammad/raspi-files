#include <stdlib.h> 
#include <stdio.h> 
#include <stddef.h> 
#include <mariadb/mysql.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include "hfile.h"


void gpiowrite(int pin, int hor); // opens the value file of the output pin and writs 1 OR 0 
static int gpiodir(int pin, int di);//opens the direction file of the pin and wirites out or in 
void gpioex(int pin);//exports the given pin 
void gpiounex(int pin); //unexports the given pin 
static int gpioread(int pin);//reads the value in the value file of the input pin 
 


int main(){ 


//Converts ascii value to binary including space and everything
    struct input{
     int in[3]
     char janee[1];
    }
   int *dPtr = input;
   gpioex(21); 
   gpiodir(21,1);
   gpioex(20); 
   gpiodir(20,1); 
   gpioex(16); 
   gpiodir(16,1);

   while(1){ 
      printf("Iets veranderen. Y/N");
      scanf("%c" , janee);
      if(janee == N){
         sleep(2);
      } else if(janee == Y) {
         printf("Temp of stand veranderen. 1=Temp 2=stand");
         scanf("%d" , &in[0]);
         if(in[0] == 1) {
            prinf("Temp hoger of lager. 1=hoger 2=lager");
            scanf("%d" , &in[1]);
            if(in[1] == 1){
               gpiowrite(20, 1); 
               usleep(500000); 
               gpiowrite(20, 0);
            } else if(in[1] == 2){
               gpiowrite(20, 1);
               usleep(100 0000);
               gpiowrite(20, 0);
            }
         } else if(in[0] == 2) {
            printf("1=automatisch 2=open 3=dicht");
            scanf("%d" , &in[3]);
            if(in[2] == 1) {
               gpiowrite(20, 0); 
               gpiowrite(21, 0);
            } else if(in[2]== 2) {
               gpiowrite(20, 1); 
               gpiowrite(21, 0);
            } else if(in[2] == 1) {
               gpiowrite(20, 0); 
               gpiowrite(21, 1);
            } 
         }
      }
      dPtr = 0;
    } 
   return 0; 
}