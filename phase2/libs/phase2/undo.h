int vim_vim_undo(){

    char address[1000] = "\0";
    strcpy(address,vim_save_address);

    char undo_address[1000];
    get_undo_address(address,undo_address);
    int err = check_file_exist(undo_address);
    if(err == 1){
        remove(address);
        rename(undo_address,address);
    }
    return 1;


}

int vim_undo(char str[]){
    char * has_name = strstr(str," --file ");
    if(has_name != NULL){
        int err = undo(str);
        if(err == 1){
            handel_phase1_command_1();
        }
        return err;
    }else{
        int err = vim_vim_undo();
        if(err  == 1){
            set_str_from_vim();
            vim_diff = 0;
            vim_save = 0;
            vim_make_screen();
            vim_change_mode_to_normal(0);
        }

        return err;
    }
}

int vim_undo_shortcut(){

    int err = vim_vim_undo();
    if(err  == 1){
        set_str_from_vim();
        vim_diff = 0;
        vim_save = 0;
        vim_make_screen();
        vim_change_mode_to_normal(0);
    }

}