CC = g++
CFLAGS = -g  

all: generator scan

generator: main.o triple.o entity_list.o sphere.o
	$(CC) $(CFLAGS) -o generator main.o -lpthread triple.o entity_list.o sphere.o

main.o: main.cpp gen_classes/ray.h gen_classes/triple.h
	$(CC) $(CFLAGS) -c main.cpp

triple.o: gen_classes/triple.cpp gen_classes/triple.h 
	$(CC) $(CFLAGS) -c gen_classes/triple.cpp

sphere.o: gen_classes/sphere.cpp gen_classes/sphere.h
	$(CC) $(CFLAGS) -c gen_classes/sphere.cpp

entity_list.o: gen_classes/entity_list.cpp gen_classes/entity_list.h
	$(CC) $(CFLAGS) -c gen_classes/entity_list.cpp

scan: scan.cpp
	$(CC) $(CFLAGS) -o scan scan.cpp

clean: 
	-rm -f generator *.o wallpaper.ppm scan data.txt *.ppm

