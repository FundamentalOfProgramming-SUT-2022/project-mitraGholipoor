void vim_paste(){

    if(clipboard != NULL && strlen(clipboard) != 0){
        int x,y;
        getyx(stdscr, y, x);
        x = x - 8;
        long long Y = y + vim_diff;
        

        int err = vim_insert_to_vim(clipboard,Y+1,x);
        if(err == 1){
            vim_save = 0;
            set_str_from_vim();
            vim_make_screen();
        }
            
        refresh();
    }

    
}