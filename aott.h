#include <gtk/gtk.h>
#include <glib.h>
#include <canberra.h>


//Lesson Type
#define LETTERS 0
#define WORDS 1
#define SENTENCE 2

//Array size
#define MAX_LETTERS 1000
#define MAX_WORDS 200000
#define MAX_SENTENCE 10000
#define MAX_LENGTH 100

//Data Directory
#define directory "/usr/share/angela-typing-tutor/"


#define FG_COLOR "#FFFFFF"
#define BG_COLOR "#000000"
#define HIGHLIGHT_FG_COLOR "#FCFF00"
#define HIGHLIGHT_BG_COLOR "#000000"

//Point
#define SKIP -10
#define ZERO 0

#define AVG_WPM 30
#define AVG_CPM 60

//Canberra sound
#define SOUND_ID 10
ca_context* context;

//Global Arrays
gchar value[MAX_LETTERS][MAX_LENGTH];
gchar letter[MAX_LETTERS][MAX_LENGTH];
gchar word[MAX_WORDS][MAX_LENGTH];
gchar sentence[MAX_SENTENCE][MAX_LENGTH];
gchar qustion_list[MAX_WORDS][MAX_LENGTH];


GtkWidget * image_hand;
GtkWidget * image_current_point_1;
GtkWidget * image_current_point_2;
GtkWidget * image_win_point_1;
GtkWidget * image_win_point_2;
GtkWidget * face_image;
GtkTextBuffer* textbuffer;
GdkPixbufAnimation * animation_test;



