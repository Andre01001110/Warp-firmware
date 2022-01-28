# Introduction
This branch contains code that runs on the FRDM-KL03Z platform. It also requires that you have an OELD breakout board using the SSD1331 Driver Chip attached. The purpose of this code is to read the acceleration from the built-in accelerometer and then visualise this on the OLED with a meter. This is particularly useful for applications where the value of the acceleration is not helpful.

**Prerequisites:** You need an arm cross-compiler such as `arm-none-eabi-gcc` installed as well as a working `cmake` (installed, e.g., via `apt-get` on Linux or via [MacPorts](https://www.macports.org) on macOS). On Ubuntu, the package you need is `gcc-arm-none-eabi`. You will also need an installed copy of the SEGGER [JLink commander](https://www.segger.com/downloads/jlink/), `JlinkExe`, which is available for Linux, macOS, and Windows (here are direct links for downloading it for [macOS](https://www.segger.com/downloads/jlink/JLink_MacOSX.pkg), and [Linux tgz 64-bit](https://www.segger.com/downloads/jlink/JLink_Linux_x86_64.tgz)).

## 1.  Compiling the Warp firmware
First, edit [setup.conf](setup.conf) to set the variable `ARMGCC_DIR` and `JLINKPATH`. If your `arm-none-eabi-gcc` is in `/usr/local/bin/arm-none-eabi-gcc`, then you want to set  `ARMGCC_DIR` to `/usr/local`. In the following, this `README.md` will refer to the top of the repository as `$TREEROOT`.

Second, edit [`tools/scripts/glaux.jlink.commands`](tools/scripts/glaux.jlink.commands) and [`tools/scripts/warp.jlink.commands`](tools/scripts/warp.jlink.commands) to replace `<full-path-to-warp-firmware>` with the full path to your Warp firmware directory.

You can erase any existing build, begin a new build, and load it onto to board with a single command

	make full

The build process copies files from `src/boot/ksdk1.1.0/` into the `build/`, builds, and converts the binary to SREC. See `Warp/src/boot/ksdk1.1.0/README.md` for more. _When editing source, edit the files in `src/boot/ksdk1.1.0/`, not the files in `build` location, since the latter are overwritten during each build._

To connect to the running hardware to see output, you will need two terminal windows. In a separate shell window from the one in which you ran `make load-warp` (or its variants), launch the JLink RTT client<sup>&nbsp;<a href="#Notes">See note 1 below</a></sup>:

	JLinkRTTClient

## 2. Using the Warp firmware on the Freescale FRDMKL03 Board
The SEGGER firmware allows you to use SEGGER’s JLink software to load your own firmware to the board, even without using their specialized JLink programming cables. You can find the SEGGER firmware at the SEGGER Page for [OpenSDA firmware](https://www.segger.com/products/debug-probes/j-link/models/other-j-links/opensda-sda-v2/).

To build the Warp firmware for the FRDM KL03, you will need to modify [this line in `src/boot/ksdk1.1.0/config.h`](https://github.com/physical-computation/Warp-firmware/blob/9e7f9e5e3f3c039cc98cbd1e6dfeb6b8fd78c86a/src/boot/ksdk1.1.0/config.h#L55).


## 3.  Editing the firmware
The firmware is currently all in `src/boot/ksdk1.1.0/`, in particular, see `src/boot/ksdk1.1.0/boot.c` and the per-sensor drivers in `src/boot/ksdk1.1.0/dev*.[c,h]`.

The firmware builds on the Kinetis SDK. You can find more documentation on the Kinetis SDK in the document [doc/Kinetis SDK v.1.1 API Reference Manual.pdf](https://github.com/physical-computation/Warp-firmware/blob/master/doc/Kinetis%20SDK%20v.1.1%20API%20Reference%20Manual.pdf).

The boot.c file contains an interactable menu that allows the user to control what the board does. The MMA8451Q file contains functions pertaining to the accelerometer, including it's initialisation function. The SSD1331 file contains similar functions.

The particular functions relevant to the operation of this device as a force visualiser are the displayArrow() function in boot.c and the two functions used within, getSensorDataMMA8451Q(&x, &y, &z) and devSSD1331arrow(x, y, z), from the MMA8451Q and SSD1331 files respectively.

The function you will be most likely to want to modify is devSSD1331arrow(x, y, z). This function receives the raw sensor data and performs all the neccesary processing and drawing functions. The drawing functions have the greatest computational cost and care should be taken to minimise their impact on the programme.

## 4.  Interacting with the boot menu
When the firmware boots, you will be dropped into a menu with a rich set of commands. The Warp boot menu allows you to conduct most of the experiments you will likely need without modifying the firmware:
````
[ *				        Warp (HW revision C)    				* ]
[  				      Cambridge / Physcomplab   				  ]

	Supply=1800mV,	Default Target Read Register=0x00
	I2C=200kb/s,	SPI=10000kb/s,	UART=115200b/s,	I2C Pull-Up=0

	SIM->SCGC6=0x20000001		RTC->SR=0x10		RTC->TSR=0x5687131D
	MCG_C1=0x42			MCG_C2=0x00		MCG_S=0x06
	MCG_SC=0x00			MCG_MC=0x00		OSC_CR=0x00
	SMC_PMPROT=0x22			SMC_PMCTRL=0x40		SCB->SCR=0x00
	PMC_REGSC=0x00			SIM_SCGC4=0xF0400030	RTC->TPR=0x651F

	0s in RTC Handler to-date,	0 Pmgr Errors
Select:
- 'a': set default sensor.
- 'b': set I2C baud rate.
- 'c': set SPI baud rate.
- 'd': set UART baud rate.
- 'e': set default register address.
- 'f': write byte to sensor.
- 'i': set pull-up enable value.
- 'j': repeat read reg 0x00 on sensor #0.
- 'k': sleep until reset.
- 'l': send repeated byte on I2C.
- 'm': send repeated byte on SPI.
- 'n': enable sensor supply voltage.
- 'o': disable sensor supply voltage.
- 'p': switch to VLPR mode.
- 'r': switch to RUN mode.
- 't': dump processor state.
- 'u': set I2C address.
- 'x': disable SWD and spin for 10 secs.
- 'y': OLED Menu.
- 'z': dump all sensor data.
Enter selection>
````
### Double echo characters
By default on Unix, you will likely see characters you enter shown twice. To avoid this, do the following:
- Make sure you are running `bash` (and not `csh`)
- Execute `stty -echo` at the command line in the terminal window in which you will run the `JLinkRTTClient`.

### Introduction to using the menu and testing the OLED
The menu contains all the functions you need for the device to work out of the box. Of particular note is the OLED Menu, which can be accessed with the `y` key. You should fine the following menu.
```
	Select:
	- '0' Clear Screen
	- 'a' Enable Arrow
	- 'r' Red Rectangle
	- 'g' Green Rectangle
	- 'b' Blue Rectangle
	- 'y' Initialise OLED
	Enter selection> 
```
There are built-in programmes to change the colour of the screen to Red, Green, or Blue - which you may use to test that your hardware is configured correctly. Commands are also included to clear (turn off) the screen and re-initialise it. On powerup, and when initilaised again, the screen will display bright green.

*NOTE: In many cases, the menu expects you to type a fixed number of characters (e.g., 0000 or 0009 for zero and nine)<sup>&nbsp;<a href="#Notes">See note 1 below</a></sup>. If using the `JLinkRTTClient`, the menu interface eats your characters as you type them, and you should not hit RETURN after typing in text. On the other hand, if using `telnet` you have to hit return.*

If you see repeated characters, you can set your terminal to not echo typed characters using `stty -echo`.

----

## Warp Acknowledgements

### If you use Warp in your research, please cite it as:
Phillip Stanley-Marbell and Martin Rinard. “A Hardware Platform for Efficient Multi-Modal Sensing with Adaptive Approximation”. ArXiv e-prints (2018). arXiv:1804.09241.

**BibTeX:**
```
@ARTICLE{1804.09241,
	author = {Stanley-Marbell, Phillip and Rinard, Martin},
	title = {A Hardware Platform for Efficient Multi-Modal 
	Sensing with Adaptive Approximation},
	journal = {ArXiv e-prints},
	archivePrefix = {arXiv},
	eprint = {1804.09241},
	year = 2018,
}
```
Phillip Stanley-Marbell and Martin Rinard. “Warp: A Hardware Platform for Efficient Multi-Modal Sensing with Adaptive Approximation”. IEEE Micro, Volume 40 , Issue 1 , Jan.-Feb. 2020.

**BibTeX:**
```
@ARTICLE{8959350,
	author = {P. {Stanley-Marbell} and M. {Rinard}},
	title = {Warp: A Hardware Platform for Efficient Multi-Modal
	Sensing with Adaptive Approximation},
	journal = {IEEE Micro},
	year = {2020},
	volume = {40},
	number = {1},
	pages = {57-66},
	ISSN = {1937-4143},
	month = {Jan},
}
```
### Acknowledgements
This research is supported by an Alan Turing Institute award TU/B/000096 under EPSRC grant EP/N510129/1, by Royal Society grant RG170136, and by EPSRC grants EP/P001246/1 and EP/R022534/1.

----
### Notes
<sup>1</sup>&nbsp; On some Unix platforms, the `JLinkRTTClient` has a double echo of characters you type in. You can prevent this by configuring your terminal program to not echo the characters you type. To achieve this on `bash`, use `stty -echo` from the terminal. Alternatively, rather than using the `JLinkRTTClient`, you can use a `telnet` program: `telnet localhost 19021`. This avoids the JLink RTT Client's "double echo" behavior but you will then need a carriage return (&crarr;) for your input to be sent to the board. Also see [Python SEGGER RTT library from Square, Inc.](https://github.com/square/pylink/blob/master/examples/rtt.py) (thanks to [Thomas Garry](https://github.com/tidge27) for the pointer).
