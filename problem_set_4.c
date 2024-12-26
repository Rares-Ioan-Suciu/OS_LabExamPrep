#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

// Build an application that recursively traverses a directory (specified as the first
// argument) and saves the sizes of all regular files into another file (specified as the
// second argument).


//this is basically exercise 2 but I write to a file

void compute_size(const char * dir, const int fd) {
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
              compute_size(fullAssPath, fd);
         } // if it's a subdirecotry call the function for it
         else if (ent->d_type == DT_REG) {
              int fd_src = open(fullAssPath, O_RDONLY);
              if (fd_src == -1) {
                   perror("the file didn't open\n" );
              }
              else {
                   const long fileSize = lseek(fd_src, 0, SEEK_END); // get size using lseek babes
                   if (fileSize == -1) {
                        fprintf(stderr, "Error getting file size for '%s'\n", fullAssPath);
                        close(fd_src);
                   }
                   else {
                        char buffer[1024] = {0};
                        int byteswritten = snprintf(buffer, sizeof(buffer), "The file %s has size: %ld\n",fullAssPath, fileSize);
                        if (byteswritten == -1) {
                             perror("the file didn't open\n" );
                        }
                        else {
                             printf("%s\n", buffer);
                             write(fd, buffer, byteswritten);
                        }
                        if (close(fd_src) == -1) {
                             perror("the file close didn't open\n" );
                        }
                   }
              }
         }
    }
}

int main(const int argc, const char **argv) {
     if (argc != 3) {
          printf("Usage: ./problem_set_2 <DirectoryName> <FileToSave>\n");
     }
     const int fd = open(argv[2], O_WRONLY| O_CREAT | O_TRUNC, 0644);
     if (fd == -1) {
          perror("opening file to write");
          return 1;
     }
     compute_size(argv[1], fd);
     if (close(fd) == -1) {
          perror("closing file to write");
          return 1;
     }
     return 0;
}