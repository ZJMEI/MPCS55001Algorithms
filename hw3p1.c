//
//  Advanced Programming
//
//  Created by ZHANGJUN MEI on 15/10/21.
//  Copyright © 2015年 bilibili. All rights reserved.
//
//  HW3
//  p1, RB Tree


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHAR_PER_LINE 500
#define RED -1
#define BLACK 1

typedef struct TREE_NODE{
    char *key;
    char *def;
    int color;
    struct TREE_NODE *left;
    struct TREE_NODE *right;
    struct TREE_NODE *parent;
}TREE_NODE;

TREE_NODE *TREE_ROOT;

//create a new tree node with key, def, parent node and color
TREE_NODE* CREATE_NODE(char *key, char *def, TREE_NODE *parent, int color){
    TREE_NODE *temp_node;
    int key_len;
    int def_len;
    
    temp_node = (TREE_NODE*)malloc(sizeof(TREE_NODE));
    
    key_len = (int)strlen(key) + 1;
    def_len = (int)strlen(def) + 1;
    temp_node->key = (char*)malloc(key_len * sizeof(char));
    temp_node->def = (char*)malloc(def_len * sizeof(char));
    memcpy(temp_node->key, key, key_len * sizeof(char));
    memcpy(temp_node->def, def, def_len * sizeof(char));
    
    temp_node->color = color;
    temp_node->left = NULL;
    temp_node->right = NULL;
    temp_node->parent = parent;
    
    return temp_node;
}

void FREE_NODE(TREE_NODE *tree){
    free(tree->key);
    free(tree->def);
    free(tree);
}

//find the grandparent of node tree
TREE_NODE* G_(TREE_NODE *tree){
    if((tree != NULL) && (tree->parent != NULL))
        return tree->parent->parent;
    else
        return NULL;
}

//find the uncle of node tree
TREE_NODE* U_(TREE_NODE *tree){
    TREE_NODE *g_tree;
    
    g_tree = G_(tree);
    if(g_tree == NULL)
        return NULL;
    else if(tree->parent == g_tree->left)
        return g_tree->right;
    else
        return g_tree->left;
}

//find the sibling of node tree
TREE_NODE* S_(TREE_NODE *tree){
    if(tree == tree->parent->left)
        return tree->parent->right;
    else
        return tree->parent->left;
}

int node_color(TREE_NODE *tree){
    if(tree == NULL)
        return BLACK;
    else
        return tree->color;
}

//rotate left (node n and its parent p)
void ROTATE_LEFT(TREE_NODE *n, TREE_NODE *p){
    TREE_NODE *g_tree;
    
    g_tree = G_(n);
    p->right = n->left;
    p->parent = n;
    n->left = p;
    n->parent = g_tree;
    
    if(g_tree != NULL){
        if(g_tree->right == p)
            g_tree->right = n;
        else
            g_tree->left = n;
    }
    
    if(p->right != NULL)
        p->right->parent = p;
}

//rotate right (node n and its parent p)
void ROTATE_RIGHT(TREE_NODE *n, TREE_NODE *p){
    TREE_NODE *g_tree;
    
    g_tree = G_(n);
    p->left = n->right;
    p->parent = n;
    n->right = p;
    n->parent = g_tree;
    
    if(g_tree != NULL){
        if(g_tree->right == p)
            g_tree->right = n;
        else
            g_tree->left = n;
    }
    
    if(p->left != NULL)
        p->left->parent = p;
}

//replace the old node by the new node
void REPLACE(TREE_NODE *old, TREE_NODE *new){
    new->parent = old->parent;
    if(old->parent ==NULL)
        new->parent = NULL;
    else if(old == old->parent->left)
        old->parent->left = new;
    else
        old->parent->right = new;
}

//insert case
void insert_case1(TREE_NODE *tree);

void insert_case5(TREE_NODE *tree){
    TREE_NODE *g_tree;
    
    g_tree = G_(tree);
    g_tree->color = RED;
    tree->parent->color = BLACK;
    if(tree == tree->parent->left)
        ROTATE_RIGHT(tree->parent, g_tree);
    else
        ROTATE_LEFT(tree->parent, g_tree);
    
}

void insert_case4(TREE_NODE *tree){
    TREE_NODE *g_tree;
    
    g_tree = G_(tree);
    if((tree == tree->parent->right) && (tree->parent == g_tree->left)){
        ROTATE_LEFT(tree, tree->parent);
        tree = tree->left;
    }
    else if((tree == tree->parent->left) && (tree->parent == g_tree->right)){
        ROTATE_RIGHT(tree, tree->parent);
        tree = tree->right;
    }
    insert_case5(tree);
}

