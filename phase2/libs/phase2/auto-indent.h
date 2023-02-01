int vim_vim_auto_indent(){
    int err;
    if((err= do_auto_indenting(vim_save_address)) != 1){
        return err;
    }
    return 1;
}


int vim_auto_indent(){
    int err = vim_vim_auto_indent();
    if(err == 1){
        set_str_from_vim();
        vim_diff = 0;
        vim_make_screen();
        vim_change_mode_to_normal(0);
    }
    
}