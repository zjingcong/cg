C	= c
CC = g++

CFLAGS		= -g -Wall -O2

LDFLAGS =  `Magick++-config  --cppflags --cxxflags --ldflags --libs` -L/usr/local/lib -lOpenImageIO -lX11 -lGL -lGLEW -lGLU -lglut -lm -lXmu -lXi -DGL_GLEXT_PROTOTYPES

INCLUDES = -I ./ `Magick++-config  --cppflags --cxxflags`

PROJECT	= bunny

.C.o:
	$(CC) ${CFLAGS} -c $(INCLUDES) $<

${PROJECT}:  ${PROJECT}.${C}
	$(CC) ${CFLAGS} ${PROJECT}.${C} $(INCLUDES) $(LDFLAGS) -o ${PROJECT}

clean:
	rm -f core.* *.o *~ ${PROJECT}

