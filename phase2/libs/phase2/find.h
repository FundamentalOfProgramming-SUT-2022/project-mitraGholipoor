int vim_find_regex(char address[], char pattern[],long long at){
    vim_count_find  = 0;
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

    char * string = str;
    long long where = 0;
    long long  at_where = 1;


    do{
        regex_t    preg;
        int        rc;
        size_t     nmatch = 1;
        regmatch_t pmatch[1];


        if(where >= count){
            vim_results_find[at_where][0] = -1;
            break;
        }

        if (0 != (rc = regcomp(&preg, pattern, 0))) {
            free(str);
            return -10;
        }
        
        if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
            vim_results_find[at_where][0] = -1;
            break;
        }
        else {
            if(pmatch[0].rm_so != -1){
                vim_results_find[at_where][0] = pmatch[0].rm_so + where;
                vim_results_find[at_where][1] = pmatch[0].rm_eo + where - 1;
                string += pmatch[0].rm_eo;
                where += pmatch[0].rm_eo;
            }else{
                vim_results_find[at_where][0] = -1;
                break;
            }
        }
        regfree(&preg);
    }while(at_where++ != at);
    at_where -= 1;


    if(at_where == 0){
        return 0;
    }
    vim_count_find = at_where;
    free(str);
    return 1;
}

int should_high_light(int pos){
    for(int i = 1; i<=vim_count_find ; i++){
        if(vim_results_find[i][0] <= pos && pos <= vim_results_find[i][1]){
            return 1;
        }
        if(pos < vim_results_find[i][0]){
            return 0;
        }
    }
    return 0;
}

void high_light_find(){
    char ch;
    int count=0;
    for(int i = 0; i < str_lines;i++){
        for(int j = 0 ;j < strlen(vim_str[i]); j++){
            if(should_high_light(count)){
                if(vim_diff <= i && i < vim_end_screen + vim_diff){
                    move(i - vim_diff , j + 8);
                    ch  = inch();
                    attron(COLOR_PAIR(11));
                    addch(ch);
                }
            }else if(vim_diff <= i && i < vim_end_screen + vim_diff){
                    move(i - vim_diff , j + 8);
                    ch  = inch();
                    if(ch == '(' || ch == ')'){
                        attron(COLOR_PAIR(21));
                        addch(ch); 
                    }else if(ch == '{' || ch == '}'){
                        attron(COLOR_PAIR(22));
                        addch(ch); 
                    }else{
                        attron(COLOR_PAIR(23));
                        addch(ch); 
                    }
            }
            count++;
        }
    }
    refresh();  
}

void move_cursor(int count){
    clear();
    int pos = vim_results_find[count][0];
    int n = 0;
    for(int i = 0; i < str_lines;i++){
        for(int j = 0 ;j < strlen(vim_str[i]); j++){
            if(n == pos){
                if(i > vim_end_screen){
                    vim_diff = i - vim_end_screen;
                    vim_make_screen();
                    high_light_find();
                    move(i,j + 8);
                    return;
                }else{
                    vim_diff = 0;
                    vim_make_screen();
                    high_light_find();
                    move(i,j + 8);
                    return;
                }
            }
            n++;
        }
    }
}


void vim_find(char str[]){
    vim_find_regex(vim_save_address,str,-1);
}