
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
        err =get_address1_string(str,str_str);
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
        err =get_address2_string(str,str_str);
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

    char ch,before;
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
      before = ch;
    }
    fclose(file);
    if(before == '\n'){
        line--;
    }
    return line;
}

int compare(char address[],char address1[]){
    long long count = get_size_of_file(address);
    long long count1 = get_size_of_file(address1);
    long long cnt = get_count_of_line(address);
    long long cnt1 = get_count_of_line(address1);

    count += 100;
    count1 += 100;


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
            fgets(str1,count1,fp1);


            if(str[strlen(str)-1] == '\n'){
                str[strlen(str)-1] = '\0';
            }
            if(str1[strlen(str1)-1] == '\n'){
                str1[strlen(str1)-1] = '\0';
            }



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

            if(str[strlen(str)-1] == '\n'){
                str[strlen(str)-1] = '\0';
            }
            if(str1[strlen(str1)-1] == '\n'){
                str1[strlen(str1)-1] = '\0';
            }

            
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
