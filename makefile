all:
	gcc -Wall -Werror -std=gnu11 -O2 -lm Main.c -o main -Wno-unused-result
release:
	./release.sh
	gcc -Wall -Werror -Wpedantic -std=gnu11 -O2 -pipe -static -s -lm __Ruzza_Autostrada_API_20-07-2023.c -o main

fsanitize:
	gcc -Wall -Werror -std=gnu11 -O2 -lm Main.c -o main -fsanitize=address -Wno-unused-result
valgrind:
	rm ./callgrind.*
	valgrind --tool=callgrind ./main < Test/open_99.txt > dump.txt
	rm dump.txt
gdb:
	gcc -Wall -Werror -std=gnu11 -O0 -lm Main.c -o main -Wno-unused-result -g

gdbSanitize:	
	gcc -Wall -Werror -std=gnu11 -O0 -lm Main.c -o main -Wno-unused-result -g -fsanitize=address

clean:
	rm ./*.txt

outClean:
	rm ./out*.txt

test:
	./test_Main.sh