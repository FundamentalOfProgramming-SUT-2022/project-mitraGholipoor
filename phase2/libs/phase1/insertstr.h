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

    if(err == 1 && strlen(str_str) == 0){
        strcat(str," ");
        err = get_str_string(str,str_str);
    }

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

    if(i == line){
        if( j == pos){
            check = 1;
            start = n;
        }
    }

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

    if(n == start){
        fputs(str,fp);
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