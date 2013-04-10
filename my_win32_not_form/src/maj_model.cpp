#include "maj_model.h"
#include "hu.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include "maj_view.h"
using namespace std;

player::player()
{
	_judges.push_back(new normal());
	_judges.push_back(new seven_pairs());
	_judges.push_back(new lan());
	_judges.push_back(new nine_one());
	_judges.push_back(new pengpeng());
}

void player::set_frame(frame &f) 
{
	_frame = &f; 
	_frame->add_player(*this); 
}

std::vector<hu *> player::judge()
{
	std::vector<hu *> hu_vec;
	for (int i = 0; i != _judges.size(); i++) {
		if (_judges[i]->judge(*this))
			hu_vec.push_back(_judges[i]);
	}
	return hu_vec;
}

maj player::fetch()
{
	return _frame->deal();
}
const maj &player::get_wildcard() const
{
	return _frame->get_wildcard(); 
}
void player::insert(maj &mj, int index)
{
	if (index != -1 && (index > _majs.size() - 1 || index < 0))
		index = -1;

	if (index == -1)
		_majs.push_back(mj);
	else 
		_majs.insert(_majs.begin() + index, mj);
	_new_fetch_mj = mj;
	update();
}

void player::drop(int index) {
	if (index >= _majs.size() || index < 0)
		return;
	maj mj = _majs[index];
	_majs.erase(_majs.begin() + index);
	_frame->drop(mj);
	//_frame->pseudo_drop();
	update();
}

bool player::chi(int index1, int index2, maj &mj)
{
	vector<maj> shunzi;
	shunzi.push_back(_majs[index1]);
	shunzi.push_back(_majs[index2]);
	shunzi.push_back(mj);
	sort(shunzi.begin(), shunzi.end(), compare_maj);
	if (shunzi[0] - shunzi[1] != -1 || shunzi[1] - shunzi[2] != -1)
		return false;
	_chi.push_back(shunzi[0]);
	_chi.push_back(shunzi[1]);
	_chi.push_back(shunzi[2]);
	if (index1 > index2) {
		_majs.erase(_majs.begin() + index1);
		_majs.erase(_majs.begin() + index2);
	} else {
		_majs.erase(_majs.begin() + index2);
		_majs.erase(_majs.begin() + index1);
	}
	vector<maj> &frame_droped_majs = get_frame()->get_droped_majs(); 
	frame_droped_majs.erase(frame_droped_majs.begin() + frame_droped_majs.size() - 1);
	update();
	get_frame()->update();
	return true;
}
bool player::peng(int index1, int index2, maj &mj)
{
	if (_majs[index1] != _majs[index2] || _majs[index2] != mj)
		return false;
	_peng.push_back(mj);
	if (index1 > index2) {
		_majs.erase(_majs.begin() + index1);
		_majs.erase(_majs.begin() + index2);
	} else {
		_majs.erase(_majs.begin() + index2);
		_majs.erase(_majs.begin() + index1);
	}
	vector<maj> &frame_droped_majs = get_frame()->get_droped_majs(); 
	frame_droped_majs.erase(frame_droped_majs.begin() + frame_droped_majs.size() - 1);
	update();
	get_frame()->update();
	return true;
}

bool player::gang(int index1, int index2, int index3, maj &mj)
{
	if (_majs[index1] != _majs[index2] || _majs[index2] != _majs[index3] || _majs[index3] != mj)
		 return false;
	
	int tmp = 0;
	if (index1 < index2) {
		tmp = index1;
		index1 = index2;
		index2 = tmp;
	}
	if (index2 < index3) {
		tmp = index2;
		index2 = index3;
		index3 = tmp;
	};
	if (index1 < index2) {
		tmp = index1;
		index1 = index2;
		index2 = tmp;
	}
	_majs.erase(_majs.begin() + index1);
	_majs.erase(_majs.begin() + index2);
	_majs.erase(_majs.begin() + index3);

			
	_gang.push_back(mj);
	vector<maj> &frame_droped_majs = get_frame()->get_droped_majs();
	frame_droped_majs.erase(frame_droped_majs.begin() + frame_droped_majs.size() - 1);
	update();
	get_frame()->update();
	return true;
}


void player::shuffle()
{
	_majs.clear();
	_chi.clear();
	_peng.clear();
	_gang.clear();
	update();
};

player_view *player::get_view()
{ 
	return static_cast<player_view*>(model::get_view());
}
void frame::shuffle()
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
	for (int i = 0; i != _players.size(); i++)
		_players[i]->shuffle();
	_droped_majs.clear();
	_fresh_majs.clear();
	srand(time(NULL));
	vector<maj> dispatch_majs(all_maj, all_maj + 136);
	for (int i = 136; i != 0; i--) {
		int index = rand() % i;
		_fresh_majs.push_back(dispatch_majs[index]);
		dispatch_majs.erase(dispatch_majs.begin() + index);
	}
	_wildcard = _fresh_majs[rand() % 136];
	update();
}

maj frame::deal()
{
	if (_fresh_majs.empty())
		return maj(0);
	maj &mj = _fresh_majs[_fresh_majs.size() - 1];
	_fresh_majs.erase(_fresh_majs.begin() + _fresh_majs.size() - 1);
	update();
	return mj;
}

void frame::deal_to_players()
{
	for (int p = 0; p != _players.size(); p++) {
		for (int m = 0; m != 13; m++)
			_players[p]->insert(deal());
	}
}

void frame::add_player(player &p)
{
	_players.push_back(&p);
}

void frame::start()
{
	shuffle();
	deal_to_players();
}