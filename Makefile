
CC = g++
CFLAGS = -Wall -g

generator: main.o triple.o sphere.o entity_list.o
	$(CC) $(CFLAGS) -o generator main.o triple.o sphere.o entity_list.o
	
image: generator
	./generator > image.ppm
	xdg-open image.ppm

main.o: main.cpp
	$(CC) -c $(CFLAGS) main.cpp

classes: triple.cpp sphere.cpp entity_list.cpp
	$(CC) -c $(CFLAGS) triple.cpp sphere.cpp entity_list.cpp

clean: 
	-rm -f generator main.o triple.o sphere.o entity_list.o image.ppm 