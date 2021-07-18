CC = g++
TESTFLAGS = -std=c++17 -Wall
OBJFLAGS = -std=c++17 -Wall -O3 -c
DLLFLAGS = -std=c++17 -Wall -O3 -shared -lstdc++ -L../../bin -lipe
OBJS = Delaunay.o GG.o RNG.o EMST.o GeometricGraphs.o
TARGET = GeometricGraphs.dll
DLL = GeometricGraphs.dll
LUA = GeometricGraphs.lua

ALL: $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET) *.txt *.exe

Delaunay.o: Delaunay.cpp Delaunay.hpp DelaunayDS.hpp
	$(CC) $(OBJFLAGS) -o $@ Delaunay.cpp

GG.o: GG.cpp GG.hpp
	$(CC) $(OBJFLAGS) -o $@ GG.cpp

RNG.o: RNG.cpp RNG.hpp
	$(CC) $(OBJFLAGS) -o $@ RNG.cpp

EMST.o: EMST.cpp EMST.hpp
	$(CC) $(OBJFLAGS) -o $@ EMST.cpp

GeometricGraphs.o: GeometricGraphs.cpp
	$(CC) $(OBJFLAGS) -o $@ GeometricGraphs.cpp

GeometricGraphs.dll: Delaunay.o GG.o RNG.o EMST.o GeometricGraphs.o
	$(CC) $(DLLFLAGS) -o $@ Delaunay.o GG.o RNG.o EMST.o GeometricGraphs.o

test: Test.cpp Delaunay.o GG.o RNG.o EMST.o
	$(CC) $(TESTFLAGS) -o Test.exe Delaunay.o GG.o RNG.o EMST.o Test.cpp


install: ALL $(DLL) $(LUA)
	cp $(DLL) ../
	cp $(LUA) ../