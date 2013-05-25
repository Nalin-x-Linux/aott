/*
This is a small programm writen by Nalin.x.Linux
Please change the value of macro named directory to 
where the data files are.

Compile using folowing command
gcc -Wall -o angela new.c -lespeak -I/usr/include/espeak/ `pkg-config --cflags --libs gtk+-3.0 libcanberra-gtk3`
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <speak_lib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <canberra.h>
#include <stdbool.h>
#include <dirent.h> 


//Lesson Type
#define LETTERS 0
#define WORDS 1
#define SENTENCE 2

//Data Directory
#define directory "usr/share/angela-open-talking-typer/"

//Point
#define SKIP -10
#define ZERO 0

gchar letter[100][10];
gchar value[100][10];
gchar word[100][50];
gchar sentence[100][100];

//Lesson Struct
struct lesson_def
{
	int type;
	int win_point;
	int from;
	int to;
	gchar instruction[200];
};

//Canberra sound context
ca_context* context=NULL;

//Time Varibles
long int time_qustion;
long int time_lesson_start;

//Mark 
int mark;



struct lesson_def lessons[100];


gchar *correct;
int lesson;
int iter;

//Qustion
gchar *qustion;

//Espeak 
gchar *voice;
gchar *speak_text;
espeak_POSITION_TYPE position_type;

//Gtk
GtkSpinButton *spinbutton;
GtkEntry* entry;
GtkTextBuffer* textbuffer;
GtkComboBoxText* combobox;
GtkLabel* instruction_label;
GtkWidget * image_hand;
GtkWidget * image_current_point_1;
GtkWidget * image_current_point_2;
GtkWidget * image_win_point_1;
GtkWidget * image_win_point_2;

GtkTextView* sub_textview;
GtkTextBuffer* sub_textbuffer;

void jump_to_next_or_previous_lesson(GtkWidget* w,int count);
void key_release_event();
void set_point_view(int win, int current);
void load(gchar language[]);
gchar* random_qustion_generator();
void set_hand(gchar* key);
void run();

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

void *real_speak()
{
	espeak_Cancel();
    int Size = strlen(speak_text)+1;    
    espeak_Synth(speak_text, Size, 0, position_type, 0,	espeakCHARS_AUTO,0, NULL);
    espeak_Synchronize();
    return NULL;
}

void *rotate()
{
	int angle = gtk_label_get_angle(instruction_label);
	g_print("\n$$$$$$%d$$$$$$$",angle);
	for(angle=0; angle<360;angle++)
	{
		usleep(2000);gdk_threads_enter();
		gtk_label_set_angle(instruction_label,angle);gdk_threads_leave();
	}
return NULL;
}


void speak(gchar st[],bool speak,bool append)
{
	
	if (append == 0){
		strcpy(speak_text,st);}
	else{
		strcat(speak_text,st);}
	

	if (speak == 1)	{
		//g_thread_new("speak",real_speak,NULL);
		}	
}


void play(char* file)
{
	gchar* temp = malloc(200);
	sprintf(temp,"%ssounds/%s",directory,file);
	ca_context_cancel(context,-1);
	while(espeak_IsPlaying())
	{
		g_print("Playing");
	}
	ca_context_play(context,0,CA_PROP_MEDIA_FILENAME,temp,NULL);
}



void load(gchar language_file[])
{
	int i;gchar* temp = malloc(200);	
		
	//Opening File
	FILE *fp;
	gchar* file = malloc(70);
	sprintf(file,"%sdata/%s",directory,language_file);
	fp = fopen(file,"r");
	
	//Setting voice
	voice = malloc(20);
	fscanf(fp,"%s",voice);
	espeak_SetVoiceByName(voice);
	
	for(i=0;;i++){
		fscanf(fp,"%s %s\n",letter[i],value[i]);
		if (strcmp(letter[i],"~")==0){
			break;}}
			
	for(i=0;;i++){
		fscanf(fp,"%s",word[i]);
		if (strcmp(word[i],"~")==0){
			fgetc(fp);fgetc(fp);
			break;}	}
	for(i=0;;i++){
		fgets(temp,100,fp);
		strcpy(sentence[i],g_utf8_substring(temp,0,g_utf8_strlen(temp,-1)-1));
		g_print("\nWord - ##%s##",sentence[i]);
		if (strcmp(sentence[i],"~ ~")==0){
			break;}}
	for(i=0;!feof(fp) && fp != NULL;i++){
		fscanf(fp,"%d %d %d-%d",&lessons[i].type,
		&lessons[i].win_point,&lessons[i].from,&lessons[i].to);
		fgets(temp,200,fp);
		strcpy(lessons[i].instruction,g_utf8_substring(temp,0,g_utf8_strlen(temp,-1)-1));}		
	fclose(fp);
}



gchar* random_qustion_generator()
{
	srand(time_qustion);
	int number = rand();
	srand(number*329);
	number %= lessons[lesson].from-lessons[lesson].to;
	number += lessons[lesson].from;
	if (lessons[lesson].type == LETTERS)
		return (gchar*)letter[number];
	else if (lessons[lesson].type == WORDS)
		return (gchar*)word[number];
	else if(lessons[lesson].type == SENTENCE)
		return (gchar*)sentence[number];
	else
		return NULL;
}

void set_hand(gchar* key)
{
	int i;
	char* file = malloc(100);	
	GdkPixbufAnimation * animation;
	for(i=0;i<100;i++)
	{
		if (strcmp(letter[i],key)==0)
		{			
			sprintf(file,"%shands_%s.gif",directory,value[i]);
			animation = gdk_pixbuf_animation_new_from_file (file, NULL);
		}
	}
	gtk_image_set_from_animation (GTK_IMAGE(image_hand), animation);
}


void run()
{
	correct = "";
gdk_threads_enter ();
gtk_widget_grab_focus(GTK_WIDGET(entry));
	qustion = random_qustion_generator();
	g_print("%s",qustion);
	gtk_text_buffer_set_text(textbuffer,qustion,-1);
	if (iter != 100)
		speak(qustion,1,0);
	else
		speak(qustion,1,1);
	iter = 0;
	if (lessons[lesson].type != LETTERS)
		gtk_text_buffer_set_text(sub_textbuffer,
					g_utf8_substring(qustion,iter,iter+1),-1);
	set_hand(g_utf8_substring(qustion,iter,iter+1));
gdk_threads_leave();
	time(&time_qustion);
}


void key_release_event()
{
	int time_taken;
	const gchar *out = gtk_entry_get_text(entry);
	if (strcmp(qustion,out) == 0)
	{
//gdk_threads_add_idle((GSourceFunc)rotate,NULL);
gdk_threads_enter ();
g_thread_new("rotate",rotate,NULL);
gdk_threads_leave();
		gtk_entry_set_text(entry,"");
		correct = "";
		time_taken = difftime(time(0),time_qustion);
		g_print ( "\nAnswer time = %d\n",time_taken);
		if (time_taken <= 4)
		play("excellent.ogg");
		else if	(time_taken <= 8)
		play("very_good.ogg");
		else if (time_taken <= 12)
		play("good.ogg");
		else if(time_taken <= 16)
		play("grading_ok.ogg");				
		else
		play("try_more_fast.ogg");
		if (mark == lessons[lesson].win_point){
			time_taken = difftime(time(0),time_lesson_start);
			g_print("\nLesson finish time = %d",time_taken);
			jump_to_next_or_previous_lesson(NULL,+1);
			}
		else{
			set_point_view(SKIP,mark);
			mark++;
			run();
		}	 
	}
	else
	{
		if (strcmp(g_utf8_substring(out,iter,iter+1),
					g_utf8_substring(qustion,iter,iter+1)) == 0)
		{
			correct=strdup(out);
			iter++;
			speak(g_utf8_substring(qustion,iter,iter+1),1,0);
			set_hand(g_utf8_substring(qustion,iter,iter+1));
			//play("next.ogg");
			if (lessons[lesson].type != LETTERS)
			gtk_text_buffer_set_text(sub_textbuffer,
					g_utf8_substring(qustion,iter,iter+1),-1);}
		else
		{
			gtk_entry_set_text(entry,correct);
			gtk_editable_set_position(GTK_EDITABLE(entry),strlen(correct));
			speak("Not that! Type ",0,0);
			speak(g_utf8_substring(qustion,iter,iter+1),1,1);
		}
	}
}

void jump_to_next_or_previous_lesson(GtkWidget* w,int count)
{
	mark = 1;
	time_lesson_start = time(0);
	lesson = gtk_spin_button_get_value_as_int(spinbutton);
	lesson += count;
	gtk_spin_button_set_value(spinbutton,lesson);
	speak(lessons[lesson].instruction,0,0);
	gtk_label_set_text(instruction_label,lessons[lesson].instruction);
	//To Pass information that which function is called run()
	iter = 100;
	if (lessons[lesson].type == LETTERS){
		gtk_widget_hide(GTK_WIDGET(sub_textview));}
	else{
		gtk_widget_show(GTK_WIDGET(sub_textview));}
	set_point_view(lessons[lesson].win_point,ZERO);
	gtk_label_set_text(instruction_label,lessons[lesson].instruction);
	run();
}



void activate()
{
	play("1.ogg");
} 

void hear_instruction(){
	speak(lessons[lesson].instruction,1,0);}

void set_language()
{	
	load(gtk_combo_box_text_get_active_text(combobox));
}

void about(){
	g_print("Called");
	GtkBuilder* about_builder;
	GtkWidget* about_window;
	about_builder = gtk_builder_new();
	gchar* glade_file = malloc(70);
	sprintf(glade_file,"%s/ui/about.glade",directory);
	gtk_builder_add_from_file(about_builder,glade_file,NULL);
	about_window = GTK_WIDGET(gtk_builder_get_object(about_builder,"window"));
	gtk_widget_show(about_window);
}

int main(int argc,char *argv[])
{

	correct = malloc(200);
	
	//Espeak
	espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 500, NULL, 0 ); 
	speak_text = malloc(500);
	
	//Creating Canbra context(Sound)
	ca_context_create(&context);  
	
	//Inetiating Gdk Threads
	//gdk_threads_init ();
	
	//Inetiating Gtk	
	gtk_init (&argc, &argv);

	//Qustion
	qustion = malloc(50);
	
	//GUI
	GtkBuilder* builder = gtk_builder_new();
	gchar* glade_file = malloc(70);
	sprintf(glade_file,"%s/ui/ui.glade",directory);
	gtk_builder_add_from_file (builder,glade_file, NULL);
	GtkWidget* window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));

	//Reading data from /usr/share/angela-open-talking-typer/data/
	combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object (builder, "comboboxtext"));
	g_signal_connect(G_OBJECT(combobox),"changed",G_CALLBACK(set_language),NULL);	
	DIR *dir;	struct dirent *ent;
	if ((dir = opendir ("/usr/share/angela-open-talking-typer/data/")) != NULL){
		while ((ent = readdir (dir)) != NULL){
			if (ent->d_name[0] != '.'){
				gtk_combo_box_text_append(combobox,NULL,ent->d_name);
				g_print("Got - %s\n",ent->d_name);}
			}closedir (dir);}
	
	//Start Button
	GtkWidget* button_start = GTK_WIDGET(gtk_builder_get_object(builder,"button_start"));
	g_signal_connect(G_OBJECT(button_start),"clicked",G_CALLBACK(jump_to_next_or_previous_lesson),(int*)0);	
	
	//Next Lesson Button
	GtkWidget* button_next = GTK_WIDGET(gtk_builder_get_object(builder,"button_next"));
	g_signal_connect(G_OBJECT(button_next),"clicked",G_CALLBACK(jump_to_next_or_previous_lesson),(int*)+1);
	
	//Previous Lesson Button
	GtkWidget* button_previous = GTK_WIDGET(gtk_builder_get_object(builder,"button_previous"));
	g_signal_connect(G_OBJECT(button_previous),"clicked",G_CALLBACK(jump_to_next_or_previous_lesson),(int*)-1);	
	
	//Instructon Button
	GtkWidget* button_instruction = GTK_WIDGET(gtk_builder_get_object(builder,"instruction_button"));
	g_signal_connect(G_OBJECT(button_instruction),"clicked",G_CALLBACK(hear_instruction),NULL);	

	//About Button
	GtkWidget* button_about = GTK_WIDGET(gtk_builder_get_object(builder,"about_button"));
	g_signal_connect(G_OBJECT(button_about),"clicked",G_CALLBACK(about),NULL);

	//Quit Button
	GtkWidget*button_quit = GTK_WIDGET(gtk_builder_get_object(builder,"button_quit"));
	g_signal_connect(G_OBJECT(button_quit),"clicked",G_CALLBACK(gtk_main_quit),NULL);
	
	//Entry
	entry = GTK_ENTRY(gtk_builder_get_object(builder,"entry"));
	g_signal_connect(G_OBJECT(entry),"activate",G_CALLBACK(activate),NULL);	
	g_signal_connect(G_OBJECT(entry),"key_release_event",G_CALLBACK(key_release_event),NULL);	


	//TextView and Buffer
	GtkTextView* textview = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"textview"));
	textbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder,"textbuffer"));
	gtk_text_buffer_set_text(textbuffer,"Welcome",-1);
	PangoFontDescription *font =  pango_font_description_from_string("Purisa 60");	
	gtk_widget_modify_font(GTK_WIDGET(textview),font);
	GdkColor color;
	gdk_color_parse("#FFFF00",&color);
	gtk_widget_modify_text(GTK_WIDGET(textview),GTK_STATE_NORMAL,&color);
	gdk_color_parse("#000000",&color);
	gtk_widget_modify_base(GTK_WIDGET(textview),GTK_STATE_NORMAL,&color);

	//Letter Textview
	sub_textview = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"sub_textview"));
	sub_textbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder,"sub_textbuffer"));
	font =  pango_font_description_from_string("Purisa 60");	
	gtk_widget_modify_font(GTK_WIDGET(sub_textview),font);
	gdk_color_parse("#FFFF00",&color);
	gtk_widget_modify_text(GTK_WIDGET(sub_textview),GTK_STATE_NORMAL,&color);
	gdk_color_parse("#000000",&color);
	gtk_widget_modify_base(GTK_WIDGET(sub_textview),GTK_STATE_NORMAL,&color);	
	
	//Image
	image_hand = GTK_WIDGET(gtk_builder_get_object(builder,"image_hand"));
	image_win_point_1 = GTK_WIDGET(gtk_builder_get_object(builder,"image_win_point_1"));
	image_win_point_2 = GTK_WIDGET(gtk_builder_get_object(builder,"image_win_point_2"));
	image_current_point_1 = GTK_WIDGET(gtk_builder_get_object(builder,"image_current_point_1"));
	image_current_point_2 = GTK_WIDGET(gtk_builder_get_object(builder,"image_current_point_2"));
	
		
	//Instruction Label
	instruction_label = GTK_LABEL(gtk_builder_get_object(builder,"instruction_label"));
	PangoFontDescription *label_font =  pango_font_description_from_string("Sans Bold 10");
	gtk_widget_modify_font(GTK_WIDGET(instruction_label),label_font);

	//Instruction Label
	GtkLabel* main_label = GTK_LABEL(gtk_builder_get_object(builder,"main_label"));
	PangoFontDescription *main_label_font =  pango_font_description_from_string("Sans Bold 15");
	gtk_widget_modify_font(GTK_WIDGET(main_label),main_label_font);

	//Level Spinn Button
	spinbutton = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"spinbutton_level"));
	
	//speak("Welcome to Angela Open Talking Typer");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combobox),0);
	
	set_point_view(ZERO,ZERO);
		
	gtk_widget_show(window);
	gtk_main();
	
	return 0;
}

