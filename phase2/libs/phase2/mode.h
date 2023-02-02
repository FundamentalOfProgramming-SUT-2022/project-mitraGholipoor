
void delete_virtual(){
    vir_x = -1;
    vir_y = -1;

    vir_x_start = -1;
    vir_y_start = -1;
    vim_selected = 0;
    select_virtual_area(vim_diff,0);
}

void vim_change_mode_to_normal(int massage){
    vim_mode = 0;
    delete_virtual();
    vim_make_screen();
    vim_make_vim_mode();
    if(massage){
        vim_make_massage_in_cmd_bar();
    }
    move(0,8);
    if(str_lines > 4 && vim_diff != 0){
        move(3,8);
    }
    refresh();
}

