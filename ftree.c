#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ftree.h"
#include "hash.h"


/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {
    // t_depth is used to distinguish the root from the others.
    static int t_depth = 0;

    // Allocate memory for FTree root.
    struct TreeNode *top_tn = malloc(sizeof(struct TreeNode));
    if (top_tn == NULL) {
        perror("malloc");
        exit(1);
    }

    // Get the information of this file and check the error.
    struct stat sb;
    if (lstat(fname, &sb) == -1) {
        perror("lstat");
        exit(1);
    }

    // Regardless of being a file or directory, they are the same in
    // fname and permissions. We will carefully handle the 'next' part later.
    // If it is the root of the whole FTree, then fname stores the entirety
    // of the command line argument.
    if (t_depth == 0) {
        (*top_tn).fname = malloc(strlen((char *)fname) + 1);
        strcpy((*top_tn).fname, (char *)fname);
        t_depth++;
    } else {
        (*top_tn).fname = malloc(strlen(basename((char *)fname)) + 1);
        strcpy((*top_tn).fname, basename((char *)fname));
    }
    (*top_tn).permissions = sb.st_mode & 0777;
    (*top_tn).next = NULL;

    // If this is a directory.
    if (S_ISDIR(sb.st_mode)) {
        // A directory has no hash. i.e. NULL
        (*top_tn).hash = NULL;

        // Open the directory and check the error.
        DIR *dirp = opendir(fname);
        if (dirp == NULL) {
            // If the error is due to Permission denied, we are able to report
            // the error and continue.
            if (errno == EACCES) {
                fprintf(stderr, "Permission denied: %s\n", fname);
                (*top_tn).contents = NULL;
            } else {
                perror("opendir");
                exit(1);
            }
        } else {
            struct TreeNode *curr;
            struct TreeNode *pre;
            // flag is used to determine which layer we are in:
            // parent -> child (1)  OR  child -> next child (>1)
            int flag = 0;
            struct dirent *dp = readdir(dirp);
            while (dp != NULL){
                if ((*dp).d_name[0] != '.'){
                    // Total len = len(parent name) + len('/') + len(child name)
                    // + null termination charactor at the end.
                    char *dname;
                    if (fname[strlen(fname) - 1] != '/') {
                        dname = malloc(strlen(fname) + strlen((*dp).d_name) + 2);
                        strcpy(dname, fname);
                        strcat(dname, "/");
                        strcat(dname, (*dp).d_name);
                    } else {
                        dname = malloc(strlen(fname) + strlen((*dp).d_name) + 1);
                        strcpy(dname, fname);
                        strcat(dname, (*dp).d_name);
                    }

                    // Generate sub-FTree rooted at current child.
                    curr = generate_ftree(dname);
                    free(dname);

                    flag++;
                    // Update the cursor and connect the FTree.
                    if (flag == 1) {
                        (*top_tn).contents = curr;
                        pre = curr;
                    } else {
                        (*pre).next = curr;
                        pre = curr;
                    }
                }
                dp = readdir(dirp);
            }
            // If we know this is an empty folder after readding this directory,
            // then it has no contents.
            if (flag == 0) {
                (*top_tn).contents = NULL;
            }
            if (closedir(dirp) == -1) {
                perror("closedir");
                exit(1);
            }
        }

    } else { // If this is a file.
        // A file has no contents. i.e. NULL
        (*top_tn).contents = NULL;

        // Open the file and check the error.
        FILE *f = fopen(fname, "r");
        if (f == NULL) {
            // If the error is due to Permission denied, we are able to report
            // the error and continue.
            if (errno == EACCES) {
                fprintf(stderr, "Permission denied: %s\n", fname);
                char *empty_hash_val = malloc(8);
                for(int i = 0; i < 8; i++) {
                    empty_hash_val[i] = '\0';
                }
                (*top_tn).hash = empty_hash_val;
            } else {
                perror("fopen");
                exit(1);
            }
        } else {
            (*top_tn).hash = hash(f);
            if (fclose(f) == EOF) {
                perror("fclose");
                exit(1);
            }
        }
    }
    return top_tn;
}


/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    printf("%*s", depth * 2, "");

    // Your implementation here.
    if (root != NULL) {
        // If the hash is NULL, which means it's a directory.
        if ((*root).hash == NULL) {
            printf("===== %s (%o) =====\n", (*root).fname, (*root).permissions);
            depth++;
            struct TreeNode *tn = (*root).contents;
            while (tn != NULL) {
                // Print sub-FTree rooted at each child.
                print_ftree(tn);
                tn = (*tn).next;
            }
            depth--;
        } else { // If it's a file.
            printf("%s (%o)\n", (*root).fname, (*root).permissions);
        }
    }
}
