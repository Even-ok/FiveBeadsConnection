#include<iostream>
#include"chess.h"
#include<ctime>
#include<cstdlib>
#include<string>
#include<vector>
#include<iterator>
#include<algorithm>
#include<cmath>
#define STEP_LENGTH 10;
using namespace std;

typedef struct mynode {//棋盘节点结构体定义
	int x;
	int y;
	int g;//起点到当前节点的长度（10/14）
	int h;//当前节点到终点的估算长度（曼哈顿）
	int attr;//状态
	struct mynode* parent;
}a_node;

enum attribute { ATTR_NULL, ATTR_OPEN, ATTR_CLOSE, ATTR_FORBID, ATTR_RIGHT };
//分别代表空指针，放在open_list中，放在close_list中，不可访问处和可以访问的地方

char chess_type[7] = { 'w','#','*','%','$','o','&' };//七个棋子形状

class menu {//菜单类，显示菜单
public:
	void setmenu() {
		cout << "******************************************************************************************" << endl;
		cout << "*                                     五子连珠                                           *" << endl;
		cout << "*                                                                                        *" << endl;
		cout << "* 游戏规则：                                                                             *" << endl;
		cout << "*1.参赛者分为甲、乙两方，游戏开始时随机产生7种不同的棋子。甲方先移动其中一个棋子至任意地 *" << endl;
		cout << "*方，若棋子不能消除，则棋子移动后棋盘上又随机产生3个棋子，此时乙方可移动棋子。每人每次只 *" << endl;
		cout << "*能移动一颗棋子。                                                                        *" << endl;
		cout << "*2.当同色的棋子有5颗连在一起排成横纵向或者斜向时，游戏者可以得10分。同时5颗棋子从棋盘上  *" << endl;
		cout << "*消失。连在一起的棋子每增加一颗，游戏者在原有分的基础上可多得2分。                       *" << endl;
		cout << "*3.若棋子在两个、三个或四个方向上消除，则该方获得一次移动棋子的机会。                    *" << endl;
		cout << "*4.棋子被占满时游戏结束，得分高者获胜。                                                  *" << endl;
		cout << "*                                                                                        *" << endl;
		cout << "******************************************************************************************" << endl;
		cout << endl;
	}

	void setxy() {
		cout << "   x→" << endl;
		cout << " y↓" << endl;
	}
};

a_chess_board chess_board;

void a_chess_board::chess_show() {//显示棋盘
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 39; j++)
			cout << a[i][j];
		cout << endl;
	}
}

void a_chess_board::set_chess_board() {
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 39; j++)
			a[i][j] = ' ';
	for (int i = 0; i < 39; i++)//第一行的数字排列
	{
		static char k1 = '1';
		if (i == 6 || i == 10 || i == 14 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38)
		{
			a[0][i] = k1;
			k1++;
		}
	}
	for (int j = 0; j < 18; j++)//第六列的数字排列
	{
		static int k2 = '1';
		if (j % 2 != 0)
		{
			a[j][5] = k2;
			k2++;
		}
	}
	for (int i = 0; i < 18; i++)//边界的设置
		for (int j = 6; j < 39; j++)
		{
			if (i % 2 == 0 && i != 0)
				if (j == 6 || j == 10 || j == 14 || j == 18 || j == 22 || j == 26 || j == 30 || j == 34 || j == 38)
					a[i][j] = '|';
			if (i % 2 != 0)
				if (j != 6 && j != 10 && j != 14 && j != 18 && j != 22 && j != 26 && j != 30 && j != 34 && j != 38)
					a[i][j] = '-';
		}

}

bool a_chess_board::is_chess(int row, int col) {//判断是否是下棋点
	if (row % 2 != 0)
		if (col == 6 || col == 10 || col == 14 || col == 18 || col == 22 || col == 26 || col == 30 || col == 34 || col == 38)
			return true;
	return false;
}

