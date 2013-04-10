#pragma once

class model;
class view
{
	model *_model;
public:
	view () {};
	virtual void set_model(model *m) {_model = m; };
	virtual model *get_model() { return _model; };
	virtual void update() = 0;
};

class model
{
	view *_view;
public:
	model () :_view(0) {}
	void set_view(view *v) {_view = v; _view->set_model(this);}
	view *get_view() { return _view; }
	virtual void update() { if (_view) _view->update(); }
};

class controller
{
public:
	
};
