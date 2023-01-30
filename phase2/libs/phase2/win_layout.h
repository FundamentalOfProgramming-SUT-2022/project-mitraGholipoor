void init_win_layout(int argc, char *argv[]){
    int x,y;
	FILE *fp;
	if(argc == 2){
		strcpy(vim_address,argv[1]);
		int err = vim_get_base_name();
		if(!err){
			has_name = 1;
		}
		
		fp = fopen(argv[1], "r");
		if(fp != NULL){
			vim_save = 1;
			fclose(fp);
			vim_make_screen_1();
		}else{
			vim_make_screen_2();
		}

	}else{
		vim_make_screen_2();
	}



	init_pair(1, COLOR_BLACK,COLOR_CYAN);
	move(LINES - 2,0);
	attron(COLOR_PAIR(1));
	printw(" NORMAL ");
	attroff(COLOR_PAIR(1));
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