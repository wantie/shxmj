#include "maj_view.h"
#include "hu.h"
using namespace std;

mj_box::mj_box(window *parent, maj &mj) : window(parent),_maj(mj)
{
		_view = 0;
		_hbitmap = LoadBitmap(get_instance(), _maj.bitmap_name().c_str());
		if (!_hbitmap )
			MessageBox(get_hwnd(), _maj.bitmap_name().c_str(), TEXT("error"),0);
};

bool mj_box::wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	player  *the_player;
	switch(msg) {
	case WM_PAINT:
		the_player = static_cast<player *>(_view->get_model());
		_view->get_state()->on_paint(*this);
		return true;
	case WM_SETFOCUS:
		return true;  
	case WM_KILLFOCUS:
			
	case WM_LBUTTONDOWN:
		the_player= static_cast<player *>(_view->get_model());
		_view->get_state()->on_left_mouse(*this);
		return true;
	case WM_RBUTTONDOWN:
		the_player = static_cast<player *>(_view->get_model());
		_view->get_state()->on_right_mouse(*this);
		return true;
	case WM_RBUTTONDBLCLK:
		the_player = static_cast<player *>(_view->get_model());
		vector<hu *> kinds_of_hu = the_player->judge();
		wstring cele_str;
		for (int i = 0; i != kinds_of_hu.size(); i++)
			cele_str += kinds_of_hu[i]->name;
		if (kinds_of_hu.size() > 0) 
			MessageBox(get_hwnd(), cele_str.c_str(), TEXT("*_*"), 0);
	}
	return false;
}

player *mj_box::get_player()
{
	return static_cast<player *>(_view->get_model());
}
void player_view::update()
{
	// close all windows,
	// recreate window for _majs;
	// show all
	destroy_mj_box_vector(_mj_windows);
	destroy_mj_box_vector(_chi_mj_windows);
	destroy_mj_box_vector(_gang_mj_windows);
	destroy_mj_box_vector(_peng_mj_windows);
	destroy_mj_box_vector(_bao);

	std::vector<maj> &majs = _player->get_majs();
	int x_position = 0;
	for (int i = 0; i != majs.size(); i++) {
		int width = 60, height = 75;
		mj_box *box = new mj_box(_container_window, majs[i]);
		box->set_view(this);
		box->set_index(i);
		box->set_position(mj::point(x_position + 1, 500));
		if (_state->choosed(i)) {
			width = 70;
			height = 88;
		}
		x_position += width;
		box->set_width(width);
		box->set_height(height);
		box->create();
		box->show();
		_mj_windows.push_back(box);
	}

	// chi;
	std::vector<maj> &chi = _player->get_chi();
	int i;
	for (i = 0; i != chi.size(); i++) {
		mj_box *box = new mj_box(_container_window, chi[i]);
		box->set_view(this);
		box->set_position(mj::point(i * 40 + 3, 500 + 90));	
		box->set_width(40);
		box->set_height(50);
		box->create();
		box->show();
		_mj_windows.push_back(box);
	}
	vector<maj> &peng = _player->get_peng();
	for (int p = 0; p != peng.size(); p++) {
		for (int j = 0; j != 3; j++) {
			mj_box *box = new mj_box(_container_window, peng[p]);
			box->set_view(this);
			box->set_position(mj::point((40 * i++) + 3, 500 + 90));	
			box->set_width(40);
			box->set_height(50);
			//box->set_index(3101 + p * 3 + j);
			box->create();
			box->show();
			_mj_windows.push_back(box);
		}
	}
	
	vector<maj> &gang = _player->get_gang();
	for (int p = 0; p != gang.size(); p++) {
		for (int j = 0; j != 4; j++) {
			mj_box *box = new mj_box(_container_window, gang[p]);
			box->set_view(this);
			box->set_position(mj::point((40 * i++) + 3, 500 + 90));	
			box->set_width(40);
			box->set_height(50);
			box->create();
			box->show();
			_mj_windows.push_back(box);	
		}
	}
}
void player_view::destroy_mj_box_vector(std::vector<mj_box*> &vec)
{
	for (int i = 0; i != vec.size(); i++) {
		vec[i]->destroy();
		delete vec[i];
	};
	vec.clear();
}

