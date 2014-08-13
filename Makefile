
all:
	gcc -o fbkb fbkb.c
	gcc -o key key.c
	gcc -o userland_keystrokes userland_keystrokes.c

clean:
	rm fbkb key userland_keystrokes