void insert_case3(TREE_NODE *tree){
    TREE_NODE *u_tree;
    TREE_NODE *g_tree;
    
    u_tree = U_(tree);
    if(u_tree == NULL)
        insert_case4(tree);
    else if(node_color(u_tree) == RED){
        g_tree = G_(tree);
        g_tree->color = RED;
        tree->parent->color = BLACK;
        u_tree->color = BLACK;
        insert_case1(g_tree);
    }
    else
        insert_case4(tree);
}

void insert_case2(TREE_NODE *tree){
    if(tree->parent->color == BLACK)
        return;
    else
        insert_case3(tree);
    
}

void insert_case1(TREE_NODE *tree){
    if(tree->parent == NULL)
        tree->color = BLACK;
    else
        insert_case2(tree);
}

//removal case
void delete_case1(TREE_NODE *tree);

void delete_case6(TREE_NODE *tree){
    TREE_NODE *s_tree;
    
    s_tree = S_(tree);
    s_tree->color = tree->parent->color;
    tree->parent->color = BLACK;
    
    if(tree == tree->parent->left){
        s_tree->right->color = BLACK;
        ROTATE_LEFT(s_tree, tree->parent);
    }
    else{
        s_tree->left->color = BLACK;
        ROTATE_RIGHT(s_tree, tree->parent);
    }
}

void delete_case5(TREE_NODE *tree){
    TREE_NODE *s_tree;
    
    s_tree = S_(tree);
    if(node_color(s_tree) == BLACK){
        if((tree == tree->parent->left) &&
           (node_color(s_tree->right) == BLACK) && (node_color(s_tree->left) == RED)){
            s_tree->color = RED;
            s_tree->left->color = BLACK;
            ROTATE_RIGHT(s_tree->left, s_tree);
        }
        else if((tree == tree->parent->right) &&
                (node_color(s_tree->left) == BLACK) && (node_color(s_tree->right) == RED)){
            s_tree->color = RED;
            s_tree->right->color = BLACK;
            ROTATE_LEFT(s_tree->right, s_tree);
        }
    }
    delete_case6(tree);
}

void delete_case4(TREE_NODE *tree){
    TREE_NODE *s_tree;
    
    s_tree = S_(tree);
    if((node_color(tree->parent) == RED) &&
       (node_color(s_tree) == BLACK) &&
       (node_color(s_tree->left) == BLACK) &&
       (node_color(s_tree->right) == BLACK)){
        s_tree->color = RED;
        tree->parent->color = BLACK;
    }
    else
        delete_case5(tree);
}

void delete_case3(TREE_NODE *tree){
    TREE_NODE *s_tree;
    
    s_tree = S_(tree);
    if((node_color(tree->parent) == BLACK) &&
       (node_color(s_tree) == BLACK) &&
       (node_color(s_tree->left) == BLACK) &&
       (node_color(s_tree->right) == BLACK)){
        s_tree->color = RED;
        delete_case1(tree->parent);
    }
    else
        delete_case4(tree);
}

void delete_case2(TREE_NODE *tree){
    TREE_NODE *s_tree;
    
    s_tree = S_(tree);
    if(node_color(s_tree) == RED){
        tree->parent->color = RED;
        s_tree->color = BLACK;
        if(tree == tree->parent->left)
            ROTATE_LEFT(s_tree, tree->parent);
        else
            ROTATE_RIGHT(s_tree, tree->parent);
    }
    delete_case3(tree);
}

void delete_case1(TREE_NODE *tree){
    if(tree->parent != NULL)
        delete_case2(tree);
}

//tree has at most one child
//because we do not use actual node as leaf node, we need handle if both left and right child are NULL
void delete_case0(TREE_NODE *tree){
    TREE_NODE *child = NULL;
    
    if(tree->left != NULL)
        child = tree->left;
    else if(tree->right != NULL)
        child = tree->right;
    //both left and right are NULL
    //use as a phatom leaf
    else{
        if(tree->color == BLACK)
            delete_case1(tree);
        //if tree node is the root node, and tree has no child, then the total tree is empty
        if(tree->parent == NULL)
            TREE_ROOT = child;
        else if(tree == tree->parent->left)
            tree->parent->left = NULL;
        else
            tree->parent->right = NULL;
        FREE_NODE(tree);
        return;
    }
    
    REPLACE(tree, child);
    if(tree->color == BLACK){
        if(child->color == RED)
            child->color = BLACK;
        else
            delete_case1(child);
    }
    if(tree->parent == NULL)
        TREE_ROOT = child;
    FREE_NODE(tree);
}

