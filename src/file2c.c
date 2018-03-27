/**
 * This is a simple file dump, it reads files and converts them to a C array with a header
 * and corresponding .c file. It is meant to be used as a resource compiler for embedding and linking.
 *
 * Be aware that big files will make your executable bigger!
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void dump(const char infile[], FILE * hfile, FILE * cfile);
int make_identifier(const char string[], char output[], size_t output_size);

int main(int argc, char * argv[]) {
  unsigned i = 0;
  FILE * cfile, * hfile;
  if (argc < 4) return EXIT_FAILURE;
  hfile = fopen(argv[1], "w+");
  cfile = fopen(argv[2], "w+");
  for (i = 3; i < argc; i++) dump(argv[i], hfile, cfile);
  fclose(cfile);
  fclose(hfile);
  return 0;
}

void dump(const char filename[], FILE * hfile, FILE * cfile) {
  FILE * infile = fopen(filename, "rb");
  size_t size = strlen(filename) +1;
  char * identifier = calloc(size, sizeof(char));
  unsigned char c = 0;
  size_t n = 0, column = 0, total = 0;
  if (!infile || !hfile || !cfile) { return; }
  make_identifier(filename, identifier, size);
  fprintf(hfile, "extern const unsigned char %s[];\n", identifier);
  fprintf(hfile, "extern unsigned long %s_size;\n", identifier);
  fprintf(cfile, "const unsigned char %s[] = {\n", identifier);
  n = fread(&c, sizeof(c), 1, infile);
  total = n;
  while (n) {
    if (column == 20) { fprintf(cfile, "\n"); column = 0; }
    if (column == 0) { fprintf(cfile, "  "); }
    fprintf(cfile, " %3d", c);
    n = fread(&c, sizeof(c), 1, infile);
    total += n;
    if (n) fprintf(cfile, ",");
    column++;
  }
  fprintf(cfile, "\n};\n");
  fprintf(cfile, "unsigned long %s_size = %lu;\n\n", identifier, total);
  free(identifier);
}

int make_identifier(const char input[], char output[], size_t outputsize) {
  size_t inputlen = strlen(input), inputsize = inputlen +1;
  long i = 0;
  for (i = inputlen-1; i >= 0 && inputsize <= outputsize; i--) {
    output[i] = isalnum(input[i]) ? input[i] : (char)'_';
  }
  if (isdigit(output[0])) output[0] = '_';
  output[inputlen] = 0;
  return (int)inputlen;
}