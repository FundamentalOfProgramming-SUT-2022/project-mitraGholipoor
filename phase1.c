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

void  get_undo_address(char address[] ,char undo_address[]){
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

int get_address_with_qot(char str[],char * address){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--file") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }

    
    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, "\"");
    if(token == NULL){
        return -1;
    }
    strcpy(address,token);
    return 1;
    
}

int get_address_string(char str[],char * address){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--file") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    

    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, " ");
    
    if(token == NULL){
        return -1;
    }
    if(token[0] != '"'){
        strcpy(address,token);
        return 1;
    }
    return get_address_with_qot(str,address);
}

int compile_address(char str[],char address[]){
    address[0] = 0;
    char * str_str = (char *)calloc(MAX_SIZE,sizeof(char));
    int err =get_address_string(str,str_str);
    if(err == 1 && strlen(str_str) == 0){
        strcat(str," ");
        err =get_address_string(str,str_str);
    }
    if(err==1){
        int len = strlen(str_str);
        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","*");


        if(strncmp(add,"/root",5) == 0){
            strcat(address,here);
            strcat(address,add);
            return 1;
        }
        if(strncmp(add,"root/",5) == 0){
            strcat(address,here);
            strcat(address,"/");
            strcat(address,add);
            return 1;
        }
        if (add[0] != '/')
        {
            strcat(address,here_with_root);
            strcat(address,"/");
            strcat(address,add);
            return 1;
        }

        strcat(address,here_with_root);
        strcat(address,add);
        return 1;
        
    }else{
        return err;
    }
    
}

int create_file(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err == 1){
        err = check_file_exist(address);
        if(err == 1){
            return -2;
        }
        err = make_path(address);
        if(err == 1){
            FILE *fp;
            fp  = fopen(address, "w");
            fclose(fp);
            err = check_file_exist(address);
            if(!err && valid == 0){
                valid = 0;
                return -1;
            }
            return 1; 
        }
        return err;
    }
    return err;
}

int get_str_with_qot(char str[],char * str1){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--str") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }

    
    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, "\"");
    if(token == NULL){
        return -1;
    }
    strcpy(str1,token);
    return 1;
}

int get_str_string(char str[],char * str1){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--str") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    

    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, " ");
    if(token == NULL){
        return -1;
    }
    if(token[0] != '"'){
        strcpy(str1,token);
        return 1;
    }
    return get_str_with_qot(str,str1);
}

int compile_str(char str[],char str1[]){
    str1[0] = 0;
    char * str_str = (char *)calloc(MAX_SIZE,sizeof(char));
    int err = get_str_string(str,str_str);
    if(err==1){
        int len = strlen(str_str);

        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","*");

        strcat(str1,add);
        return 1;
        
    }else{
        return err;
    }
}

int get_pos(char str[],int * line , int * pos){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--pos") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return -1;
    }
    token = strtok(NULL," ");
    char * sepertor = strchr(token,':');
    if(token == NULL || sepertor == NULL){
        return -5;
    }

    char str_line[10];
    char str_pos[10];
    int dist = sepertor - token;
    strncpy(str_line,token,dist);
    token += dist+1;
    *line = atoi(str_line);
    *pos = atoi(token);
    return 1;
}

int insert_to_file(char address[],char str[],int line ,int pos){
    
    int check = 0;
    char proccess[1000];
    strcpy(proccess,file_dir);
    strcat(proccess,"proccess_44336.txt");
    FILE* file = fopen(address, "r");
    char ch;
    long long i = 1;
    long long j = 0;
    long long n = 0;
    long long start = 0;

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            if(n == 0 && line == 1 && pos == 0){
                fclose(file);
                do_undo(address);
                file = fopen(address, "w");
                fputs(str,file);
                fclose(file);
                return 1;
            }
            break ;
        }


        if(i == line){
            if( j == pos){
                check = 1;
                start = n;
                break;
            }
        }


        if(ch == '\n'){
            i++;
            j = 0;
            n++;
            continue;
        }
        
        n++;
        j++;
    }

    fclose(file);
    if(check == 0){
        return -5;
    }

    do_undo(address);

    file = fopen(address, "r");
    FILE* fp = fopen(proccess,"w");

    n = 0;
    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(n == start){
            fputs(str,fp);
        }
        fputc(ch,fp);
        n++;
    }

    fclose(file);
    fclose(fp);

    remove(address);
    rename(proccess,address);
    return 1;
}

int insert_str(char str[],int is_arman){
    char address[1000];
    char str1[1000];

    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");
    strcpy(str,r_str);

    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }

    if((err= check_file_exist(address)) != 1){
        return err;
    }


    int line=1,pos=0;
    err = get_pos(str,&line,&pos);
    if(err != 1){
        return err;
    }



    if(is_arman){
        err = insert_to_file(address,arman,line,pos);
        if(err != 1){
            return err;
        } 
    }else{
        err = compile_str(str,str1);
        if(err != 1){
            return err;
        }
        err = insert_to_file(address,str1,line,pos);
        if(err != 1){
            return err;
        } 
    }
    
    return 1;
}

int cat(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    
    if((err= check_file_exist(address)) != 1){
        return err;
    }

    if(file_name[0] == '.'){
        return -6;
    }

    long long count = get_size_of_file(address);

    if(count > last_count){
        output = realloc(output,sizeof(char) * count);
        last_count = count;
    }
    do_undo(address);
    char ch;
    FILE* file = fopen(address, "r");
    long long ss = 0;
    while (1) {
        ch = fgetc(file);
        if( feof(file) ) {
            break;
        }
        output[ss] = ch;
        ss++;
    }
    fclose(file);
    output[ss] = '\0';
    return 1;
}

int get_size(char str[],long long * size){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-size") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return -1;
    }
    token = strtok(NULL," ");
    if(token == NULL){
        return -9;
    }

    *size = atoll(token);
    if(*size == 0){
        return -1;
    }
    return 1;
}

int get_bf(char str[],char *which){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-b") == 0){
            is_cmd_valid = 1;
            *which = 'b';
            return 1;
        }
        if(strcmp(token,"-f") == 0){
            is_cmd_valid = 1;
            *which = 'f';
            return 1;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return -1;
    }
    return 1;
}

int remove_from_file(char address[],int line,int pos,long long size,char which){

    int check = 0;
    long long start,end;
    char proccess[1000];
    strcpy(proccess,file_dir);
    strcat(proccess,"proccess.txt");

    long long count  = get_size_of_file(address);

    FILE* file = fopen(address, "r");
    char ch;
    long long i = 1;
    long long j = 0;
    long long n = 0;
    long long pos_pos = 0;

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(i == line){
            if( j == pos){
                check = 1;
                pos_pos = n;
                break;
            }
        }


        if(ch == '\n'){
            i++;
            j = 0;
            n++;
            continue;
        }

        n++;
        j++;
    }
    
    fclose(file);
    
    if(check == 0){
        return -5;
    }

    if(which =='b'){
        start = pos_pos - size;
        end = pos_pos - 1;
    }else{
        start = pos_pos;
        end = pos_pos + size - 1;
    }

    if(start < 0){
        start = 0;
    }
    if(end >= count){
        end = n - 1;
    }

    do_undo(address);

    file = fopen(address, "r");
    FILE* fp = fopen(proccess,"w");

    n = 0;
    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(n < start || n > end){
            fputc(ch,fp);
        }

        n++;
    }

    fclose(file);
    fclose(fp);

    remove(address);
    rename(proccess,address);
    return 1;
}

