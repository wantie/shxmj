class player
{
};

class round
{
	vector<maj> _history_maj;
	vector<maj> _unknown_maj;
	player * _player;
public:
	void shuffle();
	maj deal();
};