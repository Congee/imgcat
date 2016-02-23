#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>

#include "b64/b64.h"

size_t get_filesize(const char *filename) {
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}

void print_osc() {
  printf((strncmp(getenv("TERM"), "screen", 6) == 0) ? "\033Ptmux;\033\033]"
                                                     : "\033]");
}

void print_st() {
  printf((strncmp(getenv("TERM"), "screen", 6) == 0) ? "\a\033\\" : "\a");
}

void print_image(const char *filename, const bool isinline, const char *width,
                 const char *height, const bool preserveAspectRatio) {

  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("");
    exit(EXIT_FAILURE);
  }

  print_osc();
  printf("1337;File=");
  printf("name=%s;", b64_encode((unsigned char *)filename, strlen(filename)));

  size_t size = get_filesize(filename);
  char *content = malloc(size);
  fread(content, size, 1, file);
  char *encoded_content = b64_encode((unsigned char *)content, size);

  // not equal
  if (strcmp(width, "auto")) printf("width=%s;", width);
  if (strcmp(height, "auto")) printf("height=%s;", height);

  printf("preserveAspectRatio=%d;", preserveAspectRatio);
  printf("size=%ld;inline=%d:%s", size, isinline, encoded_content);
  print_st();
  putchar('\n');

  fclose(file);
  free(content);
  free(encoded_content);
}

void print_usage() {
  fprintf(stderr, "Usage: imgcat filename ...\n");
  fprintf(stderr, "   or: cat filename | imgcat\n");
}

int main(int argc, char *argv[]) {
  char *filename = NULL;
  char *width = "auto";
  char *height = "auto";
  bool preserveAspectRatio = false;

  if (argc == 1) {
    print_usage();
    exit(EXIT_FAILURE);
  }

  int choice;
  while (1) {
    static struct
        option long_options[] = {/* Use flags like so:
                                 {"verbose",	no_argument,
                                 &verbose_flag, 'V'}*/
                                 /* Argument styles: no_argument,
                                    required_argument, optional_argument */
                                 {"version", no_argument, 0, 'v'},
                                 {"help", no_argument, 0, 'h'},
                                 {"width", optional_argument, 0, 'w'},
                                 {"height", optional_argument, 0, 'l'},
                                 {"preserveAspectRatio", no_argument, 0, 'p'},
                                 {0, 0, 0, 0}};

    int option_index = 0;

    /* Argument parameters:
            no_argument: " "
            required_argument: ":"
            optional_argument: "::" */

    choice = getopt_long(argc, argv, "vhw:l:p", long_options, &option_index);

    if (choice == -1) break;

    switch (choice) {
      case 'v':
        /* version */
        printf("LOL\n");
        break;

      case 'h':
        /* help */
        print_usage();
        exit(EXIT_SUCCESS);
        break;

      case 'w':
        width = optarg;
        break;

      case 'l':
        height = optarg;
        break;

      case 'p':
        preserveAspectRatio = true;
        break;

      case '?':
        /* getopt_long will have already printed an error */
        break;

      default:
        /* Not sure how to get here... */
        print_usage();
        return EXIT_FAILURE;
    }
  }

  /* Deal with non-option arguments here */
  // printf("optind: %d, argc: %d\n", optind, argc);
  if (optind < argc)
    while (optind < argc) filename = argv[optind++];

  print_image(filename, true, width, height, preserveAspectRatio);
  return 0;
}