int remove_str(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }

    if((err= check_file_exist(address)) != 1){
        return err;
    }

    int line,pos;
    long long size;
    char which;
    err = get_pos(str,&line,&pos);
    if(err != 1){
        return err;
    }
    err = get_size(str,&size);
    if(err != 1){
        return err;
    }
    err = get_bf(str,&which);
    if(err != 1){
        return err;
    }

    err = remove_from_file(address,line,pos,size,which);
    if(err != 1){
        return err;
    }
    return 1;
}

int copy_to_clipboard(char address[],int line, int pos, long long size, char which){
    int check = 0;
    long long start,end;
    long long count  = get_size_of_file(address);
    FILE* file = fopen(address, "r");
    char ch;
    long long i = 1;
    long long j = 0;
    long long n = 0;
    long long pos_pos = 0;

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(i == line){
            if( j == pos){
                check = 1;
                pos_pos = n;
                break;
            }
        }


        if(ch == '\n'){
            i++;
            j = 0;
            n++;
            continue;
        }

        n++;
        j++;
    }
    fclose(file);
    if(check == 0){
        return -5;
    }

    if(which =='b'){
        start = pos_pos - size;
        end = pos_pos - 1;
    }else{
        start = pos_pos;
        end = pos_pos + size - 1;
    }

    if(start < 0){
        start = 0;
    }
    if(end >= count){
        end = n - 1;
    }

    do_undo(address);
    file = fopen(address, "r");

    if(size > last_size){
        clipboard = realloc(clipboard,sizeof(char) * size);
        last_size = size;
    }

    n = 0;
    long long ss = 0;
    while (n <= end) {

        ch = fgetc(file);
        

        if( feof(file) ) {
            break;
        }
        
        if(start <=n && n <= end){
            clipboard[ss] = ch;
            ss++;
        }

        n++;
    }
    clipboard[ss]='\0';
    fclose(file);
    return 1;
}

int copy_str(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    
    if((err= check_file_exist(address)) != 1){
        return err;
    }

    int line,pos;
    long long size;
    char which;
    err = get_pos(str,&line,&pos);
    if(err != 1){
        return err;
    }
    err = get_size(str,&size);
    if(err != 1){
        return err;
    }
    err = get_bf(str,&which);
    if(err != 1){
        return err;
    }

    err = copy_to_clipboard(address,line,pos,size,which);
    if(err != 1){
        return err;
    }
    return 1;
}

int cut_to_clipboard(char address[],int line, int pos, long long size, char which){

    int check = 0;
    long long start,end;
    char proccess[1000];
    strcpy(proccess,file_dir);
    strcat(proccess,"proccess.txt");

    long long count  = get_size_of_file(address);

    FILE* file = fopen(address, "r");
    char ch;
    long long i = 1;
    long long j = 0;
    long long n = 0;
    long long pos_pos = 0;

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(i == line){
            if( j == pos){
                check = 1;
                pos_pos = n;
                break;
            }
        }


        if(ch == '\n'){
            i++;
            j = 0;
            n++;
            continue;
        }

        n++;
        j++;
    }
    
    fclose(file);
    
    if(check == 0){
        return -5;
    }
    
    if(which =='b'){
        start = pos_pos - size;
        end = pos_pos - 1;
    }else{
        start = pos_pos;
        end = pos_pos + size - 1;
    }

    if(start < 0){
        start = 0;
    }
    if(end >= count){
        end = n - 1;
    }

    do_undo(address);
    file = fopen(address, "r");
    FILE* fp = fopen(proccess,"w");

    if(size > last_size){
        clipboard = realloc(clipboard,sizeof(char) * size);
        last_size = size;
    }

    n = 0;
    long long ss =0;

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(n < start || n > end){
            fputc(ch,fp);
        }else{
            clipboard[ss] = ch;
            ss++;
        }

        n++;
    }

    fclose(file);
    fclose(fp);

    remove(address);
    rename(proccess,address);
    clipboard[ss]='\0';
    return 1;
}

int cut_str(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }

    if((err= check_file_exist(address)) != 1){
        return err;
    }

    int line,pos;
    long long size;
    char which;
    err = get_pos(str,&line,&pos);
    if(err != 1){
        return err;
    }
    err = get_size(str,&size);
    if(err != 1){
        return err;
    }
    err = get_bf(str,&which);
    if(err != 1){
        return err;
    }


    err = cut_to_clipboard(address,line,pos,size,which);
    if(err != 1){
        return err;
    }
    return 1;
}

int paste_str(char str[]){
    char address[1000];
    char str1[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exist(address) == 0){
        return -6;
    }
    int line=1,pos=0;
    err = get_pos(str,&line,&pos);
    if(err != 1){
        return err;
    }
    err = insert_to_file(address,clipboard,line,pos);
    if(err != 1){
        return err;
    }
    return 1;
}

int compile_str_find(char str[],char str1[]){
    str1[0] = 0;
    char * str_str = (char *)calloc(MAX_SIZE,sizeof(char));
    int err = get_str_string(str,str_str);
    if(err==1){
        int len = strlen(str_str);

        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","\\*");
        add = str_replace(add,"+astrick10450+","[^[:space:]]*");

        strcat(str1,add);
        return 1;
        
    }else{
        return err;
    }
}

int check_at(char str[],long long * at){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-at") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return 0;
    }
    token = strtok(NULL," ");
    if(token == NULL){
        return -1;
    }
    *at = atoi(token);
    if(*at == 0){
        return -1;
    }
    return 1;
}

int check_byword(char str[]){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-byword") == 0){
            is_cmd_valid = 1;
            return 1;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return 0;
    }
    return 1;
}

int check_all(char str[]){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-all") == 0){
            is_cmd_valid = 1;
            return 1;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return 0;
    }
    return 1;
}

int check_count(char str[]){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-count") == 0){
            is_cmd_valid = 1;
            return 1;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return 0;
    }
    return 1;
}

