CC = gcc
CFLAGS = -Wall
LFLAGS = -lespeak -I/usr/include/espeak/
INCLUDES = `pkg-config --cflags --libs gtk+-3.0 libcanberra-gtk3`

main: 
	$(CC) $(CFLAGS) -c tts.c $(LFLAGS) $(INCLUDES)
	$(CC) $(CFLAGS) -c aott.c $(LFLAGS) $(INCLUDES)
	$(CC) $(CFLAGS) -c ui.c $(LFLAGS) $(INCLUDES)
	$(CC) $(CFLAGS) -o angela aott.o tts.o ui.o $(LFLAGS) $(INCLUDES)
	find -iname *~ -delete

setup:
	sudo apt-get install libespeak-dev libcanberra-gtk3-dev libgtk-3-dev git
	find -iname *~ -delete 

clean: 
	rm angela *.o
