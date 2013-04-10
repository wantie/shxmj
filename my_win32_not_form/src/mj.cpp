#include "gui.h"
//#include "shape.h"
#include <string>
#include <math.h>
#include "maj_view.h"
using namespace std;
using namespace mj;



class main_window : public window
{
	bool wnd_proc(UINT msg, WPARAM wparam, LPARAM lparam)
	{		
		maj mj(0);
		static int client_width = 100, client_height=100;
		switch (msg) {
		case WM_CREATE:
			return false;
		case WM_SIZE:
			client_width = LOWORD (lparam) ;
			client_height = HIWORD (lparam) ;
			InvalidateRect(get_hwnd(), NULL, true);
			return true;
		case WM_KEYDOWN:
			return true;
		case WM_LBUTTONDOWN:
			//mj = player1.fetch();
			//player1.insert(mj, -1);
			return true;
		case WM_CHAR:
		//	c = (TCHAR)wparam;
		//	buf[0]  =c;buf[1] = TCHAR('\0');
		//	MessageBox(get_hwnd(),  buf, TEXT("what"), 0);
			return true;
		case WM_NCHITTEST:
			return (LRESULT) HTNOWHERE ;
		case WM_COMMAND:
				return true;
		default:
			return false;
		}
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	main_window main_window;
	main_window.set_window_name(wstring(TEXT("shaximj")));
	main_window.set_width(900);
	main_window.set_height(700);
	main_window.create();
	main_window.show();

	frame the_frame;
	frame_view the_frame_view(&main_window, &the_frame);
	the_frame.set_view(&the_frame_view);

	player player1;
	player_view view;

	player1.set_view(&view);
	player1.set_frame(the_frame);
	view.set_model(&player1);
	view.set_container_window(main_window);
	view.set_state(idle::instance());
	the_frame.start();
	application::instance().run();
}


