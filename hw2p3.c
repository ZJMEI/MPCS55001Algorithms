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
#include <time.h>


#define MAXIMUM_ = 1000000.0
#define MINIMUM_ = -1000000.0

#define ROUND 68
#define TEST_ROUND 1000000

//return the index of key or the index of the next larger value(if key not exist)
//from array arr[i...j]
int BSEARCH_(double *arr, double key, int i, int j){
    int k;
    if(key > arr[j])
        return j + 1;
    
    if(i == j){
        if(arr[i] == key)
            return i;
        else if(arr[i] < key)
            return i + 1;
        else
            return i;
    }
    
    k = (i + j) / 2;
    if(arr[k] < key)
        return BSEARCH_(arr, key, k + 1, j);
    else
        return BSEARCH_(arr, key, i, k);
}

//there are k different arrays of L
double **INPUT_HANDLE(int k){
    int i,j,n;
    double **arr;
    
    arr = (double**)malloc(k * sizeof(double*));
    for(i = 0; i < k; i++){
        scanf("%d", &n);
        *(arr + i) = (double*)malloc((n + 1) * sizeof(double));
        //we store the length of each array in array[0]
        //data from array[1] to array[n]
        *(*(arr + i) + 0) = (double)n;
        for(j = 1; j <= n; j++)
            scanf("%lf", *(arr + i) + j);
    }
    return arr;
}

double d_rand(double min, double max){
    double d = (double)rand() / RAND_MAX;
    return min + (max - min) * d;
}

//find the minimum and maximum of the total array
void FIND_MM(double **arr, int k, double *min, double *max){
    int i, n;
    *min = 100000.0;
    *max = -100000.0;
    for(i = 0; i < k; i++){
        if(arr[i][1] < *min)
            *min = arr[i][1];
        n = (int)arr[i][0];
        if(arr[i][n] > *max)
            *max = arr[i][n];
    }
    return;
}

//function1
//binary search for each array
int* FUNC_1(double **arr, int k, double key){
    int i, n;
    int *result;
    
    result = (int*)malloc(k * sizeof(int));
    //k arrays
    for(i = 0; i< k; i++){
        n = (int)arr[i][0];
        result[i] = BSEARCH_(arr[i], key, 1, n) - 1;
    }
    
    return result;
}

//function2
//already build a unionzed array P and its index
int* FUNC_2(double **P, int k, double key){
    int i;
    int pivot;
    int *result;
    
    result = (int*)malloc(k * sizeof(int));
    pivot = BSEARCH_(P[0], key, 1, (int)P[0][0]);
    for(i = 0; i < k; i++)
        result[i] = (int)P[i + 1][pivot];
    
    return result;
}

//function3
//already build 3 array for each Li: Mi, pointer to Li and pointer to Mi+1
int* FUNC_3(double **M, int k, double key){
    int i;
    int pivot;
    int *result;
    
    result = (int*)malloc(k * sizeof(int));
    pivot = BSEARCH_(M[0], key, 1, (int)M[0][0]);
    result[0] = M[1][pivot];
    pivot = M[2][pivot] + 1;
    for(i = 1; i < k; i++){
        //single comparison
        if(pivot != 1)
            if(M[i * 3][pivot - 1] >= key)
                pivot--;
        result[i] = M[i * 3 + 1][pivot];
        pivot = M[i * 3 + 2][pivot] + 1;
    }
    
    return result;
}


