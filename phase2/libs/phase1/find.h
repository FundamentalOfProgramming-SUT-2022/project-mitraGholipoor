int compile_str_find(char str[],char str1[]){
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