TREE_NODE* FIND_KEY(TREE_NODE *tree, char *key){
    TREE_NODE *temp_tree;
    
    if(tree == NULL)
        return NULL;
    
    temp_tree = tree;
    while(strcmp(temp_tree->key, key) != 0){
        if(strcmp(temp_tree->key, key) > 0){
            if(temp_tree->left != NULL)
                temp_tree = temp_tree->left;
            else
                return NULL;
        }
        else{
            if(temp_tree->right != NULL)
                temp_tree = temp_tree->right;
            else
                return NULL;
        }
    }
    return temp_tree;
}

TREE_NODE* GET_ROOT(TREE_NODE *tree){
    if(tree == NULL)
        return tree;
    while (tree->parent != NULL)
        tree = tree->parent;
    return tree;
}

//insert function
// 1 success
//-1 failure, key already exists
int INSERT_KEY(TREE_NODE *tree, char *key, char *def){
    TREE_NODE *temp_tree;
    
    if(tree == NULL){
        tree = CREATE_NODE(key, def, NULL, RED);
        insert_case1(tree);
        return 1;
    }
    
    temp_tree = tree;
    while(strcmp(temp_tree->key, key) != 0){
        if(strcmp(temp_tree->key, key) > 0){
            if(temp_tree->left != NULL)
                temp_tree = temp_tree->left;
            else
                break;
        }
        else{
            if(temp_tree->right != NULL)
                temp_tree = temp_tree->right;
            else
                break;
        }
    }
    if(strcmp(temp_tree->key, key) == 0)
        return -1;
    //insert as temp_tree's left child
    if(strcmp(temp_tree->key, key) > 0){
        temp_tree->left = CREATE_NODE(key, def, temp_tree, RED);
        insert_case1(temp_tree->left);
    }
    else{
        temp_tree->right = CREATE_NODE(key, def, temp_tree, RED);
        insert_case1(temp_tree->right);
    }
    return 1;
}

//delete function
// 1 success
//-1 failure, key not found
//-2 failure, tree is empty
int DELETE_KEY(TREE_NODE *tree, char *key){
    TREE_NODE *temp_tree;
    TREE_NODE *temp_child;
    int key_len;
    int def_len;
    
    if(tree == NULL)
        return -2;
    
    temp_tree = FIND_KEY(tree, key);
    if(temp_tree == NULL)
        return -1;
    
    //have more than one child
    if((temp_tree->left != NULL) && (temp_tree->right != NULL)){
        temp_child = temp_tree->left;
        while(temp_child->right != NULL)
            temp_child = temp_child->right;
        //copy temp_child's data to temp_tree
        key_len = (int)strlen(temp_child->key) + 1;
        def_len = (int)strlen(temp_child->def) + 1;
        free(temp_tree->key);
        temp_tree->key = (char*)malloc(key_len * sizeof(char));
        memcpy(temp_tree->key, temp_child->key, key_len);
        free(temp_tree->def);
        temp_tree->def = (char*)malloc(def_len * sizeof(char));
        memcpy(temp_tree->def, temp_child->def, def_len);
        //delete temp_child
        delete_case0(temp_child);
    }
    //at most one child
    else
        delete_case0(temp_tree);
    return 1;
}

void PRINT_TREE(TREE_NODE *tree){
    if(tree->left != NULL)
        PRINT_TREE(tree->left);
    printf("%s:\t%s\n", tree->key, tree->def);
    if(tree->right != NULL)
        PRINT_TREE(tree->right);
}

void FREE_ALL(TREE_NODE *tree){
    if(tree == NULL)
        return;
    if(tree->left != NULL)
        FREE_NODE(tree->left);
    if(tree->right != NULL)
        FREE_NODE(tree->right);
    FREE_NODE(tree);
}

int get_next_word(char *arr, int i, char *word){
    int j = i;
    
    while(1){
        if(arr[j] == ' ')
            break;
        else if(arr[j] == '\n')
            break;
        else if(arr[j] == 0)
            break;
        j++;
    }
    memcpy(word, arr + i, (j - i ) * sizeof(char));
    word[j - i] = 0;
    return j;
}

int get_next_def(char *arr, int i, char *word){
    int j = i;
    
    //def must start with "
    if(arr[j] != 34)
        return -1;
    j++;
    //printf("in get_def : %s", arr);
    //printf("in get_def : j = %d\n", j);
    while(1){
        //printf("%c ", arr[j]);
        if(arr[j] == 34)
            break;
        else if(arr[j] == '\n')
            break;
        else if(arr[j] == 0)
            break;
        j++;
        //printf("%d ", j);
    }
    //printf("in get_def : loop over\n");
    //def must end with "
    if(arr[j] != 34)
        return -1;
    memcpy(word, arr + i + 1, (j - i - 1) * sizeof(char));
    word[j - i - 1] = 0;
    return j;
}

