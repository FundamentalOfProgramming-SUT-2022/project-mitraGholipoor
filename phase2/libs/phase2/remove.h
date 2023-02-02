
int vim_remove_from_vim(int line,int pos){

    int check = 0;
    int size = 1;
    long long start,end;
    char which = 'b';
    char proccess[1000];
    char address[1000];

    strcpy(address,here);
    strcat(address,"/.vim.txt");

    strcpy(proccess,here);
    strcat(proccess,"/.proccess.txt");
    long long count  = get_size_of_file(address);

    FILE* file = fopen(address, "r");
    char ch;
    long long i = 1;
    long long j = 0;
    long long n = 0;
    long long pos_pos = 0;

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(j >= vim_cols){
            i++;
            j = 0;
        }

        if(i == line){
            if( j == pos){
                check = 1;
                pos_pos = n;
                break;
            }
        }


        if(ch == '\n'){
            i++;
            j = 0;
            n++;
            continue;
        }

        n++;
        j++;
    }
    
    fclose(file);
    

    if(i == line){
        if( j == pos){
            check = 1;
            pos_pos = n;
        }
    }

    if(check == 0){
        return -5;
    }

    if(which == 'b'){
        start = pos_pos - size;
        end = pos_pos - 1;
    }else{
        start = pos_pos;
        end = pos_pos + size - 1;
    }

    if(start < 0){
        start = 0;
    }
    if(end >= count){
        end = count - 1;
    }
    do_undo(address);
    file = fopen(address, "r");
    FILE* fp = fopen(proccess,"w");

    n = 0;
    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(n < start || n > end){
            fputc(ch,fp);
        }

        n++;
    }

    fclose(file);
    fclose(fp);

    remove(address);
    rename(proccess,address);
    return 1;
}

void vim_remove(int shift){

    int x,y;
    getyx(stdscr, y, x);
    x = x - 8;
    int Y = y + vim_diff;

    int err = vim_remove_from_vim(Y+1,x);
    if(err == 1){
        vim_save = 0;
        if(shift){
            vim_diff--;
        }
        set_str_from_vim();
        vim_make_screen();
        refresh(); 
    }

    if(str_lines == 0){
        vim_make_screen_2();
    }
    
}