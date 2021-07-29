obj-m +=scan.o
CFLAGS += -D SUPER
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc $(CFLAGS) main.c myqueue.c mystat.c -o main
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf main
