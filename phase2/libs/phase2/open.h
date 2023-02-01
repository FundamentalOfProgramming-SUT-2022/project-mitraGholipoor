void vim_save_form_file(char address[]){
    FILE * fp;
    FILE * fp1;
    fp = fopen(address,"r");
    fp1 = fopen(vim_save_address,"w");
    char ch;
    while(1){
        ch = fgetc(fp);
        if(feof(fp)){
            break;
        }
        fputc(ch,fp1);
    }
    fclose(fp);
    fclose(fp1);
    strcpy(vim_address,address);
    vim_get_base_name();
    has_name = 1;
    vim_save = 1;
}


int vim_open(char str[]){
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

    if(has_name){
        vim_save_form_vim(vim_address);

    }
    vim_save_form_file(address);
    set_str_from_vim();
    vim_diff = 0;
    vim_make_screen();
    vim_change_mode_to_normal(0);
    return 1;
}