bool a_chess_board::is_free(int row, int col) {//判断该下棋点是否为空
	if (is_chess(row, col))
		if (a[row][col] == ' ')
			return true;
	return false;
}

int a_chess_board::less_than_3()
{
	int blank = 0;//空格数
	for (int i = 1; i < 18; i = i + 2)
		for (int j = 6; j < 39; j++)
		{
			if (j == 6 || j == 10 || j == 14 || j == 18 || j == 22 || j == 26 || j == 30 || j == 34 || j == 38)
				if (a[i][j] == ' ')
					blank++;
		}
	if (blank < 3)
		return blank;
	else
		return 0;
}

int a_chess_board::row_num(int x, int y)//计算左右相同棋子数
{
	int sum_of_row = 0;
	if (y - 4 >= 6 && a[x][y] == a[x][y - 4] && a[x][y] != ' ')
		for (int t = y - 4; a[x][y] == a[x][t]; t -= 4)
		{
			sum_of_row++;
			if (t - 4 < 6)
				break;
		}
	if (y + 4 <= 38 && a[x][y] == a[x][y + 4] && a[x][y] != ' ')
		for (int t = y + 4; a[x][y] == a[x][t]; t += 4)
		{
			sum_of_row++;
			if (t + 4 > 38)
				break;
		}
	return sum_of_row;
}

int a_chess_board::col_num(int x, int y)
{
	int sum_of_col = 0;
	if (x - 2 >= 1 && a[x][y] == a[x - 2][y] && a[x][y] != ' ')//计算上下相同棋子数
		for (int t = x - 2; a[x][y] == a[t][y]; t -= 2)
		{
			sum_of_col++;
			if (t - 2 < 1)
				break;
		}
	if (x + 2 <= 17 && a[x][y] == a[x + 2][y] && a[x][y] != ' ')
		for (int t = x + 2; a[x][y] == a[t][y]; t += 2)
		{
			sum_of_col++;
			if (t + 2 > 17)
				break;
		}
	return sum_of_col;
}

int a_chess_board::left_num(int x,int y)
{
	int sum_of_left = 0;
	if (x + 2 <= 17 && y - 4 >= 6 && a[x][y] == a[x + 2][y - 4] && a[x][y] != ' ')//计算左斜相同棋子数
		for (int t = x + 2, q = y - 4; a[x][y] == a[t][q]; t += 2, q -= 4)
		{
			sum_of_left++;
			if (t + 2 > 17 || q - 4 < 6)
				break;
		}
	if (x - 2 >= 1 && y + 4 <= 38 && a[x][y] == a[x - 2][y + 4] && a[x][y] != ' ')
		for (int t = x - 2, q = y + 4; a[x][y] == a[t][q]; t -= 2, q += 4)
		{
			sum_of_left++;
			if (t - 2 < 1 || q + 4 > 38)
				break;
		}
	return sum_of_left;
}

int a_chess_board::right_num(int x, int y)//计算右斜相同棋子数
{
	int sum_of_right = 0;
	if (x - 2 >= 1 && y - 4 >= 6 && a[x][y] == a[x - 2][y - 4] && a[x][y] != ' ')
		for (int t = x - 2, q = y - 4; a[x][y] == a[t][q]; t -= 2, q -= 4)
		{
			sum_of_right++;
			if (t - 2 < 1 || q - 4 < 6)
				break;
		}
	if (x + 2 <= 17 && y + 4 <= 38 && a[x][y] == a[x + 2][y + 4] && a[x][y] != ' ')
		for (int t = x + 2, q = y + 4; a[x][y] == a[t][q]; t += 2, q += 4)
		{
			sum_of_right++;
			if (t + 2 > 17 || q + 4 > 38)
				break;
		}
	return sum_of_right;

}

