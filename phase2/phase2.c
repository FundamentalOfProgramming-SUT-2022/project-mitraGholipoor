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

*/


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
			delete_virtual();
			vim_make_vim_mode();
			
			move(0,8);
			if(str_lines > 4){
				move(4,8);
			}
			
			refresh();
		}else if(vim_mode == 0 && (vim_mode == ':' || vim_mode == '/')){
			//
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
			vim_make_vir_coor();
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

		if(vim_mode == 2){
			int xx,yy;
			getyx(stdscr,yy,xx);
			select_virtual_area(yy,xx);
			move(yy,xx);
			refresh();
		}
		
	}





	endwin();		
	return 0;
}
