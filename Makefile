obj-m +=scan.o
all:mod
	gcc main.c myqueue.c -o main
debug:mod
	gcc main.c myqueue.c mystat.c -D SUPER -o main -D
mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf main
