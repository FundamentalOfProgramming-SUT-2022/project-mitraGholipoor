int paste_str(char str[]){
    char address[1000];
    char str1[1000];
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");

    strcpy(str,r_str);
    int err = compile_address(str,address);
    if(err != 1){
        return err;
    }
    if(check_file_exist(address) == 0){
        return -6;
    }
    int line=1,pos=0;
    err = get_pos(str,&line,&pos);
    if(err != 1){
        return err;
    }
    err = insert_to_file(address,clipboard,line,pos);
    if(err != 1){
        return err;
    }
    return 1;
}
