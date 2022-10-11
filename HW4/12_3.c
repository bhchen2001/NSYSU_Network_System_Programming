/*
 * list the process ID and command name that running <filename>
 * usage:
 *      ./12_3 <filename>
 * must provide the absolute path of the <filename>
 */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

int main(int argc, char *argv[]){

    int status_fd, pathname_num;
    struct dirent *status_dir, *fd_dir;
    DIR *proc_dir, *procfd_dir;
    char *filename = argv[1];
    if(argc < 2){
        perror("empty argument");
        exit(1);
    }

    if((proc_dir = opendir("/proc")) == NULL){
        perror("opendir error");
        exit(1);
    }

    while((status_dir = readdir(proc_dir)) != NULL){
        char status_dir_name[BUFFER_SIZE], fd_dir_name[BUFFER_SIZE];
        snprintf(status_dir_name, BUFFER_SIZE, "/proc/%s/status", status_dir->d_name);
        snprintf(fd_dir_name, BUFFER_SIZE, "/proc/%s/fd", status_dir->d_name);
        if((status_fd = open(status_dir_name, O_RDONLY)) == -1){
            if(errno == ENOENT || errno == ENOTDIR) continue;
            perror("open status file");
            exit(1);
        }

        if((procfd_dir = opendir(fd_dir_name)) == NULL){
            perror("opendir");
            exit(1);
        }
        while((fd_dir = readdir(procfd_dir)) != NULL){
            char link_pathname[BUFFER_SIZE], link_to_pathname[BUFFER_SIZE];
            snprintf(link_pathname, BUFFER_SIZE, "/proc/%s/fd/%s", status_dir->d_name, fd_dir->d_name);
            if(strcmp(fd_dir->d_name, ".") == 0 || strcmp(fd_dir->d_name, "..") == 0){
                continue;
            }
            if((pathname_num = readlink(link_pathname, link_to_pathname, BUFFER_SIZE)) == -1){
                perror("readlink");
                exit(1);
            }
            link_to_pathname[pathname_num] = '\0';
            // printf("pathname: %s\n", link_pathname);
            // printf("link_pathname: %s\n", link_to_pathname);
            // printf("filename: %s\n\n", filename);
            if(strcmp(link_to_pathname, filename) == 0){
                char status_content[BUFFER_SIZE], command[BUFFER_SIZE], output[BUFFER_SIZE], *line;
                if(read(status_fd, status_content, BUFFER_SIZE) == -1){
                    perror("read status_file");
                    exit(1);
                }
                line = strtok(status_content, "\n");
                strncpy(command, line + 5, BUFFER_SIZE);
                snprintf(output, BUFFER_SIZE, "pid: %s / command: %s\n", status_dir->d_name, command);
                if(write(STDOUT_FILENO, output, strlen(output)) == -1){
                    perror("write");
                    exit(1);
                }
            }
        }

        if(closedir(procfd_dir) == -1){
            perror("closedir for procfd_dir");
            exit(1);
        }

        if(close(status_fd) == -1){
            perror("close");
            exit(1);
        }
    }

    if(closedir(proc_dir) == -1){
        perror("closedir for proc_dir");
        exit(1);
    }

    return 0;
}