//build array P and index L1,L2...Lk use method 2
//the return value INDEX_[i][j] is a (k + 1) * (length of p) matrix
//row[0] store the P, with row[0][0] = length
//else store L1 to Lk
double** FUNC_2_INIT(double **arr, int k, double min, double max){
    int i,j;
    double *tP;
    double *P;
    double **INDEX_;
    
    int TOTAL_NUM;      //number of element(allow duplicate element)
    int INDEX[k];       //the pivot for each array      (when merge)
    int p_index;        //the main pivot of P           (when merge)
    double MERGE_MIN;   //the local min                 (when merge)
    int MERGE_INDEX;    //from wich array the local min (when merge)
    
    //compute the TOTAL_NUM, so we can malloc a space
    for(i  = 0, TOTAL_NUM = 0; i < k; i++)
        TOTAL_NUM += (int)arr[i][0];
    //temprarily store the entire unionzed array
    tP = (double*)malloc(TOTAL_NUM * sizeof(double));
    //initiate INDEX
    for(i = 0; i < k; i++)
        INDEX[i] = 1;
    //merge k arrays (find the minimum from k element each time)
    for(i = 0, p_index = 0; i < TOTAL_NUM; i++){
        MERGE_MIN = 100000.0;
        MERGE_INDEX = -1;
        for(j = 0; j < k; j++){
            //index[j] still valid
            if(INDEX[j] != (int)arr[j][0] + 1){
                if(arr[j][INDEX[j]] < MERGE_MIN){
                    MERGE_MIN = arr[j][INDEX[j]];
                    MERGE_INDEX = j;
                }
            }
        }
        if(i == 0){
            tP[p_index] = MERGE_MIN;
            INDEX[MERGE_INDEX]++;
        }
        //ignore duplicate element
        else if(tP[p_index] != MERGE_MIN){
            tP[p_index + 1] = MERGE_MIN;
            INDEX[MERGE_INDEX]++;
            p_index++;
        }
        else{
            INDEX[MERGE_INDEX]++;
        }
    }
    p_index++;
    
    //p_index now is the number of element in the new unionzed array
    //merge finish
    //create P[0...p_index], P[0] store the length
    P = (double*)malloc((p_index + 1) * sizeof(double));
    P[0] = (double)p_index;
    memcpy(P + 1, tP, p_index * sizeof(double));
    free(tP);
    
    //for each element in P, find the index
    //store as a double data in INDEX_[1..K]
    INDEX_ = (double**)malloc((k + 1) * sizeof(double*));
    INDEX_[0] = P;
    for(i = 1; i <= k; i++){
        INDEX_[i] = (double*)malloc((p_index + 1) * sizeof(double));
        INDEX_[i][0] = (double)p_index;
        for(j = 1; j <= p_index; j++){
            //INDEX_[1] means the first array index, which reffer to arr[0]
            //arr[0] is the length, so the actual index will be BSEARCH_(...) - 1
            INDEX_[i][j] =(double) BSEARCH_(arr[i - 1], P[j], 1, (int)arr[i - 1][0]) - 1;
        }
    }
    
    return INDEX_;
}


//build INDEX use method 3
//the return value M[i][j] is a (3k) * (variable length) matrix
//every 3 row store the Mi, pointer to Li and pointer to Mi+1
//every first element in a row store the length of that row
double** FUNC_3_INIT(double **arr, int k){
    int i,j;
    double **M;
    
    int TOTAL_NUM;                      //number of element(allow duplicate element)
    double MERGE_MIN;                   //the local min                 (when merge)
    double *tM;                         //temp storage of Mi
    int INDEX_M, INDEX_L, INDEX_MAIN;   //3 index for Mi+1, Li, Mi      (when merge)
    int INDEX_FLAG;                     //0 for INDEX_M, 1 for INDEX_L
    
    //each array will have 2 followed array
    M = (double**)malloc((k * 3) * sizeof(double));
    
    //Mk = LK
    for(i = 0; i < 3; i++){
        M[3 * (k - 1) + 0] = (double*)malloc(((int)arr[k - 1][0] + 1) * sizeof(double));
        M[3 * (k - 1) + 1] = (double*)malloc(((int)arr[k - 1][0] + 1) * sizeof(double));
        M[3 * (k - 1) + 2] = (double*)malloc(((int)arr[k - 1][0] + 1) * sizeof(double));
    }
    memcpy(M[3 * (k - 1) + 0], arr[k - 1], ((int)arr[k - 1][0] + 1) * sizeof(double));
    M[3 * (k - 1) + 1][0] = arr[k - 1][0];
    M[3 * (k - 1) + 2][0] = arr[k - 1][0];
    for(i = 1; i <= (int)arr[k - 1][0]; i++){
        M[3 * (k - 1) + 1][i] = i - 1;
        M[3 * (k - 1) + 2][i] = 0;
    }
    
    //recursively compute Mi and its two pointer array
    for(i = k - 2; i >=0; i--){
        //merge Li and Mi+1
        //we use every element in Li and every second element in Li+1
        //so the total_num is lengthof(Li) + lengthof(Li+1)/2
        TOTAL_NUM = (int)arr[i][0] + (int)M[3 * (i + 1)][0] / 2;
        tM = (double*)malloc(TOTAL_NUM * sizeof(double));
        INDEX_M = 1;        //index for every second element
        INDEX_L = 1;        //index for every element
        INDEX_MAIN = 0;
        for(j = 0; j < TOTAL_NUM; j++){
            MERGE_MIN = 100000;
            if(2 * INDEX_M <= (int)M[3 * (i + 1)][0]){
                if(M[3 * (i + 1)][2 * INDEX_M] < MERGE_MIN){
                    MERGE_MIN = M[3 * (i + 1)][2 * INDEX_M];
                    INDEX_FLAG = 0;
                }
            }
            if(INDEX_L <= (int)arr[i][0]){
                if(arr[i][INDEX_L] < MERGE_MIN){
                    MERGE_MIN = arr[i][INDEX_L];
                    INDEX_FLAG = 1;
                }
            }
            if(j == 0){
                tM[INDEX_MAIN] = MERGE_MIN;
                if(INDEX_FLAG == 0)
                    INDEX_M++;
                else
                    INDEX_L++;
            }
            else if(tM[INDEX_MAIN] != MERGE_MIN){
                tM[INDEX_MAIN + 1] = MERGE_MIN;
                INDEX_MAIN++;
                if(INDEX_FLAG == 0)
                    INDEX_M++;
                else
                    INDEX_L++;
            }
            else{
                if(INDEX_FLAG == 0)
                    INDEX_M++;
                else
                    INDEX_L++;
            }
            
        }
        INDEX_MAIN++;
        //now tM is the Mi
        M[3 * i] = (double*)malloc((INDEX_MAIN + 1) * sizeof(double));
        M[3 * i][0] = INDEX_MAIN;
        memcpy(M[3 * i] + 1, tM, INDEX_MAIN * sizeof(double));
        free(tM);
        
        //compute the associate pointer array of Mi
        M[3 * i + 1] = (double*)malloc((INDEX_MAIN + 1) * sizeof(double));
        M[3 * i + 2] = (double*)malloc((INDEX_MAIN + 1) * sizeof(double));
        M[3 * i + 1][0] = INDEX_MAIN;
        M[3 * i + 2][0] = INDEX_MAIN;
        for( j= 1; j <= INDEX_MAIN; j++){
            M[3 * i + 1][j] = (double) BSEARCH_(arr[i], M[3 * i][j], 1, (int)arr[i][0]) - 1;
            M[3 * i + 2][j] = (double) BSEARCH_(M[3 * (i + 1)], M[3 * i][j], 1, (int)M[3 * (i + 1)][0]) - 1;
        }
    }
    
    return M;
}

