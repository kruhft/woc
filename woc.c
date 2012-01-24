/*
 * woc - word occurance
 *
 * usage: woc [words...] -- [files...]
 *
 * Count the occurances of words... in files... printing the score to
 * standard out. If a word starts with '-' then reduce the count by 1
 * for each occurance of the given word.
 *
 * Author: Burton Samograd <burton.samograd@gmail.com> 2012
 * License: AGPL
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>



int count_occurances(char** words, int num_words, char* filename) {
  FILE* file = fopen(filename, "r");
  if(!file) {
    fprintf(stderr, "fopen: %s: ", filename); perror("");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  
  char* str = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fileno(file), 0);
  if(!str) {
    perror("mmap: ");
    exit(1);
  }

  int cur_word, i, match_count = 0;
  for(cur_word = 0; cur_word < num_words; cur_word++) {
    char* word = words[cur_word];
    int inc;
    if(*word == '-') {
      word++;
      inc = -1;
    } else {
      inc = 1;
    }
    int wordlen = strlen(word);
    
    for(i = 0; i < size; i++) {
      if(toupper(str[i]) == *word) {
	int j;
	for(j = 1;  word[j] == toupper(str[i+j]) && j < wordlen; j++)
	  if(j == wordlen) {
	    match_count += inc;
	}
      }
    }
  }

  munmap(str, size);
  fclose(file);

  return match_count;
}

void usage(void) {
  fprintf(stderr, "usage: woc [words...] -- [files...]\n");
  fprintf(stderr, "count the number of occurances of word(s) in file(s)\n");
}

int main(int argc, char **argv) {
  int help_requested = 0, i, j;
  int words_start = 1, num_words = 0;
  int files_start = -1, num_files = 0;
  for(i = 1; i < argc; i++) {
    if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      help_requested = 1;
      break;
    } else if(!strcmp(argv[i], "--")) {
      files_start = i+1;
      continue;
    }

    if(files_start > 0) {
      num_files++;
    } else {
      num_words++;
    }
  }

  if(argc < 3 || help_requested) {
    usage();
    exit(1);
  }
  char* words[num_words];
  char* files[num_files];
  for(i = words_start; i < words_start + num_words; i++) {
    /* uppercase word */
    for(j = 0; j < strlen(argv[i]); j++) {
      argv[i][j] = toupper(argv[i][j]);
    }
    words[i-words_start] = argv[i];
  }
  for(i = files_start; i < files_start + num_files; i++) {
    files[i-files_start] = argv[i];
  }

  FILE* sortpipe = popen("sort -r -V -k1,2", "w");
  for(i = 0; i < num_files; i++) {
    int match_count = count_occurances(words, num_words, files[i]);
    if(match_count > 0) {
      fprintf(sortpipe, "%d\t%s\n", match_count, files[i]);
    }
  }
  fclose(sortpipe);
  return 0;
}
