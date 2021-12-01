CC = g++
TESTFLAGS = -std=c++17 -Wall -g
OBJFLAGS = -std=c++17 -Wall -O3 -c
DLLFLAGS = -std=c++17 -Wall -O3 -shared -lstdc++ -L../../bin -lipe
OBJS = Delaunay.o Utils.o CurveReconstruction.o Crust.o BetaSkeleton.o
TARGET = CurveReconstruction.dll
DLL = $(TARGET)
LUA = $(TARGET:$.dll=$.lua)

ALL: $(TARGET)

%.o : %.cpp
	$(CC) $(OBJFLAGS) -o $@ $^

$(TARGET) : $(OBJS)
	$(CC) $(DLLFLAGS) -o $@ $^

test: Test.cpp $(OBJS)
	$(CC) $(TESTFLAGS) -o Test.exe Test.cpp $(OBJS)
	
clean:
	rm -rf $(OBJS) $(TARGET) *.txt *.exe

install: ALL $(DLL) $(LUA)
	cp $(DLL) ../
	cp $(LUA) ../
	
.PHONY: all install clean