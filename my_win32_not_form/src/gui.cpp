#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include "shape.h"
#include "gui.h"
#include "system.h"

/*
static application& application::instance()
{
		static application the_only_one;
		return the_only_one;
}
*/
/*
int application::run()
{
	RegisterClass(&_default_wndclass);
	for (std::vector<WNDCLASS>::iterator it = _wndclasses.begin(); it != _wndclasses.end(); it++) {
		RegisterClass(&(*it));
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}*/


LRESULT CALLBACK application::app_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	window *wnd;
	bool msg_handled = false;
	if (msg == WM_NCCREATE || msg == WM_CREATE) {
		CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT *>(lparam);
		wnd = reinterpret_cast<window *>(cs->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
	}
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (ptr) {
		wnd = reinterpret_cast<window *>(ptr);
		if ( wnd->common_wnd_proc(msg, wparam, lparam))
			msg_handled = true;
	}

	if (!msg_handled)
		return DefWindowProc(hwnd, msg, wparam, lparam);
	return 0;
}

/*
application::application()
{
	// init default wndclass: a empty wndclass, default icon, white brush, border frame,no anything
	_default_wndclass.style = 0;
	_default_wndclass.lpfnWndProc = app_proc;
	_default_wndclass.cbClsExtra = 0;
	_default_wndclass.cbWndExtra = 0;
	_default_wndclass.hInstance = GetModuleHandle(NULL);
	_default_wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_default_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	_default_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	_default_wndclass.lpszMenuName = NULL;
	_default_wndclass.lpszClassName = TEXT("empty class");
}
*/
base_window::base_window(HWND parent):_hparent(parent), _instance(GetModuleHandle(NULL)) 
{
	_ex_style = WS_EX_APPWINDOW;
	_style = WS_OVERLAPPEDWINDOW;
	_x = 0;
	_y = 0;
	_width = 1024;
	_height = 768;
	_menu = 0;
	_cls_name = application::instance().get_default_class_name();
	_instance = GetModuleHandle(NULL);
	_dblclk_filtered = false;
		
}

/*
bool base_window::common_wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (wnd_proc(msg, wparam, lparam))
		return true;
	switch (msg) {
	case WM_DESTROY:
		MessageBox(get_hwnd(), TEXT("destroy window"), TEXT("..."), 0);
		PostQuitMessage(0);
		return true;
	default:
		return false;
	}
}*/

/*
window::window(window *parent = 0) : _parent(parent)
{
	if (_parent) {
		_parent->add_child_window(*this);
		this->set_parent(parent->get_hwnd());
	}
}*//*
void window::add_child_window(window &child) 
{
	if (_children.find(child.get_hwnd()) == _children.end()) {
		_children[child.get_hwnd()] = &child;
	}
}*/
/*
void window::remove_child(const window &child)
{
	std::map<HWND, window*>::iterator it = _children.find(child.get_hwnd());
	if (it != _children.end())
		_children.erase(it);
}
*/