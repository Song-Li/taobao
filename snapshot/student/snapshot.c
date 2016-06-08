#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "snapshot.h"
#define debug
snapshot *pSnapHead = NULL;
snapshot *pSnapSearch;
snapshot *pSnapFront;
snapshot *pSnapNext;

entry *pEntryHead = NULL;//current entry head
entry *pEntrySearch;//current entry
entry *pEntryFront;//point to the previous one
entry *pEntryNext;//point to the next one


entry *pNewEntryHead;
entry *pNewEntrySearch;
entry *pNewEntryFront;
entry *pNewEntryNext;

entry *pValueSearch;//a temp entry for searching dynamic array of values
entry *pValueFront;// similar temp value pointing to previous
entry *pValueNext;//similar temp value pointing to next

entry *pNewValueHead;
entry *pNewValueSearch;
entry *pNewValueFront;
entry *pNewValueNext;

void Bye(){
#ifndef debug
    //delete the current entries
    pEntrySearch = pEntryHead;
    while (pEntrySearch != NULL)
    {
        pValueSearch->values = pEntrySearch->values;
        while (pValueSearch->values != NULL)
        {
            pValueFront->values = pValueSearch->values;
            pValueSearch = pValueSearch->next;
            free(pValueFront->values);
            free(pValueFront);
        }
        pEntryFront = pEntrySearch;
        pEntrySearch = pEntrySearch->next;
        free(pEntryFront);
    }
    free(pValueSearch);
    pEntryHead = NULL;
    //delete the snapshots
    pSnapSearch = pSnapHead;
    while (pSnapSearch != NULL)
    {
        pEntrySearch = pSnapSearch->entries;
        while (pEntrySearch != NULL)
        {
            pValueSearch->values = pEntrySearch->values;
            while (pValueSearch != NULL)
            {
                pValueFront = pValueSearch;
                pValueSearch = pValueSearch->next;
                free(pValueFront);
            }
            pEntryFront = pEntrySearch;
            pEntrySearch = pEntrySearch->next;
            free(pEntryFront);
        }
        pEntrySearch->values = NULL;
        pSnapFront = pSnapSearch;
        pSnapSearch = pSnapSearch->next;
        free(pSnapFront);
    }
    pSnapHead = NULL;
#endif
    //exit(1);
    printf("bye\n");
    //printf("\n");
}

void Help()
{
    printf( "%s\n", HELP);
}

void List_keys()
{
    //printf(">\n");
    if (pEntryHead == NULL)
    {
        printf("no keys\n");
    }
    else
    {
        pEntrySearch = pEntryHead;
        while (pEntrySearch != NULL)
        {
            pEntryFront = pEntrySearch;
            pEntrySearch = pEntrySearch->next;
        }
        pEntrySearch = pEntryFront;
        while (pEntrySearch != NULL)
        {
            printf("%s\n", pEntrySearch->key);
            pEntrySearch = pEntrySearch->prev;
        }
    }
    printf("\n");
}

void List_entries()
{
    //printf(">\n");
    if (pEntryHead == NULL)
    {
        printf("no entries\n");
    }
    else
    {
        pEntrySearch = pEntryHead;
        while (pEntrySearch != NULL)
        {
            pEntryFront = pEntrySearch;
            pEntrySearch = pEntrySearch->next;
        }
        pEntrySearch = pEntryFront;
        //pValueSearch->values = pEntrySearch->values;
        while (pEntrySearch != NULL)
        {
            printf("%s", pEntrySearch->key);
            printf(" [");
            pValueSearch->values = pEntrySearch->values;
            pValueSearch->length = pEntrySearch->length;
            for(int i = 0;i<pValueSearch->length;i++){
                printf("%d", pValueSearch->values[i]);
                if(i<pValueSearch->length-1){
                    printf(" ");
                }
            }
            printf("]");
            printf("\n");
            pEntrySearch = pEntrySearch->prev;
        }
    }
    printf("\n");
}

void List_snapshots()
{

    //printf(">\n");
    if (pSnapHead == NULL)
    {
        printf("no snapshots\n");
    }
    else
    {
        pSnapSearch = pSnapHead;
        while (pSnapSearch != NULL)
        {
            printf("%d\n", pSnapSearch->id);
            pSnapSearch = pSnapSearch->next;
        }
    }
    printf("\n");
}

void Get(char cKey[], int n)
{

    //printf(">\n");
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {   
            //pEntrySearch->values = pEntryHead->values;
            printf("[");
            //pValueSearch = pEntrySearch;
            if(pEntrySearch->length==0){
                printf("");
            }else{
                //print dynamic array
                for(int i =0;i<pEntrySearch->length;i++)
                {   
                    printf("%d",pEntrySearch->values[i]);
                    //dont print space at last value in the array
                    if(i<pEntrySearch->length-1){
                        printf(" ");
                    }

                }
            }
            printf("]\n");
            break;
        }
        pEntrySearch = pEntrySearch->next;
        if (pEntrySearch == NULL)
        {
            printf("no such key\n");
            break;
        }
    }
    printf("\n");

}

void Del(char cKey[], int n)
{

    //printf(">\n");
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {
            pEntryFront = pEntrySearch->prev;
            pEntryNext = pEntrySearch->next;
            if (pEntryFront == NULL)
            {
                pEntryHead = pEntryNext;
                if (pEntryNext != NULL)
                {
                    pEntryNext->prev = NULL;
                }
            }
            else
            {
                pEntryFront->next = pEntryNext;
                if (pEntryNext != NULL)
                {
                    pEntryNext->prev = pEntryFront;
                }
            }
            free(pEntrySearch->values);
            free(pEntrySearch);

            printf("ok\n");
            break;
        }
        pEntrySearch = pEntrySearch->next;
        if (pEntrySearch == NULL)
        {
            printf("no such key\n");
            break;
        }
    }
    printf("\n");
}

