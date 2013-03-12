CC=g++-4.7 -std=c++11
CFLAGS=-g -D_GLIBCXX_USE_NANOSLEEP `pkg-config opencv --cflags`
INFLAGS=-I./ 
LDFLAGS=-lpthread `pkg-config opencv --libs` -ljpeg
SOURCES=main.cc Uart.cc GPS.cc RoutesManager.cc Route.cc RoutePoint.cc Coordinate.cc LatLongUtility.cc NavigationEngine.cc Runnable.cc Control.cc MSAC.cc errorNIETO.cc lmmin.cc object_detection.cc road_detection.cc road_detection_y.cc camera.cc env_detection.cc Client.cc CommProtocol.cc pugixml.cc GPXHelper.cc SeesClient.cc

OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) -c $(INFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE) $(OBJECTS)
