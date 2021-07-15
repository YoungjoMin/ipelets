CC = g++
CFLAGS = -shared -Wall -O3
LDFLAGS = -lstdc++ -L./bin -lipe
OBJS = GeometricGraphs.o
TARGET = GeometricGraphs.dll

ALL: $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

