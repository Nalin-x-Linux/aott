CC = gcc
CFLAGS = -Wall -o
LFLAGS = -lespeak -I/usr/include/espeak/
INCLUDES = `pkg-config --cflags --libs gtk+-3.0 libcanberra-gtk3`

main: 
	$(CC) $(CFLAGS) angela aott.c $(LFLAGS) $(INCLUDES)

setup:
	sudo apt-get install libespeak-dev libcanberra-gtk3-dev libgtk-3-dev git 

clean: 
	rm angela
