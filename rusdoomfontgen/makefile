CC =g++
CFLAGS= -std=c++0x -c
LDFLAGS= -lm -mwindows -static

SOURCES=bigfont.cpp confont.cpp borderfilter.cpp exportfilter.cpp filter.cpp filterstack.cpp fontimage.cpp pcx.cpp rusdoomfontgen.cpp stdafx.cpp thresholdfilter.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=ENGdoomfontgen

all:	$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean : 
	rm *.o