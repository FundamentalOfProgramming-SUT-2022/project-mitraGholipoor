#include <ncurses.h>
#include <unistd.h>
#include <math.h>

char vim_address[1000];
char vim_bs_name[1000];
char vim_save_address[1000];
int has_name = 0;

long long str_lines = 0;
int vim_end_screen;
long long vim_diff = 0;

long long vim_lines = 1000;
int vim_cols = 100;
int vim_mode = 0;
int vim_save = 0;

char **vim_str;


long long vir_x = 0;
long long vir_y = 0;

long long vir_x_start = 0;
long long vir_y_start = 0;


int vim_selected = 0;
char vim_massage[100] = "\0";



void vim_make_screen_2();
void handel_phase1_command_1();
void handel_phase1_command_2();
void vim_handle_err(int err);


long long **vim_results_find;
long long vim_results_find_size;
int vim_count_find = 0;
int vim_find_mode = 0;





void vim_make_str(){

    vim_end_screen = LINES - 3;
    vim_cols = COLS - 9;

    vim_str = (char **) malloc((vim_lines)* sizeof(char *));

    for(int i=0; i<=vim_lines ; i++){
        vim_str[i] = malloc((vim_cols + 10) * sizeof(char));
    }

}

void vim_edit_str_lines(long long lines){

    vim_str = realloc(vim_str,lines * 5 * sizeof(char *));
    for(long long i = vim_lines ; i <= lines * 5; i++){
        vim_str[i] = malloc((vim_cols + 10) * sizeof(char));
    } 
    vim_lines = lines * 5;
}

void clear_cmd_bar(){
    move(LINES - 1,0);
    deleteln();
}

void make_vim_file(){

    strcpy(vim_save_address,here);
    strcat(vim_save_address,"/.vim.txt");
    FILE* file = fopen(vim_save_address, "w");
    fclose(file);
    do_undo(vim_save_address);
    mkdir(here_with_root,0777);
    
}

