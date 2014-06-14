
angela_typing_tutor_LDADD = -lm @GTK_LIBS@ @CANBERRA_LIBS@ -lespeak 


angela_typing_tutor_CFLAGS = \
	@GTK_CFLAGS@ \
	@CANBERRA_CFLAGS@ \
	-I/usr/include/espeak/
	$(NULL)


angela_typing_tutor_SOURCES = \
	tts.c \
	ui.c \
	aott.c \
	$NULL



bin_PROGRAMS = angela-typing-tutor

angela_typing_tutordir = $(libexecdir)/

