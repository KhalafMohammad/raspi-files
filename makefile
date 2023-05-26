# -*- MakeFile -*-

# READ FIRST

#target table: dependencies that the exicutable file depends ON.
#	the action that will make the file.[gcc]

# via [make all] it will make all the exicutable file at once, and via [make clear] it will delete every exicutable.

all: gpiodb server
	

gpiodb: header.h gpiodb.c
	gcc -Wall -Wextra -Wconversion -o gpiodb gpiodb.c header.c header.h $$(mariadb_config --include --libs)

server: header.h server.c
	gcc -Wall -Wextra -Wconversion -o server server.c header.c header.h $$(mariadb_config --include --libs)
	
clear:
	rm server gpiodb
