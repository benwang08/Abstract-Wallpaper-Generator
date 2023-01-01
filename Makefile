CC = g++
CFLAGS = -g  


all: generator scan

generator: main.o triple.o entity_list.o sphere.o
	$(CC) $(CFLAGS) -o generator main.o -lpthread triple.o entity_list.o sphere.o

image: generator scan
	./generator > image.ppm
	xdg-open image.ppm

main.o: main.cpp ray.h triple.h
	$(CC) $(CFLAGS) -c main.cpp

triple.o: triple.cpp triple.h 
	$(CC) $(CFLAGS) -c triple.cpp

sphere.o: sphere.cpp sphere.h
	$(CC) $(CFLAGS) -c sphere.cpp

entity_list.o: entity_list.cpp entity_list.h
	$(CC) $(CFLAGS) -c entity_list.cpp

scan: scan.cpp
	$(CC) $(CFLAGS) -o scan scan.cpp

	
clean: 
	-rm -f generator main.o triple.o sphere.o entity_list.o image.ppm scan data.txt test.ppm

