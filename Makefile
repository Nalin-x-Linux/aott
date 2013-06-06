CC = gcc
CFLAGS = -Wall -o
LFLAGS = -lespeak -I/usr/include/espeak/
INCLUDES = `pkg-config --cflags --libs gtk+-3.0 libcanberra-gtk3`

main: 
	$(CC) $(CFLAGS) angela aott.c $(LFLAGS) $(INCLUDES)

clean: 
	rm angela