void a_chess_board::move(int x0,int y0,int x1,int y1)//移动棋子函数
{
	int x_0, y_0, x_1, y_1;
	y_0 = 2 * y0 - 1;//坐标转换
	y_1 = 2 * y1 - 1;
	x_0 = x0 + 5 + 3 * (x0 - 1);
	x_1 = x1 + 5 + 3 * (x1 - 1);
	a[y_1][x_1] = a[y_0][x_0];
	a[y_0][x_0] = ' ';
}

//消除函数
void a_chess_board::elim_row(int x,int y)
{
	if (y - 4 >= 6 && a[x][y] == a[x][y - 4] && a[x][y] != ' ')
		for (int t = y - 4; a[x][y] == a[x][t]; t -= 4)
		{
			a[x][t] = ' ';
			if (t - 4 < 6)
				break;
		}
	if (y + 4 <= 38 && a[x][y] == a[x][y + 4] && a[x][y] != ' ')
		for (int t = y + 4; a[x][y] == a[x][t]; t += 4)
		{
			a[x][t] = ' ';
			if (t + 4 > 38)
				break;
		}
}

void a_chess_board::elim_col(int x,int y)
{
	if (x - 2 >= 1 && a[x][y] == a[x - 2][y] && a[x][y] != ' ')//计算上下相同棋子数
		for (int t = x - 2; a[x][y] == a[t][y]; t -= 2)
		{
			a[t][y] = ' ';
			if (t - 2 < 1)
				break;
		}
	if (x + 2 <= 17 && a[x][y] == a[x + 2][y] && a[x][y] != ' ')
		for (int t = x + 2; a[x][y] == a[t][y]; t += 2)
		{
			a[t][y] = ' ';
			if (t + 2 > 17)
				break;
		}
}

void a_chess_board::elim_left(int x, int y)
{
	if (x + 2 <= 17 && y - 4 >= 6 && a[x][y] == a[x + 2][y - 4] && a[x][y] != ' ')//计算左斜相同棋子数
		for (int t = x + 2, q = y - 4; a[x][y] == a[t][q]; t += 2, q -= 4)
		{
			a[t][q] = ' ';
			if (t + 2 > 17 || q - 4 < 6)
				break;
		}
	if (x - 2 >= 1 && y + 4 <= 38 && a[x][y] == a[x - 2][y + 4] && a[x][y] != ' ')
		for (int t = x - 2, q = y + 4; a[x][y] == a[t][q]; t -= 2, q += 4)
		{
			a[t][q] = ' ';
			if (t - 2 < 1 || q + 4 > 38)
				break;
		}
}

void a_chess_board::elim_right(int x,int y)
{
	if (x - 2 >= 1 && y - 4 >= 6 && a[x][y] == a[x - 2][y - 4] && a[x][y] != ' ')
		for (int t = x - 2, q = y - 4; a[x][y] == a[t][q]; t -= 2, q -= 4)
		{
			a[t][q] = ' ';
			if (t - 2 < 1 || q - 4 < 6)
				break;
		}
	if (x + 2 <= 17 && y + 4 <= 38 && a[x][y] == a[x + 2][y + 4] && a[x][y] != ' ')
		for (int t = x + 2, q = y + 4; a[x][y] == a[t][q]; t += 2, q += 4)
		{
			a[t][q] = ' ';
			if (t + 2 > 17 || q + 4 > 38)
				break;
		}

}

