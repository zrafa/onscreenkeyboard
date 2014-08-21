onscreenkeyboard
================


This is a framebuffer on-screen keyboard proof of concept
for text console or X. Might be usefull at boot time for accessibility.
Currently, you can move or roll the mouse/trackball to the proper key, then hit the left mouse button to send the keystroke to the system, like a real keyboard.


```
 * Copyright (C) 2014 Rafael Ignacio Zurita <rafaelignacio.zurita@gmail.com>
 *
 *   osk.c and osk_mouse.c is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version. Check COPYING file.
```

osk_mouse.c and osk.c source file implements this.

 - No extra libraries. Great for using at boot time :
```
	# ldd osk_mouse
        linux-gate.so.1 
        libc.so.6 => /lib/i386-linux-gnu/i686/cmov/libc.so.6 
        /lib/ld-linux.so.2 
```
 - on scren keyboard using framebuffer. Ok for console
 - it sends keystrokes using uinput

Check the video to know the idea and proof of concept working :
http://youtu.be/zHOJGq4hkpg


How to use
==========

```
# Be sure that /dev/input/event0 is your real keyboard, 
# you need it to choose keys from onscreen keyboard

make
modprobe uinput
# run on text console or X
./osk &  #YOU NEED TO BE ROOT

# Use "END" key from your keyboard to choose a key from the
# on screen keyboard. Otherwise your console or bash
# could get two keys, the one you pressed and the choosed
# from the on screen keyboard.

# When you want to exit, you should press your "END" key in your
# keyboard when green box dissappear from keyboard on screen
# for two seconds. That happens every time it reachs to the 
# end of the on screen keyboard keys

```

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
