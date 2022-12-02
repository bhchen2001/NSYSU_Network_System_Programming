/*
 * lookup5 : local file ; setup a memory map of the file
 *           Use bsearch. We assume that the words are already
 *           in dictionary order in a file of fixed-size records
 *           of type Dictrec
 *           The name of the file is what is passed as resource
 */

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#include "dict.h"

/*
 * This obscure looking function will be useful with bsearch
 * It compares a word with the word part of a Dictrec
 */

int dict_cmp(const void *a, const void *b) {
    return strcmp((char *)a, ((Dictrec *)b)->word);
}

int lookup(Dictrec * sought, const char * resource) {
	static Dictrec * table;
	static int numrec;
	Dictrec * found;
	static int first_time = 1;

	if (first_time) {  /* table ends up pointing at mmap of file */
		int fd;
		long filsiz;

		first_time = 0;

		/* Open the dictionary.
		 * Fill in code. */
		fd = open("./fixrec", O_RDONLY);
		if(fd == -1) DIE("open() fail");

		/* Get record count for building the tree. */
		filsiz = lseek(fd, 0 ,SEEK_END);
		numrec = filsiz / sizeof(Dictrec);
		lseek(fd, 0, SEEK_SET);

		/* mmap the data.
		 * Fill in code. */
		table = mmap((caddr_t)NULL, numrec * sizeof(Dictrec), PROT_READ, MAP_SHARED, fd, (off_t)0);
		close(fd);
	}
    
	/* search table using bsearch
	 * Fill in code. */
	int start = 0, end = numrec;
	int cnt = 0;
	found = NULL;
	while(start != end){
		int mid = (end - start) / 2 + start;
		int offset = mid * sizeof(Dictrec);
		int result = dict_cmp((void *)&((table + mid)->word), (void *)sought);
		if(result == 0){
			// fprintf(stderr, "found the word %s\ntext: %s\n", (table + mid)->word, (table + mid)->text);
			found = (table + mid);
			break;
		}
		else if(result < 0) start = mid;
		else if(result > 0) end = mid;
		cnt += 1;
		if(cnt == 100) break;
	}
	if (found) {
		strcpy(sought->text,found->text);
		return FOUND;
	}

	return NOTFOUND;
}
