obj-m +=scan.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	CFLAGS += -D SUPER
	gcc $(CFLAGS) main.c myqueue.c mystat.c -o main
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf main
