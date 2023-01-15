#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include<error.h>


char here[1000];
char here_with_root[1000];
char clipboard[1000];
int valid = 0;

//-------------type of error-----------------
/*
-1:command is not valid
-2:file is already exsit
-3:file path is not valid
-4:invalid directory or file name
-5:invalid position
-6:file is not exsit
-7:number of pos is out of range
-8:number of line is out of range
-9:invalid size
*/


//---------create file--------------//

int make_path(char address[]){
    char need[500];
    char add[1000];
    strcpy(add,here_with_root);
    strcpy(need,address);
    char *ptr = need;
    ptr += strlen(here_with_root);
    char *token = strtok(ptr,"/");
    valid = 0;
    while(1)
    {
        char save[100];
        strcpy(save,token);
        token = strtok(NULL,"/");
        if(token == NULL){
            char * ss_ptr = strchr(save,'.');
            if(ss_ptr == NULL){
                strcat(add,"/");
                strcat(add,save);
                mkdir(add);
                valid = 1;
            }
            break;
        }
        strcat(add,"/");
        strcat(add,save);
        mkdir(add);
    }
    return 1;
}

int check_file_exsit(char add[]){
    FILE * file;
    file = fopen(add, "r");
    if (file){
        fclose(file);
        return 1;
    }else{
        return 0;
    }
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

int get_address_with_qot(char str[],char address[]){
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

int get_address_string(char str[],char address[]){
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
    char get_str[1000];
    char check[5];
    int err =get_address_string(str,get_str);
    if(err==1){
        int len = strlen(get_str);
        char * add = str_replace(get_str,"*qot*","\"");
        add = str_replace(add,"*newline*","\n");
        add = str_replace(add,"*main_newline*","\\n");
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
    char * r_str = str_replace(str,"\\\"","*qot*");
    r_str = str_replace(r_str,"\\\\n","*main_newline*");
    r_str = str_replace(r_str,"\\n","*newline*");
    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err == 1){
        err = check_file_exsit(address);
        if(err == 1){
            return -2;
        }
        err = make_path(address);
        if(err == 1){
            FILE *fp;
            fp  = fopen(address, "w");
            fclose(fp);
            err = check_file_exsit(address);
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

int get_str_with_qot(char str[],char str1[]){
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

int get_str_string(char str[],char str1[]){
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
    char get_str[1000];
    char check[5];
    int err = get_str_string(str,get_str);
    if(err==1){
        int len = strlen(get_str);
        char * add = str_replace(get_str,"*qot*","\"");
        add = str_replace(add,"*newline*","\n");
        add = str_replace(add,"*main_newline*","\\n");
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
    int added = 0;
    FILE* file = fopen(address, "r");
    char file_str[1000][1000];
    int i = 0;
    while (fgets(file_str[i], 1000, file) || i == 0) {
        i++;
        if(i == line){
            if(strlen(file_str[i-1]) - 1 < pos){
                fclose(file);
                return -7;
            }
            char str1[1000];
            char str2[1000];
            
            strcpy(str1,file_str[i-1]);
            str1[pos]= '\0';
            strcpy(str2,file_str[i-1]);
            char * ptr = str2;
            ptr+= pos;
            strcat(str1,str);
            strcat(str1,ptr);
            strcpy(file_str[i-1],str1);
            added = 1;
        }
    }
    fclose(file);
    if(added == 0){
        return -8;
    }


    FILE* fp = fopen(address, "w");
    for(int j = 0; j < i ;j++){
        fputs(file_str[j],fp);
    }
    fclose(fp);
    return 1;
}

int insert_str(char str[]){
    char address[1000];
    char str1[1000];
    char * r_str = str_replace(str,"\\\"","*qot*");
    r_str = str_replace(r_str,"\\\\n","*main_newline*");
    r_str = str_replace(r_str,"\\n","*newline*");
    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exsit(address) == 0){
        return -6;
    }
    err = compile_str(str,str1);
    if(err != 1){
        return err;
    }
    int line=1,pos=0;
    err = get_pos(str,&line,&pos);
    if(err != 1){
        return err;
    }
    err = insert_to_file(address,str1,line,pos);
    if(err != 1){
        return err;
    }
    return 1;
}

int cat(char str[],char result[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","*qot*");
    r_str = str_replace(r_str,"\\\\n","*main_newline*");
    r_str = str_replace(r_str,"\\n","*newline*");
    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exsit(address) == 0){
        return -6;
    }

    FILE* file = fopen(address, "r");
    char sss[1000];
    while (fgets(sss, 1000, file)) {
        strcat(result,sss);
    }
    fclose(file);
    return 1;
}

int get_size(char str[],int * size){
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
    *size = atoi(token);
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

int remove_from_file(char sss[],char address[],int line,int pos,int size,char which){
    int c_line=1;
    int c_p=0;
    int start=0;
    int end = 0;
    int check =0;
    int count = strlen(sss);
    char before[3000]="\0",after[3000]="\0",result[4000]="\0";
    int pos_pos = 0;
    int len_b=0,len_a=0;
    for(int i = 0;i < count;i++){
        if(c_p == pos && c_line == line){
            pos_pos = i;
            check =1;
            break;
        }
        if(sss[i] == '\n'){
            c_line++;
            c_p = 0;
        }else{
            c_p++;
        }
    }
    
    if(!check){
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
        end = count - 1;
    }
    for (int i = 0; i < count; i++)
    {
        if(i < start){
            before[len_b] = sss[i];
            len_b++;
        }else if( end < i){
            after[len_a]= sss[i];
            len_a++;
        }
        before[len_b]='\0';
        after[len_a] ='\0';
    }
    strcat(result,before);
    strcat(result,after);

    FILE* fp = fopen(address, "w");
    fputs(result,fp);
    fclose(fp);
    return 1;
}

int remove_str(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","*qot*");
    r_str = str_replace(r_str,"\\\\n","*main_newline*");
    r_str = str_replace(r_str,"\\n","*newline*");
    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exsit(address) == 0){
        return -6;
    }
    int line,pos,size;
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
    char sss[2000]="\0";
    cat(str,sss);
    err = remove_from_file(sss,address,line,pos,size,which);
    if(err != 1){
        return err;
    }
    return 1;
}

int copy_to_clipboard(char sss[],char address[],int line, int pos, int size, char which){
    int c_line=1;
    int c_p=0;
    int start=0;
    int end = 0;
    int check = 0;
    int count = strlen(sss);
    char before[3000]="\0",after[3000]="\0",result[4000]="\0";
    int pos_pos = 0;
    int len=0;
    for(int i = 0;i < count;i++){
        if(c_p == pos && c_line == line){
            pos_pos = i;
            check =1;
            break;
        }
        if(sss[i] == '\n'){
            c_line++;
            c_p = 0;
        }else{
            c_p++;
        }
    }

    if(!check){
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
        end = count - 1;
    }
    clipboard[0]='\0';
    for (int i = start; i <= end; i++){
        clipboard[len] = sss[i];
        len++;
        clipboard[len]='\0';
    }
    return 1;
}

int copy_str(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","*qot*");
    r_str = str_replace(r_str,"\\\\n","*main_newline*");
    r_str = str_replace(r_str,"\\n","*newline*");
    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exsit(address) == 0){
        return -6;
    }
    int line,pos,size;
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
    char sss[2000]="\0";
    cat(str,sss);
    err = copy_to_clipboard(sss,address,line,pos,size,which);
    if(err != 1){
        return err;
    }
    return 1;
}

int cut_to_clipboard(char sss[],char address[],int line, int pos, int size, char which){
    int c_line=1;
    int c_p=0;
    int start=0;
    int end = 0;
    int check = 0;
    int count = strlen(sss);
    char before[3000]="\0",after[3000]="\0",result[4000]="\0";
    int pos_pos = 0;
    int len_b=0,len_a=0,len =0;
    for(int i = 0;i < count;i++){
        if(c_p == pos && c_line == line){
            pos_pos = i;
            check = 1;
            break;
        }
        if(sss[i] == '\n'){
            c_line++;
            c_p = 0;
        }else{
            c_p++;
        }
    }

    if(!check){
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
        end = count - 1;
    }
    clipboard[0]='\0';
    for (int i = 0; i < count; i++)
    {
        if(i < start){
            before[len_b] = sss[i];
            len_b++;
            before[len_b]='\0';
        }else if( end < i){
            after[len_a]= sss[i];
            len_a++;
            after[len_a] ='\0';
        }else{
            clipboard[len] = sss[i];
            len++;
            clipboard[len]='\0';
        }
    }
    strcat(result,before);
    strcat(result,after);

    FILE* fp = fopen(address, "w");
    fputs(result,fp);
    fclose(fp);
    return 1;
}

int cut_str(char str[]){
    char address[1000];
    char * r_str = str_replace(str,"\\\"","*qot*");
    r_str = str_replace(r_str,"\\\\n","*main_newline*");
    r_str = str_replace(r_str,"\\n","*newline*");
    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exsit(address) == 0){
        return -6;
    }
    int line,pos,size;
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
    char sss[2000]="\0";
    cat(str,sss);
    err = cut_to_clipboard(sss,address,line,pos,size,which);
    if(err != 1){
        return err;
    }
    return 1;
}

int paste_str(char str[]){
    char address[1000];
    char str1[1000];
    char * r_str = str_replace(str,"\\\"","*qot*");
    r_str = str_replace(r_str,"\\\\n","*main_newline*");
    r_str = str_replace(r_str,"\\n","*newline*");
    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exsit(address) == 0){
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

void handle_err(int err){
    switch (err)
    {
    case -1:
        printf("command is not valid\n");
        break;
    case -2:
        printf("file is already exsit\n");
        break;
    case -3:
        printf("file path is not valid\n");
    case -4:
        printf("invalid directory or file name\n");
        break;
    case -5:
        printf("invalid position\n");
        break;
    case -6:
        printf("file does not exsit\n");
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
            int err = insert_str(str);
            if (err != 1)
            {
                handle_err(err);
            }else{
                printf("string inserted successfully!\n");
            }
            
        }else if(strcmp(token,"cat") == 0){
            char result[2000]={};
            int err = cat(str,result);
            if (err != 1)
            {
                handle_err(err);
            }else{
                printf("%s\n",result);
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
            
        }else{
            handle_err(-1);
        }
    }
}

void make_here_root(){
    getcwd(here,1000);
    char * ptr = str_replace(here,"\\","/");
    strcpy(here,ptr);
    strcpy(here_with_root,ptr);
    strcat(here_with_root,"/root");
    printf("%s\n%s\n",here,here_with_root);
}

int main(){
    char add[1000];
    char str[1000];
    make_here_root();
    fgets(str,1000,stdin);
    while (strcmp(str,"stop\n") != 0)
    {
        if(str[strlen(str)-1] == '\n'){
            str[strlen(str)-1] = '\0';
        }
        
        check_the_command(str);
        fgets(str,1000,stdin);
    }
    
    return 0;
}