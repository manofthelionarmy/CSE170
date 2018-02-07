
# include "sn_mynode.h"
# include "glr_mynode.h"

# include <sig/gs_array.h>
# include <sig/gs_quat.h>

# include <sigogl/gl_core.h>
# include <sigogl/gl_context.h>
# include <sigogl/gl_resources.h>

//# define GS_USE_TRACE1 // Constructor and Destructor
//# define GS_USE_TRACE2 // Render
# include <sig/gs_trace.h>

//======================================= GlrLines ====================================

GlrMyNode::GlrMyNode ()
{
	GS_TRACE1 ( "Constructor" );
	_psize = 0;
}

GlrMyNode::~GlrMyNode ()
{
	GS_TRACE1 ( "Destructor" );
}

static const GlProgram* Prog=0; // These are static because they are the same for all GlrMyNode instances

void GlrMyNode::init ( SnShape* s )
{
	GS_TRACE2 ( "Generating program objects" );
	// Initialize program and buffers if needed:
	if ( !Prog )
	{	bool MyShaders=true;
		if ( MyShaders ) // Example of how to load specific shader files:
		{	const GlShader* MyVtxShader  = GlResources::declare_shader ( GL_VERTEX_SHADER, "MyVtxShader", "../../sig/shaders/3dsmoothsc.vert", 0 );
			const GlShader* MyFragShader = GlResources::declare_shader ( GL_FRAGMENT_SHADER, "MyFragShader", "../../sig/shaders/gouraud.frag", 0 );
			const GlProgram* p = GlResources::declare_program ( "MyNodeProgram", 2, MyVtxShader, MyFragShader );
			GlResources::declare_uniform ( p, 0, "vProj" );
			GlResources::declare_uniform ( p, 1, "vView" );
			GlResources::declare_uniform ( p, 2, "vColor" );
			GlResources::compile_program ( p );
			Prog = p; // Save in Prog a direct pointer to the program used by this node!
		}
		else // Here we just reuse an internal shader:
		{	Prog = GlResources::get_program("3dsmoothsc");
		}
	}

	_glo.gen_vertex_arrays ( 1 );
	_glo.gen_buffers ( 1 );
}

void GlrMyNode::render ( SnShape* s, GlContext* ctx )
{
	GS_TRACE2 ( "GL4 Render "<<s->instance_name() );
	SnMyNode& c = *((SnMyNode*)s);

	// 1. Set buffer data if node has been changed:
	if ( s->changed()&SnShape::Changed ) // flags are: Unchanged, RenderModeChanged, MaterialChanged, Changed
	{	
		
	
		GsArray<GsVec> P(0,2000); // will hold the points forming my triangles (size 0, but pre-allocate 6 spaces) -->PA 2: 360 points for now
		GsPnt o = c.init; 
		int prevPhi = 0; 
		int nextPhi = 0; 

		int prevTheta = 0; 
		int nextTheta = 0; 
		

		for (nextTheta = 1; nextTheta <= 360; nextTheta++) {
			//std::vector<GsVec>p1 = *(c.GsVecArray.at(prevPhi)); 
			//std::vector<GsVec>p2 = *(c.GsVecArray.at(nextPhi));

		

			for (nextPhi = 1; nextPhi <= 360; ++prevPhi) {

				GsVec A = c.GsVecArray.at(prevPhi)->at(prevTheta); 
				GsVec B = c.GsVecArray.at(prevPhi)->at(nextTheta); 
				GsVec C = c.GsVecArray.at(nextPhi)->at(prevTheta);
				GsVec D = c.GsVecArray.at(nextPhi)->at(nextTheta); 

				prevPhi = nextPhi;

				P.push() = A; 
				P.push() = C; 
				P.push() = B; 

				P.push() = C; 
				P.push() = D; 
				P.push() = B; 

			}

			
			prevTheta = nextTheta; 
		}
		

		


	

		glBindVertexArray ( _glo.va[0] );
		glEnableVertexAttribArray ( 0 );
		glBindBuffer ( GL_ARRAY_BUFFER, _glo.buf[0] );
		glBufferData ( GL_ARRAY_BUFFER, P.sizeofarray(), P.pt(), GL_STATIC_DRAW );
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		_psize = P.size(); // after this line array P will be deallocated (data has been sent to graphics card)
	}

	// 2. Enable/bind needed elements and draw:
	if ( _psize )
	{	GS_TRACE2 ( "Rendering w/ single color..." );
		ctx->use_program ( Prog->id ); // ctx tests if the program is being changed

		glUniformMatrix4fv ( Prog->uniloc[0], 1, GLTRANSPMAT, ctx->projection()->e );
		glUniformMatrix4fv ( Prog->uniloc[1], 1, GLTRANSPMAT, ctx->modelview()->e );
		glUniform4fv ( Prog->uniloc[2], 1, s->color().vec4() );

		glBindVertexArray ( _glo.va[0] );
		glDrawArrays ( GL_TRIANGLES, 0, _psize );
	}

	glBindVertexArray ( 0 ); // done - this call protects our data from subsequent OpenGL calls
}

//================================ EOF =================================================
