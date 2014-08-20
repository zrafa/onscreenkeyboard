
/*
 * osk.c : framebuffer on-screen keyboard
 * 
 * Copyright (C) 2014 Rafael Ignacio Zurita <rafaelignacio.zurita@gmail.com>
 *
 * Based on : http://thiemonge.org/getting-started-with-uinput
 * and www.cubieforums.com/index.php?topic=33.0
 * and fb_drawimage() from https://svn.mcs.anl.gov/repos/ZeptoOS/trunk/BGP/packages/busybox/src/miscutils/fbsplash.c 
 *
 *   osk.c is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    You should have received a copy of the GNU Library General Public
 *    License along with this software (check COPYING); if not, write to the Free
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */



#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/ioctl.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#define MOUSEFILE "/dev/input/mouse0"

#define UMBRAL 10

/* key codes REFERENCE : from /usr/include/linux/input.h
#define KEY_Q			16
#define KEY_W			17
#define KEY_E			18
#define KEY_R			19
#define KEY_T			20
#define KEY_Y			21
#define KEY_U			22
#define KEY_I			23
#define KEY_O			24
#define KEY_P			25
#define KEY_LEFTBRACE		26
#define KEY_RIGHTBRACE		27
#define KEY_ENTER		28
#define KEY_LEFTCTRL		29
#define KEY_A			30
#define KEY_S			31
#define KEY_D			32
#define KEY_F			33
#define KEY_G			34
#define KEY_H			35
#define KEY_J			36
#define KEY_K			37
#define KEY_L			38
#define KEY_SEMICOLON		39
#define KEY_APOSTROPHE		40
#define KEY_GRAVE		41
#define KEY_LEFTSHIFT		42
#define KEY_BACKSLASH		43
#define KEY_Z			44
#define KEY_X			45
#define KEY_C			46
#define KEY_V			47
#define KEY_B			48
#define KEY_N			49
#define KEY_M			50



#define KEY_RESERVED		0
#define KEY_ESC			1
#define KEY_1			2
#define KEY_2			3
#define KEY_3			4
#define KEY_4			5
#define KEY_5			6
#define KEY_6			7
#define KEY_7			8
#define KEY_8			9
#define KEY_9			10
#define KEY_0			11
#define KEY_MINUS		12
#define KEY_EQUAL		13
#define KEY_BACKSPACE		14
#define KEY_TAB			15
#define KEY_Q			16
#define KEY_W			17
#define KEY_E			18
#define KEY_R			19
#define KEY_T			20
#define KEY_Y			21
#define KEY_U			22
#define KEY_I			23
#define KEY_O			24
#define KEY_P			25
#define KEY_LEFTBRACE		26
#define KEY_RIGHTBRACE		27
#define KEY_ENTER		28
#define KEY_LEFTCTRL		29
#define KEY_A			30
#define KEY_S			31
#define KEY_D			32
#define KEY_F			33
#define KEY_G			34
#define KEY_H			35
#define KEY_J			36
#define KEY_K			37
#define KEY_L			38
#define KEY_SEMICOLON		39
#define KEY_APOSTROPHE		40
#define KEY_GRAVE		41
#define KEY_LEFTSHIFT		42
#define KEY_BACKSLASH		43
#define KEY_Z			44
#define KEY_X			45
#define KEY_C			46
#define KEY_V			47
#define KEY_B			48
#define KEY_N			49
#define KEY_M			50
#define KEY_COMMA		51
#define KEY_DOT			52
#define KEY_SLASH		53
#define KEY_RIGHTSHIFT		54
#define KEY_KPASTERISK		55
#define KEY_LEFTALT		56
#define KEY_SPACE		57
#define KEY_CAPSLOCK		58
#define KEY_F1			59
#define KEY_F2			60
#define KEY_F3			61
#define KEY_F4			62
#define KEY_F5			63
#define KEY_F6			64
#define KEY_F7			65
#define KEY_F8			66
#define KEY_F9			67
#define KEY_F10			68
#define KEY_NUMLOCK		69
#define KEY_SCROLLLOCK		70
#define KEY_KP7			71
#define KEY_KP8			72
#define KEY_KP9			73
#define KEY_KPMINUS		74
#define KEY_KP4			75
#define KEY_KP5			76
#define KEY_KP6			77
#define KEY_KPPLUS		78
#define KEY_KP1			79
#define KEY_KP2			80
#define KEY_KP3			81
#define KEY_KP0			82
#define KEY_KPDOT		83

#define KEY_ZENKAKUHANKAKU	85
#define KEY_102ND		86
#define KEY_F11			87
#define KEY_F12			88
#define KEY_RO			89
#define KEY_KATAKANA		90
#define KEY_HIRAGANA		91
#define KEY_HENKAN		92
#define KEY_KATAKANAHIRAGANA	93
#define KEY_MUHENKAN		94
#define KEY_KPJPCOMMA		95
#define KEY_KPENTER		96
#define KEY_RIGHTCTRL		97
#define KEY_KPSLASH		98
#define KEY_SYSRQ		99
#define KEY_RIGHTALT		100
#define KEY_LINEFEED		101
#define KEY_HOME		102
#define KEY_UP			103
#define KEY_PAGEUP		104
#define KEY_LEFT		105
#define KEY_RIGHT		106
#define KEY_END			107
#define KEY_DOWN		108
#define KEY_PAGEDOWN		109
#define KEY_INSERT		110
#define KEY_DELETE		111
#define KEY_MACRO		112
#define KEY_MUTE		113
#define KEY_VOLUMEDOWN		114
#define KEY_VOLUMEUP		115

*/


