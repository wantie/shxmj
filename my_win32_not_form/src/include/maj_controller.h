#include "maj.h"
#include "gui.h"

// 每个玩家会有一副手牌，共享出牌区和捉牌区,UI相关
class play_control
{
	player _my_majs;
public:
	bool chi(maj & own1, maj &own2, maj &eat_one);
	bool peng(maj &mj);
	bool gang(maj &mj);
	void out_hand(maj &mj);
	void hu(); // well designed;
};

class frame
{
	
};

/*
界面上有 视图 和 控制，

手牌  桌面   捉牌  根据玩家输入的控制进行变化， 界面上要显示变化，

各种控制由鼠标或者UI控件驱动


过程：
 控件或鼠标或定时器驱动控制：
 具体控制： 捉牌，吃（选牌，确定），更新模型，视图更新
 */