int check_regex(char address[], char pattern[],long long at,int has_all){
    long long cnt = get_size_of_file(address);
    char * str = (char *)malloc(sizeof(char) * cnt);
    long long i = 0;
    
    FILE* file = fopen(address, "r");
    char ch;

    while (1) {

      ch = fgetc(file);
  
      if( feof(file) ) {
          break ;
      }
      str[i] = ch;
      i++;
    }
    fclose(file);

    long long * results = (long long *) calloc(MAX_SIZE,sizeof(long long)) ;
    long long size = MAX_SIZE;


    char * string = str;
    long long where = 0;
    long long at_where = 1;


    do{
        regex_t    preg;
        int        rc;
        size_t     nmatch = 1;
        regmatch_t pmatch[1];


        if(at_where > size ){
            results = realloc(results,sizeof(long long) * at_where * 2);
            size = at_where * 2;
        }


        if(where >= cnt){
            results[at_where] = -1;
            break;
        }

        if (0 != (rc = regcomp(&preg, pattern, 0))) {
            free(str);
            return -10;
        }
        
        if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
            results[at_where] = -1;
            break;
        }
        else {
            if(pmatch[0].rm_so != -1){
                results[at_where] = pmatch[0].rm_so + where;
                string += pmatch[0].rm_eo;
                where += pmatch[0].rm_eo;
            }else{
                results[at_where] = -1;
                break;
            }
        }
        regfree(&preg);
    }while(at_where++ != at);
    at_where -=1;

    long long count = 0;
    if((has_all && at_where == 0) || (!has_all && !(results[at_where] != -1 && at_where != 0 && at_where == at))){
        strcpy(output,"-1");
    }else{
        char buffer[20];
        if(has_all){
            for(long long i = 1; i<=at_where; i++){
                if(i == 1){
                    int_to_str(results[i],buffer);
                    strcpy(output,buffer);
                    strcat(output," ");
                    count+= strlen(buffer) + 1;
                    continue;
                }
                int_to_str(results[i],buffer);
                if(count + strlen(buffer) > last_count){
                    output = realloc(output,(count + 20) * sizeof(char));
                }
                strcat(output,buffer);
                strcat(output," ");
                count+= strlen(buffer) + 1;
            }
        }else{
            int_to_str(results[at_where],buffer);
            strcpy(output,buffer);
        }
    }
    free(str);
    return 1;
}

int check_regex_count(char address[], char pattern[]){
    long long count = get_size_of_file(address);
    char * str = (char *)malloc(sizeof(char) * count);
    long long i = 0;
    
    FILE* file = fopen(address, "r");
    char ch;

    while (1) {

      ch = fgetc(file);
  
      if( feof(file) ) {
          break ;
      }
      str[i] = ch;
      i++;
    }
    fclose(file);

    char * string = str;
    long long where = 0;
    long long at_where = 1;
    do{
        regex_t    preg;
        int        rc;
        size_t     nmatch = 1;
        regmatch_t pmatch[1];

        if(where >= count){
            break;
        }

        if (0 != (rc = regcomp(&preg, pattern, 0))) {
            free(str);
            return -10;
        }
        
        if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
            break;
        }
        else {
            if(pmatch[0].rm_so != -1){
                string += pmatch[0].rm_eo;
                where += pmatch[0].rm_eo;
            }else{
                break;
            }
        }
        regfree(&preg);
    }while(at_where++ != 0);
    at_where -=1;
    
    char buffer[20];
    if(at_where != 0){
        int_to_str(at_where,buffer);
        strcpy(output,buffer);
    }else{
        strcpy(output,"0");
    }
    free(str);
    return 1;
}

int check_regex_byword(char address[], char pattern[],long long at,int has_all){
    long long cnt = get_size_of_file(address);
    char * str = (char *)malloc(sizeof(char) * cnt);
    long long i = 0;
    
    FILE* file = fopen(address, "r");
    char ch;

    while (1) {

      ch = fgetc(file);
  
      if( feof(file) ) {
          break ;
      }
      str[i] = ch;
      i++;
    }
    fclose(file);

    long long * results = (long long *) calloc(MAX_SIZE,sizeof(long long)) ;
    long long size = MAX_SIZE;


    char * string = str;
    long long where = 0;
    long long at_where = 1;


    do{
        regex_t    preg;
        int        rc;
        size_t     nmatch = 1;
        regmatch_t pmatch[1];


        if(at_where > size ){
            results = realloc(results,sizeof(long long) * at_where * 2);
            size = at_where * 2;
        }


        if(where >= cnt){
            results[at_where] = -1;
            break;
        }

        if (0 != (rc = regcomp(&preg, pattern, 0))) {
            free(str);
            return -10;
        }
        
        if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
            results[at_where] = -1;
            break;
        }
        else {
            if(pmatch[0].rm_so != -1){
                results[at_where] = pmatch[0].rm_so + where;
                string += pmatch[0].rm_eo;
                where += pmatch[0].rm_eo;
            }else{
                results[at_where] = -1;
                break;
            }
        }
        regfree(&preg);
    }while(at_where++ != at);
    at_where -=1;

    if((has_all && at_where == 0) || (!has_all && !(results[at_where] != -1 && at_where != 0 && at_where == at))){
        strcpy(output,"-1");
    }else{
        long long word = 1;
        long long j = 1;
        int f = 0;
        for(long long i = 0;i < cnt;i++){

            if(str[i] == ' ' && f == 0){
                word++;
                f =1;
            }else if(str[i] != ' '){
                f = 0;
            }

            if(i == results[j]){
                results[j] = word;
                j++;
            }

            if(j > at_where){
                break;
            }
        }

        char buffer[20];
        long long count = 0;
        if(has_all){
            for(long long i = 1; i<=at_where; i++){
                if(i == 1){
                    int_to_str(results[i],buffer);
                    strcpy(output,buffer);
                    strcat(output," ");
                    count += strlen(buffer) + 1;
                    continue;
                }

                if(count + strlen(buffer) > last_count){
                    output = realloc(output,(count + 20) * sizeof(char));
                }
                int_to_str(results[i],buffer);
                strcat(output,buffer);
                strcat(output," ");
                count+=strlen(buffer);
            }
        }else{
            int_to_str(results[at_where],buffer);
            strcpy(output,buffer);
        }
    }
    free(str);
    return 1;
}

int find_str(char str[],int is_arman){
    char address[1000];
    char str1[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");
    r_str = str_replace(r_str,"*","+astrick10450+");
    strcpy(str,r_str);

    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }

    if((err= check_file_exist(address)) != 1){
        return err;
    }

    if(!is_arman){
        err = compile_str_find(str,str1);
        if(err != 1){
            return err;
        } 
    }
    

    
    int is_at =0;
    long long at = 1;
    int is_all = 0;
    int is_count = 0;
    int is_byword = 0;
    is_at = check_at(str,&at);
    if(is_at == -1){
        return -1;
    }
    is_all = check_all(str);
    is_count = check_count(str);
    is_byword = check_byword(str);

    if(is_all && is_at){
        return -1;
    }

    if(is_count && is_at){
        return -1;
    }

    if(is_all && is_count){
        return -1;
    }
    if(is_all){
        at = 0;
    }

    if(is_arman){
        if(is_count){
            if((err= check_regex_count(address,arman)) != 1){
                return err;
            }
        }

        if(!is_byword && !is_count){
            if((err= check_regex(address,arman,at,is_all)) != 1){
                return err;
            }
        }
        if(is_byword && !is_count){
            if((err= check_regex_byword(address,arman,at,is_all)) != 1){
                return err;
            }
        }
    }else{
        if(is_count){
            if((err= check_regex_count(address,str1)) != 1){
                return err;
            }
        }

        if(!is_byword && !is_count){
            if((err= check_regex(address,str1,at,is_all)) != 1){
                return err;
            }
        }
        if(is_byword && !is_count){
            if((err= check_regex_byword(address,str1,at,is_all)) != 1){
                return err;
            }
        } 
    }
    
    do_undo(address);
    return 1;

}

