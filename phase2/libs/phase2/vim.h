#include <ncurses.h>
#include <unistd.h>

char vim_address[1000];
char vim_bs_name[1000];
int has_name = 0;

int str_lines;

int vim_lines = 100;
int vim_cols = 100;
int vim_mode = 0;
int vim_save = 0;
char **vim_str;


void vim_set_str(){
    vim_str = (char **) malloc((vim_lines + 1)* sizeof(char *));
    for(int i=0; i<=vim_lines ; i++){
        vim_str[i] = malloc((vim_cols + 1) * sizeof(char));
    } 
}

void vim_edit_str_lines(int lines){
    printf("h\n");
    vim_str = realloc(vim_str,lines * 5 * sizeof(char *));
    for(int i = vim_lines ; i <= lines * 5; i++){
        vim_str[i] = malloc(vim_cols * sizeof(char));
    } 
    vim_lines = lines * 5;
}

void vim_edit_str_cols(int cols){

    for(int i = 0 ; i < vim_lines; i++){
        vim_str[i] = realloc(vim_str[i],cols * 5 * sizeof(char));
    } 
    vim_cols = cols * 5;

}

int vim_get_base_name(){
    char * ptr = strchr(vim_address,'/');
    char * save = NULL;
    while(ptr != NULL){
        ptr++;
        save =  ptr;
        ptr = strchr(ptr,'/');
    }
    if(save != NULL){
        ptr = strchr(save,'.');
        if(ptr != NULL){
            strcpy(vim_bs_name,save); 
            return 0;  
        }
        
    }
    return 1;
}

void vim_make_screen_1(){
    
    FILE * fp;
    char ch;
    fp=fopen(vim_address,"r");
    int line = 0;
    int pos = 0;
    int count = 0;
    while(1){

        ch = fgetc(fp);
        if(feof(fp)){
            break;
        }

        if(line >= vim_lines){
            vim_edit_str_lines(line);
        }

        if(pos >= vim_cols){
            vim_edit_str_cols(pos);
        }

        if(ch == '\n'){
            line++;
            pos = 0;
            count++;
            continue;
        }

        vim_str[line][pos] = ch;
        pos++;
        count++;
    }
    str_lines = line;
    move(0,0);
    if(line > LINES - 3){
        for(int i = 0; i <= LINES - 3;i++){
            printw("%6.d | %s",i+1,vim_str[i]);
            move(i+1,0);
        }
    }else{
        for(int i = 0; i < line;i++){
            printw("%6.d| %s",i+1,vim_str[i]);
            move(i+1,0);
        }
        init_pair(3,COLOR_BLUE, COLOR_BLACK);
        attron(COLOR_PAIR(3));
        for(int i = line; i <= LINES - 3;i++){
            printw("~\n");
            move(i+1,0);
        }
        attroff(COLOR_PAIR(3));
    }
    refresh();
}

void vim_make_screen_2(){
    move(0,0);
    printw("%6.d | ",1);
    init_pair(3,COLOR_BLUE, COLOR_BLACK);
    attron(COLOR_PAIR(3));
    move(1,0);
    for(int i = 1; i <= LINES - 3;i++){
        printw("~\n");
        move(i+1,0);
    }
    attroff(COLOR_PAIR(3));
    refresh();
}
