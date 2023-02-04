#include "libs/phase1.h"
#include "libs/phase2.h"

/*

up : KEY_UP
down: KEY_DOWN
right: RIGHT
left: LEFT
insert mode: i
virtual mode: v
back to normal :ESC
copy : y
cut :d

*/


int main(int argc, char *argv[]){	

	int ch;
	int x,y;
	char str[1000];
	initscr();
	cbreak();	
	noecho();
	start_color();
	keypad(stdscr, TRUE);	

	//------------------------------
	init_pair(11, COLOR_BLACK,COLOR_YELLOW);
	init_pair(21, COLOR_YELLOW,COLOR_BLACK);
	init_pair(22, COLOR_BLUE,COLOR_BLACK);
	init_pair(23, COLOR_WHITE,COLOR_BLACK);
	//------------------------------
	vim_make_str();	
	init_win_layout(argc,argv);

	while(1){

		ch = getch();
		getyx(stdscr, y, x);
		clear_cmd_bar();
		move(y,x);
		refresh();

		if(vim_mode == 3 && ch == 'n'){
			move(y,x);
			if(vim_count_find){
				int r = move_cursor(y,x);
				if(!r){
					move_cursor_1(1);
				}
			}
		}else if(vim_mode == 3){
			vim_make_screen();
			vim_change_mode_to_normal(0);
		}else{

			vim_mode %= 3;
			vim_count_find = 0;
			if(ch == KEY_RIGHT || ch == KEY_LEFT || ch == KEY_UP || ch == KEY_DOWN){
				vim_handle_navbar(ch);
			}else if(ch == 27){
				vim_change_mode_to_normal(0);
				move(y,x);
			}else if(vim_mode == 0 && ch == 'i'){
				vim_mode = 1;
				vim_make_vim_mode();
				move(y,x);
				refresh();
			}else if(vim_mode == 0 && ch == 'v'){
				vim_mode = 2;
				vir_y = y + vim_diff;
				vir_x = x - 8;
				vim_make_vim_mode();
				move(y,x);
				refresh();
			}else if(vim_mode == 0 && ch == 'p'){
				vim_paste();
				move(y,x);
			}else if(vim_mode == 0 && ch == ':'){

				move(LINES - 1,0);
				addch(':');
				echo();
				getstr(str);
				noecho();
				if(strlen(str) < 2){
					vim_handle_err(-1);
					continue;
				}
				if(str[strlen(str)-1] == '\n'){
					str[strlen(str)-1] = '\0';
				}
				vim_check_the_command(str);
				refresh();
			}else if(vim_mode == 0 && ch == '/'){
				vim_count_find = 0;
				vim_mode = 3;
				move(LINES - 1,0);
				addch('/');
				echo();
				getstr(str);
				noecho();
				if(strlen(str) == 0){
					vim_handle_err(-1);
					continue;
				}

				if(str[strlen(str)-1] == '\n'){
					str[strlen(str)-1] = '\0';
				}
				
				if(strlen(str) == 0){
					vim_handle_err(-1);
					continue;
				}

				vim_find(str);
				refresh();
				
			}else if(vim_mode == 0 && ch == '='){
				vim_auto_indent();
			}else if(vim_mode == 0 && ch == 'u'){
				vim_undo_shortcut();
			}else if(vim_mode == 1 && ch == 8){
				if(x == 8){
					if(vim_diff){
						vim_remove(1);
						move(y,strlen(vim_str[y + vim_diff - 1]) + 7);
					}else if(y > 0){
						vim_remove(0);
						move(y - 1,strlen(vim_str[y - 1]) + 7);
					}
				}else{
					vim_remove(0);
					move(y,x - 1);
				}
			}else if(vim_mode == 1){
				
				if(x + 1 >= vim_cols + 9 || ch == '\n'){
					if(y + 1 == vim_end_screen){
						vim_insert(ch,1);
						move(y,8);

					}else{
						vim_insert(ch,0);
						move(y+1,8);
					}
				}else{
					vim_insert(ch,0);
					move(y,x+1);
				}
			}

			if(vim_mode == 2 && (ch == KEY_RIGHT || ch == KEY_LEFT || ch == KEY_UP || ch == KEY_DOWN)){
				int xx,yy;
				getyx(stdscr,yy,xx);
				select_virtual_area(yy,xx);
				move(yy,xx);
				refresh();
			}else if(vim_mode == 2 && ch == 'y'){
				vim_copy();
			}else if(vim_mode == 2 && ch == 'd'){
				vim_cut();
				set_str_from_vim();
				vim_diff = 0;
				vim_make_screen();
				vim_change_mode_to_normal(1);
			}

			if(vim_mode == 3){
				high_light_find();
				move(y,x);
			}
		}
	}





	endwin();		
	return 0;
}
