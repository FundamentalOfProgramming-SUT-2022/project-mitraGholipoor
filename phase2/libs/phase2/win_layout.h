void init_win_layout(int argc, char *argv[]){

	make_here_root();
	set_clipboard();
	make_vim_file();
	vim_make_screen_2();
	vim_mode = 0;
	vim_make_vim_mode();
}
