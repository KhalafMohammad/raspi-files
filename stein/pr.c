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



int main(){ 

   int in;
   int inn;
   int innn;
   int innnn;
   gpioex(21); 
   gpiodir(21,1);
   gpioex(20); 
   gpiodir(20,1); 
   gpioex(16); 
   gpiodir(16,1);

   while(1){ 
      printf("Iets veranderen. 1=Nee/2=Ja");
      scanf("%d" , &in);
      if(in == 1){
         sleep(2);
      } else if(in == 2) {
         printf("Temp of stand veranderen. 1=Temp 2=stand");
         scanf("%d" , &inn);
         if(inn == 1) {
            printf("Temp hoger of lager. 1=hoger 2=lager");
            scanf("%d" , &innn);
            if(innn == 1){
               gpiowrite(16, 1); //temp hoger
               usleep(500000); 
               gpiowrite(16, 0);
            } else if(innn == 2){
               gpiowrite(16, 1);//temp lager
               usleep(1000000);
               gpiowrite(16, 0);
            }
         } else if(inn == 2) { 
            printf("1=automatisch 2=open 3=dicht");
            scanf("%d" , &innnn);
            if(innnn == 1) {
               gpiowrite(20, 0);// automatisch
               gpiowrite(21, 0);
            } else if(innnn == 2) {
               gpiowrite(20, 1); 
               gpiowrite(21, 0);//open
            } else if(innnn == 1) {
               gpiowrite(20, 0);// dicht
               gpiowrite(21, 1);
            } 
         }
      }

} 
   return 0; 
}

#define IN 0

static int gpiodir(int pin, int di){

    static const char s_directions_str[]  = "in\0out";
    int fd;
	char path[50];
	sprintf (path, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if(fd == -1){

		fprintf(stderr,"faild to open directory dir.");
	}

    write(fd, &s_directions_str[IN == di ? 0 :3], IN == di ? 2: 3);


	close(fd);
    return 0;
}
//  https://www.w3schools.com/c/c_structs.php
//   https://www.programiz.com/c-programming/c-arrays
//  https://eng.libretexts.org/Courses/Delta_College/C___Programming_I_(McClanahan)/12%3A_Pointers/12.04%3A_Arrays%2C__Pointers_and_Such/12.4.01%3A_Pointer_to_an_Array_-_Array_Pointer#:~:text=Similarly%2C%20we%20can%20also%20declare,when%20talking%20about%20multidimensional%20arrays.https://eng.libretexts.org/Courses/Delta_College/C___Programming_I_(McClanahan)/12%3A_Pointers/12.04%3A_Arrays%2C__Pointers_and_Such/12.4.01%3A_Pointer_to_an_Array_-_Array_Pointer#:~:text=Similarly%2C%20we%20can%20also%20declare,when%20talking%20about%20multidimensional%20arrays.
