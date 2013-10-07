#include <gtk/gtk.h>
#include <glib.h>
#include <canberra.h>


//Lesson Type
#define LETTERS 0
#define WORDS 1
#define SENTENCE 2

//Data Directory
#define directory "usr/share/angela-typing-tutor/"


#define FG_COLOR "#FFFFFF"
#define BG_COLOR "#000000"
#define HIGHLIGHT_FG_COLOR "#FCFF00"
#define HIGHLIGHT_BG_COLOR "#000000"

//Point
#define SKIP -10
#define ZERO 0

//Canberra sound
#define SOUND_ID 10
ca_context* context;

gchar value[100][10];
gchar letter[100][10];

GtkWidget * image_hand;
GtkWidget * image_current_point_1;
GtkWidget * image_current_point_2;
GtkWidget * image_win_point_1;
GtkWidget * image_win_point_2;
GtkWidget * face_image;
GtkTextBuffer* textbuffer;
GdkPixbufAnimation * animation_test;



