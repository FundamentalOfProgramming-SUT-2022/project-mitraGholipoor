int vim_copy_to_clipboard(){
    int check = 0;

    long long size = vim_selected;
    char which = 'f';
    long long line = vir_y_start + 1;
    long long pos = vir_x_start;

    char address[1000];
    strcpy(address,here);
    strcat(address,"/.vim.txt");


    long long start,end;
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
    if(check == 0){
        return -5;
    }

    if(which =='b'){
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
        end =  count - 1;
    }

    file = fopen(address, "r");

    if(size > last_size){
        clipboard = realloc(clipboard,sizeof(char) * size);
        last_size = size;
    }

    n = 0;
    long long ss = 0;
    while (n <= end) {

        ch = fgetc(file);
        

        if( feof(file) ) {
            break;
        }
        
        if(start <=n && n <= end){
            clipboard[ss] = ch;
            ss++;
        }

        n++;
    }
    clipboard[ss]='\0';
    fclose(file);
    return 1;
}

void vim_copy(){

    vim_mode = 0;
    
    vim_make_vim_mode();



    if(vim_selected != 0){
        int err = vim_copy_to_clipboard();
        if(err == 1){
			move(LINES - 1,0);
            printw("copied to clipboard!");
        }else{
            move(LINES - 1,0);
            printw("error:(");  
        }
    }else{
		move(LINES - 1,0);
        printw("no selected item");
    }
    delete_virtual();
    move(0,8);
    if(str_lines > 4 && vim_diff != 0){
        move(3,8);
    }
    refresh();
}