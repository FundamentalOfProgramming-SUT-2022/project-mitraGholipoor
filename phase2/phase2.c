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
	vim_set_str();	
	init_win_layout(argc,argv);

	while(1){
		ch = getch();
		getyx(stdscr, y, x);
		if(ch == KEY_RIGHT){
			if(x + 1 < strlen(vim_str[y])+8){
				move(y,x+1);
				refresh();
			}
		}else if(ch == KEY_LEFT){
			if(x - 1  >=  8){
				move(y,x-1);
				refresh();
			}
		}else if(ch == KEY_UP){
			if(y-1 >= 0){
				if(strlen(vim_str[y - 1]) + 8 >= x){
					move(y-1,x);
				}else{
					move(y-1,strlen(vim_str[y-1]) + 7);
				}
				refresh();
			}
		}else if(ch == KEY_DOWN){
			if(y+1 <= LINES -3 && y + 1 < str_lines){
					if(strlen(vim_str[y + 1]) + 8 >= x){
						move(y+1,x);
					}else{
						move(y+1,strlen(vim_str[y+1]) + 7);
					}
				refresh();
			}
		}
	
	}





	endwin();		
	return 0;
}