void Purge(char cKey[], int n)
{


    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {
            pEntryFront = pEntrySearch->prev;
            pEntryNext = pEntrySearch->next;
            pValueSearch->values = pEntrySearch->values;
            if (pEntryFront == NULL)
            {
                pEntryHead = pEntryNext;
                pEntryNext->prev = NULL;
            }
            else
            {
                pEntryFront->next = pEntryNext;
                pEntryNext->prev = pEntryFront;
            }
            free(pEntrySearch);
            if (pValueSearch != NULL)
            {

                free(pValueSearch->values);

            }
            break;
        }
        pEntrySearch = pEntrySearch->next;
    }

    pSnapSearch = pSnapHead;
    while (pSnapSearch != NULL)
    {
        pEntrySearch = pSnapSearch->entries;
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {
                pEntryFront = pEntrySearch->prev;
                pEntryNext = pEntrySearch->next;
                pValueSearch->values = pEntrySearch->values;
                if (pEntryFront == NULL)
                {
                    pSnapSearch->entries = pEntryNext;
                    pEntryNext->prev = NULL;
                }
                else
                {
                    pEntryFront->next = pEntryNext;
                    if (pEntryNext != NULL)
                    {
                        pEntryNext->prev = pEntryFront;
                    }
                }
                free(pEntrySearch);
                if(pValueSearch != NULL)
                {

                    free(pValueSearch->values );

                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
        }
        pSnapSearch = pSnapSearch->next;
    }
    printf("ok\n");
    printf("\n");
}

void Set(char cKey[], int n, int cValue[], int m, int t)
{   

    //cKey the array stores the key
    //cValue stores the values
    //t is the size of the cValue
    //printf(">\n");
    int nKey = 0;
    pEntrySearch = pEntryHead;
    if (pEntryHead == NULL)
    {   
        //allocate memory 
        pEntrySearch = (entry *)malloc(sizeof(entry));
        pEntrySearch->values = NULL;
        pEntrySearch->length=0;
        pEntryHead = pEntrySearch;
        pEntryHead->length = pEntrySearch->length;
        pEntryHead->values = pEntrySearch->values;
        pEntrySearch->next = NULL;
        pEntrySearch->prev = NULL;
    }
    else
    {         

        if (pEntrySearch != NULL)
        {   

            if (strcmp(cKey, pEntrySearch->key) == 0)
            {   

                nKey = 1;
                //delet all the existing values
                //pValueSearch =(entry*)malloc(sizeof(entry));
                //the following part should be more less, pValuesSearch as a buffer makes the process more complicate
                pValueSearch = pEntrySearch;

                if(pValueSearch->values!=NULL){     
                    free(pValueSearch->values);
                    pValueSearch->length=0;
                }

                pEntrySearch->values = NULL;
                pEntrySearch->length = 0;
                //i realised that pValueSearch is a repeated step here
                //then add new values
                pValueSearch =(entry*)malloc(sizeof(entry));
                pValueSearch->values = (int*)malloc(sizeof(int)*t);
                pValueSearch->length = 0;
                pEntrySearch->values = pValueSearch->values;

                for (int i = 0; i < t; i++)
                {
                    //pEntrySearch->values=pValueSearch->values;
                    //pValueSearch->values = (int*)malloc(sizeof(int));
                    pValueSearch->length++;
                    pEntrySearch->length = pValueSearch->length;
                    pValueSearch->values[i] = cValue[i];


                }
                //pEntrySearch->values = pValueSearch->values;

            }
            pEntryFront = pEntrySearch;
            pEntrySearch = pEntrySearch->next;

        }
        pEntrySearch = (entry *)malloc(sizeof(entry));
        pEntrySearch->values = NULL;
        pEntrySearch->length = 0;
        pEntryFront->next = pEntrySearch;
        pEntrySearch->prev = pEntryFront;
        pEntrySearch->next = NULL;
    }
    //if there is no such key or the key doesnt have any value it will come here to allocate memory and gives the value to the dynamic array
    if (nKey == 0)
    {

        strcpy(pEntrySearch->key, cKey);

        pValueSearch =(entry*)malloc(sizeof(entry));
        pValueSearch->values = (int*)malloc(sizeof(int)*t);
        pEntrySearch->values = pValueSearch->values;
        pValueSearch->length = 0;

        for (int i = 0; i < t; i++)
        {       
            //pValueSearch->values = (int*)malloc(sizeof(int));
            //pEntrySearch->values = pValueSearch->values;
            pValueSearch->length++;
            pEntrySearch->length = pValueSearch->length;
            //pValueSearch->values = (int*)malloc(sizeof(int));
            pValueSearch->values[i] = cValue[i];

        }
        //pEntrySearch->values = pValueSearch->values;

    }
    printf("ok\n");
    printf("\n");

}

void Push(char cKey[], int n, int cValue[], int m, int t)
{

    //printf(">\n");
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {
            //printf("in push %d %d,%d %d\n",n,cValue[0],m,t);//cValue sotres input, t is the number of input
            if(pEntrySearch->values==NULL){
                //printf("1");
                free(pEntrySearch->values);
                //printf("2");
                pEntrySearch->values=(int*)malloc(sizeof(int)*t);
                //printf("3");
                pEntrySearch->length = t;
                //  printf("memory have been allocated%d\n", cValue[0]);
                for(int i = 0;i<t;i++){
                    //      printf("jinlaihoude zhi%d", cValue[i]);
                    pEntrySearch->values[i] = cValue[t-i-1];
                }
            }else{
                //  printf("4\n");
                //nlength is the new after pushing
                int nlength = pEntrySearch->length+t;
                int buffer[nlength];//buffer stores the values before pop
                for(int i = 0,j=t-1;i<t;i++){
                    buffer[i] = cValue[j--];
                }

                //int nlength = pEntrySearch->length+t;
                for(int i = t,j=0;i<nlength;i++){
                    buffer[i] = pEntrySearch->values[j++];
                }


                free(pEntrySearch->values);
                pEntrySearch->values = (int*)malloc(sizeof(int)*nlength);
                pEntrySearch->length = nlength;
                for(int i = 0;i<nlength;i++){
                    pEntrySearch->values[i]= buffer[i];
                }

            }
            printf("ok\n");
            break;
        }
        pEntrySearch = pEntrySearch->next;
        if (pEntrySearch == NULL)
        {
            printf("no such key\n");
        }
    }
    printf("\n");
}


