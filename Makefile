include setup.conf

full: clean warp load

warp:
	mkdir -p build/ksdk1.1/work
	mkdir -p build/ksdk1.1/work/boards/Warp
	mkdir -p build/ksdk1.1/work/demos/Warp/src
	mkdir -p build/ksdk1.1/work/demos/Warp/armgcc/Warp
	cp -r tools/sdk/ksdk1.1.0/*					build/ksdk1.1/work
	cp src/boot/ksdk1.1.0/SEGGER*					build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/config.h					build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/boot.c					build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/errstrs*					build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/powermodes.c				build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/warp.h					build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/startup_MKL03Z4.S				build/ksdk1.1/work/platform/startup/MKL03Z4/gcc/startup_MKL03Z4.S
	cp src/boot/ksdk1.1.0/gpio_pins.c				build/ksdk1.1/work/boards/Warp
	cp src/boot/ksdk1.1.0/gpio_pins.h				build/ksdk1.1/work/boards/Warp
	cp src/boot/ksdk1.1.0/config.h					build/ksdk1.1/work/boards/Warp
	cp src/boot/ksdk1.1.0/CMakeLists.txt				build/ksdk1.1/work/demos/Warp/armgcc/Warp/
	cp src/boot/ksdk1.1.0/devMMA8451Q.*				build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/devSI7021.*				build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/devTCS34725.*				build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/devSI4705.*				build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/devRV8803C7.*				build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/devSSD1331.*				build/ksdk1.1/work/demos/Warp/src/
	cp src/boot/ksdk1.1.0/devINA219.*				build/ksdk1.1/work/demos/Warp/src/
	cd build/ksdk1.1/work/lib/ksdk_platform_lib/armgcc/KL03Z4 && ./clean.sh; ./build_release.sh
	cd build/ksdk1.1/work/demos/Warp/armgcc/Warp && ./clean.sh; ./build_release.sh
	@echo "\n\nNow, run\n\n\tmake load-warp\n\n"

load:
	$(JLINKPATH)/JLinkExe -device MKL03Z32XXX4 -if SWD -speed 10000 -CommanderScript tools/scripts/warp.jlink.commands

client:
	$(JLINKPATH)/JLinkRTTClient

clean:
	rm -rf build/ksdk1.1/work
