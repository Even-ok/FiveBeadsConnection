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

typedef struct mynode {//���̽ڵ�ṹ�嶨��
	int x;
	int y;
	int g;//��㵽��ǰ�ڵ�ĳ��ȣ�10/14��
	int h;//��ǰ�ڵ㵽�յ�Ĺ��㳤�ȣ������٣�
	int attr;//״̬
	struct mynode* parent;
}a_node;

enum attribute { ATTR_NULL, ATTR_OPEN, ATTR_CLOSE, ATTR_FORBID, ATTR_RIGHT };
//�ֱ�����ָ�룬����open_list�У�����close_list�У����ɷ��ʴ��Ϳ��Է��ʵĵط�

char chess_type[7] = { 'w','#','*','%','$','o','&' };//�߸�������״

class menu {//�˵��࣬��ʾ�˵�
public:
	void setmenu() {
		cout << "******************************************************************************************" << endl;
		cout << "*                                     ��������                                           *" << endl;
		cout << "*                                                                                        *" << endl;
		cout << "* ��Ϸ����                                                                             *" << endl;
		cout << "*1.�����߷�Ϊ�ס�����������Ϸ��ʼʱ�������7�ֲ�ͬ�����ӡ��׷����ƶ�����һ������������� *" << endl;
		cout << "*���������Ӳ����������������ƶ������������������3�����ӣ���ʱ�ҷ����ƶ����ӡ�ÿ��ÿ��ֻ *" << endl;
		cout << "*���ƶ�һ�����ӡ�                                                                        *" << endl;
		cout << "*2.��ͬɫ��������5������һ���ųɺ��������б��ʱ����Ϸ�߿��Ե�10�֡�ͬʱ5�����Ӵ�������  *" << endl;
		cout << "*��ʧ������һ�������ÿ����һ�ţ���Ϸ����ԭ�зֵĻ����Ͽɶ��2�֡�                       *" << endl;
		cout << "*3.���������������������ĸ���������������÷����һ���ƶ����ӵĻ��ᡣ                    *" << endl;
		cout << "*4.���ӱ�ռ��ʱ��Ϸ�������÷ָ��߻�ʤ��                                                  *" << endl;
		cout << "*                                                                                        *" << endl;
		cout << "******************************************************************************************" << endl;
		cout << endl;
	}

	void setxy() {
		cout << "   x��" << endl;
		cout << " y��" << endl;
	}
};

a_chess_board chess_board;

