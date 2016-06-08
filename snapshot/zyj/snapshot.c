/**
 * comp2129 - assignment 2
 * <Youjun Zhao>
 * <yzha1172>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snapshot.h"
#define MAX_LINE_LENGTH 100
#define MAX_VALUE_NUMBER 100

snapshot * snapshot_head = NULL;
entry * entry_head = NULL;

//find a entry by key. many functions may use this 
entry *findEntryByKey(char *key){
    entry *curr_entry = entry_head;
    while(curr_entry){
        if(!strcmp(key, curr_entry->key)){
            return curr_entry;
        }
        curr_entry = curr_entry->next;
    }
    return NULL;
}

//support little case and big case. command is case insensitive
void case2Big(char *str){
    int len = strlen(str);
    while(len --){
        if(str[len] > 'Z') str[len] -= 'a' - 'A';
    }
}

//print help function
void help(){
    printf("%s\n",HELP);
}

//get a line from input. mainly used for multiple inputs
void getLine(char *line){
    char tmpc;
    int index = 0;
    do{
        scanf("%c", &tmpc);
        line[index ++] = tmpc;
    }while(tmpc != '\n');
}

//append a value to an entry
void appendValue(entry *entry, int val){
    entry->values[(entry->length) ++] = val;
}

//push a value to an entry
void pushValue(entry *entry, int val){
    memmove(&(entry->values[1]), entry->values, sizeof(int) * entry->length);
    entry->values[0] = val;
    (entry->length) ++;
}

//get a new entry, this new entry's value is same as the "from" one
entry *getNewEntry(entry *from){
    if(from == NULL) return NULL;
    entry *new_entry = (entry *)malloc(sizeof(entry));
    new_entry->values = (int *)malloc(sizeof(int) * MAX_VALUE_NUMBER);
    strcpy(new_entry->key, from->key);
    memcpy(new_entry->values, from->values, sizeof(int) * MAX_VALUE_NUMBER);
    new_entry->length = from->length;
    new_entry->next = NULL;
    return new_entry;
}

//copy all of the entries in a state
entry * copyEntry(entry *head){
    if(head == NULL) return NULL;
    entry *new_entry = getNewEntry(head);
    entry *curr = head->next;
    entry *prev = new_entry;
    entry *temp_new;
    //copy every entry one by one
    while(curr != NULL){
        temp_new = getNewEntry(curr);

        temp_new->next = NULL;
        prev->next = temp_new;

        prev = temp_new;
        curr = curr->next;
    }
    return new_entry;
}

//handle function of push command
void push()
{
    char line[MAX_LINE_LENGTH];
    getLine(line);
    char *str = strtok(line," ");
    char *key = str;

    entry *curr_entry = entry_head;
    while(curr_entry != NULL){
        if(!strcmp(curr_entry->key, key)){
            str = strtok(NULL, " ");
            while(str != NULL){
                int val = atoi(str);
                //push a value to curr_entry
                pushValue(curr_entry, val);

                str = strtok(NULL, " ");
            }
            printf("ok\n\n");
            return;
        }
        curr_entry = curr_entry->next;
    }
    printf("no such key\n\n");
}

//set or init a entry
void set()
{
    char line[MAX_LINE_LENGTH];
    getLine(line);
    char *str = strtok(line," ");
    char *key = str;

    entry * new_entry = (entry *)malloc(sizeof(entry));
    new_entry->values = (int *)malloc(sizeof(int) * MAX_VALUE_NUMBER);
    strcpy(new_entry->key, key);

    new_entry->length = 0;
    new_entry->next = entry_head;
    entry_head = new_entry;

    str = strtok(NULL, " ");
    while(str){
        int val = atoi(str);
        appendValue(new_entry, val);
        str = strtok(NULL, " ");
    }
    printf("ok\n\n");
    return ;
}

//append a value to an entry
void append()
{
    char line[MAX_LINE_LENGTH];
    getLine(line);
    char *str = strtok(line," ");
    char *key = str;

    entry *curr_entry = entry_head;
    while(curr_entry){
        if(!strcmp(curr_entry->key, key)){
            //try to get the entry by key.
            //can use findEntryByKey function
            str = strtok(NULL, " ");
            while(str){
                int val = atoi(str);
                appendValue(curr_entry, val);
                str = strtok(NULL, " ");
            }

            printf("ok\n\n");
            return;
        }
        curr_entry = curr_entry->next;
    }

    printf("no such key\n\n");
}

//pop out a value
//this operation is easy so this do not have a handle function
void pop(){
    char key[MAX_COMMAND];
    scanf("%s", key);
    entry * curr_entry = entry_head;
    while(curr_entry != NULL){
        if(!strcmp(key,curr_entry->key)){
            if(curr_entry->length){
                printf("%d\n\n", curr_entry->values[curr_entry->length - 1]);
                curr_entry->length --;
            }
            else{
                printf("nil\n\n");
            }
            return;
        }
        curr_entry = curr_entry->next;
    }
    printf("no such key\n\n");
}

//list all of the Values, called by functions who wants the value of an entry
void listValues(entry * curr_entry){
    int i;
    printf("[");
    for(i = 0; i < curr_entry->length;++ i){
        if(i) printf(" ");
        printf("%d", curr_entry->values[i]);
    }
    printf("]\n");
}

//get and output all keys of an entry
void get(){
    char key[MAX_COMMAND];
    scanf("%s", key);
    entry * curr_entry = entry_head;
    while(curr_entry != NULL){
        if(!strcmp(key,curr_entry->key)){
            listValues(curr_entry);
            printf("\n");
            return;
        }
        curr_entry = curr_entry->next;
    }
    printf("no such key\n\n");
}

//delete an entry
void del(){
    char key[MAX_COMMAND];
    scanf("%s", key);
    entry *curr_entry = entry_head;
    if(entry_head && !strcmp(key, entry_head->key)){
        entry_head = entry_head->next;
        curr_entry->next = NULL;
        free(curr_entry->values);
        free(curr_entry);
        printf("ok\n\n");
    }
    else{
        entry * prev_entry = NULL;
        while(curr_entry){
            if(!strcmp(key, curr_entry->key)){
                //this part can use findEntryByKey function
                prev_entry->next = curr_entry->next;
                curr_entry->next = NULL;
                free(curr_entry->values);
                free(curr_entry);
                printf("ok\n\n");
                return;
            }
            prev_entry = curr_entry;
            curr_entry = curr_entry->next;
        }
        printf("no such key\n\n");
    }
}

//handle all of the list functions
//the code here can be rewriten to a shorter one
void list()
{
    char key[MAX_COMMAND];
    scanf("%s", key);
    case2Big(key);
    if(!strcmp("KEYS", key)){
        if(!entry_head){
            printf("no keys\n\n");
        }
        else{
            entry * curr_entry = entry_head;
            while(curr_entry){
                printf("%s\n", curr_entry->key);
                curr_entry = curr_entry->next;
            }
            printf("\n");
        }
    }
    else if(!strcmp("ENTRIES", key)){
        if(!entry_head){
            printf("no entries\n\n");
        }
        else{
            entry * curr_entry = entry_head;
            while(curr_entry){
                printf("%s ", curr_entry->key);
                listValues(curr_entry);
                curr_entry = curr_entry->next;
            }
            printf("\n");
        }
    }
    else if(!strcmp("SNAPSHOTS", key)){
        if(!snapshot_head){
            printf("no snapshots\n\n");
        }
        else{
            snapshot *curr_snapshot = snapshot_head;
            while(curr_snapshot){
                printf("%d\n", curr_snapshot->id);
                curr_snapshot = curr_snapshot->next;
            }
            printf("\n");
        }
    }
}

//pick a value in a certain index
void pick()
{
    char key[MAX_COMMAND];
    scanf("%s",key);
    int index;
    scanf("%d", &index);

    entry *curr_entry = findEntryByKey(key);
    if(!curr_entry)
        printf("no such key\n\n");
    if(index < 1 || index > curr_entry->length){
        printf("index out of range\n\n");
        return ;
    }
    printf("%d\n\n", curr_entry->values[index - 1]);
    return ;
}

//pluck function
void pluck(){
    char key[MAX_COMMAND];
    scanf("%s",key);
    int index;
    scanf("%d", &index);

    entry *curr_entry = findEntryByKey(key);
    if(!curr_entry)
        printf("no such key\n\n");
    if(index < 1 || index > curr_entry->length){
        printf("index out of range\n\n");
        return ;
    }
    printf("%d\n\n", curr_entry->values[index - 1]);
    //a easy way to move a lot of numbers
    memmove(&curr_entry->values[index - 1], &curr_entry->values[index], sizeof(int) * (curr_entry->length - index));
    curr_entry->length --;
    return ;

}

//purge function. 
//can be rewriten to a shorter one
void purge(){//======================================================================
    char key[MAX_COMMAND];
    scanf("%s", key);
    //delete current state
    if(entry_head && !strcmp(key, entry_head->key)){
        entry * curr_entry = entry_head;
        entry_head = entry_head->next;
        curr_entry->next = NULL;
        free(curr_entry->values);
        free(curr_entry);
    }
    else{
        entry * curr_entry = entry_head;
        entry * prev_entry = NULL;
        while(curr_entry){
            if(!strcmp(key, curr_entry->key)){
                prev_entry->next = curr_entry->next;
                curr_entry->next = NULL;
                free(curr_entry->values);
                free(curr_entry);
                break;
            }

            prev_entry = curr_entry;
            curr_entry = curr_entry->next;
        }
    }

    // delete snapshots
    snapshot *curr_ss = snapshot_head;
    entry *curr_entry;
    while(curr_ss){
        if(curr_ss->entries && !strcmp(key, curr_ss->entries->key)){
            curr_entry = curr_ss->entries;
            curr_ss->entries = curr_ss->entries->next;
            curr_ss->entries->next = curr_ss->entries->next;
            curr_entry->next = NULL;
            free(curr_entry->values);
            free(curr_entry);
        }
        else{
            entry * prev_entry = curr_ss->entries;
            curr_entry = prev_entry->next;
            while(curr_entry){
                if(!strcmp(key, curr_entry->key)){
                    prev_entry->next = curr_entry->next;
                    curr_entry->next = NULL;
                    free(curr_entry->values);
                    free(curr_entry);
                    break;
                }

                prev_entry = curr_entry;
                curr_entry = curr_entry->next;
            }
        }

        curr_ss = curr_ss->next;
    }

    printf("ok\n\n");
}
//do take snapshot 
//mainly use copeEntry to get a copy and add this copy to snapshot list
void takeSnapshot()
{
    snapshot * new_ss = (snapshot *)malloc(sizeof(snapshot));
    new_ss->next = NULL;
    new_ss->entries = copyEntry(entry_head);// get copy

    if(!snapshot_head){
        new_ss->id = 1;
        snapshot_head = new_ss;
    }
    else{
        snapshot *curr_ss = snapshot_head;
        while(curr_ss->next){
            curr_ss = curr_ss->next;
        }

        curr_ss->next = new_ss;
        new_ss->id = curr_ss->id + 1;
    }

    printf("saved as snapshot %d\n\n",new_ss->id);
}


//function to free a state
//which means free all entries in a state
void freeEntryHead(entry * entryHead){
    entry *curr_entry = entryHead;
    entry *del_entry;
    while(curr_entry){
        del_entry = curr_entry;
        curr_entry = curr_entry->next;
        del_entry->next = NULL;
        free(del_entry->values);
        free(del_entry);
    }
}

//free a snapshot
void freeSnapshot(snapshot *ss){
    freeEntryHead(ss->entries);
    free(ss);
}

//delete a snapshot
void drop(){
    int id;
    scanf("%d", &id);
    snapshot *del_ss = snapshot_head;
    if(snapshot_head && snapshot_head->id == id){
        snapshot_head = del_ss->next;

        del_ss->next = NULL;
        freeSnapshot(del_ss);
        printf("ok\n\n");
        return;
    }
    else{
        snapshot *curr_snapshot = snapshot_head;
        snapshot *prev_snapshot = NULL;
        while(curr_snapshot){
            if(curr_snapshot->id == id){
                prev_snapshot->next = curr_snapshot->next;

                curr_snapshot->next = NULL;
                freeSnapshot(curr_snapshot);
                printf("ok\n\n");
                return;
            }
            prev_snapshot = curr_snapshot;
            curr_snapshot = curr_snapshot->next;
        }
        printf("no such snapshot\n\n");
    }
}

void rollback(){
    int id;
    scanf("%d", &id);
    snapshot *curr_ss = snapshot_head;
    while(curr_ss){
        if(curr_ss->id == id){
            freeEntryHead(entry_head);
            entry_head = copyEntry(curr_ss->entries); //copy the state from snapshot list
            //inface we can just use the memory from the snapshot list
            //do not need to copy here. but anyway we will delete it
            snapshot *del_ss = curr_ss;
            curr_ss = curr_ss->next;
            del_ss->next = NULL;
            //delete all of the snapshots after this one
            while(curr_ss){
                del_ss = curr_ss;
                curr_ss = curr_ss->next;
                del_ss->next = NULL;
                freeSnapshot(del_ss);
            }
            printf("ok\n\n");
            return;
        }
        curr_ss = curr_ss->next;
    }
    printf("no such snapshot\n\n");
}

//checkout a version of snapshot
void checkout(){
    int id;
    scanf("%d", &id);
    snapshot *curr_ss = snapshot_head;
    while(curr_ss){
        if(curr_ss->id == id){
            freeEntryHead(entry_head);
            entry_head = copyEntry(curr_ss->entries);
            printf("ok\n\n");
            return;
        }
        curr_ss = curr_ss->next;
    }

    printf("no such snapshot\n\n");
}

//min, max, sum, length share most of the codes
int getMMS(entry *en, int type){ //0 for min, 1 for max, 2 for sum, 3 for length
    if(type == 3) return en->length;
    int *start = en->values;
    int len = en->length, i;
    int res = start[0];
    for(i = 1;i < len;++ i){
        if(type == 0) res = res < start[i] ? res : start[i]; //min
        else if(type == 1) res = res > start[i] ? res : start[i]; //min
        else res += start[i];
    }
    return res;
}

//min, max, sum ,length function 
void MMSL(int type){ //0 for min, 1 for max, 2 for sum, 3 for length
    char key[MAX_COMMAND];
    scanf("%s", key);

    entry *curr_entry = entry_head;
    while(curr_entry){
        if(!strcmp(curr_entry->key, key)){
            if(type != 3 && curr_entry->length == 0){ //if it's not for length
                printf("nil\n\n");
            }
            else{
                printf("%d\n\n",getMMS(curr_entry, type));
            }

            return;
        }
        curr_entry = curr_entry->next;
    }
    printf("no such key\n\n");
}

//swap two numbers, used mainly for reverse
void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//reverse an array in an entry
void rev(){
    char key[MAX_COMMAND];
    int i,len;
    scanf("%s", key);

    entry *curr_entry = entry_head;

    while(curr_entry){
        if(strcmp(curr_entry->key, key) == 0){
            len = curr_entry->length;
            for(i = 0;i < (len >> 1);++ i)
                swap(curr_entry->values + i, curr_entry->values + len - i - 1);
            printf("ok\n\n");
            return;
        }
        curr_entry = curr_entry->next;
    }

    printf("no such key\n\n");
}

//used for qsort function
int cmp (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void sort()
{
    char key[MAX_COMMAND];
    scanf("%s", key);

    entry *curr_entry = entry_head;
    while(curr_entry){
        if(strcmp(curr_entry->key, key) == 0){
            qsort(curr_entry->values, curr_entry->length, sizeof(int), cmp);
            printf("ok\n\n");
            return;
        }
        curr_entry = curr_entry->next;
    }

    printf("no such key\n\n");
}

//uniq an array
//@argu: 
//  start: start address of this array
//  len: the length of this array
int uniqArray(int *start,int len){
    for(int i = 0;i < len - 1;++ i){
        if(start[i] == start[i + 1]){
            memmove(&start[i], &start[i + 1], sizeof(int) * (len - i));
            len --;
            i --;
        }
    }
    return len;
}

void uniq(){
    char key[MAX_COMMAND];
    scanf("%s", key);

    entry *curr_entry = findEntryByKey(key);
    if(!curr_entry){
        printf("no such key\n\n");
        return ;
    }
    int len = curr_entry->length;
    int *start = curr_entry->values;
    len = uniqArray(start, len);
    printf("ok\n\n");
    curr_entry->length = len;
}

//get difference of two arraries
//the res is the result array
int getDiff(int *res, int len_res, int *values, int len_val){
    if(!len_res) {
        memcpy(res, values, sizeof(int) * len_val);
        return len_val;
    }
    for(int i = 0;i < len_res;++ i){
        for(int j = 0;j < len_val;++ j){
            if(res[i] == values[j]){
                memmove(&res[i], &res[i + 1], sizeof(int) * (len_res - i));
                len_res --;
                i --;
                break;
            }
        }
    }
    return len_res;
}

//get Inter of two arries
int getInter(int *res, int len_res, int *values, int len_val){
    if(!len_res) {
        memcpy(res, values, sizeof(int) * len_val);
        return len_val;
    }
    int finalLen = 0;
    for(int i = 0;i < len_res;++ i){
        for(int j = 0;j < len_val;++ j){
            if(res[i] == values[j]){
                res[finalLen ++] = res[i];
                break;
            }
        }
    }
    return finalLen;
}

int getUnion(int *res, int len_res, int *values, int len_val){
    memcpy(&res[len_res], values, sizeof(int) * len_val);
    return len_val + len_res;
}

//handle function fo diff, inter and union
void diffInterUnion(int type){// don't know the format
    // 0 for diff, 1 for inter 2 for union
    char line[MAX_LINE_LENGTH];
    int res[MAX_VALUE_NUMBER];
    int length = 0;
    entry *curr_entry;

    getLine(line);
    char *str = strtok(line," ");

    while(str){
        curr_entry = findEntryByKey(str);
        if(curr_entry){
            //call different functions here
            if(type == 0) length = getDiff(res,length, curr_entry->values, curr_entry->length);
            else if(type == 1) length = getInter(res,length, curr_entry->values, curr_entry->length);
            else if(type == 2) length = getUnion(res,length, curr_entry->values, curr_entry->length);
        }
        str = strtok(NULL, " \n");
    }
    if(type == 2){
        //may be not necessary
        qsort(res, length, sizeof(int), cmp);
        length = uniqArray(res, length);
    }
    printf("[");
    for(int i = 0;i < length;++ i){
        if(i) printf(" ");
        printf("%d", res[i]);
    }
    printf("]\n\n");
    return ;
}

int main(){
    char command[MAX_LINE_LENGTH];
    while(1){
        printf("> ");
        scanf("%s", command);
        case2Big(command);
        if(strcmp("LIST", command) == 0)
            list();
        else if(strcmp("SET", command) == 0)
            set();
        else if(strcmp("GET", command) == 0)
            get();
        else if(strcmp("POP", command) == 0)
            pop();
        else if(strcmp("PUSH", command) == 0)
            push();
        else if(strcmp("APPEND", command) == 0)
            append();
        else if(strcmp("DEL", command) == 0)
            del();
        else if(strcmp("PICK", command) == 0)
            pick();
        else if(strcmp("PLUCK", command) == 0)
            pluck();
        else if(strcmp("PURGE", command) == 0)
            purge();
        else if(strcmp("SNAPSHOT", command) == 0)
            takeSnapshot();
        else if(strcmp("DROP", command) == 0)
            drop();
        else if(strcmp("ROLLBACK", command) == 0)
            rollback();
        else if(strcmp("CHECKOUT", command) == 0)
            checkout();
        else if(strcmp("MIN", command) == 0)
            MMSL(0);
        else if(strcmp("MAX", command) == 0)
            MMSL(1);
        else if(strcmp("SUM", command) == 0)
            MMSL(2);
        else if(strcmp("LEN", command) == 0)
            MMSL(3);
        else if(strcmp("REV", command) == 0)
            rev();
        else if(strcmp("UNIQ", command) == 0)
            uniq();
        else if(strcmp("SORT", command) == 0)
            sort();
        else if(strcmp("DIFF", command) == 0)
            diffInterUnion(0);
        else if(strcmp("INTER", command) == 0)
            diffInterUnion(1);
        else if(strcmp("UNION", command) == 0)
            diffInterUnion(2);
        else if(strcmp("HELP", command) == 0)
            help();
        else if(strcmp("BYE", command) == 0){
            printf("bye\n");
            break;
        }
    }
    return 0;
}
