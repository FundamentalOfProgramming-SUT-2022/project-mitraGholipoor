
void vim_save_form_vim(char address[]){
    FILE * fp;
    FILE * fp1;
    fp = fopen(address,"w");
    fp1 = fopen(vim_save_address,"r");
    char ch;
    while(1){
        ch = fgetc(fp1);
        if(feof(fp1)){
            break;
        }
        fputc(ch,fp);
    }
    fclose(fp);
    fclose(fp1);
    strcpy(vim_address,address);
    vim_get_base_name();
    has_name = 1;
    vim_save = 1;
}

int vim_saveas(char str[]){
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

    err = check_file_exist(address);
    if(err == 1){
        vim_save_form_vim(address);
        return 1;
    }else{
        err = make_path(address);

        if(err != 1){
            return err;
        } 

        FILE *fp;
        fp  = fopen(address, "w");
        fclose(fp);
        err = check_file_exist(address);
        if(!err){
            return -1;
        }
        vim_save_form_vim(address);
        return 1; 
        
    }
    
    return err;
    return err;
}

int vim_save_file(char str[]){
    if(has_name){
        vim_save_form_vim(vim_address);
        return 1;
    }else{
        return -15;
    }
}