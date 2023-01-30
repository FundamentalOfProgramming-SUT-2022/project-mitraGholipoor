
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
