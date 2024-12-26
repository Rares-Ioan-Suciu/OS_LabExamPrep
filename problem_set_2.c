#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>


// 2. Build an application that recursively traverses a directory passed as a parameter
// and displays the size of all regular files in this directory.

void compute_size(const char * dir) {
     struct dirent *ent; // special data type that we are using for working with directory and files
     char fullAssPath[1024]; // we will compute full path in here

    DIR *directory = opendir(dir);
    if (directory==NULL) {
         perror("the directory didn't open\n" );
          return;
    }
    while ((ent = readdir(directory)) != NULL) {
         if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
              continue; // ignore current file and directory file
         }

         if (dir[strlen(dir) - 1] == '/') {
              snprintf(fullAssPath, sizeof(fullAssPath), "%s%s", dir, ent->d_name);
         } else {
              snprintf(fullAssPath, sizeof(fullAssPath), "%s/%s", dir, ent->d_name);
         } // constrcut full path, be carfeul with double //

         if (ent->d_type == DT_DIR) {
              compute_size(fullAssPath);
         } // if it's a subdirecotry call the function for it
         else if (ent->d_type == DT_REG) {
              int fd = open(fullAssPath, O_RDONLY);
              if (fd == -1) {
                   perror("the file didn't open\n" );
              }
              else {
                   const long fileSize = lseek(fd, 0, SEEK_END); // get size using lseek babes
                   if (fileSize == -1) {
                        fprintf(stderr, "Error getting file size for '%s'\n", fullAssPath);
                        close(fd);
                   }
                   else {
                        printf("The file size for %s is %ld\n", fullAssPath, fileSize);
                   }
              }
         }
    }
}

int main(const int argc, const char **argv) {
     if (argc != 2) {
          printf("Usage: ./problem_set_2 <DirectoryName>\n");
     }
     compute_size(argv[1]);
}