int get_str1_with_qot(char str[],char * str1){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--str1") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }

    
    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, "\"");
    if(token == NULL){
        return -1;
    }
    strcpy(str1,token);
    return 1;
}

int get_str1_string(char str[],char * str1){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--str1") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    

    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, " ");
    if(token == NULL){
        return -1;
    }
    if(token[0] != '"'){
        strcpy(str1,token);
        return 1;
    }
    return get_str1_with_qot(str,str1);
}

int compile_str1(char str[],char str1[]){
    str1[0] = 0;
    char * str_str = (char *)calloc(MAX_SIZE,sizeof(char));
    int err = get_str1_string(str,str_str);
    if(err==1){
        int len = strlen(str_str);

        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","*");
        add = str_replace(add,"+astrick10450+","[^[:space:]]*");

        strcat(str1,add);
        return 1;
        
    }else{
        return err;
    }
}

int get_str2_with_qot(char str[],char * str1){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--str2") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }

    
    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, "\"");
    if(token == NULL){
        return -1;
    }
    strcpy(str1,token);
    return 1;
}

int get_str2_string(char str[],char * str1){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--str2") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    

    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, " ");
    if(token == NULL){
        return -1;
    }
    if(token[0] != '"'){
        strcpy(str1,token);
        return 1;
    }
    return get_str2_with_qot(str,str1);
}

int compile_str2(char str[],char str2[]){
    str2[0] = 0;
    char * str_str = (char *)calloc(MAX_SIZE,sizeof(char));
    int err = get_str2_string(str,str_str);
    if(err==1){
        int len = strlen(str_str);

        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","*");
        add = str_replace(add,"+astrick10450+","*");

        strcat(str2,add);
        return 1;
        
    }else{
        return err;
    }
}

int replace_regex(char address[], char pattern[],char str2[],long long at,int has_all){
    long long count = get_size_of_file(address);
    char * str = (char *)malloc(sizeof(char) * count * 2);
    long long i = 0;
    
    FILE* file = fopen(address, "r");
    char ch;

    while (1) {

      ch = fgetc(file);
  
      if( feof(file) ) {
          break ;
      }
      str[i] = ch;
      i++;
    }
    fclose(file);
    str[i] ='\0';

    long long  results [1000][2] = {0};
    char * string = str;
    long long where = 0;
    long long  at_where = 1;


    do{
        regex_t    preg;
        int        rc;
        size_t     nmatch = 1;
        regmatch_t pmatch[1];


        if(where >= count){
            results[at_where][0] = -1;
            break;
        }

        if (0 != (rc = regcomp(&preg, pattern, 0))) {
            free(str);
            return -10;
        }
        
        if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
            results[at_where][0] = -1;
            break;
        }
        else {
            if(pmatch[0].rm_so != -1){
                results[at_where][0] = pmatch[0].rm_so + where;
                results[at_where][1] = pmatch[0].rm_eo - pmatch[0].rm_so;
                string += pmatch[0].rm_eo;
                where += pmatch[0].rm_eo;
            }else{
                results[at_where][0] = -1;
                break;
            }
        }
        regfree(&preg);
    }while(at_where++ != at);
    at_where -=1;


    if((has_all && at_where == 0) || (!has_all && !(results[at_where][0] != -1 && at_where != 0 && at_where == at))){
        return -13;
    }else{
        long long diff = 0;
        int size = strlen(str2);
        char * r_str;
        if(has_all){
            for(i = 1; i<=at_where; i++){


                long long difff = count + diff + size - results[i][1];
                if( difff > count * 2){
                    str = realloc(str, sizeof(char) * difff * 2);
                    count  = diff * 2;
                }


                r_str = str_replace_with_pos(str,results[i][0] + diff,results[i][1],str2);
                diff += (size - results[i][1]);
                strcpy(str,r_str);
            }
        }else{

            long long difff = count + size - results[at_where][1];
            if( difff > count * 2){
                str = realloc(str, sizeof(char) * difff * 2);
                count  = diff;
            }

            r_str = str_replace_with_pos(str,results[at_where][0],results[at_where][1],str2);
            strcpy(str,r_str);
        }
    }
    do_undo(address);
    file = fopen(address, "w");
    fputs(str,file);
    fclose(file);
    free(str);
    return 1;
}

int replace_str(char str[]){
    char address[1000];
    char str1[1000];
    char str2[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");
    r_str = str_replace(r_str,"*","+astrick10450+");
    strcpy(str,r_str);

    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }

    if((err= check_file_exist(address)) != 1){
        return err;
    }
    
    err = compile_str1(str,str1);
    if(err != 1){
        return err;
    }
    
    err = compile_str2(str,str2);
    if(err != 1){
        return err;
    }

    int is_at =0;
    long long at = 1;
    int is_all = 0;

    is_at = check_at(str,&at);
    if(is_at == -1){
        return -1;
    }
    is_all = check_all(str);

    if(is_all && is_at){
        return -1;
    }
    if(is_all){
        at = 0;
    }
    err = replace_regex(address,str1,str2,at,is_all);
    if(err != 1){
        return err;
    }
    return 1;
}

int undo(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    
    if((err= check_file_exist(address)) != 1){
        return err;
    }

    char undo_address[1000];
    get_undo_address(address,undo_address);
    err = check_file_exist(undo_address);
    if(err == 1){
        remove(address);
        rename(undo_address,address);
    }
    return 1;
}

int get_str_all_files(char str[],char files[300][200],int * n){
    char * a_files = strstr(str,"--files ");
    if(a_files == NULL || strlen(a_files) < 9){
        return -1;
    }
    a_files += 8;
    char * start = strstr(a_files,"[");
    if(start == NULL || strlen(start) < 2){
        return -1;
    }
    start++;
    char * end  = strchr(start,']');
    if(end == NULL){
        return -1;
    }
    int length = end - start;
    start[length] = '\0';
    char all_files_str[strlen(str)];
    strcpy(all_files_str,start);


    char * token = strtok(all_files_str,",");
    
    while(token != NULL){
        strcpy(files[*n],token);
        token = strtok(NULL,",");
        *n += 1;
    }
    
    if(*n < 1){
        return -1;
    }

    for(int i = 0;i < *n; i++){
        char * has_qot = strchr(files[i],'"');
        
        if(has_qot == NULL){
            char * token = strtok(files[i]," ");
            if(token == NULL){
                return -1;
            }
            strcpy(files[i],token);
        }else{
            
            char * token = strtok(has_qot,"\"");
            if(token == NULL){
                return -1;
            }
            strcpy(files[i],token);
        }
        
    }

    return 1;
}

