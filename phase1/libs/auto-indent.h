char * insertString( char s1[], const char s2[], long long pos ){
    long long n1 = strlen( s1 );
    long long n2 = strlen( s2 );


    if ( n1 < pos ) pos = n1;

    for (long long i = 0; i < n1 - pos; i++ )
    {
        s1[n1 + n2 - i - 1] = s1[n1 - i - 1];
    }

    for ( size_t i = 0; i < n2; i++)
    {
        s1[pos+i] = s2[i];
    }

    s1[n1 + n2] = '\0';

    return s1;
}

int do_auto_indenting(char address[]){
    int tab = 0;
    long long size = get_size_of_file(address);
    char * eee = (char *)malloc(sizeof(char) * size * 10);
    long long i = 0,j;
    int add_s = 1;
    char ch,before = 0;
    FILE * fp = fopen(address,"r");
    while (1) {

      ch = fgetc(fp);
      if( feof(fp) ) {
          break;
      }


      if(ch == '{'){

        j = i - 1;
        add_s =0;

        while(j >= 0 && eee[j] == ' '){
            eee[j] = 0;
            j--;
        }

        if(i != 0 && eee[j] == '\n'){
            i = j;
            if(tab > 0){
                for(int k = 0; k < tab;k++){
                    eee[++i] = ' ';
                    eee[++i] = ' ';
                    eee[++i] = ' ';
                    eee[++i] = ' ';
                }
            }
        }else if(i != 0){
            eee[j + 1] = ' ';
            i = j + 1;
        }else{
            i = -1;
        }
        eee[++i] = '{';
        eee[++i]='\n';
        tab++;
        if(tab > 0){
            for(int k = 0; k < tab;k++){
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
            }
        }
        i++;
      }else if(ch == '}'){

        if(i != 0 && before != '{' && before != '}'){
           eee[i] = '\n';
        }else{
            j = i - 1;
            while(j >= 0 && eee[j] == ' '){
                eee[j] = 0;
                j--;
            }
            if(i != 0 && eee[j] == '\n'){
                i = j;
            } 
        }
        
        tab--;
        if(tab > 0){
            for(int k = 0; k < tab;k++){
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
            }
        }

        add_s = 0;
        eee[++i]='}';
        eee[++i]='\n';
        if(tab > 0){
            for(int k = 0; k < tab;k++){
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
                eee[++i] = ' ';
            }
        }
        i++;

      }else{
        if(add_s){
           eee[i] = ch;
           i++; 
        }else if(ch != ' '){
            add_s = 1;
            eee[i] = ch;
            i++;
        }
        
      }


      if(ch != ' '){
        before = ch;
      }
      eee[i] = '\0';

      if(i > size){
        eee =realloc(eee,sizeof(char) * i * 10);
        size = i * 5;
      }
    }
    fclose(fp);
    do_undo(address);
    fp = fopen(address,"w");
    fputs(eee,fp);
    fclose(fp);
    return 1;
}

int auto_indent(char str[]){
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
    if((err= do_auto_indenting(address)) != 1){
        return err;
    }
    return 1;
}
