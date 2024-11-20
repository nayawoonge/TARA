#include <ncurses.h>

int main()
{
	initscr();	//ncurses 시작 내부 세팅
	printw("hello world");	//hello world, printw

	refresh();	//화면 갱신

	getch();	//키를 눌러야 프로그램이 종료되도록, 안 쓰면 자동종료됨
	endwin();	//ncurses 종료

	return 0;
}
	
