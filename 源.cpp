#include<iostream>
#include<ctime>
#include<cstdlib>
#include<string>
#include<vector>
#include<iterator>
#include<algorithm>
#include<cmath>
#define STEP_LENGTH 10;
using namespace std;
typedef struct mynode {
	int x;
	int y;
	int g;//起点到当前节点的长度（10/14）
	int h;//当前节点到终点的估算长度（曼哈顿）
	int attr;//状态
	struct mynode* parent;
}a_node;

enum attribute { ATTR_NULL, ATTR_OPEN, ATTR_CLOSE, ATTR_FORBID, ATTR_RIGHT };
//分别代表空指针，放在open_list中，放在close_list中，不可访问处和可以访问的地方

class A_star {
private:
	vector<a_node*> open_list;
	a_node* _start;//要把起点的g和h设为0
	a_node* _end;
	a_node star_chess_board[18][39];

public:
	A_star(int start_x, int start_y, int end_x, int end_y) {
		_start->x = start_x;
		_start->y = start_y;
		_end->x = end_x;
		_end->y = end_y;
		open_list.push_back(_start);
		_start->g = 0;
		_start->h = 0;

	}
	void set_star_chess_board(a_chess_board cb) {
		for(int i=0;i<18;i++)
			for (int j = 0; j < 39; j++)
			{
				star_chess_board[i][j].x = i;
				star_chess_board[i][j].y = j;
				if (cb.is_chess(i, j) && cb.is_free(i, j))
					star_chess_board[i][j].attr = ATTR_NULL;
				else if(cb.is_chess(i, j) && !cb.is_free(i, j)&&i!=_start->x&& i != _end->x&&j != _start->y&& j != _end->y)
					star_chess_board[i][j].attr = ATTR_FORBID;
			}
	}
	void sort_open_list() {
		sort(open_list.begin(), open_list.end(), comp_f);  
	}
	void close_cur() 
	{
		a_node* curr_node = open_list.at(0);//curr_node要随着改变，还是定义在主函数循环里边把！
		open_list.erase(open_list.begin());
		curr_node->attr = ATTR_CLOSE;
	}
	int insert_neighbor_to_openlist(a_node* curr_node, a_node* _end, a_node _chessboard[18][39])//如果不是棋子，则放到forbid中，如果被占位了，也放到forbid
	{
		a_node* tmp = NULL;
		vector<a_node*>::iterator is_contain;
		//取到当前节点的上下左右节点加入open列表 并设置这些节点的parent为当前节点
		//坐标系与数组下标有所不同
		//上
		if (curr_node->x > 1)
		{
			tmp = &_chessboard[curr_node->x - 2][curr_node->y];
			is_contain = std::find(open_list.begin(), open_list.end(), tmp);//看它是否在openlist里边
			if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
			{
				tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
				tmp->g = curr_node->g + STEP_LENGTH;
				tmp->parent = curr_node;
				open_list.push_back(tmp);
			}
			//下
			if (curr_node->x < 17)
			{
				tmp = &_chessboard[curr_node->x + 2][curr_node->y];
				is_contain = std::find(open_list.begin(), open_list.end(), tmp);
				if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
				{
					tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
					tmp->g = curr_node->g + STEP_LENGTH;
					tmp->parent = curr_node;
					open_list.push_back(tmp);
				}
			}

			//左
			if (curr_node->y > 6)
			{
				tmp = &_chessboard[curr_node->x][curr_node->y - 4];
				is_contain = std::find(open_list.begin(), open_list.end(), tmp);
				if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
				{
					tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
					tmp->g = curr_node->g + STEP_LENGTH;
					tmp->parent = curr_node;
					open_list.push_back(tmp);
				}
			}

			//右
			if (curr_node->y < 38)
			{
				tmp = &_chessboard[curr_node->x][curr_node->y + 4];
				is_contain = std::find(open_list.begin(), open_list.end(), tmp);
				if (tmp->attr != ATTR_FORBID && tmp->attr != ATTR_CLOSE && is_contain == open_list.end())
				{
					tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
					tmp->g = curr_node->g + STEP_LENGTH;
					tmp->parent = curr_node;
					open_list.push_back(tmp);
				}
			}
			if (tmp == NULL)
				return 0;//如果tmp一直是空指针，返回错误！跳出循环
			else if (find(open_list.begin(), open_list.end(), _end) == open_list.end())//没找到
				return 1;
			else if (find(open_list.begin(), open_list.end(), _end) != open_list.end())
				return 2;//如果找到，跳出循环
		}

	}
	bool comp_f(a_node* a, a_node* b)//定义在类中
	{
		//比较f值
		int a_f = a->g + a->h;
		int b_f = b->g + b->h;
		return a_f < b_f;
	}
};