void Append(char cKey[], int n, int cValue[], int m, int t)
{

    //printf(">\n");
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {
            int nlength = pEntrySearch->length+t;
            //int buffer[nlength];
            if(nlength == t){
                free(pEntrySearch->values);
                pEntrySearch->values=(int*)malloc(sizeof(int)*t);
                pEntrySearch->length = t;
                for(int i = 0;i<t;i++){
                    pEntrySearch->values[i] = cValue[i]; 
                    //  printf("in the for loop %d\n", pEntrySearch->values[i]);
                }
                printf("ok\n");
                break;
            }else{
                //printf("1\n");
                //int nlength = pEntrySearch->length+t;
                int buffer[nlength];
                for(int i =0;i<pEntrySearch->length;i++){
                    buffer[i] = pEntrySearch->values[i];
                    //printf("in1stfor %d\n", buffer[i]);
                }
                for(int i = pEntrySearch->length,j=0;i<nlength;i++){
                    buffer[i] = cValue[j];
                    j++;
                    //printf("in2nd for %d\n", buffer[i]);
                }
                //printf("2\n");
                free(pEntrySearch->values);
                pEntrySearch->values=(int*)malloc(sizeof(int)*nlength);
                pEntrySearch->length = nlength;
                for(int i = 0;i<nlength;i++){

                    pEntrySearch->values[i] = buffer[i]; 
                    //printf("in3RD for %d\n", pEntrySearch->values[i]);
                }
                printf("ok\n");
                break;
            }

        }
        pEntrySearch = pEntrySearch->next;

        if (pEntrySearch == NULL)
        {
            printf("no such key\n");
        }
    }
    printf("\n");
}

void Pick(char cKey[], int n, int nIndex)
{

    int nKey = 0;
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {
            if (nIndex <= 0||nIndex>pEntrySearch->length)
            {
                printf("index out of range\n");
                nKey = 1;
                break;
            }
            else{
                printf("%d\n",pEntrySearch->values[nIndex-1]);
                break;
            }

        }
        pEntrySearch = pEntrySearch->next;
        if (pEntrySearch == NULL)
        {
            printf("no such key\n");
        }
    }
    printf("\n");
}

void Pluck(char cKey[], int n, int nIndex)
{
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {
            if (nIndex <= 0||nIndex>pEntrySearch->length)
            {
                printf("index out of range\n");
                break;
            }else{
                int key = 0;
                int nLength = pEntrySearch->length-1;
                printf("%d\n",pEntrySearch->values[nIndex-1]);
                int buffer[nLength];
                for(int i = 0,j=0;i<pEntrySearch->length;i++){
                    if(i ==(nIndex-1)){
                        key = 1;
                    }else{
                        buffer[j]  = pEntrySearch->values[i];

                        j++;
                    }

                }
                //for(int i =0;i<nLength;i++){
                //  printf("buffer values%d\n", buffer[i]);
                //}
                free(pEntrySearch->values);
                pEntrySearch->values = (int*)malloc(sizeof(int)*nLength);
                pEntrySearch->length = nLength;
                for(int i = 0;i<nLength;i++){
                    pEntrySearch->values[i] = buffer[i];
                    //printf("set values%d\n",pEntrySearch->values[i]);
                }
                break;
            }
        }
        pEntrySearch = pEntrySearch->next;
        if (pEntrySearch == NULL)
        {
            printf("no such key\n");
        }
        ;
    }
    printf("\n");
}

void Pop(char cKey[], int n)
{

    //printf(">\n");
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no such key\n");
    }
    while (pEntrySearch != NULL)
    {
        if (strcmp(cKey, pEntrySearch->key) == 0)
        {

            if (pEntrySearch->values == NULL)
            {
                printf("nil\n");
            }
            else
            {   
                if(pEntrySearch->length>1){
                    printf("%d\n", pEntrySearch->values[pEntrySearch->length-1]);
                    int nlength = 0;
                    int buffer[pEntrySearch->length-1];//buffer stores the values before pop
                    for(int i =0;i<pEntrySearch->length-1;i++){
                        buffer[i] = pEntrySearch->values[i];
                        //printf("values in buffer %d", buffer[i]);
                        nlength++;
                    }

                    free(pEntrySearch->values);
                    pEntrySearch->values = (int*)malloc(sizeof(int)*nlength);
                    pEntrySearch->length = nlength;
                    for(int i = 0;i<nlength;i++){
                        pEntrySearch->values[i]= buffer[i];
                    }

                }else{
                    printf("%d\n", pEntrySearch->values[pEntrySearch->length-1]);
                    pEntrySearch->values = NULL;
                    pEntrySearch->length =0;
                }
            }
            break;
        }
        pEntrySearch = pEntrySearch->next;
        if (pEntrySearch == NULL)
        {
            printf("no such key\n");
            break;
        }
    }
    printf("\n");
}

