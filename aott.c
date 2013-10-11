/*
This is a small programm writen by Nalin.x.Linux
Please change the value of macro named directory to 
where the data files are.

install dependence's with following command
apt-get install libespeak-dev libcanberra-gtk3-dev libgtk-3-dev git 

Compile using folowing command
gcc -Wall -o angela aott.c -lespeak -I/usr/include/espeak/ `pkg-config --cflags --libs gtk+-3.0 libcanberra-gtk3`
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
#include "tts.h"
#include "aott.h"
#include "ui.h"



//Lesson Struct
struct lesson_def
{
	int type;
	int win_point;
	int time;
	gchar allowed_letters[MAX_LENGTH];
	gchar target_leters[MAX_LENGTH];
	gchar instruction[500];
};

//Time Varibles
long int time_qustion;
long int time_lesson_start;

//Mark 
int point;
int word_count;
int total_errors;

//TTS Voice 
gchar *voice;

struct lesson_def lessons[100];
int ending_lesson;

gchar *correct;
int lesson;
int iter;

//Qustion
gchar *qustion;
int max_qustions;

int lesson_letter_count;
int lesson_word_count;
int lesson_sentence_count;

//Gtk
GtkSpinButton *spinbutton;
GtkEntry* entry;
GtkComboBoxText* combobox;
GtkLabel* instruction_label;


void jump_to_next_or_previous_lesson(GtkWidget* w,int count);
void key_release_event();
void set_point_view(int win, int current);
void load(gchar language[]);
void set_hand(gchar* key);
gchar *get_slited_letters(gchar string[]); 
void run();




/*
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
*/



void play(char* file)
{
	gchar* temp = malloc(200);
	sprintf(temp,"%ssounds/%s",directory,file);
	ca_context_cancel(context,-1);
	ca_context_play(context,SOUND_ID,CA_PROP_MEDIA_FILENAME,temp,NULL);
}


void play_music()
{
        gchar temp[200];
		int random_num = rand()%7;
        sprintf(temp,"%ssounds/next_level_%d.ogg",directory,random_num);
        ca_context_cancel(context,-1);
        ca_context_play(context,SOUND_ID,CA_PROP_MEDIA_FILENAME,temp,NULL);


}

void load(gchar language_file[])
{
	int i;gchar* temp = malloc(500);	
		
	//Opening File
	FILE *fp;
	gchar* file = malloc(70);
	sprintf(file,"%sdata/%s",directory,language_file);
	fp = fopen(file,"r");
	
	//Setting voice
	voice = malloc(20);
	fscanf(fp,"%s",voice);
	tts_set_voice(voice);
	
	for(i=0;;i++){
		fscanf(fp,"%s",letter[i]);
		if (strcmp(letter[i],"====")==0)
			break;
		else
			fscanf(fp," %s\n",value[i]);
		lesson_letter_count = i;}
			
	for(i=0;;i++){
		fscanf(fp,"%s",word[i]);
		if (strcmp(word[i],"====")==0){
			fgetc(fp);fgetc(fp);
			break;}
			lesson_word_count = i;}
	for(i=0;;i++){
		fgets(temp,100,fp);
		strcpy(sentence[i],g_utf8_substring(temp,0,g_utf8_strlen(temp,-1)-1));
		if (strcmp(sentence[i],"====")==0){
			break;}
			lesson_sentence_count = i;}
			
	for(i=0;!feof(fp) && fp != NULL;i++){
		fscanf(fp,"%d %d %d %s %s",&lessons[i].type,
		&lessons[i].win_point,&lessons[i].time,lessons[i].allowed_letters,lessons[i].target_leters);
		fgets(temp,500,fp);
		strcpy(lessons[i].instruction,g_utf8_substring(temp,0,g_utf8_strlen(temp,-1)-1));
		ending_lesson = i;
		g_print("\nRead a Lesson %d with allowed : %s\nLesson Target  \t\t     : %s\n",i,lessons[i].allowed_letters,lessons[i].target_leters);}		
	fclose(fp);
}

