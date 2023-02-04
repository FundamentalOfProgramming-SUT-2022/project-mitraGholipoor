#include<stdio.h>
#include<regex.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<error.h>
#include<dirent.h>



#define MAX_SIZE 1000

char here[1000];
char here_with_root[1000];
char file_dir[1000];
char file_name[200];


char * clipboard;
char * output;
char * arman;

int valid = 0;
long long last_size = 0;
long long last_count = 0;

//-------------type of error-----------------//
/*
-1:command is not valid
-2:file is already exist
-3:file path is not valid
-4:invalid directory or file name
-5:invalid position
-6:file is not exist
-7:number of pos is out of range
-8:number of line is out of range
-9:invalid size
-10:invalid pattern
-11:two file are empty
-12:invalid depth
-13:patten not match
-14:permission denied
*/




int make_path(char address[]){
    char * need = (char *) malloc(sizeof(char) * MAX_SIZE);
    char * add = (char *) malloc(sizeof(char) * MAX_SIZE);
    strcpy(add,here);
    strcpy(need,address);
    char *ptr = need;
    ptr += strlen(here);
    char *token = strtok(ptr,"/");
    valid = 0;
    while(1)
    {
        char save[200];
        strcpy(save,token);
        token = strtok(NULL,"/");
        if(token == NULL){
            char * ss_ptr = strchr(save,'.');
            if(ss_ptr == NULL){
                return -3;
            }
            break;
        }
        strcat(add,"/");
        strcat(add,save);
        mkdir(add,0777);
        //mkdir(add);
    }
    free(need);
    free(add);
    return 1;
}

int check_file_exist(char address[]){
    
    char * need = (char *) calloc(MAX_SIZE,sizeof(char));
    char * add = (char *) calloc(MAX_SIZE,sizeof(char));
    strcpy(add,here_with_root);
    strcpy(need,address);
    char *ptr = need;
    ptr += strlen(here_with_root);
    char *token = strtok(ptr,"/");
    valid = 0;
    while(1)
    {
        char save[200];
        strcpy(save,token);
        token = strtok(NULL,"/");
        if(token == NULL){
            
            strcat(add,"/");
            strcpy(file_dir,add);
            strcat(add,save);

            FILE * file;
            file = fopen(add, "r");
            if (file){
                fclose(file);
                strcpy(file_name,save);
                return 1;
            }else{
                free(need);
                free(add);
                return -6;
            }
        }

        strcat(add,"/");
        strcat(add,save);
        struct stat info;
        if( stat( add, &info ) != 0 ){
            free(need);
            free(add);
            return -3;
        }
    }
    free(need);
    free(add);
    return 1;
}

void get_undo_address(char address[] ,char undo_address[]){
    check_file_exist(address);
    char undo_file[200]=".undo_";
    strcat(undo_file,file_name);
    strcpy(undo_address,file_dir);
    strcat(undo_address,undo_file);
    return;
}

int do_undo(char address[]){
    char undo_address[1000];
    get_undo_address(address,undo_address);
    FILE * fp = fopen(undo_address,"w");
    FILE * file = fopen(address,"r");
    char ch;
    while (1) {

      ch = fgetc(file);
  
      if( feof(file) ) {
          break ;
      }
      fputc(ch,fp);
    }
    fclose(file);
    fclose(fp);
}

long long get_size_of_file(char address[]){
    FILE *fp;
    fp = fopen(address,"r");
    fseek(fp, 0L, SEEK_END);
    long long sz = ftell(fp);
    fclose(fp);
    return sz;
}

void int_to_str(long long a,char r[]){
    char str[20];
    
    if(a == -1){
        str[0] = '-';
        str[1] = '1';
        str[2] = '\0';
        strcpy(r,str);
        return;
    }
    if(a == 0){
        str[0] = '0';
        str[1] ='\0';
        strcpy(r,str);
        return;
    }
    long long i = 0;
    int q;
    while(a != 0){
        q = a%10;
        a/=10;
        str[i] = q + '0';
        i++;
    }
    
    for(int j= 0 ; j < i/2;j++){
        char temp = str[i - j - 1];
        str[i - j - 1] = str[j];
        str[j] = temp;
    }
    str[i] = '\0';
    strcpy(r,str);
    return;
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

char *str_replace_with_pos(char *orig, long long start,long long len_rep ,char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count = 1;    // number of replacements

    // sanity checks and initialization
    if (!orig)
        return NULL;
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);


    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    
    ins = orig + start;
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep; // move to next "end of rep"
    strcpy(tmp, orig);
    return result;
}

void make_here_root(){
    getcwd(here,1000);
    char * ptr = str_replace(here,"\\","/");
    strcpy(here,ptr);
    strcpy(here_with_root,ptr);
    strcat(here_with_root,"/root");
}

void set_clipboard(){
    clipboard = (char *) calloc(MAX_SIZE,sizeof(char));
    last_size = MAX_SIZE;
    output = (char *) calloc(MAX_SIZE,sizeof(char));
    arman = (char *) calloc(MAX_SIZE,sizeof(char));
    last_count = MAX_SIZE;
}

void handle_err(int err){
    switch (err)
    {
    case -1:
        printf("command is not valid\n");
        break;
    case -2:
        printf("file is already exist\n");
        break;
    case -3:
        printf("file path is not valid\n");
        break;
    case -4:
        printf("invalid directory or file name\n");
        break;
    case -5:
        printf("invalid position\n");
        break;
    case -6:
        printf("file does not exist\n");
        break;
    case -7:
        printf("number of pos is out of range\n");
        break;
    case -8:
        printf("number of line is out of range\n");
        break;
    case -9:
        printf("invalid size\n");
        break;
    case -10:
        printf("invalid pattern\n");
        break;
    case -11:
        printf("two file are empty\n");
        break;
    case -12:
        printf("invalid depth\n");
        break;
    case -13:
        printf("patten not match\n");
        break;
    case -14:
        printf("permission denied\n");
        break;
    default:
        break;
    }
}
