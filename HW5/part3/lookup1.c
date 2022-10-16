/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include "dict.h"
#define BIGLINE 512

int lookup(Dictrec * sought, const char * resource) {
	Dictrec dr;
	static FILE * in;
	static int first_time = 1;
	char line[BIGLINE];

	if (first_time) { 
		first_time = 0;
		/* open up the file
		 *
		 * Fill in code. */

		if((in = fopen(resource, "r")) == NULL) DIE(resource);
	}

	/* read from top of file, looking for match
	 *
	 * Fill in code. */
	memset(sought->text, NULL, sizeof(sought->text));

	rewind(in);
	while(fgets(line, BIGLINE, in) != NULL){
		line[strcspn(line, "\n")] = '\0';
		/* Fill in code. */
		if(strcmp(line, sought->word) == 0){
			// move the cursor according to the fixed-length content
			fseek(in, WORD - strlen(line) - 1, SEEK_CUR);
			fgets(line, BIGLINE, in);
			while(line != NULL && line[0] != '\n'){
				strcat(sought->text, line);
				fgets(line, BIGLINE, in);
			}
			// remove the last newline char
			char *find = strrchr(sought->text, '\n');
			find[0] = '\0';
			return FOUND;
		}
		// move the cursor according to the fixed-length content
		fseek(in, (WORD + TEXT) - strlen(line) - 1, SEEK_CUR);
	}

	return NOTFOUND;
}