void make_list_from_list(gchar list[][MAX_LENGTH],int size)
{
	int i=0;
	int j,k,switch_1,switch_2,temp_switch;
	max_qustions = 0;
	fprintf(stderr,"\nCurrent Lesson Allowed : [%s] Target : [%s]",lessons[lesson].allowed_letters,lessons[lesson].target_leters);
	do
	{
		//g_print("\nChecking  %s ",list[i]);
		switch_1 = 1; switch_2 = 0;
		for(j=0;j<g_utf8_strlen(list[i],-1);j++)
		{
			
			//Checking allowed
			temp_switch = 0;
			for(k=0;k<g_utf8_strlen(lessons[lesson].allowed_letters,-1);k++)
			{
				if(strcmp(g_utf8_substring(list[i],j,j+1),g_utf8_substring(lessons[lesson].allowed_letters,k,k+1)) == 0	
				 ||strcmp(g_utf8_substring(list[i],j,j+1)," ") == 0)
				{
					temp_switch = 1;
				}
			}
			
			if(temp_switch == 0)
				switch_1 = 0;
			
			//Checking for at least one target letter
			for(k=0;k<g_utf8_strlen(lessons[lesson].target_leters,-1);k++)
			{
				if(strcmp(g_utf8_substring(list[i],j,j+1),g_utf8_substring(lessons[lesson].target_leters,k,k+1)) == 0)
					switch_2 = 1;
			}
		}
		
		
		if(switch_1 && switch_2)
		{
			g_utf8_strncpy(qustion_list[max_qustions],list[i],g_utf8_strlen(list[i],-1));
			g_print("\nGot %s ",list[i]);
			max_qustions++;
		}
		i++;
	}
	while(i <= size);
}

void hear_instruction(){
	int playing1 = 1;
	while(1)
	{	
		ca_context_playing(context,SOUND_ID,&playing1);
		if (playing1 == 0)
			break;
		sleep(1);
	}
	
	tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",lessons[lesson].instruction);
	while(tts_playing())
	{ sleep(1);}
}
	
gchar *get_slited_letters(gchar string[])
{
	gchar *temp;
	temp = malloc(1000);
	temp[0] = '\0';
	int j=0;
	while(1)
	{
		if (g_utf8_collate(g_utf8_substring(string,j,j+1),"\0") == 0)
			break;
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1)," ") == 0 ){
			g_strlcat(temp,"space",100);
			break;}
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1),",") == 0)
			g_strlcat(temp,"comma",100);
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1),".") == 0)
			g_strlcat(temp,"full stop",100);
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1),"\'") == 0)
			g_strlcat(temp,"apostophe",100);
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1),";") == 0)
			g_strlcat(temp,"semicolon",100);
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1),":") == 0)
			g_strlcat(temp,"colon",100);
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1),"?") == 0)
			g_strlcat(temp,"Qustion mark",100);
		else if (g_utf8_collate(g_utf8_substring(string,j,j+1),"-") == 0)
			g_strlcat(temp,"Hyphen",100);
		else
			g_strlcat(temp,g_utf8_substring(string,j,j+1),100);
			
		g_strlcat(temp,". ",100);
		j++;
	}
	return temp;
	
}

void jump_to_next_or_previous_lesson(GtkWidget* w,int count)
{
	point = 0;
	word_count = 0;
	total_errors = 0;	
	time_lesson_start = time(0);
	lesson = gtk_spin_button_get_value_as_int(spinbutton);
	lesson += count;
	gtk_spin_button_set_value(spinbutton,lesson);
	
	//generating qustion list
	if (lessons[lesson].type == LETTERS)
		make_list_from_list(letter,lesson_letter_count);
	else if (lessons[lesson].type == WORDS)
		make_list_from_list(word,lesson_word_count);
	else if(lessons[lesson].type == SENTENCE)
		make_list_from_list(sentence,lesson_sentence_count);	
	
	hear_instruction();
	gtk_label_set_text(instruction_label,lessons[lesson].instruction);
	//To Pass information that which function is called run()
	iter = 100;
	set_point_view(lessons[lesson].win_point,ZERO);
	gtk_label_set_text(instruction_label,lessons[lesson].instruction);
	run();
}

