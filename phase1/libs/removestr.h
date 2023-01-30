
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
    

    if(i == line){
        if( j == pos){
            check = 1;
            pos_pos = n;
        }
    }

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
        end = count - 1;
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
