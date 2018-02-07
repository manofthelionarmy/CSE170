
# include "my_viewer.h"
# include "sn_mynode.h"
# include <math.h>
# include <sigogl/gl_tools.h>
# include <sigogl/ui_button.h>
#include <vector>
#include <iostream>
static std::vector<SnMyNode*> nodes; 

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	add_ui ();
		//WsViewer::message("Left Key: Move Left \t\tUp Key: Move Up \t\tRight Key: Move Right \t\tDown Key: Move Down \t\tEnter Key: Change Color");

	//add_mynode (4); //the code provided for the sample code
	//add_node(); 

		torus_node(0.5, 0.5, 20); 
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

void MyViewer::torus_node(float r, float R, int n) {


	int phi = 0; 
	int theta = 0; 

	float x = 0.0f; 
	float y = 0.0f; 
	float z = 0.0f; 

	SnMyNode *c; 

	 
	std::vector<GsVec>* points; 


	
	c = new SnMyNode();
	
	for (int i = 0; i <= 360; ++i) {
		c->GsVecArray.push_back(NULL); 
	}
	

	for (phi = 0; phi <= 360; phi++) {

		points = new std::vector<GsVec>(361);
		for (theta = 0; theta <= 360; theta++) {
			x = float((R + r * cos(theta))*cos(phi));
			y = float((R + r * cos(theta)) * sin(phi)); 
			z = float(r * sin(phi)); 			

			GsVec  p =  GsVec(x, y, z);

			points->at(theta) = p; 
			
		}

		c->GsVecArray.at(phi) = points; 
		

		//points->clear(); 



	}

	c->color(GsColor::darkblue);
	
	c->init.set(0.0f, 0.0f, 0.0f); 

	rootg()->add(c); 

	return; 
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

	std::vector<char*> name = { "L", "E", "O", "N" }; 

	float r = 0.15f; 
	float x = 0.0f; 
	float y = 0.0f; 
	float z = 0.0f; 
	

	for (int i = 0; i < 4; ++i) {
		c = new SnMyNode;
		c->init.set(x, y, z);
		c->width = r;
		c->height= 2*r;
		c->color(GsColor::black);
		c->shape = name.at(i);

		nodes.push_back(c);

		rootg()->add(nodes.at(i));
		
		x = x + 0.5f; 	
	}

	return; 
}

void MyViewer:: update_node(float x, float y) { //The function I implemented in lab
	
	std::vector<SnMyNode *>::iterator it; 

	for (it = nodes.begin(); it != nodes.end(); ++it) {
		(*it)->init.x = x; 
		(*it)->init.y = y; 

		(*it)->touch();
		
	
		render(); 

		x = x + 0.5f;
	}

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
		{
			//gsout << "Key pressed: " << e.key << gsnl;
			
			std::vector<SnMyNode*>::iterator it;
			for (it = nodes.begin(); it != nodes.end(); ++it) {
				
				(*it)->init.x -= 0.1f; 

				(*it)->touch(); 
				
				render();
				
			}
		
			return 1;
		}
		case GsEvent::KeyRight:
		{
			//gsout << "Key pressed: " << e.key << gsnl;

			std::vector<SnMyNode*>::iterator it;
			for (it = nodes.begin(); it != nodes.end(); ++it) {
				
				(*it)->init.x += 0.1f;

				(*it)->touch();

				render();

			}

			return 1;
		}
		case GsEvent::KeyDown:
		{
			//gsout << "Key pressed: " << e.key << gsnl;

			std::vector<SnMyNode*>::iterator it;
			for (it = nodes.begin(); it != nodes.end(); ++it) {
				
				(*it)->init.y -= 0.1f;

				(*it)->touch();

				render();

			}

			return 1;
		}
		case GsEvent::KeyUp:
		{
			//gsout << "Key pressed: " << e.key << gsnl;

			std::vector<SnMyNode*>::iterator it;
			for (it = nodes.begin(); it != nodes.end(); ++it) {
				
				(*it)->init.y += 0.1f;

				(*it)->touch();

				render();

			}

			return 1;
		}
		case GsEvent::KeyEnter: 
		{
			std::vector<SnMyNode*>::iterator it;
			for (it = nodes.begin(); it != nodes.end(); ++it) {
				(*it)->color(GsColor::random());
				render();

			}
			return 1;
		}
		
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
