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

    for(int i = 0 ; i< strlen(cmd1);i++){
        copy1[i] = cmd1[i];
    }
    
    for(int i = 0 ; i< strlen(cmd2);i++){
        copy2[i] = cmd2[i];
    }

    
    char * token1 = strtok(copy1," ");
    char * token2 = strtok(copy2," ");

    if(token1 == NULL || strlen(token1) == 0){
        strcat(cmd1 ," ");
        strcpy(copy1,cmd1);
        token1 = strtok(copy1," ");
    }

    if(token2 == NULL || strlen(token2) == 0){
        strcat(cmd2 ," ");
        strcpy(copy2,cmd2);
        token2 = strtok(copy2," ");
    }  

    if(token1 == NULL || token2 == NULL){
        return -1;
    }

    if(strcmp(token1 ,"tree") != 0 && strcmp(token1 ,"cat") != 0 && strcmp(token1 ,"grep") != 0 && strcmp(token1 ,"find") != 0 && strcmp(token1 ,"compare") != 0){
        return -1;
    }
    if(strcmp(token2 ,"insertstr") != 0 && strcmp(token2 ,"grep") != 0 && strcmp(token2 ,"find") != 0 && strcmp(token2 ,"replace") != 0){
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
    }else if(strcmp(token1,"compare") == 0){
        int err = compare_files(cmd1);
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
    }else if(strcmp(token2,"replace") == 0){
        int err = replace_str(cmd2,1);
        if (err != 1){
            return err;
        }else{
            printf("replaced successfully!\n");
        }
        
    }

    return 1;
}
