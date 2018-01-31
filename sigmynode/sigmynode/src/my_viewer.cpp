
# include "my_viewer.h"
# include "sn_mynode.h"

# include <sigogl/gl_tools.h>
# include <sigogl/ui_button.h>
#include <vector>

static std::vector<SnMyNode*> triangles; 

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	add_ui ();
	//add_mynode (4); //the code provided for the sample code
	add_node(); 
}

void MyViewer::add_ui ()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "Add", EvAdd ) );
	p->add ( new UiButton ( "Info", EvInfo ) );
	p->add ( new UiButton ( "Exit", EvExit ) );
}

void MyViewer::add_mynode ( int n )
{
	SnMyNode* c;

	float r=0.15f; // position range
	
	while ( n-->0 )
	{	c = new SnMyNode;
		c->init.set(gs_random(-r, r), gs_random(-r, r), gs_random(-r, r));
		c->width = gs_random(0.001f, r);
		c->height = gs_random(0.001f, r * 2);
		c->color(GsColor::random());

		// Example how to print/debug your generated data:
		// gsout<<n<<": "<<c->color()<<gsnl;
		rootg()->add(c);
	}
}

void MyViewer::add_node() { //The function I implemented in lab

	SnMyNode * c; 

	float r = 0.15f; 
	float x = 0.0f; 
	float y = 0.0f; 
	float z = 0.0f; 

	

	for (int i = 0; i < 2; ++i) {
		c = new SnMyNode;
		c->init.set(x, y, z);
		c->width = r;
		c->width = r;
		c->color(GsColor::random());

		triangles.push_back(c);

		rootg()->add(triangles.at(i));
		
		x = x + 0.5f; 	
	}

	triangles.at(0)->shape = "L";
	triangles.at(1)->shape = "E";

	return; 
}



int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : 
			gs_exit(); 
			return 1;
		case GsEvent::KeyLeft: 
			gsout<<"Left\n"; 
			return 1;
		case GsEvent::KeyEnter :
			triangles.at(0)->color(GsColor::darkblue); 	
			render(); 
			return 1; 
		// etc
		default: 
			gsout<<"Key pressed: "<<e.key<<gsnl;
			return 1; 
	}

	return 0;
}


int MyViewer::uievent ( int e )
{

	switch ( e )
	{	case EvAdd: add_mynode(1); return 1;

		case EvInfo:
		{	if ( output().len()>0 ) { output(""); return 1; }
			output_pos(0,30);
			activate_ogl_context(); // we need an active context
			GsOutput o; o.init(output()); gl_print_info(&o); // print info to viewer
			return 1;
		}

		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
