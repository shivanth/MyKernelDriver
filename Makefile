obj-m += mine.o

modules:
	make -C ../linux-2.6/ M=$(PWD)  modules

clean:
	make -C ../linux-2.6/ M=$(PWD)  clean 