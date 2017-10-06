CC=g++
CFLAGS=`libpng-config --cflags`
EXEC=evaluate_object

.PHONY: all clean

all: $(EXEC) 

clean:
	rm -f $(EXEC)
	rm -f *.o
	rm -f *~

.cpp.o:
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(EXEC).o
	$(CC) -o $@ $^ $(CFLAGS) `libpng-config --ldflags`