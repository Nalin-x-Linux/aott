#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef char text[10];
word words[100][100];


void load(char filename[],text container)
{
	
	
}


int main()
{
	char *line;
	int i;
	line = malloc(100);
	
	
	FILE *fp;
	fp = fopen("malayalam_letter.txt","r");

	
	for(i=0;!feof(fp) && fp != NULL;i++)
	{
		fgets(line,100,fp);
		printf("%s",line);
		strcpy(words[i][0],line);
	}
	printf("%s",text[0][0]);
	return 0;
}
