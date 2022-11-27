/*
 * drawing process tree
 * usage:
 *      ./12_2
 */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

struct proc_node{
    char name[BUFFER_SIZE];
    int pid;
    int ppid;
    struct proc_node *parent;
    struct proc_node *children[1024];
    struct proc_node *next;
};

struct proc_node *list_root = NULL;

void insert_node(char *name, pid_t pid, pid_t ppid){
    struct proc_node *node_ptr = (struct proc_node *)malloc(sizeof(struct proc_node));

    strcpy(node_ptr->name, name);
    node_ptr->pid = pid;
    node_ptr->ppid = ppid;
    node_ptr->parent = NULL;
    node_ptr->children[0] = NULL;
    node_ptr->next = list_root;
    list_root = node_ptr;
}

struct proc_node *find_node(pid_t pid){
    struct proc_node *cur_ptr = list_root;
    for(cur_ptr; cur_ptr != NULL; cur_ptr = cur_ptr->next){
        if(cur_ptr->pid == pid) return cur_ptr;
    }
    return NULL;
}

void build_pstree(){
    struct proc_node *cur_ptr;

    for(cur_ptr = list_root; cur_ptr != NULL; cur_ptr = cur_ptr->next){
        int child_index = 0;
        struct proc_node *parent_ptr = find_node(cur_ptr->ppid);
        if(parent_ptr != NULL){
            cur_ptr->parent = parent_ptr;
            while(parent_ptr->children[child_index] != NULL) child_index++;
            parent_ptr->children[child_index] = cur_ptr;
            parent_ptr->children[child_index + 1] = NULL;
        }
    }
}

void print_pstree_recursive(struct proc_node *node, int level){
    struct proc_node *cnode;
    char *output[BUFFER_SIZE] = {NULL}, *space = malloc(10 * sizeof(char));
    int i = 0;
    for(i = 0; i < 2 * level ; i++) strcat(space, " ");
    snprintf(output, BUFFER_SIZE, "%s-command: %s, pid: %d, ppid: %d\n", space, node->name, node->pid, node->ppid);
    if(write(STDOUT_FILENO, output, strlen(output)) == -1){
        perror("write");
        exit(1);
    }
    int q = 0;
    cnode = node->children[q];
    while(cnode != NULL){
        print_pstree_recursive(cnode, level + 1);
        q = q + 1;
        cnode = node->children[q];
    }
}

void print_pstree(){
    struct proc_node *cur_ptr = list_root;
    for(cur_ptr; cur_ptr != NULL; cur_ptr = cur_ptr->next){
        if(cur_ptr->parent == NULL){
            // printf("id = %d\n", cur_ptr->pid);
            print_pstree_recursive(cur_ptr, 0);
        }    
    }
}


int main(int argc, char *argv){
    DIR *proc_dir;
    struct dirent *status_dir;
    int status_fd;

    if((proc_dir = opendir("/proc")) == NULL){
        perror("opendir");
        exit(1);
    }

    while((status_dir = readdir(proc_dir)) != NULL){
        char status_dir_name[BUFFER_SIZE] = {NULL}, status_content[BUFFER_SIZE] = {NULL}, *line, name[BUFFER_SIZE] = {NULL};
        int name_flag = 0, pid_flag = 0, ppid_flag = 0;
        pid_t pid, ppid;
        snprintf(status_dir_name, BUFFER_SIZE, "/proc/%s/status", status_dir->d_name);
        if((status_fd = open(status_dir_name, O_RDONLY)) == -1){
            if(errno == ENOENT || errno == ENOTDIR) continue;
            perror("open status file");
            exit(1);
        }

        if(read(status_fd, status_content, BUFFER_SIZE) == -1){
            perror("read status_file");
            exit(1);
        }

        // printf("content: %s\n", status_content);

        line = strtok(status_content, "\n");
        while(line != NULL || (name_flag == 0 && pid_flag == 0 && ppid_flag == 0)){
            // printf("line: %s\n", line);
            char buf[BUFFER_SIZE];
            if(strncmp(line, "Name", 4) == 0){
                strncpy(buf, line + 6, BUFFER_SIZE);
                strcpy(name, buf);
                name_flag = 1;
                // printf("name = %s\n", name);
            }
            else if(strncmp(line, "Pid", 3) == 0){
                strncpy(buf, line + 5, BUFFER_SIZE);
                pid = atoi(buf);
                pid_flag = 1;
                // printf("pid = %d\n", pid);
            }
            else if(strncmp(line, "PPid", 4) == 0){
                strncpy(buf, line + 6, BUFFER_SIZE);
                ppid = atoi(buf);
                ppid_flag = 1;
                // printf("ppid = %d\n\n", ppid);
            }
            line = strtok(NULL, "\n");
        }

        if(name_flag == 0 || pid_flag == 0 || ppid_flag == 0){
            perror("process info empty");
            exit(1);
        }

        insert_node(name, pid, ppid);
    }

    build_pstree();
    print_pstree();

    return 0;
}