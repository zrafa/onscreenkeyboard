
all:
	# gcc -o fbkb fbkb.c
	# gcc -o key key.c
	# gcc -o userland_keystrokes userland_keystrokes.c
	gcc -o osk osk.c
	gcc -o osk_mouse osk_mouse.c

clean:
	# rm fbkb key userland_keystrokes osk
	rm osk osk_mouse