void Drop(int nID)
{

    printf(">\n");
    pSnapSearch = pSnapHead;
    if (pSnapSearch == NULL)
    {
        printf("no such snapshot\n");
    }
    while (pSnapSearch != NULL)
    {
        if (nID == pSnapSearch->id)
        {
            pSnapFront = pSnapSearch->prev;
            pSnapNext = pSnapSearch->next;
            pEntrySearch = pSnapSearch->entries;
            while (pEntrySearch != NULL)
            {

                if(pEntrySearch->values != NULL)
                {
                    free(pEntrySearch->values);
                    pEntrySearch->length = 0;
                }
                pEntryFront = pEntrySearch;
                pEntrySearch = pEntrySearch->next;
                free(pEntryFront);
            }
            pSnapFront->next = pSnapNext;
            pSnapNext->prev = pSnapFront;
            free(pSnapSearch);
            printf("ok\n");
            break;
        }
        pSnapSearch = pSnapSearch->next;
        if (pSnapSearch == NULL)
        {
            printf("no such snapshot\n");
        }
    }
    printf("\n");
}

void Rollback(int nID)
{
#ifndef debug
    int nKey = 0;
    pSnapSearch = pSnapHead;
    if (pSnapSearch == NULL)
    {
        printf("no such snapshot\n");
    }
    while (pSnapSearch != NULL)
    {
        if (pSnapSearch->id == nID)
        {
            nKey = 1;
            break;
        }
        pSnapSearch = pSnapSearch->next;
        if (pSnapSearch == NULL)
        {
            printf("no such snapshot\n");
        }
    }
    if (nKey == 1)
    {
        //==========================================================
        pEntrySearch = pEntryHead;
        while (pEntrySearch != NULL)
        {
            pValueSearch = pEntrySearch->values;
            while (pValueSearch != NULL)
            {
                pValueFront = pValueSearch;
                pValueSearch = pValueSearch->next;
                free(pValueFront);
            }
            pEntryFront = pEntrySearch;
            pEntrySearch = pEntrySearch->next;
            free(pEntrySearch->key);
            free(pEntryFront);
            pEntryFront = NULL;
        }
        pEntryHead = NULL;
        //再导入新状态
        pNewEntrySearch = pSnapSearch->entries;//获取Entry头指针
        while (pNewEntrySearch != NULL)
        {
            if (pEntryHead == NULL)
            {
                pEntrySearch = (entry *)malloc(sizeof(entry));
                pEntryHead = pEntrySearch;
                strcpy(pEntrySearch->key, pNewEntrySearch->key);
                pEntryHead->next = NULL;
                pEntryHead->prev = NULL;
                pEntryHead->values = NULL;
                pNewValueSearch = pNewEntrySearch->values;//进入值区
                while (pNewValueSearch != NULL)
                {
                    if (pEntrySearch->values == NULL)
                    {
                        pValueSearch = (value *)malloc(sizeof(value));
                        pEntrySearch->values = pValueSearch;
                        pValueSearch->Value = pNewValueSearch->Value;
                        pValueSearch->prev = NULL;
                        pValueSearch->next = NULL;
                        pValueFront = pValueSearch;
                    }
                    else
                    {
                        pValueSearch = (value *)malloc(sizeof(value));
                        pValueSearch->Value = pNewValueSearch->Value;
                        pValueSearch->prev = pValueFront;
                        pValueFront->next = pValueSearch;
                        pValueSearch->next = NULL;
                        pValueFront = pValueSearch;
                    }
                    pNewValueSearch = pNewValueSearch->next;
                }
                pEntryFront = pEntryHead;
            }
            else
            {
                pEntrySearch = (entry *)malloc(sizeof(entry));
                pEntryFront->next = pEntrySearch;
                pEntrySearch->prev = pEntryFront;
                pEntrySearch->next = NULL;
                pNewValueSearch = pNewEntrySearch->values;//进入值区
                while (pNewValueSearch != NULL)
                {
                    if (pEntrySearch->values == NULL)
                    {
                        pValueSearch = (value *)malloc(sizeof(value));
                        pEntrySearch->values = pValueSearch;
                        pValueSearch->Value = pNewValueSearch->Value;
                        pValueSearch->prev = NULL;
                        pValueSearch->next = NULL;
                        pValueFront = pValueSearch;
                    }
                    else
                    {
                        pValueSearch = (value *)malloc(sizeof(value));
                        pValueSearch->Value = pNewValueSearch->Value;
                        pValueSearch->prev = pValueFront;
                        pValueFront->next = pValueSearch;
                        pValueSearch->next = NULL;
                        pValueFront = pValueSearch;
                    }
                    pNewValueSearch = pNewValueSearch->next;
                }

            }
            pNewEntrySearch = pNewEntrySearch->next;
        }
        //==========================================================
        pSnapFront = pSnapSearch;
        pSnapFront->next = NULL;
        pSnapSearch = pSnapSearch->next;
        while (pSnapSearch != NULL)
        {
            pEntrySearch = pSnapSearch->entries;
            while (pEntrySearch != NULL)
            {
                pValueSearch = pEntrySearch->values;
                while (pValueSearch != NULL)
                {
                    pValueFront = pValueSearch;
                    pValueSearch = pValueSearch->next;
                    free(pValueFront);
                }
                pEntryFront = pEntrySearch;
                pEntrySearch = pEntrySearch->next;
                free(pEntryFront->key);
                free(pEntryFront);
                pEntryFront = NULL;
            }
            pSnapNext = pSnapSearch;
            pSnapSearch = pSnapSearch->next;
            free(pSnapNext);
        }
        printf("ok\n");
    }
#endif
}

