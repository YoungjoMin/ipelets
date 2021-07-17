CC = g++
OBJFLAGS = -std=c++17 -Wall -O3 -c
DLLFLAGS = -std=c++17 -Wall -O3 -shared -lstdc++ -L../../bin -lipe
OBJS = Delaunay.o GeometricGraphs.o
TARGET = GeometricGraphs.dll
DLL = GeometricGraphs.dll
LUA = GeometricGraphs.lua

ALL: $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

Delaunay.o: Delaunay.cpp Delaunay.hpp DelaunayDS.hpp
	$(CC) $(OBJFLAGS) -o $@ Delaunay.cpp

GeometricGraphs.o: GeometricGraphs.cpp
	$(CC) $(OBJFLAGS) -o $@ GeometricGraphs.cpp

GeometricGraphs.dll: Delaunay.o GeometricGraphs.o
	$(CC) $(DLLFLAGS) -o $@ Delaunay.o GeometricGraphs.o

install: ALL $(DLL) $(LUA)
	cp $(DLL) ../
	cp $(LUA) ../