void a_chess_board::chess_show() {//��ʾ����
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
	for (int i = 0; i < 39; i++)//��һ�е���������
	{
		static char k1 = '1';
		if (i == 6 || i == 10 || i == 14 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38)
		{
			a[0][i] = k1;
			k1++;
		}
	}
	for (int j = 0; j < 18; j++)//�����е���������
	{
		static int k2 = '1';
		if (j % 2 != 0)
		{
			a[j][5] = k2;
			k2++;
		}
	}
	for (int i = 0; i < 18; i++)//�߽������
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

bool a_chess_board::is_chess(int row, int col) {//�ж��Ƿ��������
	if (row % 2 != 0)
		if (col == 6 || col == 10 || col == 14 || col == 18 || col == 22 || col == 26 || col == 30 || col == 34 || col == 38)
			return true;
	return false;
}

bool a_chess_board::is_free(int row, int col) {//�жϸ�������Ƿ�Ϊ��
	if (is_chess(row, col))
		if (a[row][col] == ' ')
			return true;
	return false;
}

int a_chess_board::less_than_3()
{
	int blank = 0;//�ո���
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

int a_chess_board::row_num(int x, int y)//����������ͬ������
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
	if (x - 2 >= 1 && a[x][y] == a[x - 2][y] && a[x][y] != ' ')//����������ͬ������
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
	if (x + 2 <= 17 && y - 4 >= 6 && a[x][y] == a[x + 2][y - 4] && a[x][y] != ' ')//������б��ͬ������
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

int a_chess_board::right_num(int x, int y)//������б��ͬ������
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

void a_chess_board::move(int x0,int y0,int x1,int y1)//�ƶ����Ӻ���
{
	int x_0, y_0, x_1, y_1;
	y_0 = 2 * y0 - 1;//����ת��
	y_1 = 2 * y1 - 1;
	x_0 = x0 + 5 + 3 * (x0 - 1);
	x_1 = x1 + 5 + 3 * (x1 - 1);
	a[y_1][x_1] = a[y_0][x_0];
	a[y_0][x_0] = ' ';
}

//��������
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
	if (x - 2 >= 1 && a[x][y] == a[x - 2][y] && a[x][y] != ' ')//����������ͬ������
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
	if (x + 2 <= 17 && y - 4 >= 6 && a[x][y] == a[x + 2][y - 4] && a[x][y] != ' ')//������б��ͬ������
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

int a_chess_board::score(int x1, int y1, int index)//�Ʒֺ���
{
	int sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0, score;
	int x = 2 * y1 - 1;
	int y = x1 + 5 + 3 * (x1 - 1);
	switch (index)
	{
	case 1://����������
	{
		sum_of_row = row_num(x, y);
		elim_row(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + 1) * 2;
		return score;
	}; break;
	case 2://����������
	{
		sum_of_col = col_num(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_col + 1) * 2;
		return score;
	}; break;
	case 3://����б������
	{
		sum_of_left = left_num(x, y);
		elim_left(x, y);
		a[x][y] = ' ';
		score = (sum_of_left + 1) * 2;
		return score;
	}; break;
	case 4://����б������
	{
		sum_of_right = right_num(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_right + 1) * 2;
		return score;
	}; break;
	case 5://�ᡢ��������
	{
		sum_of_row = row_num(x, y);
		sum_of_col = col_num(x, y);
		elim_row(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}break;
	case 6://�ᡢ��������
	{
		sum_of_row = row_num(x, y);
		sum_of_left = left_num(x, y);
		elim_row(x, y);
		elim_left(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_left + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 7://�ᡢ��������
	{
		sum_of_row = row_num(x, y);
		sum_of_right = right_num(x, y);
		elim_row(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_row + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 8://��б����������
	{
		sum_of_left = left_num(x, y);
		sum_of_col = col_num(x, y);
		elim_left(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_left + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 9://��б����������
	{
		sum_of_right = right_num(x, y);
		sum_of_col = col_num(x, y);
		elim_right(x, y);
		elim_col(x, y);
		a[x][y] = ' ';
		score = (sum_of_right + sum_of_col + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 10://����б������
	{
		sum_of_left = left_num(x, y);
		sum_of_right = right_num(x, y);
		elim_left(x, y);
		elim_right(x, y);
		a[x][y] = ' ';
		score = (sum_of_left + sum_of_right + 1 - 5) * 2 + 10;
		return score;
	}; break;
	case 11://�ᡢ������б������
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
	case 12://�ᡢ������б������
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
	case 13://�ᡢ����������
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
	case 14://��������������
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
	case 15://�ĸ����������
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

void a_chess_board::random_chess_less_than3(int times) {//����1/2/3���������
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

void  a_chess_board::random_chess_7() {//�����߸���ͬ���������
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
		for (int j = 0; j < i; j++)//��֮ǰ���������ӽ��бȽϣ�����ͬ�����ٽ���һ��ѭ��
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
	try//������������Ƿ���ȷ
	{
		if (cin.fail())
			throw 'a';
	}
	catch (char) {
		cin.clear();
		//cin.ignore();
		cin.ignore(std::numeric_limits< streamsize >::max(), '\n');
		cout << "�����������������룡" << endl;
		return false;
	}
	return true;
}

bool Judge::end(a_chess_board cb) {//���������û�б�ռ��
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

bool Judge::judge_move(int x0, int y0, int x1, int y1, a_chess_board cb) {//��������λ���Ƿ���Ч
	int x_0, y_0, x_1, y_1;
	y_0 = 2 * y0 - 1;
	y_1 = 2 * y1 - 1;
	x_0 = x0 + 5 + 3 * (x0 - 1);
	x_1 = x1 + 5 + 3 * (x1 - 1);
	if (cb.is_free(y_0, x_0))
	{
		cout << "ѡ���ʼ��������������ѡ��" << endl;
		return false;
	}
	else if (!(cb.is_free(y_1, x_1)))
	{
		cout << "ѡ������λ������������ѡ��" << endl;
		return false;
	}
	else if (x0 < 1 || x0>9 || y0 < 1 || y0>9 || x1 < 1 || x1>9 || y1 < 1 || y1>9)
	{
		cout << "����ķ�Χ����������ѡ��" << endl;
		return false;
	}
	else
		return true;
}

int Judge::judge_elim(int x1, int y1)//���յ�Ϊ���ģ��������Χ�ܷ���������������������������ʽ��š�
{
	int x = 2 * y1 - 1;
	int y = x1 + 5 + 3 * (x1 - 1);
	int sum = 0, sum_of_row = 0, sum_of_col = 0, sum_of_left = 0, sum_of_right = 0;//�ֱ������ᣬ���ᣬ��б����б�ķ���
	sum_of_row = chess_board.row_num(x, y);
	if (sum_of_row < 4)//����ͬ������С��4�������Ϊ0����ͬ
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

void Judge::judge_win(int ascore, int bscore) {//�ж�˫����һ��ʤ������������Ϸ��
	if (ascore > bscore)
	{
		cout << endl;
		cout << "��Ϸ�������׷�ʤ����" << endl;
	}
	else if (ascore < bscore)
	{
		cout << endl;
		cout << "��Ϸ�������ҷ�ʤ����" << endl;
	}
	else
	{
		cout << endl;
		cout << "��Ϸ������˫�����ƽ�֣�" << endl;
	}
	cout << "��" << "   " << "��" << endl;
	cout << ascore << " : " << bscore << endl;
	cout << endl;
	cout << "**********************************************��Ϸ�ѽ���**********************************************" << endl;
	system("pause");
}

int max_4(int a, int b, int c, int d) {
	int M = 0;
	M = max(a, b);
	M = max(M, c);
	M = max(M, d);
	return M;
}

class A_star {//��Ϊa_nodeû�ж�����ͷ�ļ��У����Բ���A�������ͷ�ļ�����
private:
	a_node* _start=NULL;//Ҫ������g��h��Ϊ0
	a_node* _end=NULL;
	a_node star_chess_board[18][39];//�ڵ�����

public:
	vector<a_node*> open_list;
	A_star(int start_x, int start_y, int end_x, int end_y, a_chess_board cb);
	void set_star_chess_board(a_chess_board cb);//��ʼ���ڵ�����ĺ���
	a_node* close_cur();//�����ڵ��Ƴ�open_list
	int insert_neighbor_to_openlist(a_node* curr_node);//����Χ��Ч�ڵ����open_list
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

int A_star::insert_neighbor_to_openlist(a_node* curr_node)//����������ӣ���ŵ�forbid�У������ռλ�ˣ�Ҳ�ŵ�forbid
{
	int space = 0;
	a_node* tmp = NULL;
	vector<a_node*>::iterator is_contain;
	//ȡ����ǰ�ڵ���������ҽڵ����open�б� ��������Щ�ڵ��parentΪ��ǰ�ڵ�
	//����ϵ�������±�������ͬ
	//��
	if (curr_node->x > 1)
	{
		tmp = &star_chess_board[curr_node->x - 2][curr_node->y];
		is_contain = std::find(open_list.begin(), open_list.end(), tmp);//�����Ƿ���openlist���
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
	//��
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

	//��
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

	//��
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
		return 0;//���tmpһֱ�ǿ�ָ�룬���ش�������ѭ��
	else if (find(open_list.begin(), open_list.end(), _end) == open_list.end())//û�ҵ�
		return 1;
}

bool comp_f(a_node* a, a_node* b)//����fֵ��Ľڵ�
{
	//�Ƚ�fֵ
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
			cout << "�׷���\t\t\t\t\t�׷�Ŀǰ�÷֣�" << A_score << "\t\t" << "�ҷ�Ŀǰ�÷֣�" << B_score << endl;
			cout << "��ʼλ��:";
			cin >> x0 >> y0;
			if (!judge.valid_cin())//�ж��������������Ƿ���ȷ
				goto more1;//�����벻��ȷ��������ת���������
			cout << "Ŀ��λ��:";
			cin >> x1 >> y1;
			if (!judge.valid_cin())
				goto more1;
		} while (!(judge.judge_move(x0, y0, x1, y1,chess_board)));
		A_star search_path = A_star(x0, y0, x1, y1,chess_board);//A����������Ƚ��г�ʼ��
		while (search_path.open_list.size()!=0)
		{
			a_node* curr = search_path.close_cur();
			int can_find = search_path.insert_neighbor_to_openlist(curr);
			if (can_find == 0)//��ĳ�����ڵ���Χ�ĸ��Ӷ�������
			{
				cout << "�����ƶ���ָ��λ�ã����������룡" << endl;
				goto more1;
			}
			else if (can_find == 2)//�ҵ��յ�
			{
				cout << "���ƶ�����ǰλ�ã�" << endl;
				chess_board.move(x0, y0, x1, y1);
				break;
			}
			sort(search_path.open_list.begin(), search_path.open_list.end(), comp_f);//��open_list��������ȡfֵ��С��һ��
		}
		if (judge.judge_elim(x1, y1))
		{
			int index = judge.judge_elim(x1, y1);
			A_score += chess_board.score(x1, y1, index);
			if (index > 4)//�෽������
			{
				system("cls");
				menu.setmenu();
				menu.setxy();
				chess_board.chess_show();
				cout << "��ϲ�����һ���ƶ����ӵĻ��ᣡ" << endl;
				goto more1;
			}
		}
		else
		{
			if (chess_board.less_than_3())//ʣ����������3
			{
				int blank = chess_board.less_than_3();
				switch (blank) {
				case 1:chess_board.random_chess_less_than3(1); break;
				case 2:chess_board.random_chess_less_than3(2); break;
				}//�������1/2���Ӻ��������������������֮����������������δ��
				system("cls");
				menu.setmenu();
				menu.setxy();
				chess_board.chess_show();
				if (judge.end(chess_board))//�ж�������û����
				{
					judge.judge_win(A_score, B_score);
					return 0;
				}
			}
			else
				chess_board.random_chess_less_than3(3);//��������������ʣ��ո���ڵ���3��������3���������
		}
		system("cls");
		menu.setmenu();
		menu.setxy();
		chess_board.chess_show();
		if (judge.end(chess_board))//�ж�����������Ӻ������Ƿ�ռ��
		{
			judge.judge_win(A_score, B_score);
			return 0;
		}
	more2:
		do
		{
			cout << "�ҷ���\t\t\t\t\t�׷�Ŀǰ�÷֣�" << A_score << "\t\t" << "�ҷ�Ŀǰ�÷֣�" << B_score << endl;
			cout << "��ʼλ��:";
			cin >> x2 >> y2;
			if (!judge.valid_cin())
				goto more2;
			cout << "Ŀ��λ��:";
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
				cout << "�����ƶ���ָ��λ�ã����������룡" << endl;
				goto more2;
			}
			else if (can_find == 2)
			{
				cout << "���ƶ�����ǰλ�ã�" << endl;
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
				cout << "��ϲ�����һ���ƶ����ӵĻ��ᣡ" << endl;
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