void Checkout(int nID)
{
#ifndef debug
    int nKey = 0;
    pSnapSearch = pSnapHead;
    while (pSnapSearch != NULL)
    {
        if (pSnapSearch->id == nID)
        {
            nKey = 1;
            //先删除现有的状态
            pEntrySearch = pEntryHead;
            while (pEntrySearch != NULL)
            {
                pValueSearch = pEntrySearch->values;
                while (pValueSearch != NULL)
                {
                    pValueFront = pValueSearch;
                    pValueSearch = pValueSearch->next;
                    free(pValueFront);
                }
                pEntryFront = pEntrySearch;
                pEntrySearch = pEntrySearch->next;
                free(pEntryFront->key);
                free(pEntryFront);
                pEntryFront = NULL;
            }
            pEntryHead = NULL;
            //再导入新状态
            pNewEntrySearch = pSnapSearch->entries;//获取Entry头指针
            while (pNewEntrySearch != NULL)
            {
                if (pEntryHead == NULL)
                {
                    pEntrySearch = (entry *)malloc(sizeof(entry));
                    pEntryHead = pEntrySearch;
                    strcpy(pEntrySearch->key, pNewEntrySearch->key);
                    pEntryHead->next = NULL;
                    pEntryHead->prev = NULL;
                    pEntryHead->values = NULL;
                    pNewValueSearch = pNewEntrySearch->values;//进入值区
                    while (pNewValueSearch != NULL)
                    {
                        if (pEntrySearch->values == NULL)
                        {
                            pValueSearch = (value *)malloc(sizeof(value));
                            pEntrySearch->values = pValueSearch;
                            pValueSearch->Value = pNewValueSearch->Value;
                            pValueSearch->prev = NULL;
                            pValueSearch->next = NULL;
                            pValueFront = pValueSearch;
                        }
                        else
                        {
                            pValueSearch = (value *)malloc(sizeof(value));
                            pValueSearch->Value = pNewValueSearch->Value;
                            pValueSearch->prev = pValueFront;
                            pValueFront->next = pValueSearch;
                            pValueSearch->next = NULL;
                            pValueFront = pValueSearch;
                        }
                        pNewValueSearch = pNewValueSearch->next;
                    }
                    pEntryFront = pEntryHead;
                }
                else
                {
                    pEntrySearch = (entry *)malloc(sizeof(entry));
                    pEntryFront->next = pEntrySearch;
                    pEntrySearch->prev = pEntryFront;
                    pEntrySearch->next = NULL;
                    pNewValueSearch = pNewEntrySearch->values;//进入值区
                    while (pNewValueSearch != NULL)
                    {
                        if (pEntrySearch->values == NULL)
                        {
                            pValueSearch = (value *)malloc(sizeof(value));
                            pEntrySearch->values = pValueSearch;
                            pValueSearch->Value = pNewValueSearch->Value;
                            pValueSearch->prev = NULL;
                            pValueSearch->next = NULL;
                            pValueFront = pValueSearch;
                        }
                        else
                        {
                            pValueSearch = (value *)malloc(sizeof(value));
                            pValueSearch->Value = pNewValueSearch->Value;
                            pValueSearch->prev = pValueFront;
                            pValueFront->next = pValueSearch;
                            pValueSearch->next = NULL;
                            pValueFront = pValueSearch;
                        }
                        pNewValueSearch = pNewValueSearch->next;
                    }

                }
                pNewEntrySearch = pNewEntrySearch->next;
            }
            printf("ok\n");
            break;
        }
        pSnapSearch = pSnapSearch->next;
    }
    if (nKey == 0)
    {
        printf("no such snapshot\n");
    }
#endif
}

