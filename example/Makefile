CC	= gcc
C	= c

LFLAGS	= -I/usr/include -I/usr/X11R6/include -L/usr/lib -L/usr/X11R6/lib
LDFLAGS	= -lX11 -lGL -lGLU -lglut -lm -lXmu -lXi

PROJECT	= $1

${PROJECT}:     ${PROJECT}.o
	${CC} ${LFLAGS} -O2 ${PROJECT} ${PROJECT}.o ${LDFLAGS}

${PROJECT}.o:   ${PROJECT}.${C}
	${CC} ${CFLAGS} -c ${PROJECT}.${C}

clean:
	rm -rf core.* *.o *~ ${PROJECT}