int compile_files(char files[300][200],int count){
    char address[200];
    int err;
    for(int i= 0; i < count;i++){
        address[0] = '\0';
        int len = strlen(files[i]);
        
        char * add = str_replace(files[i],"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","*");
        
        if(strncmp(add,"/root",5) == 0){
            strcat(address,here);
            strcat(address,add);
        }else if(strncmp(add,"root/",5) == 0){
            strcat(address,here);
            strcat(address,"/");
            strcat(address,add);
        }else if (add[0] != '/'){
            strcat(address,here_with_root);
            strcat(address,"/");
            strcat(address,add);
        }else{
            strcat(address,here_with_root);
            strcat(address,add);
        }
        
        if((err= check_file_exist(address)) != 1){
            return err;
        }
        
        strcpy(files[i],address);
    }
    return 1;
}

int check_l(char str[]){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-l") == 0){
            is_cmd_valid = 1;
            return 1;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return 0;
    }
    return 1;
}

int check_c(char str[]){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"-c") == 0){
            is_cmd_valid = 1;
            return 1;
        }
        token = strtok(NULL, " ");
    }
    
    if(!is_cmd_valid){
        return 0;
    }
    return 1;
}

int do_grep(char files[300][200],int count, char str[]){
    output[0] ='\0';
    long long size_output = 0;
    for(int i = 0; i < count; i++){
        char * address = files[i];
        long long size = get_size_of_file(address);
  
        check_file_exist(address);
        
        FILE * fp = fopen(address,"r");
        char * s = (char *)malloc(sizeof(char) * size);
        while(fgets(s,size,fp)){
            
            char * is_true = strstr(s,str);
            if(is_true != NULL){
                
                size_output += strlen(file_name) + strlen(s) + 2;
                
                if(size_output > last_count){
                    
                    output = realloc(output, sizeof(char) * size_output);
                    last_count = size_output;
                }
                
                strcat(output,file_name);
                strcat(output,":");
                strcat(output,s);
                if(s[strlen(s) - 1] != '\n'){
                    strcat(output,"\n");
                }
            }
        }
        fclose(fp);
        do_undo(address);
    }
    return 1;
}

int do_grep_for_c(char files[300][200],int count, char str[]){
    output[0] ='\0';
    long long n = 0;
    for(int i = 0; i < count; i++){
        char * address = files[i];
        long long size = get_size_of_file(address);
        check_file_exist(address);
        FILE * fp = fopen(address,"r");
        char * s = (char *)malloc(sizeof(char) * size);
        while(fgets(s,size,fp)){
            char * is_true = strstr(s,str);
            if(is_true != NULL){
                n++;
            }
        }
        fclose(fp);
        do_undo(address);
    }
    char buffer[20];
    int_to_str(n,buffer);
    strcpy(output,buffer);
    strcat(output,"\n");
    return 1;
} 

int do_grep_for_l(char files[300][200],int count, char str[]){
    output[0] ='\0';
    long long size_output = 0;
    for(int i = 0; i < count; i++){
        char * address = files[i];
        long long size = get_size_of_file(address);
  
        check_file_exist(address);
        
        FILE * fp = fopen(address,"r");
        char * s = (char *)malloc(sizeof(char) * size);
        while(fgets(s,size,fp)){
            
            char * is_true = strstr(s,str);
            if(is_true != NULL){
                
                size_output += strlen(file_name) + 1;
                
                if(size_output > last_count){
                    output = realloc(output, sizeof(char) * size_output);
                    last_count = size_output;
                }
                
                strcat(output,file_name);
                strcat(output,"\n");
                break;
            }
        }
        fclose(fp);
        do_undo(address);
    }
    return 1;
}

int grep(char str[],int is_arman){
    char address[1000];
    char str1[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");
    r_str = str_replace(r_str,"*","+astrick10450+");
    strcpy(str,r_str);

    if(is_arman){
         int err = compile_str(str,arman);
        if(err != 1){
            return err;
        }

        char files[300][200] = {0};
        int count = 0;
        err = get_str_all_files(str,files,&count);    
        if(err != 1){
            return err;
        }

        err = compile_files(files,count);    
        if(err != 1){
            return err;
        }

        int is_l = 0,is_c = 0;
        is_l = check_l(str);
        is_c = check_c(str);
        if(is_l && is_c){
            return -1;
        }
        if(!is_l && !is_c){
            err = do_grep(files,count,arman);
            if(err != 1){
                return err;
            }
        }
        if(is_l){
            err = do_grep_for_l(files,count,arman);
            if(err != 1){
                return err;
            }
        }
        if(is_c){
            err = do_grep_for_c(files,count,arman);
            if(err != 1){
                return err;
            }
        }
    }else{
        int err = compile_str(str,str1);
        if(err != 1){
            return err;
        }

        char files[300][200] = {0};
        int count = 0;
        err = get_str_all_files(str,files,&count);    
        if(err != 1){
            return err;
        }

        err = compile_files(files,count);    
        if(err != 1){
            return err;
        }

        int is_l = 0,is_c = 0;
        is_l = check_l(str);
        is_c = check_c(str);
        if(is_l && is_c){
            return -1;
        }
        if(!is_l && !is_c){
            err = do_grep(files,count,str1);
            if(err != 1){
                return err;
            }
        }
        if(is_l){
            err = do_grep_for_l(files,count,str1);
            if(err != 1){
                return err;
            }
        }
        if(is_c){
            err = do_grep_for_c(files,count,str1);
            if(err != 1){
                return err;
            }
        } 
    }
    return 1;
}

int get_address1_with_qot(char str[],char * address){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--file1") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }

    
    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, "\"");
    if(token == NULL){
        return -1;
    }
    strcpy(address,token);
    return 1;
    
}

int get_address1_string(char str[],char * address){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--file1") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    

    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, " ");
    
    if(token == NULL){
        return -1;
    }
    if(token[0] != '"'){
        strcpy(address,token);
        return 1;
    }
    return get_address1_with_qot(str,address);
}

int compile_address1(char str[],char address[]){
    address[0] = 0;
    char * str_str = (char *)calloc(MAX_SIZE,sizeof(char));
    int err =get_address1_string(str,str_str);
    if(err == 1 && strlen(str_str) == 0){
        strcat(str," ");
        err =get_address_string(str,str_str);
    }
    if(err==1){
        int len = strlen(str_str);
        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","*");


        if(strncmp(add,"/root",5) == 0){
            strcat(address,here);
            strcat(address,add);
            return 1;
        }
        if(strncmp(add,"root/",5) == 0){
            strcat(address,here);
            strcat(address,"/");
            strcat(address,add);
            return 1;
        }
        if (add[0] != '/')
        {
            strcat(address,here_with_root);
            strcat(address,"/");
            strcat(address,add);
            return 1;
        }

        strcat(address,here_with_root);
        strcat(address,add);
        return 1;
        
    }else{
        return err;
    }
    
}

