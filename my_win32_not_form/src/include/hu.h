#pragma once
#include "maj_model.h"

/*

顺子： 三张连续牌
坎子： 三张相同牌
胡牌： 牌型

牌型： 平胡　| 碰碰胡  | 七对  | 九妖  |　十三烂　
平湖： 顺子 {0,4} 对子
碰碰胡 ：坎子 {0,4} 对子
七对： 对子{7}
九妖： [字 | 九妖子]{2|5|8|11|14}
十三烂：所有牌间距大于2,

平胡判断的过程
 如果牌数 != 2 5 8 11 14，
  相公，退出
 如果牌数 == 2
  相同则胡，否则不胡
 如果牌数 > 2
  找出牌中所有可能的坎子和顺子，
  如果找不到，（都是孤张）
   不胡，退出
  将找到的坎子和顺子入队列
  1，从队列取出一个坎子或顺子，如果队列已空，不胡
  2，手牌删除这个坎子或顺子，对剩下的牌进行判胡，（递归调用此判胡过程）
  3，如果能胡
   打印这个坎子或顺子，退出
  4，如果不胡
   将取出的坎子或顺子重新放回
  5，跳转至1
*/

class hu
{
protected:
	maj _wildcard;
	maj _new_fetch;
	bool _have_wildcard;
	int _num_pseudos;
public:
	hu(const std::wstring &name) : name(name){};
	std::wstring name;
	virtual bool judge(const player &majs) = 0;
};

class lan : public hu
{
public:
	lan() : hu((wchar_t *)("十三烂")) {};
	bool judge(const player &majs);	
};

class nine_one : public hu
{
	bool is_91(const maj &mj) 
	{
		int code = mj.get_code();
		if (code % 10 != 1 && code % 10 != 9 && code < 81)
			return false;
		return true;
	}
public:
	nine_one() : hu(TEXT("九一")) {};
	bool judge(const player &majs);
};

class seven_pairs : public hu
{
	bool exist_pair(const std::vector<maj>&, const maj &);
	void reduce_pair(std::vector<maj> &, const maj &);
	void put_back_pair(std::vector<maj> &, const maj &);
	bool do_judge(std::vector<maj> &judge_set, const maj &wildcard);
public:
	seven_pairs() : hu(TEXT("七对")) {};
	bool judge(const player &majs);
};

class pengpeng : public hu
{
	bool do_judge(std::vector<maj> &);
public:
	pengpeng() : hu(TEXT("碰碰胡")) {_have_wildcard = false;};
	bool judge(const player &majs);
};

class normal : public hu
{
public:
	normal() : hu(TEXT("平胡")) {};
	bool judge(const player &majs);
private:
	bool do_judge(std::vector<maj> &judge_set);
	bool exist_shunzi(std::vector<maj> &set, maj &mj);
	bool exist_kanzi(std::vector<maj> &set, maj &mj);
	void delete_shunzi(std::vector<maj> &set, maj &mj);
	void delete_kanzi(std::vector<maj> &set, maj&mj);
	void put_back_shunzi(std::vector<maj> &set, maj &mj);
	void put_back_kanzi(std::vector<maj> &set, maj &mj);
};
