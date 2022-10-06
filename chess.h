#pragma once
class a_chess_board {
private:
	char a[18][39];//装载棋盘符号
	int rand_1, rand_2, rand_3;//3个生成随机棋子的参数
	friend class A_star;
	friend class Judge;
public:
	void set_chess_board();//棋盘初始化
	void chess_show();
	bool is_chess(int row, int col);
	bool is_free(int row, int col);
	void move(int x0, int y0, int x1, int y1);//移动棋子
	int less_than_3();//判断棋盘是否少于3个空格
	int row_num(int x, int y);//计算横向相连棋子数，下同
	int col_num(int x, int y);
	int left_num(int x, int y);
	int right_num(int x, int y);
	void elim_row(int x, int y);//消除横向相连棋子，下同
	void elim_col(int x, int y);
	void elim_left(int x, int y);
	void elim_right(int x, int y);
	int score(int x1, int y1, int index);//计算分数，并消除棋子
	void random_chess_less_than3(int times);//随机生成1/2/3个棋子
	void random_chess_7();
};
class Judge {
public:
	bool valid_cin();//判断输入是否是有效的

	bool end(a_chess_board cb);//判断游戏是否结束

	bool judge_move(int x0, int y0, int x1, int y1, a_chess_board cb);//判断是否能移动

	int judge_elim(int x1, int y1);//判断是否能消除

	void judge_win(int ascore, int bscore);//判断胜利方并结束游戏
}judge;

