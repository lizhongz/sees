CC=g++ -std=c++11
CFLAGS='-g'
INFLAGS=-I./
LDFLAGS=-lpthread
SOURCES=main.cc Server.cc SeesServer.cc CommProtocol.cc GPXHelper.cc Route.cc RoutePoint.cc Coordinate.cc pugixml.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) -c $(INFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE) $(OBJECTS)
