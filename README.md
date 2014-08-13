onscreenkeyboard
================

on-screen keyboard ideas


Some proof of concept of on-screen keyboard

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
