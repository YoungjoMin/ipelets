CC = g++
TESTFLAGS = -std=c++17 -Wall -g
OBJFLAGS = -std=c++17 -Wall -O3 -c
DLLFLAGS = -std=c++17 -Wall -O3 -shared -lstdc++ -L../../bin -lipe
OBJS = Delaunay.o GG.o RNG.o EMST.o GeometricGraphs.o
TARGET = GeometricGraphs.dll
DLL = $(TARGET)
LUA = $(TARGET:$.dll=$.lua)

ALL: $(TARGET)

%.o : %.cpp
	$(CC) $(OBJFLAGS) -o $@ $^

$(TARGET) : $(OBJS)
	$(CC) $(DLLFLAGS) -o $@ $^

test: Test.cpp $(OBJS)
	$(CC) $(TESTFLAGS) -o Test.exe $^
	
clean:
	rm -rf $(OBJS) $(TARGET) *.txt *.exe

install: ALL $(DLL) $(LUA)
	cp $(DLL) ../
	cp $(LUA) ../
	
.PHONY: all install clean