all: clean light normal

normal:
	gcc normal.c -lm -o normal

light:
	gcc light.c -lm -o light

texture:
	gcc -I/usr/include -I/usr/X11R6/include -L/usr/lib -L/usr/X11R6/lib -O2 texture.c -lX11 -lGLEW -lGL -lGLU -lglut -lm -lXmu -lXi -o texture

clean:
	rm -f normal
	rm -f texture
	rm -f light