void vim_make_vim_mode(){

	int y,x;



	if(vim_mode == 0 || vim_mode == 3){
		init_pair(1, COLOR_BLACK,COLOR_CYAN);
		move(LINES - 2,0);
		attron(COLOR_PAIR(1));
		printw(" NORMAL ");
		attroff(COLOR_PAIR(1));
	}else if(vim_mode == 1){
		init_pair(1, COLOR_BLACK,COLOR_GREEN);
		move(LINES - 2,0);
		attron(COLOR_PAIR(1));
		printw(" INSERT ");
		attroff(COLOR_PAIR(1));
	}else{
		init_pair(1, COLOR_BLACK,COLOR_BLUE);
		move(LINES - 2,0);
		attron(COLOR_PAIR(1));
		printw(" VIRTUAL ");
		attroff(COLOR_PAIR(1));
	}
	
	init_pair(2, COLOR_BLACK,COLOR_WHITE);
	if(has_name){
		attron(COLOR_PAIR(2));
		printw(" %s ",vim_bs_name);
		attroff(COLOR_PAIR(2));
	}else{
		
		attron(COLOR_PAIR(2));
		printw(" NULL ");
		attroff(COLOR_PAIR(2));
	}
	if(!vim_save){
		attron(COLOR_PAIR(2));
		printw(" + ");
		attroff(COLOR_PAIR(2));
	}
    getyx(stdscr, y, x);
    for(int i = 0; i < COLS - x;i++){
        attron(COLOR_PAIR(2));
		printw(" ");
		attroff(COLOR_PAIR(2));
    }
	move(0,8);
	refresh();
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

void set_str_from_vim(){

    FILE * fp;
    char ch,before = 0;
    fp=fopen(vim_save_address,"r");
    long long line = 0;
    int pos = 0;
    long long count = 0;
    while(1){

        ch = fgetc(fp);
        if(feof(fp)){
            break;
        }

        if(line >= vim_lines){
            vim_edit_str_lines(line);
        }

        if(pos >= vim_cols){
            line++;
            pos = 0;
        }

        if(ch == '\n'){
            vim_str[line][pos] = ch;
            line++;
            pos = 0;
            count++;
            vim_str[line][pos] = ' ';
            vim_str[line][pos + 1] = '\0';
            before = ch;
            continue;
        }

        vim_str[line][pos] = ch;
        pos++;
        count++;
        vim_str[line][pos] = '\0';
        before = ch;
    }
    fclose(fp);
    if(before != '\n' && count != 0){
        vim_str[line][pos] = '\n';
        pos++;
        count++;
        vim_str[line][pos] = '\0';
    }
    if(count != 0){
        line++;
    }
    str_lines = line;
    if( str_lines - vim_end_screen < vim_diff){
        vim_diff = 0;
    }
}

void set_str_from_file(){
    if(has_name){
        FILE * fp;
        char ch,before = 0;
        fp=fopen(vim_address,"r");
        long long line = 0;
        int pos = 0;
        long long count = 0;
        while(1){

            ch = fgetc(fp);
            if(feof(fp)){
                break;
            }

            if(line >= vim_lines){
                vim_edit_str_lines(line);
            }

            if(pos >= vim_cols){
                line++;
                pos = 0;
            }

            if(ch == '\n'){
                vim_str[line][pos] = ch;
                line++;
                pos = 0;
                count++;
                vim_str[line][pos] = ' ';
                vim_str[line][pos + 1] = '\0';
                before = ch;
                continue;
            }

            vim_str[line][pos] = ch;
            pos++;
            count++;
            vim_str[line][pos] = '\0';
            before = ch;
        }
        fclose(fp);
        if(before != '\n' && count != 0){
            vim_str[line][pos] = '\n';
            pos++;
            count++;
            vim_str[line][pos] = '\0';
        }
        if(count != 0){
            line++;
        }
        str_lines = line;  
        if( str_lines - vim_end_screen < vim_diff){
            vim_diff = 0;
        }
    }      
}

void vim_make_screen(){

    clear();
    move(0,0);
    if(str_lines == 0){
        vim_make_screen_2();
    }else if(str_lines >= vim_end_screen){
        for(long long i = vim_diff ; i < vim_end_screen + vim_diff ;i++){
            move(i- vim_diff,0);
            printw("%6.d| ",i+1);
            for(int j = 0; j < strlen(vim_str[i]);j++){
                if(vim_str[i][j] == '(' || vim_str[i][j] == ')'){
                    attron(COLOR_PAIR(21));
                    addch(vim_str[i][j]); 
                }else if(vim_str[i][j] == '{' || vim_str[i][j] == '}'){
                    attron(COLOR_PAIR(22));
                    addch(vim_str[i][j]); 
                }else{
                   attron(COLOR_PAIR(23));
                   addch(vim_str[i][j]); 
                }
                
                if(vim_str[i][j] == '\n'){
                    vim_str[i][j + 1] ='\0';
                    break;
                }
            }
            
            move(i- vim_diff + 1,0);
        }   
    }else{
        for(int i = 0; i < str_lines;i++){
            printw("%6.d| ",i+1);
            for(int j = 0; j < strlen(vim_str[i]);j++){
                if(vim_str[i][j] == '(' || vim_str[i][j] == ')'){
                    attron(COLOR_PAIR(21));
                    addch(vim_str[i][j]); 
                }else if(vim_str[i][j] == '{' || vim_str[i][j] == '}'){
                    attron(COLOR_PAIR(22));
                    addch(vim_str[i][j]); 
                }else{
                   attron(COLOR_PAIR(23));
                   addch(vim_str[i][j]); 
                }
                if(vim_str[i][j] == '\n'){
                    vim_str[i][j+1] ='\0';
                    break;
                }
                
            }
            move(i+1,0);
        }

        init_pair(3,COLOR_BLUE, COLOR_BLACK);
        attron(COLOR_PAIR(3));
        for(int i = str_lines; i < vim_end_screen;i++){
            printw("~\n");
            move(i+1,0);
        }
        attroff(COLOR_PAIR(3));
    }

    vim_make_vim_mode();
    refresh();
}

void vim_make_screen_2(){
    clear();
    move(0,0);
    printw("%6.d| ",1);
    init_pair(3,COLOR_BLUE, COLOR_BLACK);
    attron(COLOR_PAIR(3));
    move(1,0);
    for(int i = 1; i < vim_end_screen;i++){
        printw("~\n");
        move(i+1,0);
    }
    attroff(COLOR_PAIR(3));
    vim_make_vim_mode();
    refresh();
}

void vim_handle_navbar(int ch){
    int x,y;
    getyx(stdscr, y, x);

    if(ch == KEY_RIGHT){
        if(x + 1 < strlen(vim_str[y + vim_diff])+8){
            move(y,x+1);
            refresh();
        }
	}else if(ch == KEY_LEFT){
        if(x - 1  >=  8){
            move(y,x-1);
            refresh();
        }
    }else if(ch == KEY_UP){
        
        if(y - 1 >= 0 && vim_diff == 0){
            if(strlen(vim_str[y - 1]) + 7 >= x){
                move(y-1,x);
            }else{
                move(y-1,strlen(vim_str[y - 1]) + 7);
            }
            refresh();
        }else if(vim_diff != 0 && y - 1 == 3){
            vim_diff--;
            vim_make_screen();
            if(strlen(vim_str[y + vim_diff - 1]) + 7 >= x){
                move(y,x);
            }else{
                move(y,strlen(vim_str[y + vim_diff - 1]) + 7);
            }
            refresh();
        }else if(vim_diff != 0){
            if(strlen(vim_str[y + vim_diff - 1]) + 7 >= x){
                move(y-1,x);
            }else{
                move(y-1,strlen(vim_str[y + vim_diff -1]) + 7);
            }
            refresh();
        }	
        
    }else if(ch == KEY_DOWN){
        if((y + vim_diff + 1) < str_lines){

            if(y + 1 < vim_end_screen && abs(str_lines - (y + vim_diff)) <= 5){
                if(strlen(vim_str[y + vim_diff + 1]) + 7 >= x){
                    move(y+1,x);
                }else{
                    move(y+1,strlen(vim_str[y+ vim_diff +1]) + 7);
                }
                refresh();
            }else if(y + 1 >= vim_end_screen - 4 && y + 1 < str_lines){
                vim_diff++;
                vim_make_screen();
                if(strlen(vim_str[y + vim_diff]) + 7 >= x){
                    move(y,x);
                }else{
                    move(y,strlen(vim_str[y+ vim_diff]) + 7);
                }
                refresh();

            }else if(y + 1 < vim_end_screen - 4){
                if(strlen(vim_str[y + vim_diff + 1]) + 7 >= x){
                    move(y+1,x);
                }else{
                    move(y+1,strlen(vim_str[y+ vim_diff +1]) + 7);
                }
                refresh();
            } 
        }
        
    }
}

void vim_make_massage_in_cmd_bar(){
    if(strlen(vim_massage) != 0){
        move(LINES - 1, 0);
        printw("%s",vim_massage);
        vim_massage[0] = '\0';
    }
}

long long ** make_vim_results_find(){
    long long  **results = (long long **) malloc(sizeof(long long *) * MAX_SIZE);
    for(int i = 0 ; i < MAX_SIZE;i++){
        results[i] = (long long *) malloc(sizeof(long long) * 3);
    }
    vim_results_find_size = MAX_SIZE;
    return results;
}