int a_chess_board::score(int x1, int y1, int index)//计分函数
{
	int sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0, score;
	int x = 2 * y1 - 1;
	int y = x1 + 5 + 3 * (x1 - 1);
	switch (index)
	{
	case 1://仅横向消除
	{
		sum_of_row = row_num(x, y);
		elim_row(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + 1) * 2;
		return score;
	}; break;
	case 2://仅纵向消除
	{
		sum_of_col = col_num(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_col + 1) * 2;
		return score;
	}; break;
	case 3://仅左斜向消除
	{
		sum_of_left = left_num(x, y);
		elim_left(x, y);
		a[x][y] = ' ';
		score = (sum_of_left + 1) * 2;
		return score;
	}; break;
	case 4://仅右斜向消除
	{
		sum_of_right = right_num(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_right + 1) * 2;
		return score;
	}; break;
	case 5://横、竖向消除
	{
		sum_of_row = row_num(x, y);
		sum_of_col = col_num(x, y);
		elim_row(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}break;
	case 6://横、左向消除
	{
		sum_of_row = row_num(x, y);
		sum_of_left = left_num(x, y);
		elim_row(x, y);
		elim_left(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_left + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 7://横、右向消除
	{
		sum_of_row = row_num(x, y);
		sum_of_right = right_num(x, y);
		elim_row(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 8://左斜、竖向消除
	{
		sum_of_left = left_num(x, y);
		sum_of_col = col_num(x, y);
		elim_left(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_left + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 9://右斜、竖向消除
	{
		sum_of_right = right_num(x, y);
		sum_of_col = col_num(x, y);
		elim_right(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_right + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 10://左、右斜向消除
	{
		sum_of_left = left_num(x, y);
		sum_of_right = right_num(x, y);
		elim_left(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 11://横、竖、左斜向消除
	{
		sum_of_row = row_num(x, y);
		sum_of_col = col_num(x, y);
		sum_of_left = left_num(x, y);
		elim_row(x, y);
		elim_col(x, y);
		elim_left(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_left + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 12://横、竖、右斜向消除
	{
		sum_of_row = row_num(x, y);
		sum_of_col = col_num(x, y);
		sum_of_right = right_num(x, y);
		elim_row(x, y);
		elim_col(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_right + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 13://横、左、右向消除
	{
		sum_of_row = row_num(x, y);
		sum_of_right = right_num(x, y);
		sum_of_left = left_num(x, y);
		elim_row(x, y);
		elim_right(x, y);
		elim_left(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 14://竖、左、右向消除
	{
		sum_of_col = col_num(x, y);
		sum_of_left = left_num(x, y);
		sum_of_right = right_num(x, y);
		elim_col(x, y);
		elim_left(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_col + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 15://四个方向均消除
	{
		sum_of_row = row_num(x, y);
		sum_of_col = col_num(x, y);
		sum_of_left = left_num(x, y);
		sum_of_right = right_num(x, y);
		elim_row(x, y);
		elim_col(x, y);
		elim_left(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_col + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	}
}

void a_chess_board::random_chess_less_than3(int times) {//产生1/2/3个随机棋子
	srand((int)time(0));
	for (int i = 0; i < times; i++)
	{
		do
		{
			rand_1 = rand() % 18;
			rand_2 = rand() % 39;
		} while ((!is_chess(rand_1, rand_2)) || (!is_free(rand_1, rand_2)));
		rand_3 = rand() % 7;
		a[rand_1][rand_2] = chess_type[rand_3];
		if (judge.judge_elim(rand_1, rand_2)) {
			int new_x = (rand_2 - 2) / 4;
			int new_y = (rand_1 + 1) / 2;
			int index = judge.judge_elim(new_x, new_y);
			int s = score(new_x, new_y, index);
		}
	}
}

void  a_chess_board::random_chess_7() {//产生七个不同的随机棋子
	srand((int)time(0));
	int chess[7];
	for (int i = 0; i < 7; i++)
	{
		bool repeated = false;
		do
		{
			rand_1 = rand() % 18;
			rand_2 = rand() % 39;
		} while ((!is_chess(rand_1, rand_2)) || (!is_free(rand_1, rand_2)));
		chess[i] = rand() % 7;
		for (int j = 0; j < i; j++)//与之前产生的棋子进行比较，若相同，则再进入一次循环
			if (chess[i] == chess[j])
			{
				repeated = true;
				break;
			}
		if (repeated)
			i--;
		else
			a[rand_1][rand_2] = chess_type[chess[i]];
	}
}

bool Judge::valid_cin() {
	try//检查输入类型是否正确
	{
		if (cin.fail())
			throw 'a';
	}
	catch (char) {
		cin.clear();
		//cin.ignore();
		cin.ignore(std::numeric_limits< streamsize >::max(), '\n');
		cout << "输入有误，请重新输入！" << endl;
		return false;
	}
	return true;
}

bool Judge::end(a_chess_board cb) {//检查棋盘有没有被占满
	bool end_judge = true;
	for (int i = 1; i < 18; i = i + 2)
		for (int j = 6; j < 39; j++)
		{
			if (j == 6 || j == 10 || j == 14 || j == 18 || j == 22 || j == 26 || j == 30 || j == 34 || j == 38)
				if (cb.a[i][j] == ' ')
					end_judge = false;
		}
	return end_judge;
}

bool Judge::judge_move(int x0, int y0, int x1, int y1, a_chess_board cb) {//检查输入的位置是否有效
	int x_0, y_0, x_1, y_1;
	y_0 = 2 * y0 - 1;
	y_1 = 2 * y1 - 1;
	x_0 = x0 + 5 + 3 * (x0 - 1);
	x_1 = x1 + 5 + 3 * (x1 - 1);
	if (cb.is_free(y_0, x_0))
	{
		cout << "选择初始棋子有误，请重新选择" << endl;
		return false;
	}
	else if (!(cb.is_free(y_1, x_1)))
	{
		cout << "选择落子位置有误，请重新选择" << endl;
		return false;
	}
	else if (x0 < 1 || x0>9 || y0 < 1 || y0>9 || x1 < 1 || x1>9 || y1 < 1 || y1>9)
	{
		cout << "输入的范围错误，请重新选择" << endl;
		return false;
	}
	else
		return true;
}

int Judge::judge_elim(int x1, int y1)//以终点为中心，检查其周围能否消除。若能消除，返回消除方式序号。
{
	int x = 2 * y1 - 1;
	int y = x1 + 5 + 3 * (x1 - 1);
	int sum = 0, sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0;//分别代表横轴，纵轴，左斜，右斜的分数
	sum_of_row = chess_board.row_num(x, y);
	if (sum_of_row < 4)//若相同棋子数小于4个，则变为0，下同
		sum_of_row = 0;

	sum_of_col = chess_board.col_num(x, y);
	if (sum_of_col < 4)
		sum_of_col = 0;

	sum_of_left = chess_board.left_num(x, y);
	if (sum_of_left < 4)
		sum_of_left = 0;

	sum_of_right = chess_board.right_num(x, y);
	if (sum_of_right < 4)
		sum_of_right = 0;

	if (sum_of_row == 0 && sum_of_col == 0 && sum_of_left == 0 && sum_of_right == 0)
		return 0;
	else if (sum_of_row > 0 && sum_of_col == 0 && sum_of_left == 0 && sum_of_right == 0)
		return 1;
	else if (sum_of_row == 0 && sum_of_col > 0 && sum_of_left == 0 && sum_of_right == 0)
		return 2;
	else if (sum_of_row == 0 && sum_of_col == 0 && sum_of_left > 0 && sum_of_right == 0)
		return 3;
	else if (sum_of_row == 0 && sum_of_col == 0 && sum_of_left == 0 && sum_of_right > 0)
		return 4;
	else if (sum_of_row > 0 && sum_of_col > 0 && sum_of_left == 0 && sum_of_right == 0)
		return 5;
	else if (sum_of_row > 0 && sum_of_col == 0 && sum_of_left > 0 && sum_of_right == 0)
		return 6;
	else if (sum_of_row > 0 && sum_of_col == 0 && sum_of_left == 0 && sum_of_right > 0)
		return 7;
	else if (sum_of_row == 0 && sum_of_col > 0 && sum_of_left > 0 && sum_of_right == 0)
		return 8;
	else if (sum_of_row == 0 && sum_of_col > 0 && sum_of_left == 0 && sum_of_right > 0)
		return 9;
	else if (sum_of_row == 0 && sum_of_col == 0 && sum_of_left > 0 && sum_of_right > 0)
		return 10;
	else if (sum_of_row > 0 && sum_of_col > 0 && sum_of_left > 0 && sum_of_right == 0)
		return 11;
	else if (sum_of_row > 0 && sum_of_col > 0 && sum_of_left == 0 && sum_of_right > 0)
		return 12;
	else if (sum_of_row > 0 && sum_of_col == 0 && sum_of_left > 0 && sum_of_right > 0)
		return 13;
	else if (sum_of_row == 0 && sum_of_col > 0 && sum_of_left > 0 && sum_of_right > 0)
		return 14;
	else if (sum_of_row > 0 && sum_of_col > 0 && sum_of_left > 0 && sum_of_right > 0)
		return 15;
}

void Judge::judge_win(int ascore, int bscore) {//判断双方哪一方胜利，并结束游戏。
	if (ascore > bscore)
	{
		cout << endl;
		cout << "游戏结束，甲方胜利！" << endl;
	}
	else if (ascore < bscore)
	{
		cout << endl;
		cout << "游戏结束，乙方胜利！" << endl;
	}
	else
	{
		cout << endl;
		cout << "游戏结束，双方打成平局！" << endl;
	}
	cout << "甲" << "   " << "乙" << endl;
	cout << ascore << " : " << bscore << endl;
	cout << endl;
	cout << "**********************************************游戏已结束**********************************************" << endl;
	system("pause");
}

int max_4(int a, int b, int c, int d) {
	int M = 0;
	M = max(a, b);
	M = max(M, c);
	M = max(M, d);
	return M;
}

class A_star {//因为a_node没有定义在头文件中，所以不把A星类放在头文件声明
private:
	a_node* _start=NULL;//要把起点的g和h设为0
	a_node* _end=NULL;
	a_node star_chess_board[18][39];//节点数组

public:
	vector<a_node*> open_list;
	A_star(int start_x, int start_y, int end_x, int end_y, a_chess_board cb);
	void set_star_chess_board(a_chess_board cb);//初始化节点数组的函数
	a_node* close_cur();//将父节点移出open_list
	int insert_neighbor_to_openlist(a_node* curr_node);//将周围有效节点加入open_list
};
A_star::A_star(int start_x, int start_y, int end_x, int end_y, a_chess_board cb) {
	set_star_chess_board(cb);
	_start = &star_chess_board[2 * start_y - 1][start_x + 5 + 3 * (start_x - 1)];
	star_chess_board[_start->x][_start->y].attr = ATTR_CLOSE;
	_end = &star_chess_board[2 * end_y - 1][end_x + 5 + 3 * (end_x - 1)];
	open_list.push_back(_start);
	_start->g = 0;
	_start->h = 0;
}

void A_star::set_star_chess_board(a_chess_board cb) {
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 39; j++)
		{
			star_chess_board[i][j].x = i;
			star_chess_board[i][j].y = j;
			if (cb.is_chess(i, j) && cb.is_free(i, j))
				star_chess_board[i][j].attr = ATTR_NULL;

			else
				star_chess_board[i][j].attr = ATTR_FORBID;
		}
}

a_node* A_star::close_cur()
{
	a_node* curr_node = open_list.at(0);
	open_list.erase(open_list.begin());
	curr_node->attr = ATTR_CLOSE;
	return curr_node;
}

int A_star::insert_neighbor_to_openlist(a_node* curr_node)//如果不是棋子，则放到forbid中，如果被占位了，也放到forbid
{
	int space = 0;
	a_node* tmp = NULL;
	vector<a_node*>::iterator is_contain;
	//取到当前节点的上下左右节点加入open列表 并设置这些节点的parent为当前节点
	//坐标系与数组下标有所不同
	//上
	if (curr_node->x > 1)
	{
		tmp = &star_chess_board[curr_node->x - 2][curr_node->y];
		is_contain = std::find(open_list.begin(), open_list.end(), tmp);//看它是否在openlist里边
		if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
		{
			tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
			tmp->g = curr_node->g + STEP_LENGTH;
			tmp->parent = curr_node;
			open_list.push_back(tmp);
			space++;
		}
		if (tmp->x == _end->x && tmp->y == _end->y)
			return 2;
	}
	//下
	if (curr_node->x < 17)
	{
		tmp = &star_chess_board[curr_node->x + 2][curr_node->y];
		is_contain = std::find(open_list.begin(), open_list.end(), tmp);
		if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
		{
			tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
			tmp->g = curr_node->g + STEP_LENGTH;
			tmp->parent = curr_node;
			open_list.push_back(tmp);
			space++;
		}
		if (tmp->x == _end->x && tmp->y == _end->y)
			return 2;
	}

	//左
	if (curr_node->y > 6)
	{
		tmp = &star_chess_board[curr_node->x][curr_node->y - 4];
		is_contain = std::find(open_list.begin(), open_list.end(), tmp);
		if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
		{
			tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
			tmp->g = curr_node->g + STEP_LENGTH;
			tmp->parent = curr_node;
			open_list.push_back(tmp);
			space++;
		}
		if (tmp->x == _end->x && tmp->y == _end->y)
			return 2;
	}

	//右
	if (curr_node->y < 38)
	{
		tmp = &star_chess_board[curr_node->x][curr_node->y + 4];
		is_contain = std::find(open_list.begin(), open_list.end(), tmp);
		if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
		{
			tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
			tmp->g = curr_node->g + STEP_LENGTH;
			tmp->parent = curr_node;
			open_list.push_back(tmp);
			space++;
		}
		if (tmp->x == _end->x && tmp->y == _end->y)
			return 2;
	}
	if (space == 0 && open_list.size() == 0)
		return 0;//如果tmp一直是空指针，返回错误！跳出循环
	else if (find(open_list.begin(), open_list.end(), _end) == open_list.end())//没找到
		return 1;
}

bool comp_f(a_node* a, a_node* b)//返回f值大的节点
{
	//比较f值
	int a_f = a->g + a->h;
	int b_f = b->g + b->h;
	return a_f < b_f;
}

int main()
{
	menu menu;
	int A_score = 0, B_score = 0;
	menu.setmenu();
	menu.setxy();
	chess_board.set_chess_board();
	chess_board.random_chess_7();
	chess_board.chess_show();
	for (int time = 0;; time++)
	{
		int x0, y0, x1, y1, x2, y2, x3, y3;
	more1:
		do
		{
			cout << "甲方：\t\t\t\t\t甲方目前得分：" << A_score << "\t\t" << "乙方目前得分：" << B_score << endl;
			cout << "初始位置:";
			cin >> x0 >> y0;
			if (!judge.valid_cin())//判断输入数据类型是否正确
				goto more1;//若输入不正确，重新跳转到输入界面
			cout << "目标位置:";
			cin >> x1 >> y1;
			if (!judge.valid_cin())
				goto more1;
		} while (!(judge.judge_move(x0, y0, x1, y1,chess_board)));
		A_star search_path = A_star(x0, y0, x1, y1,chess_board);//A星类的数据先进行初始化
		while (search_path.open_list.size()!=0)
		{
			a_node* curr = search_path.close_cur();
			int can_find = search_path.insert_neighbor_to_openlist(curr);
			if (can_find == 0)//若某个父节点周围的格子都不能走
			{
				cout << "不能移动到指定位置，请重新输入！" << endl;
				goto more1;
			}
			else if (can_find == 2)//找到终点
			{
				cout << "可移动到当前位置！" << endl;
				chess_board.move(x0, y0, x1, y1);
				break;
			}
			sort(search_path.open_list.begin(), search_path.open_list.end(), comp_f);//对open_list进行排序，取f值最小的一个
		}
		if (judge.judge_elim(x1, y1))
		{
			int index = judge.judge_elim(x1, y1);
			A_score += chess_board.score(x1, y1, index);
			if (index > 4)//多方向消除
			{
				system("cls");
				menu.setmenu();
				menu.setxy();
				chess_board.chess_show();
				cout << "恭喜你多获得一次移动棋子的机会！" << endl;
				goto more1;
			}
		}
		else
		{
			if (chess_board.less_than_3())//剩余棋子少于3
			{
				int blank = chess_board.less_than_3();
				switch (blank) {
				case 1:chess_board.random_chess_less_than3(1); break;
				case 2:chess_board.random_chess_less_than3(2); break;
				}//随机生成1/2棋子函数中有消除函数，如果之后能消除，则棋盘未满
				system("cls");
				menu.setmenu();
				menu.setxy();
				chess_board.chess_show();
				if (judge.end(chess_board))//判断棋盘有没有满
				{
					judge.judge_win(A_score, B_score);
					return 0;
				}
			}
			else
				chess_board.random_chess_less_than3(3);//若无消除且棋盘剩余空格大于等于3，则生成3个随机棋子
		}
		system("cls");
		menu.setmenu();
		menu.setxy();
		chess_board.chess_show();
		if (judge.end(chess_board))//判断生成随机棋子后棋盘是否被占满
		{
			judge.judge_win(A_score, B_score);
			return 0;
		}
	more2:
		do
		{
			cout << "乙方：\t\t\t\t\t甲方目前得分：" << A_score << "\t\t" << "乙方目前得分：" << B_score << endl;
			cout << "初始位置:";
			cin >> x2 >> y2;
			if (!judge.valid_cin())
				goto more2;
			cout << "目标位置:";
			cin >> x3 >> y3;
			if (!judge.valid_cin())
				goto more2;
		} while (!judge.judge_move(x2, y2, x3, y3,chess_board));
		search_path = A_star(x2, y2, x3, y3, chess_board);
		while (search_path.open_list.size() != 0)
		{
			a_node* curr = search_path.close_cur();
			int can_find = search_path.insert_neighbor_to_openlist(curr);
			if (can_find == 0)
			{
				cout << "不能移动到指定位置，请重新输入！" << endl;
				goto more2;
			}
			else if (can_find == 2)
			{
				cout << "可移动到当前位置！" << endl;
				chess_board.move(x2, y2, x3, y3);
				break;
			}
			sort(search_path.open_list.begin(), search_path.open_list.end(), comp_f);
		}
		if (judge.judge_elim(x3, y3))
		{
			int index = judge.judge_elim(x3, y3);
			B_score += chess_board.score(x3, y3, index);
			if (index > 4)
			{
				system("cls");
				menu.setmenu();
				menu.setxy();
				chess_board.chess_show();
				cout << "恭喜你多获得一次移动棋子的机会！" << endl;
				goto more2;
			}
		}
		else
		{
			if (chess_board.less_than_3())
			{
				int blank = chess_board.less_than_3();
				switch (blank) {
				case 1:chess_board.random_chess_less_than3(1); break;
				case 2:chess_board.random_chess_less_than3(2); break;
				}
				system("cls");
				menu.setmenu();
				menu.setxy();
				chess_board.chess_show();
				if (judge.end(chess_board))
				{
					judge.judge_win(A_score, B_score);
					return 0;
				}
			}
			else
				chess_board.random_chess_less_than3(3);
		}
		system("cls");
		menu.setmenu();
		menu.setxy();
		chess_board.chess_show();
		if (judge.end(chess_board))
		{
			judge.judge_win(A_score, B_score);
			return 0;
		}
	}
}