/*
 * recursive inotify function implementation
 * usage: ./19_1 <dir_pathname>
 */

#define _XOPEN_SOURCE 500

#include <sys/inotify.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>

#define FD_LIMIT 128
#define BUFSIZE 10 * (sizeof(struct inotify_event) + NAME_MAX + 1)

struct subdir_item{
    int wd;
    char pathname[PATH_MAX];
    struct subdir_item *next;
};

struct subdir_item *list_root = NULL;
struct subdir_item *list_tail = NULL;

int inot_fd;

void print_list(){
    int cur_index;
    fprintf(stderr, "\n============================\n");
    struct subdir_item *cur_ptr;
    for(cur_ptr = list_root; cur_ptr != NULL; cur_ptr = cur_ptr->next){
        fprintf(stderr, "wd: %d, pathname: %s\n", cur_ptr->wd, cur_ptr->pathname);
    }
    fprintf(stderr, "============================\n");
}

int fn(const char *path, const struct stat *stat_ptr, int flag, struct FTW *ftw_ptr){
    if(flag == FTW_DNR || flag == FTW_NS) return 0;
    if(S_ISDIR(stat_ptr->st_mode)) watchSubdir(path);
    return 0;
}

void watchSubdir(const char *path){
    int wd;
    struct subdir_item *new_dir;
    
    wd = inotify_add_watch(inot_fd, path, IN_ALL_EVENTS);
    if(wd == -1){
        perror("inotify_add_watch() fail");
        exit(1);
    }

    /* add new item of subdir linked list */
    new_dir = malloc(sizeof(struct subdir_item));
    if(new_dir == NULL){
        perror("malloc() fail");
        exit(1);
    }
    new_dir->wd = wd;
    strcpy(new_dir->pathname, path);
    new_dir->next = NULL;
    
    if(list_root == NULL){
        list_root = new_dir;
        list_tail = new_dir;
    }
    else{
        list_tail->next = new_dir;
        list_tail = new_dir;
    }
    fprintf(stderr, "wd: %d, watch dir: %s\n", wd, path);
    print_list();
}

void rmWatchSubdir(int wd){
    int cur_index;
    struct subdir_item *cur_ptr = list_root;
    if(cur_ptr->wd == wd){
        /* root is target to be removed */
        if(list_root == list_tail){
            /* only one item in list */
            free(cur_ptr);
            list_root = list_tail = NULL;
        }
        else{
            /* multiple items in list */
            list_root = cur_ptr->next;
            free(cur_ptr);
        }
        print_list();
        return;
    }
    for(cur_ptr; cur_ptr != NULL; cur_ptr = cur_ptr->next){
        /* if found the subdir with wd, remove it*/
        if(cur_ptr->next->wd != wd) continue;
        else{
            if(cur_ptr->next == list_tail){
                /* two items remain */
                list_tail = cur_ptr;
                cur_ptr->next = NULL;
                print_list();
                return;
            }
            struct subdir_item *new_next = cur_ptr->next->next;
            free(cur_ptr->next);
            cur_ptr->next = new_next;
            print_list();
        }
        return;
    }
    return;
}

char *parentPath(int wd){
    int cur_index;
    struct subdir_item *cur_ptr;
    for(cur_ptr = list_root; cur_ptr != NULL; cur_ptr = cur_ptr->next){
        if(cur_ptr->wd == wd) return cur_ptr->pathname;
    }
    return NULL;
}

void displayLogEvent(struct inotify_event *event){
    char *path_prefix = NULL;
    path_prefix = parentPath(event->wd);
    if(path_prefix == NULL){
        perror("parentPath");
        exit(1);
    }
    if(event->len == 0){
        /* directory event*/
        fprintf(stderr, "Directory: %s, ", path_prefix);
    }
    else{
        /* file event (may in the subdir)*/
        if(event->mask & IN_ISDIR) fprintf(stderr, "Subdirectory in %s: %s, ", path_prefix, event->name);
        else fprintf(stderr, "File in %s: %s, ", path_prefix, event->name);
    }

    /* print the wd and cookie information */
    fprintf(stderr, "wd: %d, cookie: %4d, mask: ", event->wd, event->cookie);

    /* print the mask information */
    char *info;
    if(event->mask & IN_ACCESS) fprintf(stderr, "IN_ACCESS ");
    if(event->mask & IN_ATTRIB) fprintf(stderr, "IN_ATTRIB ");
    if(event->mask & IN_CLOSE_NOWRITE) fprintf(stderr, "IN_CLOSE_NOWRITE ");
    if(event->mask & IN_CLOSE_WRITE) fprintf(stderr, "IN_CLOSE_WRITE ");
    if(event->mask & IN_CREATE) fprintf(stderr, "IN_CREATE ");
    if(event->mask & IN_DELETE) fprintf(stderr, "IN_DELETE ");
    if(event->mask & IN_DELETE_SELF) fprintf(stderr, "IN_DELETE_SELF ");
    if(event->mask & IN_IGNORED){
        /* if subdir is removed by user */
        fprintf(stderr, "IN_IGNORED ");
        rmWatchSubdir(event->wd);
    }
    if(event->mask & IN_MODIFY) fprintf(stderr, "IN_MODIFY ");
    if(event->mask & IN_MOVE_SELF) fprintf(stderr, "IN_MOVE_SELF ");
    if(event->mask & IN_MOVED_FROM) fprintf(stderr, "IN_MOVED_FROM ");
    if(event->mask & IN_MOVED_TO) fprintf(stderr, "IN_MOVED_TO ");
    if(event->mask & IN_OPEN) fprintf(stderr, "IN_OPEN ");
    if(event->mask & IN_Q_OVERFLOW) fprintf(stderr, "IN_Q_OVERFLOW ");
    if(event->mask & IN_UNMOUNT) fprintf(stderr, "IN_UNMOUNT ");

    fprintf(stderr, "\n");

    if((event->mask & IN_ISDIR) && (event->mask & IN_CREATE)){
        /* if new subdirectory detected*/
        char full_pathname[PATH_MAX];
        snprintf(full_pathname, PATH_MAX, "%s/%s", parentPath(event->wd), event->name);
        watchSubdir(full_pathname);
    }

}


int main(int argc, char *argv[]){
    int nftw_re, read_num;
    char buf[BUFSIZE];
    
    if(argc < 2){
        fprintf(stderr, "Usage: 19_1 <dir_pathname>\n");
        exit(1);
    }

    inot_fd = inotify_init();
    if(inot_fd == -1){
        perror("inotify_init() fail");
        exit(1);
    }

    /* obtain list of subdir and add to the inotify watch list*/
    nftw_re = nftw(argv[1], fn, FD_LIMIT, FTW_PHYS);
    if(nftw_re == -1){
        perror("nftw() fail");
        exit(1);
    }

    fprintf(stderr, "recursive done\n");

    while(1){
        read_num = read(inot_fd, buf, BUFSIZE);
        if(read_num == -1){
            perror("read() fail");
            exit(1);
        }
        char *p;
        struct inotify_event *event;
        for(p = buf; p < buf + read_num; ){
            event = (struct inotify_event *)p;
            displayLogEvent(event);
            p += sizeof(struct inotify_event) + event->len;
        }
    }

    exit(EXIT_SUCCESS);
}