unsigned char abc[] = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_ENTER, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_DOT, KEY_ENTER, 0, KEY_SPACE, KEY_SPACE, KEY_SPACE, KEY_COMMA, KEY_DOT, KEY_DOT, KEY_DOT, KEY_DOT, KEY_BACKSPACE};




/*
 *	Draw image from PPM file
 */
static void fb_drawimage(const char *filename, char *fbp, int xo, int yo, int bpp, int length, int xres) 
{
	char head[256];
	char s[80];
	FILE *theme_file;
	unsigned char *pixline;
	unsigned i, j, width, height, line_size;

	memset(head, 0, sizeof(head));
	theme_file = fopen(filename, "r");

	/* parse ppm header */
	while (1) {
		if (fgets(s, sizeof(s), theme_file) == NULL) {
			printf("bad PPM file '%s'", filename);
			exit(1);
		}

		if (s[0] == '#')
			continue;

		if (strlen(head) + strlen(s) >= sizeof(head)) {
			printf("bad PPM file '%s'", filename);
			exit(1);
		}

		strcat(head, s);
		if (head[0] != 'P' || head[1] != '6') {
			printf("bad PPM file '%s'", filename);
			exit(1);
		}

		/* width, height, max_color_val */
		if (sscanf(head, "P6 %u %u %u", &width, &height, &i) == 3)
			break;
		/* TODO: i must be <= 255! */
	}

	line_size = width*3;

	long int location = 0;
	pixline = malloc(line_size);
	for (j = 0; j < height; j++) {
		unsigned char *pixel = pixline;

		if (fread(pixline, 1, line_size, theme_file) != line_size) {
			printf("bad PPM file '%s'", filename);
			exit(1);
		}

		for (i = 0; i < width; i++) {
			pixel += 3;
			location = ((xres-width)+i+xo) * (bpp/8) + (j+yo) * length;

			*(fbp + location) = (((unsigned)pixel[0]));
			*(fbp + location + 1) = (((unsigned)pixel[1]));
			*(fbp + location + 2) = (((unsigned)pixel[2]));
			*(fbp + location + 3) = -1;      /* No transparency */
		}
	}
	free(pixline);
	fclose(theme_file);
}



int send_event(int fd, __u16 type, __u16 code, __s32 value)
{
    struct input_event event;

    memset(&event, 0, sizeof(event));
    event.type = type;
    event.code = code;
    event.value = value;

    if (write(fd, &event, sizeof(event)) != sizeof(event)) {
        fprintf(stderr, "Error on send_event");
        return -1;
    }

    return 0;
}

static void fb_drawbox(char *fbp, int x1, int y1, int x2, int y2, int bpp, int length)
{
	int x, y;
	long int location = 0;

     		for (y = y1; y < y2; y++)
       		for (x = x1; x < x2; x++) {

			location = (x) * (bpp/8) + (y) * length;

			*(fbp + location) = 100;
			*(fbp + location + 1) = 100;
			*(fbp + location + 2) = 200;
			*(fbp + location + 3) = -1;      /* No transparency */

			}

}