int get_address2_with_qot(char str[],char * address){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--file2") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }

    
    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, "\"");
    if(token == NULL){
        return -1;
    }
    strcpy(address,token);
    return 1;
    
}

int get_address2_string(char str[],char * address){
    int len  = strlen(str);
    int is_cmd_valid = 0;
    char command[len];
    strcpy(command,str);

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if(strcmp(token,"--file2") == 0){
            is_cmd_valid = 1;
            break;
        }
        token = strtok(NULL, " ");
    }
    

    if(!is_cmd_valid){
        return -1;
    }

    token = strtok(NULL, " ");
    
    if(token == NULL){
        return -1;
    }
    if(token[0] != '"'){
        strcpy(address,token);
        return 1;
    }
    return get_address2_with_qot(str,address);
}

int compile_address2(char str[],char address[]){
    address[0] = 0;
    char * str_str = (char *)calloc(MAX_SIZE,sizeof(char));
    int err =get_address2_string(str,str_str);
    if(err == 1 && strlen(str_str) == 0){
        strcat(str," ");
        err =get_address_string(str,str_str);
    }
    if(err==1){
        int len = strlen(str_str);
        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\n");
        add = str_replace(add,"+star10449+","*");


        if(strncmp(add,"/root",5) == 0){
            strcat(address,here);
            strcat(address,add);
            return 1;
        }
        if(strncmp(add,"root/",5) == 0){
            strcat(address,here);
            strcat(address,"/");
            strcat(address,add);
            return 1;
        }
        if (add[0] != '/')
        {
            strcat(address,here_with_root);
            strcat(address,"/");
            strcat(address,add);
            return 1;
        }

        strcat(address,here_with_root);
        strcat(address,add);
        return 1;
        
    }else{
        return err;
    }
    
}

long long get_count_of_line(char address[]){

    char ch;
    long long line = 1;

    FILE* file = fopen(address, "r");
    
    while (1) {

      ch = fgetc(file);
  
      if( feof(file) ) {
          break ;
      }
      if(ch == '\n'){
        line++;
      }
    }
    fclose(file);
    return line;
}

int compare(char address[],char address1[]){
    long long count = get_size_of_file(address);
    long long count1 = get_size_of_file(address1);
    long long cnt = get_count_of_line(address);
    long long cnt1 = get_count_of_line(address1);

    char * str = (char *) malloc(count * sizeof(char));
    char *str1 = (char *) malloc(count1 * sizeof(char));

    output[0] = '\0';

    FILE * fp = fopen(address,"r");
    FILE * fp1 = fopen(address1,"r");

    int finish = 0;
    int finish1 = 0;
    long long line = 1;
    long long start,end;
    long long size = 0;
    char buffer[20];
    char buffer1[20];
    if(cnt > cnt1 && count1){
        start = cnt1 + 1;
        end = cnt;
        for( line = 1;line <= cnt1;line++){
            fgets(str,count,fp);
            fgets(str1,count,fp1);
            if(strcmp(str,str1) != 0){
                int_to_str(line,buffer);
                size += strlen(buffer) + strlen(str) + strlen(str1) + 30;
                if(size > last_count){
                    output = realloc(output,sizeof(char) * size);
                    last_count = size;
                }
                strcat(output,"============ #");
                strcat(output,buffer);
                strcat(output," ============\n");
                strcat(output,str);
                if(str[strlen(str)-1] !='\n'){
                    strcat(output,"\n");
                }
                strcat(output,str1);
                if(str1[strlen(str1)-1] !='\n'){
                    strcat(output,"\n");
                }
            }
        }

        int_to_str(start,buffer);
        int_to_str(end,buffer1);
        size += strlen(buffer)+ strlen(buffer1) + 46;
        if(size > last_count){
            output = realloc(output,sizeof(char) * size);
            last_count = size;
        }
        strcat(output,">>>>>>>>>>>> #");
        strcat(output,buffer);
        strcat(output," - #");
        strcat(output,buffer1);
        strcat(output," >>>>>>>>>>>>\n");

        for(line = start;line <= end;line++){
            fgets(str,count,fp);
            size += strlen(str);
            if(size > last_count){
                output = realloc(output,sizeof(char) * size);
                last_count = size;
            }
            strcat(output,str);
            if(str[strlen(str)-1] !='\n'){
                strcat(output,"\n");
            }
        }

    }else if(cnt > cnt1){
        start = 1;
        end = cnt;
        int_to_str(start,buffer);
        int_to_str(end,buffer1);
        size += strlen(buffer)+ strlen(buffer1) + 46;
        if(size > last_count){
            output = realloc(output,sizeof(char) * size);
            last_count = size;
        }
        strcat(output,">>>>>>>>>>>> #");
        strcat(output,buffer);
        strcat(output," - #");
        strcat(output,buffer1);
        strcat(output," >>>>>>>>>>>>\n");

        for(line = start;line <= end;line++){
            fgets(str,count,fp);
            size += strlen(str);
            if(size > last_count){
                output = realloc(output,sizeof(char) * size);
                last_count = size;
            }
            strcat(output,str);
            if(str[strlen(str)-1] !='\n'){
                strcat(output,"\n");
            }
        }

    }else if(cnt1 > cnt && count){
        start = cnt + 1;
        end = cnt1;
        for( line = 1;line <= cnt;line++){
            fgets(str,count,fp);
            fgets(str1,count1,fp1);
            if(strcmp(str,str1) != 0){
                int_to_str(line,buffer);
                size += strlen(buffer) + strlen(str) + strlen(str1) + 30;
                if(size > last_count){
                    output = realloc(output,sizeof(char) * size);
                    last_count = size;
                }
                strcat(output,"============ #");
                strcat(output,buffer);
                strcat(output," ============\n");
                strcat(output,str);
                if(str[strlen(str)-1] !='\n'){
                    strcat(output,"\n");
                }
                strcat(output,str1);
                if(str1[strlen(str1)-1] !='\n'){
                    strcat(output,"\n");
                }
            }
            

        }

        int_to_str(start,buffer);
        int_to_str(end,buffer1);
        size += strlen(buffer)+ strlen(buffer1) + 46;
        if(size > last_count){
            output = realloc(output,sizeof(char) * size);
            last_count = size;
        }
        strcat(output,">>>>>>>>>>>> #");
        strcat(output,buffer);
        strcat(output," - #");
        strcat(output,buffer1);
        strcat(output," >>>>>>>>>>>>\n");

        for(line = start;line <= end;line++){
            fgets(str1,count1,fp1);
            size += strlen(str1);
            if(size > last_count){
                output = realloc(output,sizeof(char) * size);
                last_count = size;
            }
            strcat(output,str1);
            if(str1[strlen(str1)-1] !='\n'){
                strcat(output,"\n");
            }
        }
    }else if(cnt1 > cnt){
        start = 1;
        end = cnt1;
        int_to_str(start,buffer);
        int_to_str(end,buffer1);
        size += strlen(buffer)+ strlen(buffer1) + 46;
        if(size > last_count){
            output = realloc(output,sizeof(char) * size);
            last_count = size;
        }
        strcat(output,">>>>>>>>>>>> #");
        strcat(output,buffer);
        strcat(output," - #");
        strcat(output,buffer1);
        strcat(output," >>>>>>>>>>>>\n");

        for(line = start;line <= end;line++){
            fgets(str1,count1,fp1);
            size += strlen(str1);
            if(size > last_count){
                output = realloc(output,sizeof(char) * size);
                last_count = size;
            }
            strcat(output,str1);
            if(str1[strlen(str1)-1] !='\n'){
                strcat(output,"\n");
            }
        }
    }else if(cnt1 == cnt && count && count1){
        for( line = 1;line <= cnt1;line++){
            fgets(str,count,fp);
            fgets(str1,count1,fp1);
            if(strcmp(str,str1) != 0){
                int_to_str(line,buffer);
                size += strlen(buffer) + strlen(str) + strlen(str1) + 30;
                if(size > last_count){
                    output = realloc(output,sizeof(char) * size);
                    last_count = size;
                }
                strcat(output,"============ #");
                strcat(output,buffer);
                strcat(output," ============\n");
                strcat(output,str);
                if(str[strlen(str)-1] !='\n'){
                    strcat(output,"\n");
                }
                strcat(output,str1);
                if(str1[strlen(str1)-1] !='\n'){
                    strcat(output,"\n");
                }
            }
        }
    }else{
        return -11;
    }
    return 1;
}

