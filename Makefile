obj-m +=scan.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc main.c myqueue.c mystat.c -D SUPER -o main
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf main
