#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#define BUFFSIZE 1024

// 3. Write a C program for Unix, callable from the command line as:
// copytree path1 path2
// This program copies the directory path1, including all its subdirectories, to the
// destination path2. If path2 already exists, an error message is generated. If symbolic links are found, symbolic links will be created at the destination referring to
// the same file.

//If you ain't on the team, you playin' for team D
// 'Cause we A-listers, we paid sisters

void copy_file(const char *src, const char *dst)
{
    int fd = open(src, O_RDONLY);
    if (fd == -1) {
        perror("error opening source file");
        return;
    }
    int fd2 = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 == -1) {
        perror("error creating new file");
        return;
    }
    int n;
    char buff[BUFFSIZE];
    while ((n=read(fd, buff, BUFFSIZE)) > 0) {
         buff[n] = '\0';
         write(fd2, buff, n);
    }
    if (close(fd) == -1) {
        perror("error closing source file");
    }
    if (close(fd2) == -1) {
        perror("error closing new file");
    }
}

void copy_directory(const char *src_path, const char *dest_path)
{

    struct dirent *ent;
    char OriginFullPath[1024];
    char DestinationFullPath[1024];

    DIR *dir = opendir(src_path);
    if (dir == NULL) {
        perror("Error opening source directory");
    }

    if (mkdir(dest_path, 0755) == -1) {
        perror("Error creating destination directory");
        closedir(dir);
        return;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        sprintf(OriginFullPath, "%s/%s", src_path, ent->d_name);
        sprintf(DestinationFullPath, "%s/%s", dest_path, ent->d_name);

        if (ent->d_type == DT_DIR) {
            copy_directory(OriginFullPath, DestinationFullPath);
        }else if (ent->d_type == DT_REG) {
            copy_file(OriginFullPath,DestinationFullPath);
        }else if (ent->d_type == DT_LNK) {
            char linkTarget[1024];
            ssize_t linkLen = readlink(OriginFullPath, linkTarget, sizeof(linkTarget) - 1); // reading the symbolic link
            if (linkLen < 0) {
                perror("Unable to read symbolic link");
                continue; // skipping to the next entry if we are unable to read it
            }
            linkTarget[linkLen] = '\0';
            if (symlink(linkTarget, DestinationFullPath) == -1) {
                perror("Unable to create symbolic link");
            } // copy the link to the new directory
         }
        //the next lines are for some god forsaken files that I belived we would learn about, but we only heard of them one time and that was it
        //so yeah fuck them
        // else if (ent->d_type == DT_UNKNOWN) {
        //     fprintf(stderr, "File %s of unknown type, it will be skipped\n", OriginFullPath);
        //     continue;
        // } else { // this should take care of DT_FIFO, DT_CHR, DT_SOCK, DT_BLK
        //     struct stat fileStat;
        //     if (stat(OriginFullPath, &fileStat) == -1) {
        //         perror("Unable to get file info in the case for DT_FIFO, DT_CHR, DT_SOCK, DT_BLK");
        //         continue;
        //     }
        //
        //     const mode_t mode = fileStat.st_mode;
        //     const dev_t dev = fileStat.st_rdev;
        //
        //     // we use mknod to create the special file at the destination path, again not really sure what happens here
        //     // from what I've read doesn't really make sense to copy these, but did it for the sake of the exercise
        //     if (ent->d_type == DT_FIFO) { // FIFO (Named Pipe)
        //         if (mknod(DestinationFullPath, mode | S_IFIFO, 0) == -1) {
        //             perror("Unable to create FIFO");
        //         }
        //     } else if (ent->d_type == DT_CHR) { // Character device
        //         if (mknod(DestinationFullPath, mode | S_IFCHR, dev) == -1) {
        //             perror("Unable to create character device");
        //         }
        //     } else if (ent->d_type == DT_BLK) { // Block device
        //         if (mknod(DestinationFullPath, mode | S_IFBLK, dev) == -1) {
        //             perror("Unable to create block device");
        //         }
        //     } else if (ent->d_type == DT_SOCK) { // Domain socket
        //         if (mknod(DestinationFullPath, mode | S_IFSOCK, 0) == -1) {
        //             perror("Unable to create socket");
        //         }
        //     } else {
        //         fprintf(stderr, "Unknown special file type, skipping: %s\n", OriginFullPath);
        //     }
        }
}

int main(const int argc, const char * argv[]) {
    if (argc != 3) {
        printf("Usage of program: %s <original_path> <new_path\n", argv[0]);
    }
    DIR *dir = opendir(argv[2]);
    if (dir != NULL) {
        perror("New directory already exists, please choose another");
        closedir(dir);
        return 1;
    }
    copy_directory(argv[1], argv[2]);
    return 0;
}