void Snapshot()
{
#ifndef debug
    printf(">\n");
    int nId = 1;
    pSnapNext = (snapshot *)malloc(sizeof(snapshot));
    if (pSnapHead == NULL)
    {
        pSnapNext->id = 1;
        printf("saved as snapshot 1\n");
        pSnapHead = pSnapNext;
        pSnapNext->next = NULL;
        pSnapNext->prev = NULL;
        pSnapNext->entries = NULL;
        pEntrySearch = pEntryHead;
        while (pEntrySearch != NULL)
        {
            if (pSnapNext->entries == NULL)
            {
                pNewEntrySearch = (entry *)malloc(sizeof(entry));
                pNewEntryHead = pNewEntrySearch;
                pSnapNext->entries = pNewEntryHead;
                strcpy(pNewEntrySearch->key, pEntrySearch->key);
                pNewEntrySearch->next = NULL;
                pNewEntrySearch->prev = NULL;
                pNewEntrySearch->values = NULL;
                pValueSearch = pEntrySearch->values;
                if (pValueSearch == NULL)
                {
                    pNewEntrySearch->values = NULL;
                }
                else
                {
                    while (pValueSearch != NULL)
                    {
                        if (pNewEntrySearch->values == NULL)
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewEntrySearch->values = pNewValueSearch;
                            pNewValueHead = pNewValueSearch;
                            pNewValueHead->next = NULL;
                            pNewValueHead->prev = NULL;
                            pNewValueHead->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        else
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewValueFront->next = pNewValueSearch;
                            pNewValueSearch->prev = pNewValueFront;
                            pNewValueSearch->next = NULL;
                            pNewValueSearch->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        pValueSearch = pValueSearch->next;
                    }
                }
                pNewEntryFront = pNewEntrySearch;
            }
            else
            {
                pNewEntrySearch = (entry *)malloc(sizeof(entry));
                pNewEntryFront->next = pNewEntrySearch;
                pNewEntrySearch->prev = pNewEntryFront;
                pNewEntrySearch->next = NULL;
                pNewEntrySearch->values = NULL;
                strcpy(pNewEntrySearch->key, pEntrySearch->key);
                pValueSearch = pEntrySearch->values;
                if (pValueSearch == NULL)
                {
                    pNewEntrySearch->values = NULL;
                }
                else
                {
                    while (pValueSearch != NULL)
                    {
                        if (pNewEntrySearch->values == NULL)
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewEntrySearch->values = pNewValueSearch;
                            pNewValueHead = pNewValueSearch;
                            pNewValueHead->next = NULL;
                            pNewValueHead->prev = NULL;
                            pNewValueHead->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        else
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewValueFront->next = pNewValueSearch;
                            pNewValueSearch->prev = pNewValueFront;
                            pNewValueSearch->next = NULL;
                            pNewValueSearch->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        pValueSearch = pValueSearch->next;
                    }
                }
                pNewEntryFront = pNewEntrySearch;
            }
            pEntrySearch = pEntrySearch->next;
        }
    }
    else
    {
        pSnapSearch = pSnapHead;
        while (pSnapSearch != NULL)
        {
            pSnapFront = pSnapSearch;
            nId++;
            pSnapSearch = pSnapSearch->next;
        }
        pSnapNext->id = nId;
        printf("saved as snapshot %d\n", nId);
        pSnapNext->prev = pSnapFront;
        pSnapFront->next = pSnapNext;
        pSnapNext->next = NULL;
        pSnapNext->entries = NULL;
        pEntrySearch = pEntryHead;
        while (pEntrySearch != NULL)
        {
            if (pSnapNext->entries == NULL)
            {
                pNewEntrySearch = (entry *)malloc(sizeof(entry));
                pNewEntryHead = pNewEntrySearch;
                pSnapNext->entries = pNewEntrySearch;
                strcpy(pSnapNext->entries->key, pEntrySearch->key);
                pNewEntrySearch->next = NULL;
                pNewEntrySearch->prev = NULL;
                pNewEntrySearch->values = NULL;
                pValueSearch = pEntrySearch->values;
                if (pValueSearch == NULL)
                {
                    pNewEntrySearch->values = NULL;
                }
                else
                {
                    while (pValueSearch != NULL)
                    {
                        if (pNewEntrySearch->values == NULL)
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewEntrySearch->values = pNewValueSearch;
                            pNewValueHead = pNewValueSearch;
                            pNewValueHead->next = NULL;
                            pNewValueHead->prev = NULL;
                            pNewValueHead->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        else
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewValueFront->next = pNewValueSearch;
                            pNewValueSearch->prev = pNewValueFront;
                            pNewValueSearch->next = NULL;
                            pNewValueSearch->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        pValueSearch = pValueSearch->next;
                    }
                }
                pNewEntryFront = pNewEntrySearch;
            }
            else
            {
                pNewEntrySearch = (entry *)malloc(sizeof(entry));
                pNewEntryFront->next = pNewEntrySearch;
                pNewEntrySearch->prev = pNewEntryFront;
                pNewEntrySearch->next = NULL;
                pNewEntrySearch->values = NULL;
                strcpy(pNewEntrySearch->key, pEntrySearch->key);
                pValueSearch = pEntrySearch->values;
                if (pValueSearch == NULL)
                {
                    pNewEntrySearch->values = NULL;
                }
                else
                {
                    while (pValueSearch != NULL)
                    {
                        if (pNewEntrySearch->values == NULL)
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewEntrySearch->values = pNewValueSearch;
                            pNewValueHead = pNewValueSearch;
                            pNewValueHead->next = NULL;
                            pNewValueHead->prev = NULL;
                            pNewValueHead->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        else
                        {
                            pNewValueSearch = (value *)malloc(sizeof(value));
                            pNewValueFront->next = pNewValueSearch;
                            pNewValueSearch->prev = pNewValueFront;
                            pNewValueSearch->next = NULL;
                            pNewValueSearch->Value = pValueSearch->Value;
                            pNewValueFront = pNewValueSearch;
                        }
                        pValueSearch = pValueSearch->next;
                    }
                }
                pNewEntryFront = pNewEntrySearch;
            }
            pEntrySearch = pEntrySearch->next;
        }
    }
#endif
}

