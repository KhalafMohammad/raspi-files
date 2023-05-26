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

int *raamcheck(MYSQL *connection);
void raamcontrol(int *columnvalue);
static int gpiodir(int pin, int di);
// #define test

typedef struct stoplight
{
	int red;
	int yellow;
	int green;
}STOPLIGHT;


int main() {



#ifdef test

 	printf("\nstoplight 1 is %d,%d,%d\n", stoplight1[0].red, stoplight1[0].yellow, stoplight1[0].green);
    printf("\nstoplight 2 is %d,%d,%d\n", stoplight1[1].red, stoplight1[1].yellow, stoplight1[1].green);
#endif

	MYSQL *connection = mysql_init(NULL);
	mysql_real_connect(connection, "localhost", "pipo", "theclown", "mijndb", 0, NULL, 0);
	if(connection == NULL) {
		printf("Kan geen verbinding met de MariaDB server maken \n");
	exit(-1);
	}

	gpioex(21);
	gpioex(20);
	gpioex(16);

	// sets pins as output
	gpiodir(21,1);
	gpiodir(20,1);
	gpiodir(16,1);

	
	while(1){

		int * columnvalue =  raamcheck(connection);
		raamcontrol(columnvalue);

		free(columnvalue);

		sleep(5);

	}
	
	gpiounex(21);
	gpiounex(20);
	gpiounex(16);

	
	mysql_close(connection);
	return 0;
}

int *raamcheck(MYSQL *connection){

	int *columnvalue = malloc(6*sizeof(int));
	int rows = 0;
	// int columnvalue[rows];
		// checks the value of the column LEDstate
	if (mysql_query(connection, "select LEDstate from LED LIMIT 6")) {
    	printf("%s\n", mysql_error(connection));
	}
	MYSQL_ROW row;

	//stores the results in the pointer resutl
	MYSQL_RES *result = mysql_use_result(connection);
	if(result == NULL){
		printf("%s\n", mysql_error(connection));
	}
	// fetches the results in row 
	while((row = mysql_fetch_row(result)))
	{
		for (int j = 0; j < 6; j++)
		{
			columnvalue[rows] = atoi(row[0]);
		}
			rows++;

		
		mysql_data_seek(result,0);
	}
		
#ifdef test
	for (int i = 0; i < 6; i++)
	{	
					
		printf("column value is: %d\n", columnvalue[i]);
	}
#endif


	mysql_free_result(result);// frees the results

	return columnvalue;
}

void raamcontrol(int *columnvalue){
	
	struct stoplight stoplight1[]= {{21, 20, 16}};

	int *column = malloc(6*sizeof(columnvalue));

	for (int c = 0; c < 6; c++)
	{
		column[c] = columnvalue[c];
		printf("col %d is %d\n", c, column[c]);
		
	}
	printf("\n");

#ifdef test
	for (int t = 0; t < 6; t++)
	{
		printf("colllllllll is %d\n", column[c]);
	}
	
#endif

	if(column[0] == 1){

		//if columnvalue is other than 1 sets pin21 valuse to 1

		gpiowrite(stoplight1[0].green, 1); //temp hoger
        usleep(500000); 
        gpiowrite(stoplight1[0].green, 0);

	}else if(column[0] != 1){
	
		//if columnvalue is other than 1 sets pin21 valuse to 0

		    	gpiowrite(stoplight1[0].green, 1);//temp lager
               	usleep(1000000);
               	gpiowrite(stoplight1[0].green, 0);
	}
	if(column[1] == 1){

		//if columnvalue is equal to 1, then sets pin17 to 1
		gpiowrite(stoplight1[0].yellow, 0);// automatisch
        gpiowrite(stoplight1[0].red, 0);

	}//else if(column[1] != 1){
	
	// 	//if columnvalue is other than 1 sets pin17 valuse to 0
	// 	gpiowrite(stoplight1[1].green,0);
		
	//}
	if(column[2] == 1){

		//if columnvalue is equal to 1, then sets pin20 to 1.
		gpiowrite(stoplight1[0].yellow, 1); 
        gpiowrite(stoplight1[0].red, 0);//open
			
	}//else if(column[2] != 1){
	
		//if columnvalue is other than 1 sets pin20 valuse to 0
	// 	gpiowrite(stoplight1[0].yellow,0);
	// }
	if(column[3] == 1){

		//if columnvalue is equal to 1, then sets pin27 to 1.
		gpiowrite(stoplight1[0].yellow, 0);// dicht
        gpiowrite(stoplight1[0].red, 1);

	}
	// else if(column[3] != 1){
	
	// 	//if columnvalue is other than 1 sets pin27 valuse to 0
	// 	gpiowrite(stoplight1[1].yellow,0);
		
	// }
	// if(column[4] == 1){

	// 	//if columnvalue is equal to 1, then sets pin16 to 1.
	// 	gpiowrite(stoplight1[0].green,1);
		
	// }else if(column[4] != 1){
	
	// 	//if columnvalue is other than 1 sets pin16 valuse to 0

	// 	gpiowrite(stoplight1[0].green,0);
		
	// }

	// if(column[5] == 1){

	// 	//if columnvalue is equal to 1, then sets pin22 to 1.

	// 	gpiowrite(stoplight1[1].red,1);
			
		
		
	// }else if(column[5] != 1){
	
	// 	//if columnvalue is other than 1 sets pin22 valuse to 0
	// 	gpiowrite(stoplight1[1].red,0);
		
	// }
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