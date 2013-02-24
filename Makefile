CC=g++
CFLAGS='-g'
INFLAGS=-I./
LDFLAGS=
SOURCES=main.cc Uart.cc GPS.cc RoutesManager.cc Route.cc RoutePoint.cc Coordinate.cc LatLongUtility.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) -c $(INFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE) $(OBJECTS)
