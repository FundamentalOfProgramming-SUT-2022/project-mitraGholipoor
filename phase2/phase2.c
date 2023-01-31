#include "libs/phase1.h"
#include "libs/phase2.h"


int main(int argc, char *argv[]){	

	int ch;
	int x,y;

	initscr();
	cbreak();	
	noecho();
	start_color();
	keypad(stdscr, TRUE);	

	//------------------------------

	vim_make_str();	
	init_win_layout(argc,argv);

	while(1){
		ch = getch();
		getyx(stdscr, y, x);
		if(ch == KEY_RIGHT || ch == KEY_LEFT || ch == KEY_UP || ch == KEY_DOWN){
			vim_handle_navbar(ch);
		}else if(ch == 27){
			vim_mode = 0;
			vim_make_vim_mode();
		}else if(vim_mode == 0 && (vim_mode == ':' || vim_mode == '/')){
			//
		}else if(vim_mode == 0){
			vim_mode = 1;
			vim_make_vim_mode();
			move(y,x);
			refresh();
		}else if(vim_mode == 1 && ch == 8){
			if(x == 8){
				if(vim_diff){
					vim_remove(1);
					move(y,strlen(vim_str[y + vim_diff]) + 7);
				}else if(y > 0){
					vim_remove(0);
					move(y - 1,strlen(vim_str[y - 1]) + 7);
				}
			}else{
				vim_remove(0);
				move(y,x - 1);
			}
		}else if(vim_mode == 1){
			
			if(x + 1 >= vim_cols + 8 || ch == '\n'){
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
		
	}





	endwin();		
	return 0;
}