bool idle::on_fetch(player *p) 
{ 
	p->insert(p->fetch());
	p->get_view()->set_state(fetch::instance());
	return true;
}
bool fetch::on_right_mouse(mj_box &mj)
{
	player *the_player = mj.get_player();
	mj.get_view()->set_state(idle::instance());
	the_player->drop(mj.get_index());
	return true;
}

bool drop::on_right_mouse(mj_box &mj)
{
	player *the_player = mj.get_player();
	mj.get_view()->set_state(idle::instance());
	the_player->drop(mj.get_index());
	return true;
}
bool chi::on_left_mouse(mj_box &mj)
{
	if (choosed(mj.get_index())) {
		unchoose(mj);
		return true;
	} 
	
	if (_choosed_mj.size() == 2)
		return true;

	choose(mj);
	return true;
}

bool chi::on_right_mouse(mj_box &mj)
{
	if (_choosed_mj.size() == 2) {
		player_view *view = mj.get_view();
		set<int>::iterator it = _choosed_mj.begin();
		int i1 = *it++;
		int i2 = *it++;
		view->get_state()->clear();
		mj.get_player()->chi(i1, i2, _target);
		view->set_state(drop::instance());
	}
	return true;
}

bool peng::on_left_mouse(mj_box &mj)
{
	if (choosed(mj.get_index())) {
		unchoose(mj);
		return true;
	}
	
	if (_choosed_mj.size() == 2)
		return true;

	choose(mj);	
	return true;
}

bool peng::on_right_mouse(mj_box &mj)
{
	if (_choosed_mj.size() == 2) {
		player_view *view = mj.get_view();
		set<int>::iterator it = _choosed_mj.begin();
		int i1 = *it++;
		int i2 = *it++;
		mj.get_player()->peng(i1, i2, _target);
		view->get_state()->clear();
		view->set_state(drop::instance());
	}
	return true;
}

bool gang::on_left_mouse(mj_box &mj)
{
	if (choosed(mj.get_index())) {
		unchoose(mj);
		return true;
	}
	if (_choosed_mj.size() == 3)
		return true;
	choose(mj);	
	return true;
}

bool gang::on_right_mouse(mj_box &mj)
{
	if (_choosed_mj.size() == 3) {
		player_view *view = mj.get_view();
		set<int>::iterator it = _choosed_mj.begin();
		int i1 = *it++;
		int i2 = *it++;
		int i3 = *it++;
		mj.get_player()->gang(i1,i2,i3, _target);
		view->get_state()->clear();
		view->set_state(drop::instance());
	}
	return true;
}
bool fresh_maj_window::wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	player  *the_player;
	HDC hdc, hdcmj, hdcmem;
	HBITMAP hdcmap, wildmap;
	PAINTSTRUCT ps;
	int num_paint_majs;
	int i,j;
	switch(msg) {
	case WM_PAINT:
		InvalidateRect(get_hwnd(), NULL, TRUE);
		hdc = BeginPaint(get_hwnd(),&ps);
		hdcmj = CreateCompatibleDC(hdc);
		hdcmem = CreateCompatibleDC(hdc);
		hdcmap  = CreateCompatibleBitmap(hdc, get_width(), get_height());
		SelectObject(hdcmem, hdcmap);
		BitBlt(hdcmem, 0, 0, get_width(), get_height(), hdcmem, 0, 0, WHITENESS);
		SelectObject(hdcmj, _2012);
		num_paint_majs = _frame->get_fresh_majs().size();
		for (int row = 0; row != get_height() / (37 + 2) && num_paint_majs > 0; row++) {
			for (int col = 0; col != get_width() / (30 + 1) && num_paint_majs > 0; col++) {
				StretchBlt(hdcmem, col * 30 + 1, row * 37 + 2, 30, 37, hdcmj, 0, 0,60,75, SRCCOPY);
				num_paint_majs--;
			}
		}
		BitBlt(hdc, 0, 0, get_width(), get_height(), hdcmem, 0, 0, SRCCOPY);
		wildmap = LoadBitmap(get_instance(), _frame->get_wildcard().bitmap_name().c_str());
		SelectObject(hdcmj, wildmap);
		StretchBlt(hdc, 0, 230, 60, 75, hdcmj, 0, 0, 90, 112, SRCCOPY);
		DeleteObject(wildmap);
		DeleteDC(hdcmj);
		DeleteDC(hdcmem);
		DeleteObject(hdcmap);
		EndPaint(get_hwnd(), &ps);
		return true;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		the_player = _frame->get_self_player();
		if (the_player != _frame->get_current_player())
			return true;
		the_player->get_view()->fetch();
		return true;
	}
	return false;
}

