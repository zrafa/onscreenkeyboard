onscreenkeyboard
================

2014 - Rafael Ignacio Zurita

This is an on-screen keyboard proof of concept

osk.c source file implements this.
 - No extra libraries. Great for using at boot time :
	# ldd osk
        linux-gate.so.1 
        libc.so.6 => /lib/i386-linux-gnu/i686/cmov/libc.so.6 
        /lib/ld-linux.so.2 
 - on scren keyboard using framebuffer. Ok for console
 - it sends keystrokes using uinput

Check the video to know the idea and proof of concept working :
http://youtu.be/zHOJGq4hkpg


How to use
==========

make
modprobe uinput
# be sure that /dev/input/event0 is your real keyboard (you need it to choose keys from onscreen keyboard)
./osk #YOU NEED TO BE ROOT

You will see the on screen keyboard on console on top of framebuffer.
There is green box which go from "a" to "z" delaying one second
on every letter. YOu have "E" for ENTER after "z"
When you press the "End" key on your real keyboard, you choose
the letter where green box is, and the program sends a 
proper keystrokes to the system using uinput.

For example, if green box is under "l", and you press the "END" key
on your keyboard, you will see the "l" showed on console, like
if you would had pressed the "l" key.



Old ideas
=========

The idea is :

 - draw a keyboard on framebuffer
 - someway to choose a key from that keyboard
 - send keystrokes to the Linux system as a real keyboard

For all the 3 ideas we already have examples to start.

If we ldd all the binaries for these examples we 
verify that these use just the below libraries :
        linux-gate.so.1 (0xb7785000)
        libc.so.6 => /lib/i386-linux-gnu/i686/cmov/libc.so.6 (0xb75a2000)
        /lib/ld-linux.so.2 (0xb7786000)

which is that we want, in order to have on-screen keyboard ready on boot time.


The image.ppm is the keyboard image. We did it with Gimp and exported as raw ppm.