void display(double **arr, int k){
    int i;
    int j;
    for(i = 0; i < k; i++){
        for(j = 0; j <=(int) arr[i][0]; j++)
            printf("%lf ", arr[i][j]);
        printf("\n");
    }
}

int main(int argc, char **argv){
    int i;
    double **tarr;
    double min, max;
    double **P;
    double **M;
    int *result;
    double key;
    
    clock_t TIME_START;
    clock_t TIME_STOP;
    
    double TFUNC1;
    double TFUNC2;
    double TFUNC3;
    
    srand((unsigned int)clock());
    
    TIME_START = clock();
    tarr = INPUT_HANDLE(ROUND);
    TIME_STOP = clock();
    printf("Data input costs %f ms\n", (float)(TIME_STOP - TIME_START) * 1000 / CLOCKS_PER_SEC);
    /*
    for(i = 0; i < ROUND; i++){
        for(j = 0; j <= (int)tarr[i][0]; j++)
            printf("%lf ", tarr[i][j]);
        printf("\n");
    }
     */
    
    FIND_MM(tarr, ROUND, &min, &max);
    //printf("%lf %lf\n", min, max);
    
    TIME_START = clock();
    P = FUNC_2_INIT(tarr, ROUND, min, max);
    TIME_STOP = clock();
    printf("FUNC_2_INIT runs %f s\n", (float)(TIME_STOP - TIME_START) / CLOCKS_PER_SEC);
    
    TIME_START = clock();
    M = FUNC_3_INIT(tarr, ROUND);
    TIME_STOP = clock();
    printf("FUNC_3_INIT runs %f s\n", (float)(TIME_STOP - TIME_START) / CLOCKS_PER_SEC);
    
    //display(M, 3*ROUND);
    
    TFUNC1 = 0.0;
    TFUNC2 = 0.0;
    TFUNC3 = 0.0;
    
    for(i = 0; i < TEST_ROUND; i++){
        key = d_rand(min, max);
        
        TIME_START = clock();
        result = FUNC_1(tarr, ROUND, key);
        /*for(i = 0; i < ROUND; i++)
            printf("FUNC1: %d ", result[i]);
        printf("\n");*/
        TIME_STOP = clock();
        TFUNC1 += (double)(TIME_STOP - TIME_START) / CLOCKS_PER_SEC;
        free(result);
        
        TIME_START = clock();
        result = FUNC_2(P, ROUND, key);
        /*for(i = 0; i < ROUND; i++)
            printf("FUNC2: %d ", result[i]);
        printf("\n");*/
        TIME_STOP = clock();
        TFUNC2 += (double)(TIME_STOP - TIME_START) / CLOCKS_PER_SEC;
        free(result);
        
        TIME_START = clock();
        result = FUNC_3(M, ROUND, key);
        /*for(i = 0; i < ROUND; i++)
            printf("FUNC3: %d ", result[i]);
        printf("\n");*/
        TIME_STOP = clock();
        TFUNC3 += (double)(TIME_STOP - TIME_START) / CLOCKS_PER_SEC;
        free(result);
    }
    printf("Totally runs %d times.\n", TEST_ROUND);
    printf("Func 1 runs %lf ms\n", TFUNC1 * 1000);
    printf("Func 2 runs %lf ms\n", TFUNC2 * 1000);
    printf("Func 3 runs %lf ms\n", TFUNC3 * 1000);
    
    return 0;
}
