#pragma once
class a_chess_board {
private:
	char a[18][39];//װ�����̷���
	int rand_1, rand_2, rand_3;//3������������ӵĲ���
	friend class A_star;
	friend class Judge;
public:
	void set_chess_board();//���̳�ʼ��
	void chess_show();
	bool is_chess(int row, int col);
	bool is_free(int row, int col);
	void move(int x0, int y0, int x1, int y1);//�ƶ�����
	int less_than_3();//�ж������Ƿ�����3���ո�
	int row_num(int x, int y);//���������������������ͬ
	int col_num(int x, int y);
	int left_num(int x, int y);
	int right_num(int x, int y);
	void elim_row(int x, int y);//���������������ӣ���ͬ
	void elim_col(int x, int y);
	void elim_left(int x, int y);
	void elim_right(int x, int y);
	int score(int x1, int y1, int index);//�������������������
	void random_chess_less_than3(int times);//�������1/2/3������
	void random_chess_7();
};
class Judge {
public:
	bool valid_cin();//�ж������Ƿ�����Ч��

	bool end(a_chess_board cb);//�ж���Ϸ�Ƿ����

	bool judge_move(int x0, int y0, int x1, int y1, a_chess_board cb);//�ж��Ƿ����ƶ�

	int judge_elim(int x1, int y1);//�ж��Ƿ�������

	void judge_win(int ascore, int bscore);//�ж�ʤ������������Ϸ
}judge;

