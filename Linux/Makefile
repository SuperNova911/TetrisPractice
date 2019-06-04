obj-m = led_driver.o push_switch_driver.o dot_matrix_driver.o

PWD = $(shell pwd)

all: driver game

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

game:
	gcc Game.c Block.c BlockBag.c Control.c DotAnimator.c Map.c Point.c Tetris.c TickTimer.c DeviceManager.c -o Game -Wall -static -lm -pthread

clean:
	rm -rf *.ko *.mod.* *.o Module.symvers modules.order .led* .push_switch* .dot_matrix* .tmp* Game

install:
	mknod /dev/csemad_led c 260 0; mknod /dev/csemad_push_switch c 265 0; mknod /dev/csemad_dot_matrix c 262 0; insmod led_driver.ko; insmod push_switch_driver.ko; insmod dot_matrix_driver.ko; lsmod
