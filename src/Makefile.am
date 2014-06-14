
typing_tutor_LDADD = -lm @GTK_LIBS@ @CANBERRA_LIBS@ -lespeak 


typing_tutor_CFLAGS = \
	@GTK_CFLAGS@ \
	@CANBERRA_CFLAGS@ \
	-I/usr/include/espeak/
	$(NULL)


typing_tutor_SOURCES = \
	tts.c \
	ui.c \
	aott.c \
	$NULL



bin_PROGRAMS = angela-typing-tutor

typing_tutordir = $(libexecdir)/

