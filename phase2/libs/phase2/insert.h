int vim_insert_to_vim(char str[],long long line ,int pos){

    int check = 0;
    char proccess[1000];
    char address[1000];

    strcpy(address,here);
    strcat(address,"/.vim.txt");

    strcpy(proccess,here);
    strcat(proccess,"/.proccess.txt");


    FILE* file = fopen(address, "r");

    char ch;
    long long i = 1;
    long long j = 0;
    long long n = 0;
    long long start = 0;
    

    while (1) {

        ch = fgetc(file);

        if( feof(file) ) {
            if(n == 0 && line == 1 && pos == 0){
                fclose(file);
                file = fopen(address, "w");
                fputs(str,file);
                fclose(file);
                return 1;
            }
            break ;
        }

        if(j >= vim_cols){
            i++;
            j = 0;
        }

        if(i == line){
            if( j == pos){
                check = 1;
                start = n;
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
            start = n;
        }
    }

    if(check == 0){
        return -5;
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

        if(n == start){
            fputs(str,fp);
        }
        fputc(ch,fp);
        n++;
    }

    if(n == start){
        fputs(str,fp);
    }

    fclose(file);
    fclose(fp);

    remove(address);
    rename(proccess,address);
    return 1;
}

void vim_insert(int ascii,int shift){
    
    vim_save = 0;
    char ch = (char)ascii;
    char str[10] = "a\0";
    str[0] = ch;



    int x,y;
    getyx(stdscr, y, x);
    x = x - 8;
    long long Y = y + vim_diff;


    int err = vim_insert_to_vim(str,Y+1,x);
    if(shift){
        vim_diff++;
    }
    set_str_from_vim();
    vim_make_screen();
    refresh();
}