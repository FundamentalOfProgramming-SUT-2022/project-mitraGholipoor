#include <ncurses.h>
#include <unistd.h>
#include <math.h>

char vim_address[1000];
char vim_bs_name[1000];
char vim_save_address[] = "./.vim.txt";
int has_name = 0;

int str_lines = 0;
int vim_end_screen;
int vim_diff = 0;

int vim_lines = 100;
int vim_cols = 100;
int vim_mode = 0;
int vim_save = 0;

char **vim_str;


int vir_x = 0;
int vir_y = 0;

int vir_x_start = 0;
int vir_y_start = 0;


int ** vir_coor;
int vim_selected = 0;

void vim_make_str(){

    vim_end_screen = LINES - 3;
    vim_cols = COLS - 9;

    vim_str = (char **) malloc((vim_lines + 1)* sizeof(char *));

    for(int i=0; i<=vim_lines ; i++){
        vim_str[i] = malloc((vim_cols + 2) * sizeof(char));
    }

}

void vim_make_vir_coor(){

    if(vir_coor!= NULL){
        free(vir_coor);
    }

    vir_coor = (int **) calloc((str_lines + 1), sizeof(int *));

    for(int i=0; i <= str_lines ; i++){
        vir_coor[i] = calloc((vim_cols + 2) , sizeof(int));
    }

}

void vim_edit_str_lines(int lines){

    vim_str = realloc(vim_str,lines * 5 * sizeof(char *));
    for(int i = vim_lines ; i <= lines * 5; i++){
        vim_str[i] = malloc((vim_cols + 2) * sizeof(char));
    } 
    vim_lines = lines * 5;
}

void make_vim_file(){

    char address[1000];
    strcpy(address,here);
    strcat(address,"/.vim.txt");
    FILE* file = fopen(address, "w");
    fclose(file);
    
}

void vim_make_vim_mode(){

	int y,x;




	if(vim_mode == 0){
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
}

void vim_make_screen(){
    clear();
    move(0,0);
    for(int i = vim_diff ; i < vim_end_screen + vim_diff ;i++){
        move(i- vim_diff,0);
        printw("%6.d| %s",i+1,vim_str[i]);
        move(i- vim_diff + 1,0);
    }
    vim_make_vim_mode();
    refresh();
}

void vim_make_screen_1(){
    clear();
    move(0,0);
    if(str_lines >= vim_end_screen){
        for(int i = 0; i < vim_end_screen;i++){
            printw("%6.d| %s",i+1,vim_str[i]);
            move(i+1,0);
        }
    }else{
        for(int i = 0; i < str_lines;i++){
            printw("%6.d| %s",i+1,vim_str[i]);
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

void vim_make_screen_3(){
    clear();
    move(0,0);
    for(int i = vim_diff ; i < vim_end_screen + vim_diff ;i++){
        move(i- vim_diff,0);
        printw("%6.d| %s",i+1,vim_str[i]);
        move(i- vim_diff + 1,0);
    }
    vim_make_vim_mode();
    refresh();
}

void vim_make_screen_4(){
    clear();
    move(0,0);
    for(int i = vim_diff ; i < vim_end_screen + vim_diff ;i++){
        move(i- vim_diff,0);
        printw("%6.d| %s",i+1,vim_str[i]);
        move(i- vim_diff + 1,0);
    }
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
            vim_make_screen_4();
            if(strlen(vim_str[y + vim_diff]) + 7 >= x){
                move(y,x);
            }else{
                move(y,strlen(vim_str[y + vim_diff]) + 7);
            }
            refresh();
        }else if(vim_diff != 0){
            if(strlen(vim_str[y + vim_diff - 1]) + 8 >= x){
                move(y-1,x);
            }else{
                move(y-1,strlen(vim_str[y + vim_diff -1]) + 7);
            }
            refresh();
        }	
        
    }else if(ch == KEY_DOWN){
        if((y + vim_diff + 1) < str_lines){

            if(y + 1 < vim_end_screen && abs(str_lines - (y + vim_diff)) <= 4){
                if(strlen(vim_str[y + vim_diff + 1]) + 7 >= x){
                    move(y+1,x);
                }else{
                    move(y+1,strlen(vim_str[y+ vim_diff +1]) + 7);
                }
                refresh();
            }else if(y + 1 == vim_end_screen - 3){
                vim_diff++;
                vim_make_screen_3();
                if(strlen(vim_str[y + vim_diff]) + 7 >= x){
                    move(y,x);
                }else{
                    move(y,strlen(vim_str[y+ vim_diff]) + 7);
                }
                refresh();

            }else if(y + 1 < vim_end_screen - 3){
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
