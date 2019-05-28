obj-m = led_driver.o push_switch_driver.o dot_matrix_driver.o

PWD = $(shell pwd)

all: driver app

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

game:
	gcc Game.c Block.c BlockBag.c Control.c Map.c Point.c Tetris.c TickTimer.c DeviceManager.c -o Game -Wall -static -lm -pthread

clean:
	rm -rf *.ko *.mod.* *.o Module.symvers modules.order .led* .push_switch* .dot_matrix* .tmp* Game
