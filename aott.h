#include <gtk/gtk.h>
#include <glib.h>

//Lesson Type
#define LETTERS 0
#define WORDS 1
#define SENTENCE 2

//Data Directory
#define directory "usr/share/angela-typing-tutor/"

//Point
#define SKIP -10
#define ZERO 0

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
