int vim_cut_to_clipboard(){

    int check = 0;
    long long start,end;
    long long size = vim_selected;
    char which = 'f';
    long long line = vir_y_start + 1;
    long long pos = vir_x_start;
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
        end = count - 1;
    }

    do_undo(address);
    file = fopen(address, "r");
    FILE* fp = fopen(proccess,"w");

    if(size > last_size){
        clipboard = realloc(clipboard,sizeof(char) * size);
        last_size = size;
    }

    n = 0;
    long long ss =0;

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            break ;
        }

        if(n < start || n > end){
            fputc(ch,fp);
        }else{
            clipboard[ss] = ch;
            ss++;
        }

        n++;
    }

    fclose(file);
    fclose(fp);

    remove(address);
    rename(proccess,address);
    clipboard[ss]='\0';
    return 1;
}

void vim_cut(){
    if(vim_selected != 0){
        int err = vim_cut_to_clipboard();
        if(err == 1){
            vim_save = 0;
            strcpy(vim_massage,"cut to clipboard!\0");
        }else{
            strcpy(vim_massage,"error:(\0");
        }
    }else{
        strcpy(vim_massage,"no selected item\0");
    }
}