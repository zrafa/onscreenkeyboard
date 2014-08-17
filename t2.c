
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

#define MOUSEFILE "/dev/input/mouse0"

#define UMBRAL 5
int main()
{
    int fd;
    struct input_event ie;

    unsigned char button,bLeft,bMiddle,bRight;
    char x,y;
    int absolute_x,absolute_y;

    if((fd = open(MOUSEFILE, O_RDONLY | O_NONBLOCK)) == -1) {
        printf("Device open ERROR\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Device open OK\n");
    }

	int ret, xp=0, yp=0;
    printf("right-click to set absolute x,y coordinates origin (0,0)\n");
	int ptr = 0;
    while (1)
    {
    	ret=read(fd, &ie, sizeof(struct input_event));
	if (ret!=-1) {
        unsigned char *ptr = (unsigned char*)&ie;
        int i;       

        button=ptr[0];
        bLeft = button & 0x1;
        bMiddle = ( button & 0x4 ) > 0;
        bRight = ( button & 0x2 ) > 0;
        x=(char) ptr[1];y=(char) ptr[2];



/*
        if (x>0) printf("X DERECHA");
	else if(x<0) printf("X IZQUIERDA");
	else printf(" X NADA");

        if (y>0) printf("y ARRIBA");
	else if(y<0) printf("y ABAJO");
	else printf(" y NADA");
*/

        if (x>0) xp++;
	else if(x<0) xp--;

        if (y>0) yp--;
	else if(y<0) yp++;

	if (xp==UMBRAL) {
		xp=0;
		printf("x DERECHA");
	} else if (xp==-UMBRAL) {
		xp=0;
		printf("x IZQUIERDA");
	}

	if (yp==UMBRAL) {
		yp=0;
		printf("x ARRIBA");
	} else if (yp==-UMBRAL) {
		yp=0;
		printf("x ABAJO");
	}

//printf("bLEFT:%d, bMIDDLE: %d, bRIGHT: %d, rx: %d  ry=%d\n",bLeft,bMiddle,bRight, x,y);

        absolute_x+=x;
        absolute_y-=y;
        if (bRight==1)
        {
            absolute_x=0;
            absolute_y=0;
            printf("Absolute x,y coords origin recorded\n");
        }

        printf("\n");
	}
    }

return 0;
}
