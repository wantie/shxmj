#pragma once 

#include "gui.h"
#include "maj_model.h"
#include "../resource.h"
#include <set>

class player_view;
class mj_box : public window
{
	maj _maj;
	HBITMAP _hbitmap;
	player_view *_view;
	int _index; // index in model
public:
	mj_box(window *parent, maj &mj);
	void set_view(player_view *v) {_view = v;}
	player_view *get_view() { return _view; };
	void set_index(int pos) { _index = pos; }
	int get_index()const { return _index; }
	player *get_player();
	HBITMAP get_bitmap() {return _hbitmap; }
	void set_bitmap(HBITMAP hbitmap)
	{
		//close bitmap first;
		_hbitmap = hbitmap;
	}
	bool wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam);
};

class play_state
{
public:
	virtual bool on_paint(mj_box &mj)
	{
		HDC hdc, hdcmem;
		PAINTSTRUCT ps;
		hdc = BeginPaint(mj.get_hwnd(),&ps);
		hdcmem = CreateCompatibleDC(hdc);
		SelectObject(hdcmem,mj.get_bitmap());
		StretchBlt(hdc, 0, 0, mj.get_width(), mj.get_height(), hdcmem, 0, 0,90,112, SRCCOPY);
		DeleteDC(hdcmem);
		EndPaint(mj.get_hwnd(), &ps);
		return true;
	}
	virtual bool on_left_mouse(mj_box &mj) {return true;}
	virtual bool on_right_mouse(mj_box &mj) {return true;}
	virtual bool on_mouse_move(mj_box &mj) {return true;}
	virtual bool on_fetch(player *p) { return true; }
	virtual void clear() {};
	virtual bool choosed(int index) { return false; }
};

class player_view : public view
{
	player *_player;
	play_state *_state;
	std::vector<mj_box *> _mj_windows;
	std::vector<mj_box *> _chi_mj_windows;
	std::vector<mj_box *> _gang_mj_windows;
	std::vector<mj_box *> _peng_mj_windows;
	std::vector<mj_box *> _bao;

	window *_container_window;
public:
	void set_model(model *m) { _player = static_cast<player *>(m); }
	model *get_model() { return _player; }
	void set_state(play_state &state) { _state = &state; }
	play_state *get_state() { return _state; }
	bool fetch() { _state->on_fetch(_player); return true;}
	void set_container_window(window &win)
	{
		_container_window = &win;
	}
	void update();
private:
	void destroy_mj_box_vector(std::vector<mj_box*> &vec);
};



class idle : public play_state
{
public:
	static play_state &instance() { static idle the_state; return the_state;}
	bool on_fetch(player *p);
};

class fetch : public play_state
{
public:
	static play_state &instance() {static fetch the_state; return the_state;};
	bool on_right_mouse(mj_box &mj);
};

class drop : public play_state
{
public:
	static play_state &instance() { static drop the_state; return the_state; };
	bool on_right_mouse(mj_box &mj);
};
class chi : public play_state
{
	std::set<int> _choosed_mj;
	maj _target;
	void choose(mj_box &mj) 
	{
		_choosed_mj.insert(mj.get_index());
		mj.get_view()->update();
	}
	void unchoose(mj_box &mj)
	{
		_choosed_mj.erase(mj.get_index());
		mj.get_view()->update();
	}

public:
	chi(){};
	static play_state &instance() { static chi the_state; return the_state;}
	void set_target(maj &mj) { _target = mj; }
	bool on_left_mouse(mj_box &mj);
	bool on_right_mouse(mj_box &mj);
	void clear() { _choosed_mj.clear(); }
	bool choosed(int index) { return _choosed_mj.find(index) != _choosed_mj.end();}
};
class peng: public play_state
{
	std::set<int> _choosed_mj;
	maj _target;
	
	void choose(mj_box &mj) 
	{
		_choosed_mj.insert(mj.get_index());
		mj.get_view()->update();
	}
	void unchoose(mj_box &mj)
	{
		_choosed_mj.erase(mj.get_index());
		mj.get_view()->update();
	}
public:
	void set_target(maj &mj) { _target = mj; }
	static play_state &instance() { static peng the_state; return the_state; }
	bool on_left_mouse(mj_box &mj);
	bool on_right_mouse(mj_box &mj);
	void clear() { _choosed_mj.clear(); }
	bool choosed(int index) { return _choosed_mj.find(index) != _choosed_mj.end(); }
};
class gang : public play_state
{
	std::set<int> _choosed_mj;
	maj _target;
	
	void choose(mj_box &mj) 
	{
		_choosed_mj.insert(mj.get_index());
		mj.get_view()->update();
	}
	void unchoose(mj_box &mj)
	{
		_choosed_mj.erase(mj.get_index());
		mj.get_view()->update();
	}
public:
	static play_state &instance() { static gang the_state; return the_state;}
	void set_target(maj &mj) { _target = mj; }
	bool on_left_mouse(mj_box &mj);
	bool on_right_mouse(mj_box &mj);
	void clear() { _choosed_mj.clear(); }
	bool choosed(int index) { return _choosed_mj.find(index) != _choosed_mj.end(); }
};

class fresh_maj_window : public window
{
	frame *_frame;
	HBITMAP _2012;
public:
	fresh_maj_window(window *parent) : window(parent)
	{
		_2012 = LoadBitmap(get_instance(), TEXT("black")); 
				if (!_2012 )
			MessageBox(parent->get_hwnd(), strsyserror().c_str(), TEXT("error"),0);
	};
	~fresh_maj_window()
	{
		DeleteObject(_2012);
	}
	void set_frame(frame *f) { _frame = f; }
	frame *get_frame() { return _frame; }

	bool wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam);
};

class drop_maj_window : public window
{
	frame *_frame;
	maj _mj;
	HBITMAP _bitmap;
public:
	~drop_maj_window() { DeleteObject(_bitmap); }
	drop_maj_window(window *parent) : window(parent) {};
	void set_maj(maj &mj) { _mj = mj; _bitmap = LoadBitmap(get_instance(), _mj.bitmap_name().c_str());}
	void set_frame(frame *f) { _frame = f; }
	bool wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam);
};

class frame_view : public view
{
	fresh_maj_window _fresh_window;
	std::vector<drop_maj_window *> _drop_windows;
	window *_container_window;
public:
	frame_view(window *container_window, frame *f);
	void set_frame(frame *f) { set_model(f); }
	frame *get_frame() { return static_cast<frame *>(get_model()); }
	void update();
};