void Min(char cKey[], int n)
{

    printf(">\n");
    int nMinvalue;
    pEntrySearch = pEntryHead;
    if (pEntryHead == NULL)
    {
        printf("no key\n");
    }
    else
    {
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {

                if (pEntrySearch->values == NULL)
                {
                    printf("no value\n");
                }
                else
                {
                    nMinvalue = pEntrySearch->values[0];
                    for(int i =0;i<pEntrySearch->length;i++){
                        if(nMinvalue>pEntrySearch->values[i]){
                            nMinvalue = pEntrySearch->values[i];
                        }
                    }
                    printf("%d\n", nMinvalue);
                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
            if (pEntrySearch == NULL)
            {
                printf("no such key\n");
            }
        }
    }

}

void Max(char cKey[], int n)
{

    printf(">\n");
    int nMaxvalue;
    pEntrySearch = pEntryHead;
    if (pEntryHead == NULL)
    {
        printf("no key\n");
    }
    else
    {
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {

                if (pEntrySearch->values == NULL)
                {
                    printf("no value\n");
                }
                else
                {
                    nMaxvalue = pEntrySearch->values[0];
                    for(int i =0;i<pEntrySearch->length;i++){
                        if(nMaxvalue<pEntrySearch->values[i]){
                            nMaxvalue = pEntrySearch->values[i];
                        }
                    }
                    printf("%d\n", nMaxvalue);
                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
            if (pEntrySearch == NULL)
            {
                printf("no such key\n");
            }
        }
    }

}

void Sum(char cKey[], int n)
{

    printf(">\n");
    int nSum = 0;
    pEntrySearch = pEntryHead;
    if (pEntryHead == NULL)
    {
        printf("no key\n");
    }
    else
    {
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {

                if (pEntrySearch->values == NULL)
                {
                    printf("no value\n");
                }
                else
                {
                    for(int i = 0;i<pEntrySearch->length;i++){
                        nSum += pEntrySearch->values[i];
                    }
                    printf("%d\n", nSum);
                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
            if (pEntrySearch == NULL)
            {
                printf("no such key\n");
            }
        }
    }

}

void Len(char cKey[], int n)
{
#ifndef debug
    printf(">\n");
    int nLen = 0;
    pEntrySearch = pEntryHead;
    if (pEntryHead == NULL)
    {
        printf("no key\n");
    }
    else
    {
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {
                pValueSearch = pEntrySearch->values;
                if (pValueSearch == NULL)
                {
                    printf("no value\n");
                }
                else
                {
                    while (pValueSearch != NULL)
                    {
                        nLen++;
                        pValueSearch = pValueSearch->next;
                    }
                    printf("%d\n", nLen);
                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
            if (pEntrySearch == NULL)
            {
                printf("no such key\n");
            }
        }
    }
#endif
}


void Uniq(char cKey[], int n)
{

    printf(">\n");

    pEntrySearch = pEntryHead;
    //int buffer[pEntrySearch->length];
    if (pEntrySearch == NULL)
    {
        printf("no key\n");
    }
    else
    {
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {

                if (pEntrySearch->values == NULL)
                {
                    printf("no value\n");
                }
                else
                {

                    //pEntrySearch->values = pEntryHead->values;
                    //pEntrySearch->length = pEntrySearch->length;
                    int length = pEntrySearch->length;
                    int buffer[length];
                    for(int i = 0;i<length;i++){
                        buffer[i] = pEntrySearch->values[i];        
                    }
                    int nlength=1;

                    for(int i = 1;i<length;i++){
                        //printf("\nits the %d time\n",i-1);
                        int count = 0;

                        for(int j = i-1;j>=0;){
                            //printf("\n j value %d \n", j);


                            if(buffer[i] == buffer[j]){
                                count=1;    
                                j=-1;
                            }else{
                                j--;
                            }

                        }
                        //printf("its after the %d time and count values is %d \n",i-1,count);
                        if(count == 0){
                            nlength++;
                        }


                    }
                    //printf("new length\n %d", nlength);
                    int temp[nlength];
                    temp[0] = buffer[0];
                    for(int i = 1,j = 1;i<length;i++){
                        int count = 0;

                        for(int k = i-1;k>=0;k--){
                            if(buffer[i] == buffer[k]){
                                count++;                        
                            }   
                        }
                        if(count == 0){
                            temp[j] = buffer[i];
                            j++;
                        }

                    }
                    for(int i = 0;i<nlength;i++){
                        //printf("temp array%d", temp[i]);
                    }
                    free(pEntrySearch->values);
                    pEntrySearch->length = 0;
                    pEntrySearch->values = (int *)malloc(sizeof(int) * nlength);
                    pEntrySearch->length = nlength;
                    for(int i = 0;i<nlength;i++){
                        pEntrySearch->values[i] = temp[i];
                    }
                    printf("ok\n");
                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
        }
    }

}

void Sort(char cKey[], int n)
{
#ifndef debug
    printf(">\n");
    int nNum[100];
    int i = 0;
    int temp;
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no key\n");
    }
    else
    {
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {
                pValueSearch = pEntrySearch->values;
                if (pValueSearch == NULL)
                {
                    printf("no value\n");
                }
                else
                {
                    while (pValueSearch != NULL)
                    {
                        nNum[i] = pValueSearch->Value;
                        pValueSearch = pValueSearch->next;
                        if (pValueSearch != NULL)
                        {
                            i++;
                        }
                    }
                    for (int k = 0; k < i; k++)
                    {
                        for (int h = k; h < i; h++)
                        {
                            if (nNum[k] < nNum[h])
                            {
                                temp = nNum[k];
                                nNum[k] = nNum[h];
                                nNum[h] = temp;
                            }
                        }
                    }
                    pValueSearch = pEntrySearch->values;
                    while (pValueSearch != NULL)
                    {
                        pValueSearch->Value = nNum[i];
                        pValueSearch = pValueSearch->next;
                        i--;
                    }
                    printf("ok\n");
                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
        }
    }
#endif
}
void Rev(char cKey[], int n)
{
#ifndef debug
    printf(">\n");
    int nNum[100];
    int i = 0;
    pEntrySearch = pEntryHead;
    if (pEntrySearch == NULL)
    {
        printf("no key\n");
    }
    else
    {
        while (pEntrySearch != NULL)
        {
            if (strcmp(cKey, pEntrySearch->key) == 0)
            {
                pValueSearch = pEntrySearch->values;
                if (pValueSearch == NULL)
                {
                    printf("no value\n");
                }
                else
                {
                    while (pValueSearch != NULL)
                    {
                        nNum[i] = pValueSearch->Value;
                        pValueSearch = pValueSearch->next;
                        if (pValueSearch != NULL)
                        {
                            i++;
                        }
                    }
                    pValueSearch = pEntrySearch->values;
                    while (pValueSearch != NULL)
                    {
                        pValueSearch->Value = nNum[i];
                        pValueSearch = pValueSearch->next;
                        i--;
                    }
                    printf("ok\n");
                }
                break;
            }
            pEntrySearch = pEntrySearch->next;
        }
    }
#endif
}

char * toUpper(char *str)
{
    int len = strlen(str);
    for(int i = 0;i < len;++ i)
        if(str[i] > 'Z') str[i] -= 'a' - 'A';

    return str;
}
int main(int argc, char *argv[])
{
    char Order[25][20] = { "BYE", "HELP", "LIST KEYS", "LIST ENTRIES", "LIST SNAPSHOTS", "GET"
        , "DEL", "PURGE", "SET", "PUSH", "APPEND", "PICK", "PLUCK", "POP", "DROP", "ROLLBACK",
            "CHECKOUT", "SNAPSHOT", "MIN", "MAX", "SUM", "LEN", "REV", "UNIQ", "SORT" };
    char InputOrder[50];
    char OneOrder[50];
    char OneKey[50];
    char OneValue[10];
    char OneIndex[10];
    int nIndex;
    int nNum[100];
    int nBit;
    int nEnd = 1;
    int InputId;
    int i;
    int j;
    int k;
    int q;
    int nCount;
    while (nEnd)
    {   printf("> ");
        for (j = 0; j < 50; j++)
        {
            InputOrder[j] = -52;
            OneOrder[j] = -52;
            OneKey[j] = -52;
        }
        for (j = 0; j < 10; j++)
        {
            OneValue[j] = -52;
            OneIndex[j] = -52;
        }
        fgets(InputOrder, 50, stdin);
        nBit = strlen(InputOrder);
        for (i = 0; i < 50; i++)
        {
            if (InputOrder[i] == '\n')
            {
                InputOrder[i] = '\0';
                break;
            }
        }
        for (i = 0; i < 50; i++)
        {
            if (InputOrder[i] != ' ')
            {
                OneOrder[i] = InputOrder[i];
            }
            else
            {
                OneOrder[i] = '\0';
                break;
            }
        }
        i++;
        for (k = i; k < 50; k++)
        {
            if (InputOrder[k] != ' ')
            {
                OneKey[k - i] = InputOrder[k];
            }
            else
            {
                OneKey[k - i] = '\0';
                break;
            }

        }
        for (j = 0; j < 25; j++)
        {
            if (strcmp(toUpper(OneOrder), Order[j]) == 0)
            {
                break;
            }
        }
        switch (j)
        {
            case 0:
                //printf("> ");
                Bye();
                nEnd = 0;
                break;
            case 1:
                Help();
                break;
            case 5:
                Get(OneKey, 50);
                break;
            case 6:
                Del(OneKey, 50);
                break;
            case 7:
                Purge(OneKey, 50);
                break;
            case 8:
                k++;
                nCount = 0;
                for (q = k; q < nBit; q++)
                {
                    if (InputOrder[q] != ' ')
                    {
                        OneValue[q - k] = InputOrder[q];
                        if (q == nBit - 1)
                        {
                            nNum[nCount] = atoi(OneValue);
                            nCount++;
                        }
                    }
                    else
                    {
                        nNum[nCount] = atoi(OneValue);//convert char 
                        for (i = 0; i < 10; i++)
                        {
                            OneValue[i] = '\0';
                        }
                        nCount++;
                        k = q + 1;
                    }
                }
                Set(OneKey, 50, nNum, 100, nCount);//one key to key,nNum to the input values, nCount to the size of nCount array 
                break;
            case 9:
                k++;
                nCount = 0;
                for (q = k; q < nBit; q++)
                {
                    if (InputOrder[q] != ' ')
                    {
                        OneValue[q - k] = InputOrder[q];
                        if (q == nBit - 1)
                        {
                            nNum[nCount] = atoi(OneValue);
                            nCount++;
                        }
                    }
                    else
                    {
                        nNum[nCount] = atoi(OneValue);
                        for (i = 0; i < 10; i++)
                        {
                            OneValue[i] = '\0';
                        }
                        nCount++;
                        k = q + 1;
                    }
                }
                Push(OneKey, 50, nNum, 100, nCount);
                break;
            case 10:
                k++;
                nCount = 0;
                for (q = k; q < nBit; q++)
                {
                    if (InputOrder[q] != ' ')
                    {
                        OneValue[q - k] = InputOrder[q];
                        if (q == nBit - 1)
                        {
                            nNum[nCount] = atoi(OneValue);
                            nCount++;
                        }
                    }
                    else
                    {
                        nNum[nCount] = atoi(OneValue);
                        for (i = 0; i < 10; i++)
                        {
                            OneValue[i] = '\0';
                        }
                        nCount++;
                        k = q + 1;
                    }
                }
                Append(OneKey, 50, nNum, 100, nCount);
                break;
            case 11:
                k++;
                for (int q = k; q < nBit; q++)
                {
                    if (InputOrder[q] != ' ')
                    {
                        OneIndex[q - k] = InputOrder[q];
                    }
                    else
                    {
                        OneIndex[q - k] = '\0';
                        break;
                    }
                }
                nIndex = atoi(OneIndex);
                Pick(OneKey, 50, nIndex);
                break;
            case 12:
                k++;
                for (int q = k; q < nBit; q++)
                {
                    if (InputOrder[q] != ' ')
                    {
                        OneIndex[q - k] = InputOrder[q];
                    }
                    else
                    {
                        OneIndex[q - k] = '\0';
                        break;
                    }
                }
                nIndex = atoi(OneIndex);
                Pluck(OneKey, 50, nIndex);
                break;
            case 13:
                Pop(OneKey, 50);
                break;
            case 14:
                InputId = atoi(OneKey);
                Drop(InputId);
                break;
            case 15:
                InputId = atoi(OneKey);
                Rollback(InputId);
                break;
            case 16:
                InputId = atoi(OneKey);
                Checkout(InputId);
                break;
            case 17:
                Snapshot();
                break;
            case 18:
                Min(OneKey, 50);
                break;
            case 19:
                Max(OneKey, 50);
                break;
            case 20:
                Sum(OneKey, 50);
                break;
            case 21:
                Len(OneKey, 50);
                break;
            case 22:
                Rev(OneKey, 50);
                break;
            case 23:
                Uniq(OneKey, 50);
                break;
            case 24:
                Sort(OneKey, 50);
                break;
            default:
                toUpper(OneKey);
                if (strcmp(OneKey, "KEYS") == 0)
                {
                    List_keys();
                }
                else
                {
                    if (strcmp(OneKey, "ENTRIES") == 0)
                    {
                        List_entries();
                    }
                    else
                    {
                        if (strcmp(OneKey, "SNAPSHOTS") == 0)
                        {
                            List_snapshots();
                        }
                        else
                        {
                            printf("Error Order\n");
                        }
                    }
                }
                break;
        }
    }

    return 0;
}