bool drop_maj_window::wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	player  *the_player = _frame->get_self_player();
	player_view *the_player_view = the_player->get_view();
	play_state &idle_state = idle::instance();
	HDC hdc, hdcmem;
	PAINTSTRUCT ps;
	
	the_player = _frame->get_self_player();
	switch(msg) {
	case WM_PAINT:
		hdc = BeginPaint(get_hwnd(),&ps);
		hdcmem = CreateCompatibleDC(hdc);
		SelectObject(hdcmem, _bitmap);
		StretchBlt(hdc, 0, 0, 30, 37, hdcmem, 0, 0,90,112, SRCCOPY);
		DeleteDC(hdcmem);
		EndPaint(get_hwnd(), &ps);
		return true;

	case WM_LBUTTONDOWN:
		if (the_player_view->get_state() != &idle_state)
			return true;
		the_player_view->set_state(chi::instance());
		static_cast<chi *>(the_player_view->get_state())->set_target(_mj);
		return true;
	case WM_RBUTTONDOWN:	
		if (the_player->get_view()->get_state() != &idle_state)
			return true;
		the_player_view->set_state(peng::instance());
		static_cast<peng *>(the_player_view->get_state())->set_target(_mj);
		return true;
	case WM_LBUTTONDBLCLK:
		if (the_player->get_view()->get_state() != &idle_state)
			return true;
		the_player_view->set_state(gang::instance());
		static_cast<gang*>(the_player_view->get_state())->set_target(_mj);
		return true;
	}
	return false;
}

void frame_view::update()
{
	InvalidateRect(_fresh_window.get_hwnd(), NULL, TRUE);
	
	for (int i = 0; i != _drop_windows.size(); i++) {
		_drop_windows[i]->destroy();
		delete _drop_windows[i];
	};
	_drop_windows.clear();

	for (int i = 0; i != get_frame()->get_droped_majs().size(); i++) {
		drop_maj_window *droped_mj_win = new drop_maj_window(_container_window);
		droped_mj_win->set_maj(get_frame()->get_droped_majs()[i]);
		droped_mj_win->set_frame(get_frame());
		droped_mj_win->set_position(point(20 + 30 * (i % 30), 300 + 37 * (i / 30)));
		droped_mj_win->set_width(30);
		droped_mj_win->set_height(37);
		droped_mj_win->create();
		droped_mj_win->show();
		_drop_windows.push_back(droped_mj_win);
	}
}

frame_view::frame_view(window *container_win, frame *f)
 : _fresh_window(container_win)
{
	_container_window = container_win;
	_fresh_window.set_position(point(0, 0));
	_fresh_window.set_width(800);
	_fresh_window.set_height(300);
	_fresh_window.set_frame(f);
	_fresh_window.create();
	_fresh_window.show();
}