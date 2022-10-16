/*
 * convert.c : take a file in the form 
 *  word1
 *  multiline definition of word1
 *  stretching over several lines, 
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dict.h"
#define BIGLINE 512

int main(int argc, char **argv) {
	FILE *in;
	FILE *out;        /* defaults */
	char line[BIGLINE];
	static Dictrec dr, blank;
	int count = 0;

	memset(dr.word, NULL, WORD);
	memset(dr.text, NULL, TEXT);
	
	/* If args are supplied, argv[1] is for input, argv[2] for output */
	if (argc==3) {
		if ((in =fopen(argv[1],"r")) == NULL){DIE(argv[1]);}
		if ((out =fopen(argv[2],"wb")) == NULL){DIE(argv[2]);}	
	}
	else{
		printf("Usage: convert [input file] [output file].\n");
		return -1;
	}

	/* Main reading loop : read word first, then definition into dr */

	/* Loop through the whole file. */
	while (!feof(in)) {
		
		/* Create and fill in a new blank record.
		 * First get a word and put it in the word field, then get the definition
		 * and put it in the text field at the right offset.  Pad the unused chars
		 * in both fields with nulls.
		 */

		/* Read word and put in record.  Truncate at the end of the "word" field.
		 *
		 * Fill in code. */

		errno = 0;
		if(fgets(line, BIGLINE, in) == NULL && errno != 0) DIE("fgets");
		if(line[0] == '\n') continue;
		int i;
		for(i = 0; i < strlen(line); i++) dr.word[i] = line[i];

		/* Read definition, line by line, and put in record.
		 *
		 * Fill in code. */

		if(fgets(line, BIGLINE, in) == NULL && errno != 0) DIE("fgets");		
		while(line[0] != '\n'){
			for(i = 0; i < strlen(line); i++) dr.text[count + i] = line[i];
			count += strlen(line);
			errno = 0;
			if(fgets(line, BIGLINE, in) == NULL && errno != 0) DIE("fgets");
		}
		// add newline at the end of text
		dr.text[count] = '\n';
		count = 0;

		/* Write record out to file.
		 *
		 * Fill in code. */

		// write the content and padded NULL into file
		if(fwrite(dr.word, WORD, 1, out) < 0) DIE("fwrite");
		if(fwrite(dr.text, TEXT, 1, out) < 0) DIE("fwrite");

		memset(dr.word, NULL, WORD);
		memset(dr.text, NULL, TEXT);
	}

	fclose(in);
	fclose(out);
	return 0;
}
