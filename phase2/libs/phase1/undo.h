int undo(char str[]){
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

    char undo_address[1000];
    get_undo_address(address,undo_address);
    err = check_file_exist(undo_address);
    if(err == 1){
        remove(address);
        rename(undo_address,address);
    }
    return 1;
}