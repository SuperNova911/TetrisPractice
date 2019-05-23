obj-m = led_driver.o push_switch_driver.o dot_matrix_driver.o

PWD = $(shell pwd)

all: driver app

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
	gcc -o tetris tetris.c -Wall -static

game:
	gcc Game.c Block.c BlockBag.c Map.c Point.c Tetris.c TickTimer.c driverManager.c -o Game -Wall -static

clean:
	rm -rf *.ko *.mod.* *.o Module.symvers modules.order .led* .push_switch* .dot_matrix* .tmp*
