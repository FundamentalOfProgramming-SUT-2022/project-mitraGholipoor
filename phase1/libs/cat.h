int cat(char str[]){
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

    if(file_name[0] == '.'){
        return -6;
    }

    long long count = get_size_of_file(address);

    if(count > last_count){
        output = realloc(output,sizeof(char) * count);
        last_count = count;
    }
    do_undo(address);
    char ch;
    FILE* file = fopen(address, "r");
    long long ss = 0;
    while (1) {
        ch = fgetc(file);
        if( feof(file) ) {
            break;
        }
        output[ss] = ch;
        ss++;
    }
    fclose(file);
    output[ss] = '\0';
    return 1;
}
