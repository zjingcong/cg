C	= c

LINKS =  -L/usr/local/lib -lX11 -lGL -lGLEW -lGLU -lglut -lm -lXmu -lXi -DGL_GLEXT_PROTOTYPES

INCLUDES = -I ./

CXX = g++ -Wall -g -O2

PROJECT	= light

.C.o:
	$(CXX) -c $(INCLUDES) $<

${PROJECT}:  ${PROJECT}.${C}
	$(CXX) ${PROJECT}.${C} $(INCLUDES) $(LINKS) -o ${PROJECT}

clean:
	rm -rf core.* *.o *~ ${PROJECT}
