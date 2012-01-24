#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>



void count_occurances(char* word, char* filename, int invert_flag) {
  FILE* file = fopen(filename, "r");
  if(!file) {
    perror("fopen: ");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  
  char* str = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fileno(file), 0);
  if(!str) {
    perror("mmap: ");
    exit(1);
  }

  int i, match_count = 0, wordlen = strlen(word), inc = invert_flag ? -1 : 1;
  
  for(i = 0; i < wordlen; i++) {
    word[i] = toupper(word[i]);
  }
  for(i = 0; i < size; i++) {
    if(toupper(str[i]) == *word) {
      int j;
      for(j = 1;  word[j] == toupper(str[i+j]) && j < wordlen; j++);
      if(j == wordlen) {
	match_count += inc;
      }
    }
  }

  if(match_count) {
    printf("%s\t%d\n", filename, match_count);
  }

  munmap(str, size);
  fclose(file);
}

void usage(void) {
  fprintf(stderr, "usage: woc [word] [files...]\n");
  fprintf(stderr, "count the number of occurances of word in file(s)\n");
}

int main(int argc, char **argv) {
  int help_requested = 0, invert_flag, i;
  for(i = 1; i < argc; i++) {
    if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      help_requested = 1;
    }
  }
  
  if(argc < 3 || help_requested) {
    usage();
    exit(1);
  }
  char* word = argv[1];
  char** files = argv+2;
  int nfiles = argc-2;

  for(i = 0; i < nfiles; i++) {
    if(files[i])
      count_occurances(word, files[i], invert_flag);
  }
  return 0;
}
