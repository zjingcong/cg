C	= c

LINKS =  `Magick++-config  --cppflags --cxxflags --ldflags --libs` -L/usr/local/lib -lOpenImageIO -lX11 -lGL -lGLU -lglut -lm -lXmu -lXi

INCLUDES = -I ./ `Magick++-config  --cppflags --cxxflags`

CXX = g++ -Wall -g -O2

PROJECT	= bunny

.C.o:
	$(CXX) -c $(INCLUDES) $<

${PROJECT}:  ${PROJECT}.${C}
	$(CXX) ${PROJECT}.${C} $(INCLUDES) $(LINKS) -o ${PROJECT}

clean:
	rm -rf core.* *.o *~ ${PROJECT}
