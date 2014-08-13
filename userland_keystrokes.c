/*
 * Example of keystrokes sent from userland. Rafael Zurita 2014
 * Useful to work with on screen keyboard 
 * The fb on screen keyboard would send keystrokes using uinput driver
 * There is already a kernel driver, so we use it in this example
 * (the kernel driver is uinput)
 *
 * This example sends KEY_D events to the system from a user program. 
 * During 20 seconds
 *
 * Based on http://thiemonge.org/getting-started-with-uinput
 *
 * Usage: 
 *   gcc -o userland_keystrokes. userland_keystrokes.c 
 *   modprobe uinput
 *   ./userland_keystrokes
 */

#define UINPUT_MAX_NAME_SIZE    80
#include <fcntl.h>
#include <linux/ioctl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

void main(void){
	int fd;
	/* Set up a fake keyboard device */
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK); // or /dev/input/uinput
	ioctl(fd, UI_SET_EVBIT, EV_KEY);

	/* Send an event */
	int ret;
	ret = ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ret = ioctl(fd, UI_SET_EVBIT, EV_SYN);
	ret = ioctl(fd, UI_SET_KEYBIT, KEY_D);

	struct uinput_user_dev {
	    char name[UINPUT_MAX_NAME_SIZE];
	    struct input_id id;
       	 int ff_effects_max;
       	 int absmax[ABS_MAX + 1];
       	 int absmin[ABS_MAX + 1];
       	 int absfuzz[ABS_MAX + 1];
       	 int absflat[ABS_MAX + 1];
	};

	struct uinput_user_dev uidev;
	memset(&uidev, 0, sizeof(uidev));

	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1234;
	uidev.id.product = 0xfedc;
	uidev.id.version = 1;

	ret = write(fd, &uidev, sizeof(uidev));
	ret = ioctl(fd, UI_DEV_CREATE);


	struct input_event ev;

	memset(&ev, 0, sizeof(ev));

	int i;
	for (i=0;i<20;i++) {
		ev.type = EV_KEY;
		ev.code = KEY_D;
		ev.value = 1;

		ret = write(fd, &ev, sizeof(ev));
		sleep(1);

		ev.value = 0;
		ret = write(fd, &ev, sizeof(ev));
	}

	/* Clean up */
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
}
