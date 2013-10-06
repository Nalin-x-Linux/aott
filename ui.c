#include "aott.h"
#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>


void set_hand(gchar* key)
{
	int i;
	char file[100];	
	GdkPixbufAnimation * animation;
	for(i=0;i<100;i++)
	{
		if (strcmp(letter[i],key)==0)
		{			
			sprintf(file,"%shands/hands_%s.gif",directory,value[i]);
			animation = gdk_pixbuf_animation_new_from_file (file, NULL);
			gtk_image_set_from_animation (GTK_IMAGE(image_hand), animation);
		}
	}
	
}

void set_point_view(int win, int current)
{
	gchar file[80];
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


void clear_tag()
{
	GtkTextIter iter_1;
	GtkTextIter iter_2;
	GtkTextTag *tag;
	tag = gtk_text_buffer_create_tag(textbuffer,NULL,NULL);
	g_object_set(tag,"foreground-rgba","#002CFF", NULL);
	gtk_text_buffer_get_bounds(textbuffer,&iter_1,&iter_2); 
	gtk_text_buffer_apply_tag(textbuffer,tag,&iter_1,&iter_2);
	
}

void set_tag(int start,int end,char* fg_color,char* bg_color)
{
	GtkTextTag *tag;
	GtkTextIter iter_1;
	GtkTextIter iter_2;
	
	tag = gtk_text_buffer_create_tag(textbuffer,NULL,NULL);
	if (fg_color != NULL)
		g_object_set(tag,"foreground-rgba",fg_color, NULL);
	if (bg_color != NULL)
		g_object_set(tag,"background-rgba",bg_color, NULL);
	
	gtk_text_buffer_get_iter_at_offset(textbuffer,&iter_1,start);
	gtk_text_buffer_get_iter_at_offset(textbuffer,&iter_2,end);
	gtk_text_buffer_apply_tag(textbuffer,tag,&iter_1,&iter_2);
}