char chess_type[7] = { 'w','#','*','%','$','o','&' };

class menu {
	void setmenu() {
		cout << "******************************************************************************************" << endl;
		cout << "*                                     五子连珠                                           *" << endl;
		cout << "*                                                                                        *" << endl;
		cout << "* 游戏规则：                                                                             *" << endl;
		cout << "*1.参赛者分为甲、乙两方，游戏开始时随机产生7种不同的棋子。甲方先移动其中一个棋子至任意地 *" << endl;
		cout << "*方，棋子移动后棋盘上又随机产生3个棋子，此时乙方可移动棋子。每人每次只能移动一颗棋子。   *" << endl;
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

class a_chess_board {
private:
	char a[18][39];
	friend class random_chess;
	friend class A_star;
public:
	void set_chess_board() {
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

	void chess_show() {
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 39; j++)
				cout << a[i][j];
			cout << endl;
		}
	}

	bool end() {
		bool judge = true;
		for (int i = 1; i < 18; i = i + 2)
			for (int j = 6; j < 39; j++)
			{
				if (j == 6 || j == 10 || j == 14 || j == 18 || j == 22 || j == 26 || j == 30 || j == 34 || j == 38)
					if (a[i][j] == ' ')
						judge = false;
			}
		return judge;
	}

	bool is_chess(int row, int col) {
		if (row % 2 != 0)
			if (col == 6 || col == 10 || col == 14 || col == 18 || col == 22 || col == 26 || col == 30 || col == 34 || col == 38)
				return true;
		return false;
	}

	bool is_free( int row, int col) {
		if (row % 2 != 0)
			if (col == 6 || col == 10 || col == 14 || col == 18 || col == 22 || col == 26 || col == 30 || col == 34 || col == 38)
				if (a[row][col] == ' ')
					return true;
		return false;
	}


	bool move(int x0, int y0, int x1, int y1) {
		int x_0, y_0, x_1, y_1;
		y_0 = 2 * y0 - 1;
		y_1 = 2 * y1 - 1;
		x_0 = x0 + 5 + 3 * (x0 - 1);
		x_1 = x1 + 5 + 3 * (x1 - 1);
		if (is_free(y_0, x_0))
		{
			cout << "选择初始棋子有误，请重新选择" << endl;
			return false;
		}
		else if (!(is_free(y_1, x_1)))
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
		{
			a[y_1][x_1] = a[y_0][x_0];
			a[y_0][x_0] = ' ';
			return true;
		}

	}

	int less_than_3()
	{
		int blank = 0;
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

	int row_num(int x, int y)
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

	int col_num(int x, int y)
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

	int left_num(int x, int y)
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

	int right_num(int x, int y)
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

	int judge_elim(int x1, int y1)
	{
		int x = 2 * y1 - 1;
		int y = x1 + 5 + 3 * (x1 - 1);
		int sum = 0, sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0;//分别代表横轴，纵轴，左斜，右斜的分数
		sum_of_row = row_num(x, y);
		if (sum_of_row < 4)//若相同棋子数小于4个，则变为0，下同
			sum_of_row = 0;

		sum_of_col = col_num(x, y);
		if (sum_of_col < 4)
			sum_of_col = 0;

		sum_of_left = left_num(x, y);
		if (sum_of_left < 4)
			sum_of_left = 0;

		sum_of_right = right_num(x, y);
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

	void elim_row(int x, int y)
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

	void elim_col( int x, int y)
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

	void elim_left(int x, int y)
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

	void elim_right(int x, int y)
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

	int score(int x1, int y1, int index)
	{
		int sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0, score;
		int x = 2 * y1 - 1;
		int y = x1 + 5 + 3 * (x1 - 1);
		switch (index)
		{
		case 1:
		{
			sum_of_row = row_num(x, y);
			elim_row(x, y);
			a[x][y] = ' ';
			score = (sum_of_row + 1) * 2;
			return score;
		}; break;
		case 2:
		{
			sum_of_col = col_num(x, y);
			elim_col(x, y);
			a[x][y] = ' ';
			score = (sum_of_col + 1) * 2;
			return score;
		}; break;
		case 3:
		{
			sum_of_left = left_num(x, y);
			elim_left(x, y);
			a[x][y] = ' ';
			score = (sum_of_left + 1) * 2;
			return score;
		}; break;
		case 4:
		{
			sum_of_right = right_num(x, y);
			elim_right(x, y);
			a[x][y] = ' ';
			score = (sum_of_right + 1) * 2;
			return score;
		}; break;
		case 5:
		{
			sum_of_row = row_num(x, y);
			sum_of_col = col_num(x, y);
			elim_row(x, y);
			elim_col(x, y);
			a[x][y] = ' ';
			score = (sum_of_row + sum_of_col + 1 - 5) * 2 + 10;
			return score;
		}break;
		case 6:
		{
			sum_of_row = row_num(x, y);
			sum_of_left = left_num(x, y);
			elim_row(x, y);
			elim_left(x, y);
			a[x][y] = ' ';
			score = (sum_of_row + sum_of_left + 1 - 5) * 2 + 10;
			return score;
		}; break;
		case 7:
		{
			sum_of_row = row_num(x, y);
			sum_of_right = right_num(x, y);
			elim_row(x, y);
			elim_right(x, y);
			a[x][y] = ' ';
			score = (sum_of_row + sum_of_right + 1 - 5) * 2 + 10;
			return score;
		}; break;
		case 8:
		{
			sum_of_left = left_num(x, y);
			sum_of_col = col_num(x, y);
			elim_left(x, y);
			elim_col(x, y);
			a[x][y] = ' ';
			score = (sum_of_left + sum_of_col + 1 - 5) * 2 + 10;
			return score;
		}; break;
		case 9:
		{
			sum_of_right = right_num(x, y);
			sum_of_col = col_num(x, y);
			elim_right(x, y);
			elim_col(x, y);
			a[x][y] = ' ';
			score = (sum_of_right + sum_of_col + 1 - 5) * 2 + 10;
			return score;
		}; break;
		case 10:
		{
			sum_of_left = left_num( x, y);
			sum_of_right = right_num( x, y);
			elim_left(x, y);
			elim_right(x, y);
			a[x][y] = ' ';
			score = (sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
			return score;
		}; break;
		case 11:
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
		case 12:
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
		case 13:
		{
			sum_of_row = row_num( x, y);
			sum_of_right = right_num( x, y);
			sum_of_left = left_num(x, y);
			elim_row(x, y);
			elim_right( x, y);
			elim_left(x, y);
			a[x][y] = ' ';
			score = (sum_of_row + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
			return score;
		}; break;
		case 14:
		{
			sum_of_col = col_num( x, y);
			sum_of_left = left_num(x, y);
			sum_of_right = right_num(x, y);
			elim_col( x, y);
			elim_left( x, y);
			elim_right( x, y);
			a[x][y] = ' ';
			score = (sum_of_col + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
			return score;
		}; break;
		case 15:
		{
			sum_of_row = row_num(x, y);
			sum_of_col = col_num( x, y);
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

};

class random_chess {
private:
	int rand_1, rand_2, rand_3;
public:
	void random_chess_1(a_chess_board cb) {//产生一个随机棋子
		srand((int)time(0));
		do
		{
			rand_1 = rand() % 18;
			rand_2 = rand() % 39;
		} while ((!cb.is_chess(rand_1, rand_2)) || (!is_free(cb.a, rand_1, rand_2)));
		rand_3 = rand() % 7;
		cb.a[rand_1][rand_2] = chess_type[rand_3];
	}

	void random_chess_2(a_chess_board cb) {//产生两个随机棋子
		srand((int)time(0));
		for (int i = 0; i < 2; i++)
		{
			do
			{
				rand_1 = rand() % 18;
				rand_2 = rand() % 39;
			} while ((!is_chess(rand_1, rand_2)) || (!is_free(cb.a, rand_1, rand_2)));
			rand_3 = rand() % 7;
			cb.a[rand_1][rand_2] = chess_type[rand_3];
		}
	}

	void random_chess_3(a_chess_board cb) {//产生3个随机棋子
		srand((int)time(0));
		for (int i = 0; i < 3; i++)
		{
			do
			{
				rand_1 = rand() % 18;
				rand_2 = rand() % 39;
			} while ((!is_chess(rand_1, rand_2)) || (!is_free(cb.a, rand_1, rand_2)));
			rand_3 = rand() % 7;
			cb.a[rand_1][rand_2] = chess_type[rand_3];
		}
	}

	void random_chess_7(a_chess_board cb) {//产生七个不同的随机棋子
		srand((int)time(0));
		int chess[7];
		for (int i = 0; i < 7; i++)
		{
			bool repeated = false;
			do
			{
				rand_1 = rand() % 18;
				rand_2 = rand() % 39;
			} while ((!is_chess(rand_1, rand_2)) || (!is_free(cb.a, rand_1, rand_2)));
			chess[i] = rand() % 7;
			for (int j = 0; j < i; j++)
				if (chess[i] == chess[j])
				{
					repeated = true;
				}
			if (repeated)
				i--;
			else
				cb.a[rand_1][rand_2] = chess_type[chess[i]];
		}
	}
};

void setmenu() {
	cout << "******************************************************************************************" << endl;
	cout << "*                                     五子连珠                                           *" << endl;
	cout << "*                                                                                        *" << endl;
	cout << "* 游戏规则：                                                                             *" << endl;
	cout << "*1.参赛者分为甲、乙两方，游戏开始时随机产生7种不同的棋子。甲方先移动其中一个棋子至任意地 *" << endl;
	cout << "*方，棋子移动后棋盘上又随机产生3个棋子，此时乙方可移动棋子。每人每次只能移动一颗棋子。   *" << endl;
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

void set_chess_board(char a[18][39]) {
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 39; j++)
			a[i][j] = ' ';
	for (int i = 0; i < 39; i++)//第一行的数字排列
	{
		static char k1 ='1';
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
	for(int i=0;i<18;i++)//边界的设置
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

void chess_show(char a[18][39]) {
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 39; j++)
			cout << a[i][j];
		cout << endl;
	}
}

bool end(char a[18][39]) {
	bool judge = true;
	for (int i = 1; i < 18; i = i + 2)
		for (int j = 6; j < 39; j++)
		{
			if (j == 6 || j == 10 || j == 14 || j == 18 || j == 22 || j == 26 || j == 30 || j == 34 || j == 38)
				if (a[i][j] == ' ')
					judge = false;
		}
	return judge;
}

bool is_chess(int row,int col) {
	if (row % 2 != 0) 
		if (col== 6 || col == 10 || col == 14 || col == 18 || col == 22 || col == 26 || col == 30 || col == 34 || col == 38)
			return true;
	return false;
}

bool is_free(char a[18][39],int row, int col){
	if (row % 2 != 0)
		if (col == 6 || col == 10 || col == 14 || col == 18 || col == 22 || col == 26 || col == 30 || col == 34 || col == 38)
			if (a[row][col] == ' ')
				return true;
	return false;
	}

void random_chess_1(char a[18][39]) {//产生一个随机棋子
	srand((int)time(0));
	int rand_1, rand_2, rand_3;
		do
		{
			rand_1 = rand() % 18;
			rand_2 = rand() % 39;
		} while ((!is_chess(rand_1, rand_2)) || (!is_free(a, rand_1, rand_2)));
		rand_3 = rand() % 7;
		a[rand_1][rand_2] = chess_type[rand_3];
}

void random_chess_2(char a[18][39]) {//产生两个随机棋子
	srand((int)time(0));
	int rand_1, rand_2, rand_3;
	for (int i = 0; i < 2; i++)
	{
		do
		{
			rand_1 = rand() % 18;
			rand_2 = rand() % 39;
		} while ((!is_chess(rand_1, rand_2)) || (!is_free(a, rand_1, rand_2)));
		rand_3 = rand() % 7;
		a[rand_1][rand_2] = chess_type[rand_3];
	}
}

void random_chess_3(char a[18][39]) {//产生3个随机棋子
	srand((int)time(0));
	int rand_1, rand_2,rand_3;
	for (int i = 0; i < 3; i++)
	{
		do
		{
		rand_1 = rand() % 18;
		rand_2 = rand() % 39;
		} while ((!is_chess(rand_1, rand_2)) || (!is_free(a, rand_1, rand_2)));
		rand_3 = rand() % 7;
		a[rand_1][rand_2] = chess_type[rand_3];
	}
}

void random_chess_7(char a[18][39]) {//产生七个不同的随机棋子
	srand((int)time(0));
	int rand_1, rand_2;
	int chess[7];
	for (int i = 0; i < 7; i++)
	{
		bool repeated = false;
		do
		{
			rand_1 = rand() % 18;
			rand_2 = rand() % 39;
		} while ((!is_chess(rand_1, rand_2)) || (!is_free(a, rand_1, rand_2)));
		chess[i] = rand() % 7;
		for (int j = 0; j < i; j++)
			if (chess[i] == chess[j])
			{
				repeated = true;
			}
		if (repeated)
			i--;
		else
			a[rand_1][rand_2] = chess_type[chess[i]];
	}
}

bool move(char (*a)[39],int x0, int y0,int x1,int y1) {
	int x_0, y_0, x_1, y_1;
	y_0 = 2 * y0 - 1;
	y_1 = 2 * y1 - 1;
	x_0 = x0 + 5 + 3 * (x0 - 1);
	x_1 = x1 + 5 + 3 * (x1 - 1);
	if (is_free(a, y_0,x_0))
	{
		cout << "选择初始棋子有误，请重新选择" << endl;
		return false;
	}
	else if (!(is_free(a, y_1,x_1)))
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
	{
		a[y_1][x_1] = a[y_0][x_0];
		a[y_0][x_0] = ' ';
		return true;
	}
	
}

int less_than_3(char a[18][39])
{
	int blank = 0;
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

int max_4(int a, int b, int c, int d) {
	int M = 0;
	M = max(a, b);
	M = max(M, c);
	M = max(M, d);
	return M;
}

int row_num(char a[18][39], int x, int y)
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

int col_num(char a[18][39], int x, int y)
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

int left_num(char a[18][39], int x, int y)
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

int right_num(char a[18][39], int x, int y)
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

int judge_elim(char a[18][39],int x1,int y1) 
{
	int x = 2 * y1 - 1;
	int y = x1 + 5 + 3 * (x1 - 1);
	int sum = 0, sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0;//分别代表横轴，纵轴，左斜，右斜的分数
	sum_of_row = row_num(a, x, y);
	if (sum_of_row < 4)//若相同棋子数小于4个，则变为0，下同
		sum_of_row = 0;

	sum_of_col = col_num(a, x, y);
	if (sum_of_col < 4)
		sum_of_col = 0;

	sum_of_left = left_num(a, x, y);
	if (sum_of_left < 4)
		sum_of_left = 0;

	sum_of_right = right_num(a, x, y);
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

void elim_row(char a[18][39], int x, int y)
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

void elim_col(char a[18][39], int x, int y)
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

void elim_left(char a[18][39], int x, int y)
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

void elim_right(char a[18][39], int x, int y)
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

int score(char a[18][39],int x1,int y1, int index)
{
	int sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0,score;
	int x = 2 * y1 - 1;
	int y = x1 + 5 + 3 * (x1 - 1);
	switch (index)
	{
	case 1:
	{
		sum_of_row = row_num(a, x, y);
		elim_row(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + 1) * 2;
		return score;
	}; break;
	case 2:
	{
		sum_of_col = col_num(a, x, y);
		elim_col(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_col + 1) * 2;
		return score;
	}; break;
	case 3:
	{
		sum_of_left = left_num(a, x, y);
		elim_left(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_left + 1) * 2;
		return score;
	}; break;
	case 4:
	{
		sum_of_right = right_num(a, x, y);
		elim_right(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_right + 1) * 2;
		return score;
	}; break;
	case 5:
	{
		sum_of_row = row_num(a, x, y);
		sum_of_col = col_num(a, x, y);
		elim_row(a, x, y);
		elim_col(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}break;
	case 6:
	{
		sum_of_row = row_num(a, x, y);
		sum_of_left = left_num(a, x, y);
		elim_row(a, x, y);
		elim_left (a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_left + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 7:
	{
		sum_of_row = row_num(a, x, y);
		sum_of_right = right_num(a, x, y);
		elim_row(a, x, y);
		elim_right(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 8:
	{
		sum_of_left = left_num(a, x, y);
		sum_of_col = col_num(a, x, y);
		elim_left(a, x, y);
		elim_col(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_left + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 9:
	{
		sum_of_right = right_num(a, x, y);
		sum_of_col = col_num(a, x, y);
		elim_right(a, x, y);
		elim_col(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_right + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 10:
	{
		sum_of_left = left_num(a, x, y);
		sum_of_right = right_num(a, x, y);
		elim_left(a, x, y);
		elim_right(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 11:
	{
		sum_of_row = row_num(a, x, y);
		sum_of_col = col_num(a, x, y);
		sum_of_left = left_num(a, x, y);
		elim_row(a, x, y);
		elim_col(a, x, y);
		elim_left(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_left + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 12:
	{
		sum_of_row = row_num(a, x, y);
		sum_of_col = col_num(a, x, y);
		sum_of_right = right_num(a, x, y);
		elim_row(a, x, y);
		elim_col(a, x, y);
		elim_right(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_right + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 13:
	{
		sum_of_row = row_num(a, x, y);
		sum_of_right = right_num(a, x, y);
		sum_of_left = left_num(a, x, y);
		elim_row(a, x, y);
		elim_right(a, x, y);
		elim_left(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 14:
	{
		sum_of_col = col_num(a, x, y);
		sum_of_left = left_num(a, x, y);
		sum_of_right = right_num(a, x, y);
		elim_col(a, x, y);
		elim_left(a, x, y);
		elim_right(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_col + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 15:
	{
		sum_of_row = row_num(a, x, y);
		sum_of_col = col_num(a, x, y);
		sum_of_left = left_num(a, x, y);
		sum_of_right = right_num(a, x, y);
		elim_row(a, x, y);
		elim_col(a, x, y);
		elim_left(a, x, y);
		elim_right(a, x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_col + sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	}
}

int main()
{
	int A_score = 0, B_score = 0;
	setmenu();
	setxy();
	char chess_board[18][39];
	set_chess_board(chess_board);
	random_chess_7(chess_board);
	chess_show(chess_board);
	for (int time = 0;; time++)
	{
		int x0, y0, x1, y1, x2, y2, x3, y3;
		more1:
		do
		{
			cout << "甲方：\t\t\t\t\t甲方目前得分：" << A_score << "\t\t" << "乙方目前得分：" << B_score << endl;
			cout << "初始位置:";
			cin >> x0 >> y0;
			cout << "目标位置:";
			cin >> x1 >> y1;
		} while (!(move(chess_board, x0, y0, x1, y1)));
		if (less_than_3(chess_board))
		{
			int blank = less_than_3(chess_board);
			switch (blank) {
			case 1:random_chess_1(chess_board); break;
			case 2:random_chess_2(chess_board); break;
			}
			system("cls");
			setmenu();
			setxy();
			chess_show(chess_board);
			if (A_score > B_score)
			{
				cout << endl;
				cout << "游戏结束，甲方胜利！" << endl;
			}
			else if (A_score < B_score)
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
			cout << A_score << " : " << B_score << endl;
			cout << endl;
			cout << "**********************************************游戏已结束**********************************************" << endl;
			return 0;
		}
		else if (judge_elim(chess_board, x1,y1))
		{
			int index = judge_elim(chess_board,x1,y1);
			A_score += score(chess_board, x1,y1,index);
			if (index > 4)
			{
				system("cls");
				setmenu();
				setxy();
				chess_show(chess_board);
				cout << "恭喜你多获得一次移动棋子的机会！" << endl;
				goto more1;
			}
		}
		else
			random_chess_3(chess_board);
		system("cls");
		setmenu();
		setxy();
		chess_show(chess_board);
		if (end(chess_board))
		{
			if (A_score > B_score)
			{
				cout << endl;
				cout << "游戏结束，甲方胜利！" << endl;
			}
			else if (A_score < B_score)
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
			cout << A_score << " : " << B_score << endl;
			cout << endl;
			cout << "**********************************************游戏已结束**********************************************" << endl;
			return 0;
		}
		more2:
		do
		{
			cout << "乙方：\t\t\t\t\t甲方目前得分：" << A_score << "\t\t" << "乙方目前得分：" << B_score << endl;
			cout << "初始位置:";
			cin >> x2 >> y2;
			cout << "目标位置:";
			cin >> x3 >> y3;
		} while (!move(chess_board, x2, y2, x3, y3));
		if (less_than_3(chess_board))
		{
			int blank = less_than_3(chess_board);
			switch (blank) {
			case 1:random_chess_1(chess_board); break;
			case 2:random_chess_2(chess_board); break;
			}
			system("cls");
			setmenu();
			setxy();
			chess_show(chess_board);
			if (A_score > B_score)
			{
				cout << endl;
				cout << "游戏结束，甲方胜利！" << endl;
			}
			else if (A_score < B_score)
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
			cout << A_score << " : " << B_score << endl;
			cout << endl;
			cout << "**********************************************游戏已结束**********************************************" << endl;
			return 0;
		}
		else if (judge_elim(chess_board, x3,y3))
		{
			int index = judge_elim(chess_board, x3,y3);
			B_score += score(chess_board,x3,y3, index);
			if (index > 4)
			{
				system("cls");
				setmenu();
				setxy();
				chess_show(chess_board);
				cout << "恭喜你多获得一次移动棋子的机会！" << endl;
				goto more2;
			}
		}
		else
			random_chess_3(chess_board);
		system("cls");
		setmenu();
		setxy();
		chess_show(chess_board);
		if (end(chess_board))
		{
			if (A_score > B_score)
			{
				cout << endl;
				cout << "游戏结束，甲方胜利！" << endl;
			}
			else if (A_score < B_score)
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
			cout << A_score << " : " << B_score << endl;
			cout << endl;
			cout << "**********************************************游戏已结束**********************************************" << endl;
			return 0;
		}
	}
}