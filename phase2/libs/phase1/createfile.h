
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
