//
//  Advanced Programming
//
//  Created by ZHANGJUN MEI on 15/10/21.
//  Copyright © 2015年 bilibili. All rights reserved.
//
//  HW2
//  p1, simple model 1 BST


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_node_t{
    char *key;
    void *left;
    void *right;
}TREE_NODE;

//create a tree node
TREE_NODE* CREATE_NODE(char *key, char *def){
    TREE_NODE *temp_node;
    int key_len;
    int def_len;
    
    temp_node = (TREE_NODE*)malloc(sizeof(TREE_NODE));
    key_len = (int)strlen(key) + 1;
    def_len = (int)strlen(def) + 1;
    temp_node -> key = (char*)malloc(key_len * sizeof(char));
    temp_node -> left = (char*)malloc(def_len * sizeof(char));
    memcpy(temp_node->key, key, key_len * sizeof(char));
    memcpy(temp_node->left, def, def_len * sizeof(char));
    temp_node -> right = NULL;
    
    return temp_node;
}

int INSERT_(TREE_NODE *tree, char *new_key, char *new_def){
    TREE_NODE *temp_node;
    TREE_NODE *old_leaf;
    TREE_NODE *new_leaf;
    int key_len;
    int def_len;
    
    //Tree is currently empty
    if(tree -> left == NULL){
        key_len = (int)strlen(new_key) + 1;
        def_len = (int)strlen(new_def) + 1;
        tree -> key = (char*)malloc(key_len * sizeof(char));
        tree -> left = (char*)malloc(def_len * sizeof(char));
        memcpy(tree->key, new_key, key_len * sizeof(char));
        memcpy(tree->left, new_def, def_len * sizeof(char));
        tree -> right = NULL;
        //printf("%s\n", (char*)tree->left);
        return 1;
    }
    //else go until leaf node
    else{
        temp_node = tree;
        while(temp_node -> right != NULL){
            if(strcmp(temp_node->key, new_key) > 0)
                temp_node = (TREE_NODE*)temp_node -> left;
            else
                temp_node = (TREE_NODE*)temp_node -> right;
        }
    }
    
    //key exists, not add
    if(strcmp(temp_node->key, new_key) == 0){
        return -1;
    }
    
    old_leaf = CREATE_NODE((char*)temp_node->key, (char*)temp_node->left);
    new_leaf = CREATE_NODE((char*)new_key, (char*)new_def);
    if(strcmp(temp_node->key, new_key) < 0){
        //if left stroe data, we should firstly free it
        free(temp_node -> left);
        temp_node -> left = (TREE_NODE*)old_leaf;
        temp_node -> right = (TREE_NODE*)new_leaf;
        //to change key value, we should recreate it
        free(temp_node -> key);
        temp_node -> key = (char*)malloc(key_len * sizeof(char));
        memcpy(temp_node->key, new_key, key_len);
    }
    else{
        free(temp_node -> left);
        temp_node -> left = (TREE_NODE*)new_leaf;
        temp_node -> right = (TREE_NODE*)old_leaf;
    }
    
    return 1;
}

int DELETE_(TREE_NODE *tree, char *key){
    TREE_NODE *temp_node;
    TREE_NODE *upper_node = NULL;
    TREE_NODE *other_node = NULL;
    int other_key_len;
    int other_def_len;
    
    //tree is empty
    if(tree->left == NULL)
        return -1;
    
    //only one node
    if(tree->right == NULL){
        if(strcmp(key, tree->key) == 0){
            free(tree->left);
            free(tree->key);
            tree->left = NULL;
            tree->key = NULL;
            return 1;
        }
        //key not found
        else
            return -2;
    }
    
    temp_node = tree;
    while(temp_node->right != NULL){
        upper_node = temp_node;
        if(strcmp(key, temp_node->key) < 0){
            other_node = (TREE_NODE*)upper_node->right;
            temp_node = (TREE_NODE*)upper_node->left;
        }
        else{
            other_node = (TREE_NODE*)upper_node->left;
            temp_node = (TREE_NODE*)upper_node->right;
        }
    }
    
    //key not found
    if(strcmp(key, temp_node->key) != 0)
        return -2;
    
    //upper_node->key = other_node->key
    free(upper_node->key);
    other_key_len = (int)strlen(other_node->key) + 1;
    upper_node->key = (char*)malloc(other_key_len * sizeof(char));
    memcpy(upper_node->key, other_node->key, other_key_len);
    //upper_node->left = other_node->left
    upper_node->left = other_node->left;
    memcpy(upper_node->left, other_node->left, other_def_len * sizeof(char));
    //upper_node->right = other_node->right
    upper_node->right = other_node->right;
    //free temp_node
    free(temp_node->left);
    free(temp_node->key);
    free(temp_node);
    //free other_node
    free(other_node->key);
    free(other_node);
    
    return 1;
}

char* FIND_(TREE_NODE *tree, char *key){
    TREE_NODE *temp_node;
    
    if(tree->left == NULL)
        return NULL;
    
    temp_node = tree;
    while(temp_node->right != NULL){
        if(strcmp(key, temp_node->key) < 0)
            temp_node = (TREE_NODE*)temp_node->left;
        else
            temp_node = (TREE_NODE*)temp_node->right;
    }
    
    if(strcmp(key, temp_node->key) == 0)
        return (char*)temp_node->left;
    else
        return NULL;
}

int PRINT_(TREE_NODE *tree){
    if(tree->left == NULL)
        return -1;
    
    if(tree->right == NULL){
        printf("%s:\t%s\n", (char*)tree->key, (char*)tree->left);
        return 1;
    }
    
    PRINT_((TREE_NODE*)tree->left);
    PRINT_((TREE_NODE*)tree->right);
    
    return 1;
}

void FREE_ALL(TREE_NODE *tree){
    //empty tree
    if(tree->left == NULL){
        free(tree);
        return;
    }
    
    //leaf node
    if(tree->right == NULL){
        free(tree->left);
        free(tree->key);
        free(tree);
        return;
    }
    
    //else recursively free
    FREE_ALL(tree->left);
    FREE_ALL(tree->right);
    free(tree->key);
    free(tree);
    return;
}

int main(int argc, char **argv){
    TREE_NODE *tree;
    char key[100];
    char def[100];
    char CMD[1000];
    int tag;
    
    tree = (TREE_NODE*)malloc(sizeof(TREE_NODE));
    tree -> key = NULL;
    tree -> left = NULL;
    tree -> right = NULL;
    while(1){
        scanf("%s", CMD);
        if(strcmp(CMD, "quit") == 0)
            break;
        if(strcmp(CMD, "add") == 0){
            scanf("%s", key);
            scanf("%s", def);
            tag = INSERT_(tree, key, def);
            if(tag == 1)
                printf("success\n");
            else
                printf("failure\n");
        }
        else if(strcmp(CMD, "delete") == 0){
            scanf("%s", key);
            tag = DELETE_(tree, key);
            if(tag == 1)
                printf("success\n");
            else
                printf("failure\n");
        }
        else if(strcmp(CMD, "find") == 0){
            scanf("%s", key);
            if(FIND_(tree, key) != NULL)
                printf("%s\n", FIND_(tree, key));
            else
                printf("key not found\n");
        }
        else if(strcmp(CMD, "print") == 0){
            tag = PRINT_(tree);
            if(tag == -1)
                printf("tree is empty\n");
        }
        
    }
    FREE_ALL(tree);
    return 0;
}

