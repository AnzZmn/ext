file_explorer: main.o gterm.o hasht.o
	gcc -g -Wall -Wextra -O0 main.o gterm.o hasht.o -o file_explorer -lcurses -lutil

main.o: ./src/main.c
	gcc -c -Wall -Wextra -g  -O0 ./src/main.c -Iinclude -lcurses -o main.o

gterm.o: ./src/gterm.c
	gcc -c -Wall -Wextra -g  -O0 ./src/gterm.c -Iinclude -lcurses -o gterm.o -lutil

hasht.o: ./src/hasht.c
	gcc -c -Wall -Wextra -g  -O0 ./src/hasht.c -Iinclude  -o hasht.o -lutil -lutil

