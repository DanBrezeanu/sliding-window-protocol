all: send recv

link_emulator/lib.o:
	$(MAKE) -C link_emulator

send: send.o link_emulator/lib.o utils.o
	gcc -g send.o link_emulator/lib.o utils.o -o send

recv: recv.o link_emulator/lib.o utils.o
	gcc -g recv.o link_emulator/lib.o utils.o -o recv

.c.o:
	gcc -Wall -Werror -std=gnu99 -g -c $?

clean:
	$(MAKE) -C link_emulator clean
	-rm -f *.o send recv
