#include "aott.h"
#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>


void set_hand(gchar* key)
{
	int i;
	char* file = malloc(100);	
	GdkPixbufAnimation * animation;
	for(i=0;i<100;i++)
	{
		if (strcmp(letter[i],key)==0)
		{			
			sprintf(file,"%shands/hands_%s.gif",directory,value[i]);
			animation = gdk_pixbuf_animation_new_from_file (file, NULL);
		}
	}
	gtk_image_set_from_animation (GTK_IMAGE(image_hand), animation);
}

void set_point_view(int win, int current)
{
	gchar *file = malloc(80);
	if (win != SKIP){
		sprintf(file,"%snumber/%d.png",directory,win/10);
		gtk_image_set_from_file(GTK_IMAGE(image_win_point_1),file);
		sprintf(file,"%snumber/%d.png",directory,win%10);
		gtk_image_set_from_file(GTK_IMAGE(image_win_point_2),file);}
	if (current != SKIP){
		sprintf(file,"%snumber/%d.png",directory,current/10);
		gtk_image_set_from_file(GTK_IMAGE(image_current_point_1),file);
		sprintf(file,"%snumber/%d.png",directory,current%10);
		gtk_image_set_from_file(GTK_IMAGE(image_current_point_2),file);}
}

void about()
{
	GtkBuilder* about_builder;
	GtkWidget* about_window;
	about_builder = gtk_builder_new();
	gchar* glade_file = malloc(70);
	sprintf(glade_file,"%s/ui/about.glade",directory);
	gtk_builder_add_from_file(about_builder,glade_file,NULL);
	about_window = GTK_WIDGET(gtk_builder_get_object(about_builder,"window"));
	gtk_widget_show(about_window);
}