void CMD_HANDLE(TREE_NODE *tree, double *t_add, double *t_find){
    char *CMD = (char*) malloc(MAX_CHAR_PER_LINE * sizeof(char));
    char *CMD_1 = (char*) malloc(MAX_CHAR_PER_LINE * sizeof(char));
    char *CMD_2 = (char*) malloc(MAX_CHAR_PER_LINE * sizeof(char));
    char *CMD_3 = (char*) malloc(MAX_CHAR_PER_LINE * sizeof(char));
    int i, j, k;
    int tag;
    int xxx = 1;
    clock_t TIME_ADD;
    clock_t TIME_FIND;
    clock_t T_TIME;
    
    TREE_NODE *temp_tree = NULL;
    TREE_ROOT = NULL;
    
    TIME_ADD = 0;
    TIME_FIND = 0;
    
    while(fgets(CMD, MAX_CHAR_PER_LINE, stdin) != NULL){
        //printf("%d\n", xxx);
        xxx++;
        i = get_next_word(CMD, 0, CMD_1);
        /*if(xxx == 3){
         printf("%s\n", CMD);
         printf("%s\n", CMD_1);
         }
         */
        TREE_ROOT = GET_ROOT(TREE_ROOT);
        tree = TREE_ROOT;
        if(strcmp(CMD_1, "print") == 0){
            //like "print .....", invalid input
            if(CMD[i] != '\n'){
                printf("Invalid input\n");
                continue;
            }
            if(tree == NULL)
                printf("Tree is empty\n");
            else
                PRINT_TREE(tree);
        }
        else if(strcmp(CMD_1, "delete") == 0){
            j = get_next_word(CMD, i + 1, CMD_2);
            if(CMD_2[0] == 0){
                printf("Invalid input\n");
                continue;
            }
            else if(CMD[j] != '\n'){
                printf("Invalid input\n");
                continue;
            }
            tag = DELETE_KEY(tree, CMD_2);
            if(tag == 1)
                printf("Success\n");
            else
                printf("Failure\n");
        }
        else if(strcmp(CMD_1, "find") == 0){
            T_TIME = clock();
            j = get_next_word(CMD, i + 1, CMD_2);
            if(CMD_2[0] == 0){
                printf("Invalid input\n");
                continue;
            }
            else if(CMD[j] != '\n'){
                printf("Invalid input\n");
                continue;
            }
            temp_tree = FIND_KEY(tree, CMD_2);
            /*
            if(temp_tree == NULL)
                printf("Key not found\n");
            else
                printf("%s\n", temp_tree->def);
             */
            T_TIME = clock() - T_TIME;
            TIME_FIND += T_TIME;
        }
        else if(strcmp(CMD_1, "add") == 0){
            T_TIME = clock();
            /*if(xxx == 3){
             printf("reach add section OK\n");
             }
             */
            j = get_next_word(CMD, i + 1, CMD_2);
            /*if(xxx == 3){
             printf("%s\n", CMD_2);
             }
             */
            k = get_next_def(CMD, j + 1, CMD_3);
            if(k == -1){
                printf("Invalid input\n");
                continue;
            }
            else if(CMD_2[0] == 0){
                printf("Invalid input\n");
                continue;
            }
            else if(CMD_3[0] == 0){
                printf("Invalid input\n");
                continue;
            }
            else if(CMD[k + 1] != '\n'){
                printf("Invalid input\n");
                continue;
            }
            
            //printf("CMD GET OK\n");
            if(tree == NULL){
                tree = CREATE_NODE(CMD_2, CMD_3, NULL, BLACK);
                tag = 1;
                TREE_ROOT = tree;
            }
            else
                tag = INSERT_KEY(tree, CMD_2, CMD_3);
            /*if(tag == 1)
                printf("Success\n");
            else
                printf("Failure\n");
             */
            T_TIME = clock() - T_TIME;
            TIME_ADD += T_TIME;
        }
        else if(strcmp(CMD_1, "quit") == 0)
            break;
        else
            printf("Invalid input\n");
    }
    *t_add = (double)TIME_ADD / CLOCKS_PER_SEC;
    *t_find = (double)TIME_FIND / CLOCKS_PER_SEC;
}


int main(int argc, char **argv){
    TREE_NODE *tree = NULL;
    double t_add, t_find;
    
    CMD_HANDLE(tree, &t_add, &t_find);
    printf("add operation costs %lfms\n", t_add);
    printf("find operation costs %lfms\n", t_find);
    tree = GET_ROOT(TREE_ROOT);
    FREE_ALL(tree);
    return 0;
}