all:
	gcc -Wall -Werror -std=gnu11 -O2 -lm Main.c -o main -Wno-unused-result

fsanitize:
	gcc -Wall -Werror -std=gnu11 -O2 -lm Main.c -o main -fsanitize=address -Wno-unused-result

gdb:
	gcc -Wall -Werror -std=gnu11 -O0 -lm Main.c -o main -Wno-unused-result -g

gdbSanitize:	
	gcc -Wall -Werror -std=gnu11 -O0 -lm Main.c -o main -Wno-unused-result -g -fsanitize=address
clean:
	rm ./*.txt

outClean:
	rm ./out*.txt