void run()
{
	int number = rand();
	correct = "";
	//gdk_threads_enter ();
	gtk_widget_grab_focus(GTK_WIDGET(entry));
	
	//geting a random qustion from list
	srand(time_qustion);
	srand(number*329);
	number %= max_qustions;
	g_utf8_strncpy(qustion,qustion_list[number],g_utf8_strlen(qustion_list[number],-1));
	
	//setting the qustion
	g_print("%s",qustion);
	gtk_text_buffer_set_text(textbuffer,qustion,-1);

	//Resetting iter
	iter = 0;
	
	//Highlighting the letter
	if (lessons[lesson].type != LETTERS){
		clear_tag();
		set_tag(iter,iter+1,HIGHLIGHT_FG_COLOR,HIGHLIGHT_BG_COLOR);}
	
	if (lessons[lesson].type == WORDS)
		tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s, %s",qustion,get_slited_letters(qustion));
	else if(lessons[lesson].type == SENTENCE)
		tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",qustion);
	else
		tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",get_slited_letters(qustion));
		
	set_hand(g_utf8_substring(qustion,iter,iter+1));
	time(&time_qustion);
}

void key_release_event()
{
	int time_taken;
	int wpm,cpm;
	float efficiency;
	gchar result[100];
	const gchar *out = gtk_entry_get_text(entry);
	if (strcmp(qustion,out) == 0)
	{
		gtk_entry_set_text(entry,"");
		correct = "";
		time_taken = difftime(time(0),time_qustion);
		g_print ( "\nAnswer time = %d\n",time_taken);
		if (time_taken <= lessons[lesson].time){
			play("excellent.ogg");
			set_face("hard_smile");}
		else if(time_taken <= lessons[lesson].time+(lessons[lesson].time/2)){
			play("very_good.ogg");
			set_face("laugh");}
		else if (time_taken <= lessons[lesson].time*2){
			play("good.ogg");
			set_face("wink");}
		else if(time_taken <= (lessons[lesson].time*2)+(lessons[lesson].time/2)){
			play("ok.ogg");
			set_face("uncertain");
			point--;}		
		else{
			play("try_more_fast.ogg");
			set_face("sad");
			point--;}

		if ( point == lessons[lesson].win_point){
			time_taken = difftime(time(0),time_lesson_start);
			g_print("\nLesson finish time = %d",time_taken);
			play("clap.ogg");
			word_count += 1;
			if (lessons[lesson].type == LETTERS)
			{
				wpm = ((60*word_count)/time_taken);
				efficiency = (wpm*100)/150;
				efficiency -= total_errors;			
				sprintf(result,"Result for administrator %d Characters, %d Characters per minute, %d Errors, In %d Seconds  And Efficiency = %d!",
				word_count,wpm,total_errors,time_taken,(int)efficiency);
			}
			else
			{
				cpm = ((60*word_count)/time_taken);
				efficiency = (cpm*100)/30;
				efficiency -= total_errors;			
				sprintf(result,"Result for administrator %d Words, %d Word per minute, %d Errors, In %d Seconds  And Efficiency = %d!",
					word_count,cpm,total_errors,time_taken,(int)efficiency);
			}	
			tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",result);
			int playing1 = 1;
			int playing2 = 1;
			while(1)
			{
				sleep(1);
				playing2 = espeak_IsPlaying();
				ca_context_playing(context,SOUND_ID,&playing1);
				if (playing1 == 0 && playing2 == 0)
					break;
				g_print("playing ");
			}
			
			
			
			if (lesson+1 < ending_lesson){
				play_music();
				jump_to_next_or_previous_lesson(NULL,+1);}
			else{
				tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"Congratulations! You have finished all lessons!"); 				
			}
			}
		else{
			set_point_view(SKIP,point);
			point++;
			word_count += 1;
			run();
		}	 
	}
	else
	{
		if (strcmp(g_utf8_substring(out,iter,iter+1),
					g_utf8_substring(qustion,iter,iter+1)) == 0)
		{
			//Correct letter pressed
			correct=strdup(out);
			iter++;
			set_hand(g_utf8_substring(qustion,iter,iter+1));
			play("type.ogg");
			if (lessons[lesson].type != LETTERS){
				clear_tag();
				set_tag(iter,iter+1,HIGHLIGHT_FG_COLOR,HIGHLIGHT_BG_COLOR);	}
			
			
			if (lessons[lesson].type == SENTENCE && g_utf8_collate(g_utf8_substring(qustion,iter-1,iter)," ") == 0)	{
				word_count += 1;
				tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",g_utf8_substring(qustion,iter,g_utf8_strlen(qustion,-1)));
			}
						
		}
		else
		{
			//Wrong pressed
			play("wrong_pressed.ogg");
			gtk_entry_set_text(entry,correct);
			gtk_editable_set_position(GTK_EDITABLE(entry),strlen(correct));
			total_errors += 1;
			tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",get_slited_letters(g_utf8_substring(qustion,iter,g_utf8_strlen(qustion,-1))));
		}
	}
}


