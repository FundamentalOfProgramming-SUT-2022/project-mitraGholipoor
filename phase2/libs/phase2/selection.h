void select_virtual_area(int y,int x){
    char ch;
    int Y = y + vim_diff;
    int X = x - 8;
    int high_light = 0;
    int first = 0;
    int second = 0;
    vim_selected = 0;

    for(int i = 0; i < str_lines;i++){
        for(int j = 0 ;j < strlen(vim_str[i]); j++){


            if((i == Y && j == X)  && high_light == 0){
                high_light = 1;
                first = 1;
                vir_x_start = Y;
                vir_y_start = X;

            }

            if((i == vir_y && j == vir_x)  && high_light == 0){
                high_light = 1;
                second = 1;
                vir_x_start = vir_x;
                vir_y_start = vir_y;

            }

            if(high_light){
                if(vim_diff <= i && i < vim_end_screen + vim_diff){
                    move(i - vim_diff , j + 8);
                    ch  = inch();
                    attron(COLOR_PAIR(11));
                    addch(ch);
                }
                vim_selected++;
            }else if(vim_diff <= i && i < vim_end_screen + vim_diff){
                    move(i - vim_diff , j + 8);
                    ch  = inch();
                    if(ch == '(' || ch == ')'){
                        attron(COLOR_PAIR(21));
                        addch(ch); 
                    }else if(ch == '{' || ch == '}'){
                        attron(COLOR_PAIR(22));
                        addch(ch); 
                    }else{
                        attron(COLOR_PAIR(23));
                        addch(ch); 
                    }
            }
        

            if((i == Y && j == X)  && high_light == 1 && second == 1){
                high_light = 0;
            }

            if((i == vir_y && j == vir_x)  && high_light == 1 && first == 1){
                high_light = 0;
            }
        }
    }
    refresh();  
}