int main(void) {

     int fbfd = 0;
     struct fb_var_screeninfo vinfo;
     struct fb_fix_screeninfo finfo;
     long int screensize = 0;
     char *fbp = 0;

	/* uinput init */
	int i,fd;
	struct uinput_user_dev device;
	memset(&device, 0, sizeof device);

	fd=open("/dev/uinput",O_WRONLY);
	strcpy(device.name,"test keyboard");

	device.id.bustype=BUS_USB;
	device.id.vendor=1;
	device.id.product=1;
	device.id.version=1;

	if (write(fd,&device,sizeof(device)) != sizeof(device))
	{
		fprintf(stderr, "error setup\n");
	}

	if (ioctl(fd,UI_SET_EVBIT,EV_KEY) < 0)
		fprintf(stderr, "error evbit key\n");

	for (i=0;i<50;i++)
		if (ioctl(fd,UI_SET_KEYBIT, abc[i]) < 0)
			fprintf(stderr, "error evbit key\n");

	if (ioctl(fd,UI_SET_EVBIT,EV_REL) < 0)
		fprintf(stderr, "error evbit rel\n");

	for(i=REL_X;i<REL_MAX;i++)
		if (ioctl(fd,UI_SET_RELBIT,i) < 0)
			fprintf(stderr, "error setrelbit %d\n", i);

	if (ioctl(fd,UI_DEV_CREATE) < 0)
	{
		fprintf(stderr, "error create\n");
	}

	sleep(2);
	/* end uinput init */


	/* init framebuffer */
	/* Open the file for reading and writing */
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	/* Get fixed screen information */
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}

	/* Get variable screen information */
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	/* Figure out the size of the screen in bytes */
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	/* Map the device to memory */
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                        fbfd, 0);
	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	/* end init framebuffer */



	/* init mouse */
	int fdm;
	// fdm = open("/dev/input/event0", O_RDONLY);
	struct input_event evm;

    if((fdm = open(MOUSEFILE, O_RDONLY | O_NONBLOCK)) == -1) {
        printf("Device open ERROR\n");
        exit(EXIT_FAILURE);
    }

	int ret, xp=0, yp=0, rkp=0, ckp=0;
    unsigned char button,bLeft,bMiddle,bRight;
    char xm,ym;
    int absolute_x,absolute_y;
	/* end init mouse */

	int pressed=0;
	int pressedk=0;

	int j, m=0;
	while(1) {
		m++;
		if (m==29) m=0; /* 27 keys, and 2 extra seconds for exit */


		fb_drawimage("image_mouse.ppm", fbp, vinfo.xoffset, vinfo.yoffset, vinfo.bits_per_pixel, finfo.line_length, vinfo.xres);


		/* check for mouse */

 	ret=read(fdm, &evm, sizeof(struct input_event));
	if (ret!=-1) {
        unsigned char *ptr = (unsigned char*)&evm;

        button=ptr[0];
        bLeft = button & 0x1;
        bMiddle = ( button & 0x4 ) > 0;
        bRight = ( button & 0x2 ) > 0;
        xm=(char) ptr[1];ym=(char) ptr[2];


        if (xm>0) xp++;
	else if(xm<0) xp--;

        if (ym>0) yp--;
	else if(ym<0) yp++;

	if (xp==UMBRAL) {
		xp=0;
		ckp++; if (ckp>9) ckp=9;
	} else if (xp==-UMBRAL) {
		xp=0;
		ckp--; if (ckp<0) ckp=0;
	}

	if (yp==UMBRAL) {
		yp=0;
		rkp++; if (rkp>4) rkp=4;
	} else if (yp==-UMBRAL) {
		yp=0;
		rkp--; if (rkp<0) rkp=0;
	}


        absolute_x+=xm;
        absolute_y-=ym;
        if (bRight==1)
        {
            absolute_x=0;
            absolute_y=0;
            printf("Absolute x,y coords origin recorded\n");
        }

        if (bLeft==1) {
		pressed=1;
		pressedk=(rkp*10)+ckp;
        } else if ((bLeft==0) && (pressed==1)) {
		if (abc[pressedk] == 0) break;
		pressed=0;
		send_event(fd, EV_KEY, abc[pressedk], 1);
		send_event(fd, EV_SYN, SYN_REPORT, 0);
		send_event(fd, EV_KEY, abc[pressedk], 0);
		send_event(fd, EV_SYN, SYN_REPORT, 0);
	}

	}


	// fb_drawbox(fbp, (vinfo.xres-220)+((220/10)*(ckp+1)), (120/5)*(rkp+1), vinfo.bits_per_pixel, finfo.line_length);
	int x1, x2, y1, y2;
	int anchoi = 240;
	int altoi = 125;
	int nro_columnas=10;
	int nro_filas=5;
	x1=(vinfo.xres-anchoi)+((anchoi/nro_columnas)*(ckp));
	x2=x1+(anchoi/nro_columnas);
	y1=(altoi/nro_filas)*(rkp);
	y2=y1+4;
	//fb_drawbox(fbp, (vinfo.xres-220)+((220/10)*(ckp+1)), (120/5)*(rkp+1), vinfo.bits_per_pixel, finfo.line_length);
	fb_drawbox(fbp, x1, y1, x2, y2, vinfo.bits_per_pixel, finfo.line_length);

	y1=(altoi/nro_filas)*(rkp)+(altoi/nro_filas-4); // The "4" here is a ugly hardcoded number of pixels less
	y2=y1+4;		/* 4 is the height of the line */
	fb_drawbox(fbp, x1, y1, x2, y2, vinfo.bits_per_pixel, finfo.line_length);
	usleep(1000);
	// 240 width image keyboard on screen
	// 125 height
	// vinfo.xres x res






	}




	close(fd);
	close(fdm);




     munmap(fbp, screensize);
     close(fbfd);
     return 0;
}