void set_language()
{	
	load(gtk_combo_box_text_get_active_text(combobox));
	gtk_spin_button_set_range(spinbutton,0,ending_lesson-1);
}

int main(int argc,char *argv[])
{

	correct = malloc(200);
	
	//File
	char file[100];
	
	//Espeak
	tts_init();
	
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
	sprintf(glade_file,"%sui/ui.glade",directory);
	gtk_builder_add_from_file (builder,glade_file, NULL);
	GtkWidget* window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));

	//Reading data from directory/data/
	gchar* data_dir = malloc(200);;
	sprintf(data_dir,"%sdata",directory);	

	combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object (builder, "comboboxtext"));
	g_signal_connect(G_OBJECT(combobox),"changed",G_CALLBACK(set_language),NULL);	
	DIR *dir;	struct dirent *ent;
	if ((dir = opendir (data_dir)) != NULL){
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
	g_signal_connect(G_OBJECT(entry),"key_release_event",G_CALLBACK(key_release_event),NULL);	


	//TextView and Buffer
	GtkTextView* textview = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"textview"));
	textbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder,"textbuffer"));
	gtk_text_buffer_set_text(textbuffer,"Welcome",-1);
	PangoFontDescription *font =  pango_font_description_from_string("Georgia Bold 60");	
	gtk_widget_modify_font(GTK_WIDGET(textview),font);
	
	
	GdkRGBA color;
	gdk_rgba_parse(&color,BG_COLOR);
	gtk_widget_override_background_color(GTK_WIDGET(textview),GTK_STATE_NORMAL,&color);
	gdk_rgba_parse(&color,FG_COLOR);
	gtk_widget_override_color(GTK_WIDGET(textview),GTK_STATE_NORMAL,&color);
	
	
	//Image
	image_hand = GTK_WIDGET(gtk_builder_get_object(builder,"image_hand"));
	image_win_point_1 = GTK_WIDGET(gtk_builder_get_object(builder,"image_win_point_1"));
	image_win_point_2 = GTK_WIDGET(gtk_builder_get_object(builder,"image_win_point_2"));
	image_current_point_1 = GTK_WIDGET(gtk_builder_get_object(builder,"image_current_point_1"));
	image_current_point_2 = GTK_WIDGET(gtk_builder_get_object(builder,"image_current_point_2"));
	
	//smile image
	face_image = GTK_WIDGET(gtk_builder_get_object(builder,"face_image"));
	sprintf(file,"%sfaces/plain.png",directory);
	animation_test = gdk_pixbuf_animation_new_from_file (file, NULL);
		
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
	
	play("start.ogg");
	//tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"Welcome to Angela-Typing-Tutor");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combobox),0);
	
	set_point_view(ZERO,ZERO);
	set_face("smile");
		
	gtk_widget_show(window);
	gtk_main();
	
	return 0;
}

