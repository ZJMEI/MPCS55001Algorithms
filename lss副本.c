//
//  largest_solid_square.c
//  Algorithms
//
//  Created by ZHANGJUN MEI on 15/10/22.
//  Copyright © 2015年 bilibili. All rights reserved.
//
//  Algorithms HW4
//  Programming Assignment

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//for n*n size matrix, return the largest size of solid square with bottom-right coordinate (x, y)
int FIND_LSS(int **MATRIX, int n, int *x, int *y){
    int i, j;
    int **S_MATRIX;
    int MINIMUM;
    int MAXIMUM;
    
    //create S_MATRIX to store S(i, j)
    //S(i, j) means the largest size of solid square which use (i, j) as its bottom-right coordinate
    S_MATRIX = (int**)malloc(n * sizeof(int*));
    for(i = 0; i < n; i++)
        S_MATRIX[i] = (int*)malloc(n * sizeof(int));
    
    MAXIMUM = 0;
    //initiate S_MATRIX
    for(i = 0; i < n; i++){
        S_MATRIX[i][0] = MATRIX[i][0];
        if(MATRIX[i][0] > MAXIMUM){
            MAXIMUM = MATRIX[i][0];
            *x = i;
            *y = 0;
        }
        S_MATRIX[0][i] = MATRIX[0][i];
        if(MATRIX[0][i] > MAXIMUM){
            MAXIMUM = MATRIX[0][i];
            *x = 0;
            *y = i;
        }
    }
    
    //for each S(i,j), we compare S(i-1, j), S(i, j - 1), S(i - 1, j - 1)
    //S(i, j) = min{S(i-1, j), S(i, j - 1), S(i - 1, j - 1)} + 1, if matrix(i, j) = 1
    //if matrix(i, j) = 0, S(i, j) = 0
    for(i = 1; i < n; i++){
        for(j = 1; j < n; j++){
            if(MATRIX[i][j] == 0)
                S_MATRIX[i][j] = 0;
            else{
                MINIMUM = n * n + 1;
                if(S_MATRIX[i - 1][j] < MINIMUM)
                    MINIMUM = S_MATRIX[i - 1][j];
                if(S_MATRIX[i][j - 1] < MINIMUM)
                    MINIMUM = S_MATRIX[i][j - 1];
                if(S_MATRIX[i - 1][j - 1] < MINIMUM)
                    MINIMUM = S_MATRIX[i - 1][j - 1];
                S_MATRIX[i][j] = MINIMUM + 1;
                if(S_MATRIX[i][j] > MAXIMUM){
                    MAXIMUM = S_MATRIX[i][j];
                    *x = i;
                    *y = j;
                }
            }
        }
    }
    
    //free memory
    for(i = 0; i < n; i++)
        free(S_MATRIX[i]);
    free(S_MATRIX);
    
    return MAXIMUM;
}


int main(int argc, char **argv){
    int n;
    int x,y;
    int i,j;
    int **matrix;
    int size;
    
    while(1){
        scanf("%d", &n);
        if(n == 0)
            break;
        
        matrix = (int**)malloc(n * sizeof(int*));
        for(i = 0; i < n; i++){
            matrix[i] = (int*)malloc(n * sizeof(int));
            for(j = 0; j < n; j++)
                scanf("%d", &matrix[i][j]);
        }
        size = FIND_LSS(matrix, n, &x, &y);
        //normalize the ouput
        if(size == 0)
            printf("0\n1 1\n");
        else
            printf("%d\n%d %d\n", size, x - size + 2, y - size + 2);
        
        //free memory
        for(i = 0; i < n; i++)
            free(matrix[i]);
        free(matrix);
    }
    
    return 0;
}