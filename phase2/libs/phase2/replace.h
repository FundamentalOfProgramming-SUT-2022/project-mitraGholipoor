int vim_replace_str(char str[],int is_arman){
    
    char address[1000];
    char str1[1000];
    char str2[1000];
    int err;
    char * r_str = str_replace(str,"\\\"","+qot10446+");
    r_str = str_replace(r_str,"\\\\n","+main_newline10447+");
    r_str = str_replace(r_str,"\\n","+newline10448+");
    r_str = str_replace(r_str,"\\*","+star10449+");
    r_str = str_replace(r_str,"*","+astrick10450+");
    strcpy(str,r_str);
    strcpy(address,vim_save_address);
    
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
    vim_save = 0;
    return 1;
}


int vim_replace(char str[],int is_arman){
    char * has_file = strstr(str," --file ");
    if(has_file != NULL){
        int err = replace_str(str,is_arman);
        if(err == 1){
            handel_phase1_command_1();
        }
        return err;
    }else{

        int err = vim_replace_str(str,is_arman);
        if(err == 1){
            set_str_from_vim();
            vim_diff = 0;
            vim_make_screen();
            vim_change_mode_to_normal(0);
        }
        return err;
        
    }
}