int compare_files(char str[]){
    char address[1000];
    char address1[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address1(str,address);
    if(err != 1){
        return err;
    }
    
    if((err= check_file_exist(address)) != 1){
        return err;
    }

    err = compile_address2(str,address1);
    if(err != 1){
        return err;
    }

    if((err= check_file_exist(address1)) != 1){
        return err;
    }
    if((err= compare(address,address1)) != 1){
        return err;
    }

    do_undo(address);
    do_undo(address1);
    return 1;
}

int make_branch(int size){

    int count = strlen(output) + 2 * size + 2;

    if(count > last_count){
        output = realloc(output,sizeof(char) * count);
        last_count = count;
    }

    for(int i = 0;i < size -1;i++){
        strcat(output," ");
    }
    strcat(output,"|\n");
    for(int i = 0;i < size -1;i++){
        strcat(output," ");
    }
    strcat(output,"|_");
}

int edit_branch(int line, int end_line, int size){
    long long count = strlen(output);
    int l = 1;
    int pos = 1;
    for(int i = 0; i < count;i++){
        if(output[i] == '\n'){
            l++;
            pos = 1;
            continue;
        }
        if(pos == size && line < l && l <= end_line){
            output[i] = '|';
        }
        if(pos == size && l >= end_line){
            return 1;
        }
        pos++;
    }
    return 1;
}

int make_tree(int depth,int line,int size,char * p_path,int tree){
    
    int end_line = line;
    char path[200];
    
    if(tree == depth){
        return line;
    }
    
    if(depth == 0){
        size = strlen("root:");
        strcat(output,"root:\n");
        
        struct dirent *files;
        DIR *dir = opendir("./root");
        
        
        while ((files = readdir(dir)) != NULL){
            char * name = files->d_name;
            
            if(name[0] != '.'){
                char path_of_dir[200];
                strcpy(path_of_dir,"./root/");
                strcat(path_of_dir, name);
                make_branch(size);
                

                struct stat buf;
                stat(path_of_dir, &buf);

                strcat(output,name);
                if(S_ISDIR(buf.st_mode)) {
                    strcat(output, ":");
                }
                strcat(output,"\n");

                if(end_line != line){
                    edit_branch(line,end_line,size);
                    line = end_line;
                }

                line +=2;

                if(S_ISDIR(buf.st_mode)) {
                    end_line = make_tree(depth + 1,line,size + strlen(name) + 1,path_of_dir,tree);
                }else{
                    end_line+=2;
                }
            }

        }
        closedir(dir);

        return 1;
    }else{
        strcpy(path,p_path);
        
        struct dirent *files;
        DIR *dir = opendir(path);
        
        while ((files = readdir(dir)) != NULL){
            char * name = files->d_name;
            if(name[0] != '.'){

                char path_of_dir[200];
                strcpy(path_of_dir,path);
                strcat(path_of_dir,"/");
                strcat(path_of_dir, name);

                make_branch(size);

                struct stat buf;
                stat(path_of_dir, &buf);
                strcat(output,name);
                
                

                if(S_ISDIR(buf.st_mode)) {
                    strcat(output, ":");
                }
                strcat(output,"\n");
                if(end_line != line){
                    edit_branch(line,end_line,size);
                    line = end_line;
                }

                line +=2;
                
                if(S_ISDIR(buf.st_mode)) {
                    end_line = make_tree(depth + 1,line,size + strlen(name) + 1,path_of_dir,tree);
                }else{
                    end_line+=2;
                }
            }

        }
        closedir(dir);
        return end_line;
    }


    
}

int tree(char str[]){

    char * token = strtok(str," ");
    token = strtok(NULL," ");
    if(token == NULL){
        return -1;
    }
    int depth = atoi(token);
    if(depth == 0 && token[0] != '0'){
        return -1;
    }
    if(depth < -1){
        return -12;
    }
    if(depth == 0){
        strcpy(output,"root:\n\0");
        return 1;
    }
    char p_path[200] ="\0";
    output[0] = '\0';
    make_tree(0,1,0,p_path,depth);
    return 1;
}

char * insertString( char s1[], const char s2[], long long pos ){
    long long n1 = strlen( s1 );
    long long n2 = strlen( s2 );


    if ( n1 < pos ) pos = n1;

    for (long long i = 0; i < n1 - pos; i++ )
    {
        s1[n1 + n2 - i - 1] = s1[n1 - i - 1];
    }

    for ( size_t i = 0; i < n2; i++)
    {
        s1[pos+i] = s2[i];
    }

    s1[n1 + n2] = '\0';

    return s1;
}

int do_auto_indenting(char address[]){
    int tab = 0;
    long long size = get_size_of_file(address);
    char * eee = (char *)malloc(sizeof(char) * size * 10);
    long long i = 0,j;
    int add_s = 1;
    char ch,before = 0;
    FILE * fp = fopen(address,"r");
    while (1) {

      ch = fgetc(fp);
      if( feof(fp) ) {
          break;
      }


      if(ch == '{'){

        j = i - 1;
        add_s =0;

        while(j >= 0 && eee[j] == ' '){
            eee[j] = 0;
            j--;
        }

        if(i != 0 && eee[j] == '\n'){
            i = j;
            if(tab > 0){
                for(int k = 0; k < tab;k++){
                    eee[++i] = ' ';
                    eee[++i] = ' ';
                    eee[++i] = ' ';
                    eee[++i] = ' ';
                }
            }
        }else if(i != 0){
            eee[j + 1] = ' ';
            i = j + 1;
        }else{
            i = -1;
        }
        eee[++i] = '{';
        eee[++i]='\n';
        tab++;
        if(tab > 0){
            for(int k = 0; k < tab;k++){
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
            }
        }
        i++;
      }else if(ch == '}'){

        if(i != 0 && before != '{' && before != '}'){
           eee[i] = '\n';
        }else{
            j = i - 1;
            while(j >= 0 && eee[j] == ' '){
                eee[j] = 0;
                j--;
            }
            if(i != 0 && eee[j] == '\n'){
                i = j;
            } 
        }
        
        tab--;
        if(tab > 0){
            for(int k = 0; k < tab;k++){
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
            }
        }

        add_s = 0;
        eee[++i]='}';
        eee[++i]='\n';
        if(tab > 0){
            for(int k = 0; k < tab;k++){
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
            }
        }
        i++;

      }else{
        if(add_s){
           eee[i] = ch;
           i++; 
        }else if(ch != ' '){
            add_s = 1;
            eee[i] = ch;
            i++;
        }
        
      }


      if(ch != ' '){
        before = ch;
      }
      eee[i] = '\0';

      if(i > size){
        eee =realloc(eee,sizeof(char) * i * 10);
        size = i * 5;
      }
    }
    fclose(fp);
    do_undo(address);
    fp = fopen(address,"w");
    fputs(eee,fp);
    fclose(fp);
    return 1;
}

int auto_indent(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    
    if((err= check_file_exist(address)) != 1){
        return err;
    }
    if((err= do_auto_indenting(address)) != 1){
        return err;
    }
    return 1;
}

int handle_arman(char str[]){
    char copy[strlen(str)];
    strcpy(copy,str);
    char * cmd1 = strtok(copy,"=D");
    char * cmd2 = strstr(str," =D ");
    cmd2 += 4;

    if(cmd1 == NULL || cmd2 ==NULL){
        return -1;
    }

    int a;
    char copy1[(a = strlen(cmd1))?a:10];
    char copy2[(a = strlen(cmd2))?a:10];
    strcpy(copy1,cmd1);
    strcpy(copy2,cmd2);
    char * token1 = strtok(copy1," ");
    char * token2 = strtok(copy2," ");

 

    if(strcmp(token1 ,"tree") != 0 && strcmp(token1 ,"cat") != 0 && strcmp(token1 ,"grep") != 0 && strcmp(token1 ,"find") != 0){
        return -1;
    }
    if(strcmp(token2 ,"insertstr") != 0 && strcmp(token2 ,"grep") != 0 && strcmp(token2 ,"find") != 0){
        return -1;
    }

    



    if(strcmp(token1,"tree") == 0){
        int err = tree(cmd1);
        if (err != 1){
            return err;
        }
    }else if(strcmp(token1,"cat") == 0){
        int err = cat(cmd1);
        if (err != 1)
        {
            return err;
        }
    }else if(strcmp(token1,"grep") == 0){
        int err = grep(cmd1,0);
        if (err != 1){
            return err;
        }
    }else if(strcmp(token1,"find") == 0){
        int err = find_str(cmd1,0);
        if (err != 1){
            return err;
        }
    }

    if(last_count > MAX_SIZE){
        arman = realloc(arman,sizeof(char)* last_count);
    }

    strcpy(arman,output);



    if(strcmp(token2,"insertstr") == 0){
        int err = insert_str(cmd2,1);
        if (err != 1)
        {
            return err;
        }else{
            printf("string inserted successfully!\n");
        }
        
    }else if(strcmp(token2,"grep") == 0){
        int err = grep(cmd2,1);
        if (err != 1){
            return err;
        }else{
            printf("%s",output);
        }
    }else if(strcmp(token2,"find") == 0){
        int err = find_str(cmd2,1);
        if (err != 1){
            return err;
        }else{
            printf("%s\n",output);
        }
    }
    return 1;
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

void check_the_command(char str[]){
    char copy[1000];
    strcpy(copy,str);
    char * token = strtok(copy," ");
    char * check_arman = strstr(str," =D ");
    if(check_arman == NULL){
        if(strcmp(token,"createfile") == 0){
            int err = create_file(str);
            if(err != 1){
                handle_err(err);
            }else{
                printf("file created successfully!\n");
            }
        }else if(strcmp(token,"insertstr") == 0){
            int err = insert_str(str,0);
            if (err != 1)
            {
                handle_err(err);
            }else{
                printf("string inserted successfully!\n");
            }
            
        }else if(strcmp(token,"cat") == 0){
            int err = cat(str);
            if (err != 1)
            {
                handle_err(err);
            }else{
                printf("%s\n",output);
            }
            
        }else if(strcmp(token,"removestr") == 0){
            int err = remove_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("removed successfully!\n");
            }
            
        }else if(strcmp(token,"copystr") == 0){
            int err = copy_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("copied to clipboard successfully!\n");
            }
            
        }else if(strcmp(token,"cutstr") == 0){
            int err = cut_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("cut to clipboard successfully!\n");
            }
            
        }else if(strcmp(token,"pastestr") == 0){
            int err = paste_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("paste successfully!\n");
            }
            
        }else if(strcmp(token,"find") == 0){
            int err = find_str(str,0);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s\n",output);
            }
            
        }else if(strcmp(token,"replace") == 0){
            int err = replace_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("replaced successfully!\n");
            }
            
        }else if(strcmp(token,"undo") == 0){
            int err = undo(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("done!\n");
            }
            
        }else if(strcmp(token,"grep") == 0){
            int err = grep(str,0);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s",output);
            }
            
        }else if(strcmp(token,"compare") == 0){
            int err = compare_files(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s",output);
            }
        }else if(strcmp(token,"tree") == 0){
            int err = tree(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s",output);
            }
        }else if(strcmp(token,"auto-indent") == 0){
            int err = auto_indent(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("done!\n");
            }
            
        }else{
            handle_err(-1);
        }
    }else{
        int err = handle_arman(str);
        if(err != 1){
            handle_err(err);
        }
    }
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

int main(){

    char add[1000];
    char str[1000];
    make_here_root();
    set_clipboard();

    fgets(str,1000,stdin);
    
    while (strcmp(str,"stop\n") != 0)
    {
        
        if(strlen(str) < 2){
            handle_err(-1);
            fgets(str,1000,stdin);
            continue;
        }
        if(str[strlen(str)-1] == '\n'){
            str[strlen(str)-1] = '\0';
        }
    
        check_the_command(str);
        fgets(str,1000,stdin);
    }
    
    return 0;
}