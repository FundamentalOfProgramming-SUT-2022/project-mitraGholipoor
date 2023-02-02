
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
    if(err == 1 && strlen(str_str) == 0){
        strcat(str," ");
        err = get_str1_string(str,str_str);
    }
    if(err==1){
        int len = strlen(str_str);

        char * add = str_replace(str_str,"+qot10446+","\"");
        add = str_replace(add,"+newline10448+","\n");
        add = str_replace(add,"+main_newline10447+","\\\\n");
        add = str_replace(add,"+star10449+","\\*");
        add = str_replace(add,".","\\.");
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
    if(err == 1 && strlen(str_str) == 0){
        strcat(str," ");
        err = get_str2_string(str,str_str);
    }
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

    long long  **results = (long long **) malloc(sizeof(long long *) * MAX_SIZE);
    for(int i = 0 ; i < MAX_SIZE;i++){
        results[i] = (long long *) malloc(sizeof(long long) *3);
    }

    long long size_size = MAX_SIZE;


    char * string = str;
    long long where = 0;
    long long  at_where = 1;


    do{
        regex_t    preg;
        int        rc;
        size_t     nmatch = 1;
        regmatch_t pmatch[1];

        if(at_where >= size_size){
            results = realloc(results,sizeof(long long *) * at_where * 2);
            for(int i = at_where ; i < at_where * 2;i++){
                results[i] = (long long *)malloc(sizeof(long long) *3);
            }
            size_size = at_where;
        }

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

int replace_str(char str[],int is_arman){
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

    if(!is_arman){
        err = compile_str1(str,str1);
        if(err != 1){
            return err;
        }
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

    
    if(is_arman){
        err = replace_regex(address,arman,str2,at,is_all);
    }else{
        err = replace_regex(address,str1,str2,at,is_all);
    }
    
    if(err != 1){
        return err;
    }
    return 1;
}
