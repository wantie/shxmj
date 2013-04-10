#pragma once
#include <vector>
#include <windows.h>
#include "mvc.h"
#include <string>

/*
整副牌，局开始后： {在手牌，出手牌， 摸牌区}
在手牌：
１，明牌：吃，碰，杠
２，暗牌：１　４　７　１０　１３，or，相公 ：(

出手牌：
玩家出手的牌，不包括被吃，被碰，被杠的牌
杠起的牌

摸牌区：
如果牌被捉完，仍没人胡， start new frame

牌值对应表：
	1  2  3  4  5  6  7  8  9   
	21 22 23 24 25 26 27 28 29  
	41 42 43 44 45 46 47 48 49  
	81 84 87 90 93 96 99        
*/
class maj
{
	int _code;
	friend bool compare_maj(const maj&, const maj &);
public:
	maj() {_code = -1;};
	maj(int code) :_code(code) {};
	int get_code() const { return _code;}
	bool operator < (const maj &rhs) { return _code < rhs._code; }
	bool operator > (const maj &rhs) { return _code > rhs._code; }
	bool operator > (const maj &rhs)const  { return _code > rhs._code; }
	bool operator < (const maj &rhs) const { return _code < rhs._code; }
	int operator- (const maj &rhs) const { return _code - rhs._code; }
	maj operator+ (int i) const { return maj(_code + i); }
	bool operator== (const maj &rhs) const { return _code == rhs._code; }
	bool operator!= (const maj &rhs) const { return !this->operator==(rhs); }
	std::wstring bitmap_name() const{
		wchar_t name[10] = TEXT("");
		switch (_code / 10) {
		case 0:
			wsprintf(name, TEXT("%dtong"), _code % 10);
			break;
		case 2:
			wsprintf(name, TEXT("%dtiao"), _code % 10);
			break;
		case 4:
			wsprintf(name, TEXT("%dwan"), _code % 10);
			break;
		case 8:
		case 9:
			switch(_code) {
			case 81: return TEXT("dongfeng");
			case 84: return TEXT("nanfeng");
			case 87: return TEXT("xifeng");
			case 90: return TEXT("beifeng");
			case 93: return TEXT("zhong");
			case 96: return TEXT("fa");
			case 99: return TEXT("bai");
			}
		}
		return name;
	}

	maj &operator[] (int code)
		{
			static maj all_maj[136] = {
				maj(1),maj(2),maj(3),maj(4),maj(5),maj(6),maj(7),maj(8),maj(9),
				maj(1),maj(2),maj(3),maj(4),maj(5),maj(6),maj(7),maj(8),maj(9),
				maj(1),maj(2),maj(3),maj(4),maj(5),maj(6),maj(7),maj(8),maj(9),
				maj(1),maj(2),maj(3),maj(4),maj(5),maj(6),maj(7),maj(8),maj(9),
				maj(21),maj(22),maj(23),maj(24),maj(25),maj(26),maj(27),maj(28),maj(29),
				maj(21),maj(22),maj(23),maj(24),maj(25),maj(26),maj(27),maj(28),maj(29),
				maj(21),maj(22),maj(23),maj(24),maj(25),maj(26),maj(27),maj(28),maj(29),
				maj(21),maj(22),maj(23),maj(24),maj(25),maj(26),maj(27),maj(28),maj(29),
				maj(41),maj(42),maj(43),maj(44),maj(45),maj(46),maj(47),maj(48),maj(49),
				maj(41),maj(42),maj(43),maj(44),maj(45),maj(46),maj(47),maj(48),maj(49),
				maj(41),maj(42),maj(43),maj(44),maj(45),maj(46),maj(47),maj(48),maj(49),
				maj(41),maj(42),maj(43),maj(44),maj(45),maj(46),maj(47),maj(48),maj(49),
				maj(81),maj(84),maj(87),maj(90),maj(93),maj(96),maj(99),
				maj(81),maj(84),maj(87),maj(90),maj(93),maj(96),maj(99),
				maj(81),maj(84),maj(87),maj(90),maj(93),maj(96),maj(99),
				maj(81),maj(84),maj(87),maj(90),maj(93),maj(96),maj(99)
			};
		}
};

class hu;
class lan;
class normal;
class nine_one;
class pengpeng;
class seven_pairs;
class frame;
class player_view;
class player : public model
{
	frame *_frame;
	std::vector<maj> _majs;
	std::vector<maj> _chi; // 只存三张的第一张, 1 2 3,代表: 123, 234, 345, 
	std::vector<maj> _peng;
	std::vector<maj> _gang;
	maj _new_fetch_mj;
	int _num_wild_hearts;
	std::vector<hu *> _judges;
	friend class hu;
	friend class lan;
	friend class normal;
	friend class nine_one;
	friend class pengpeng;
	friend class seven_pairs;
	
public:
	player();
	void set_frame(frame &f);
	frame *get_frame() { return _frame; }
	player_view *get_view();
	const maj &get_wildcard() const;
	const maj &get_new_fetch() const { return _new_fetch_mj; };
	std::vector<maj> &get_majs() {return _majs;}
	std::vector<maj> &get_chi() { return _chi; }
	std::vector<maj> &get_peng() { return _peng; };
	std::vector<maj> &get_gang() { return _gang;};
	maj fetch();
	void insert(maj &mj, int index = -1);
	void drop(int index);
	bool chi(int index1, int index2, maj &mj);
	bool peng(int index1, int index2, maj &mj);
	bool gang(int index1, int index2, int index3, maj &mj);
	void shuffle();
	std::vector<hu *> judge();
};

class frame : public model
{
	std::vector<maj> _fresh_majs;
	std::vector<maj> _droped_majs;
	std::vector<player *> _players;
	player *_current;
	player *_self;
	maj _wildcard;
public:
	frame() {_current = _self = 0;}
	//static frame &instance() { static frame the_one; return the_one;}
	std::vector<maj> &get_fresh_majs() { return _fresh_majs; };
	std::vector<maj> &get_droped_majs() { return _droped_majs; };
	void add_player(player &p);
	void shuffle();
	void drop(maj &mj) { _droped_majs.push_back(mj); update(); }
	void pseudo_drop() { _droped_majs.push_back(deal()); update(); }
	maj deal();
	player *wait_peng_kang(maj);
	player *get_current_player() { return _players.empty() ? NULL : _players[0]; }
	player *get_self_player() { return _players.empty() ? NULL : _players[0]; }
	const maj &get_wildcard() {return _wildcard;}
	void deal_to_players();
	void start();
};
