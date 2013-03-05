CC=g++ -std=c++11
CFLAGS='-g'
INFLAGS=-I./
LDFLAGS=-lpthread
SOURCES=main.cc Uart.cc GPS.cc RoutesManager.cc Route.cc RoutePoint.cc Coordinate.cc LatLongUtility.cc NavigationEngine.cc Runnable.cc Control.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) -c $(INFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE) $(OBJECTS)
