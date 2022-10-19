#include <glad.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include <typeinfo>
#include <stdexcept>

#include <cstdio>

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

#include "../support/error.hpp"
#include "../support/context.hpp"

namespace
{
	constexpr int kInitialWindowWidth = 1280;
	constexpr int kInitialWindowHeight = 720;
	constexpr char const* kWindowTitle = "COMP3811-Exercise 1";

	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );
	void glfw_cb_motion_( GLFWwindow*, double, double );
	void glfw_cb_button_( GLFWwindow* , int , int, int );


	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};

	struct MousePos {
		double x;
		double y;
	};

	struct ClickPos {
		double xMin = 0, yMin = 0, xMax = 0, yMax = 0;
		bool isSecondClick = false;
	};
}

int main() try
{
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	glfwSetErrorCallback( &glfw_callback_error_ );

	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE ); // Prevent resizing for now...

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		kInitialWindowWidth,
		kInitialWindowHeight,
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
	}

	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	assert( iwidth >= 0 && iheight >= 0 );
	auto const fbwidth = std::size_t(iwidth);
	auto const fbheight = std::size_t(iheight);

	MousePos position;
	ClickPos click;
	glfwSetWindowUserPointer(window, &click);
	glfwSetWindowUserPointer(window, &position);

	glfwSetKeyCallback( window, &glfw_callback_key_ );
	glfwSetCursorPosCallback(window, &glfw_cb_motion_);
	glfwSetMouseButtonCallback(window, &glfw_cb_button_);

	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 );

	Context context( fbwidth, fbheight );
	Surface surface( fbwidth, fbheight );

	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwWaitEvents();
		//glfwPollEvents();

		// Update state
		// Nothing to do at the moment...
	
		// Draw scene
		surface.clear();

		ClickPos* clickPos = (ClickPos*)glfwGetWindowUserPointer(window);


		if(clickPos->xMin > 0 && clickPos->xMax > 0) {
			std::cout << "Xmin:" << clickPos->xMin << "yMin:" << clickPos->yMin << "\n";
			std::cout << "Xmax:" << clickPos->xMax << "yMax:" << clickPos->yMax << "\n";
			draw_rectangle_solid(surface, { (float)clickPos->xMin, (float)clickPos->yMin },{ (float)clickPos->xMax, (float)clickPos->yMax },{ 255, 255, 0 } );
			clickPos->xMin = 0;
			clickPos->yMin = 0;
			clickPos->xMax = 0;
			clickPos->yMax = 0;
			glfwSetWindowUserPointer(window, clickPos);
		}

		// draw_rectangle_outline(surface, { 50.f, 300.f },{ 200.f, 450.f },{ 255, 0, 0 } );

		//TODO: drawing code goes here
		// surface.set_pixel_srgb( 10, 100, { 255, 255, 255 } );
		// surface.set_pixel_srgb( 5, 80, { 255, 255, 255 } );
		// surface.set_pixel_srgb( 1, 10, { 255, 255, 255 } );

		context.draw( surface );

		// Display results
		glfwSwapBuffers( window );
	}

	// Cleanup.
	// For now, all objects are automatically cleaned up when they go out of
	// scope.
	
	return 0;
}
catch( std::exception const& eErr )
{
	std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() );
	std::fprintf( stderr, "%s\n", eErr.what() );
	std::fprintf( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
	}

	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}
	}
	// MOUSE POSITION TASK
	void glfw_cb_motion_( GLFWwindow* aWindow, double aMouseXPos, double aMouseYPos ) {
		MousePos* position = (MousePos*)glfwGetWindowUserPointer(aWindow);

		position->x = aMouseXPos;
		position->y = aMouseYPos;

		glfwSetWindowUserPointer(aWindow, position);
	}

	void glfw_cb_button_( GLFWwindow* aWindow, int aButton, int aAction, int mod) {
		if (GLFW_MOUSE_BUTTON_LEFT == aAction) {
			MousePos* position = (MousePos*)glfwGetWindowUserPointer(aWindow);
			ClickPos* clickPos = (ClickPos*)glfwGetWindowUserPointer(aWindow);
			// std::cout << "X:" << position->x << "Y:" << position->y << "\n";
			if (clickPos->isSecondClick != true) {
				clickPos->xMin = position->x;
				clickPos->yMin = position->y;
				clickPos->isSecondClick = true;
				std::cout << "Xmin:" << clickPos->xMin << "Ymin:" << clickPos->yMin << "isSecondClick: " << clickPos->isSecondClick << "\n";
			} else {
				clickPos->xMax = position->x;
				clickPos->yMax = position->y;
				clickPos->isSecondClick = false;
				std::cout << "Xmax:" << clickPos->xMax << "Ymax:" << clickPos->yMax << "isSecondClick: " << clickPos->isSecondClick << "\n";
			}

			glfwSetWindowUserPointer(aWindow, clickPos);
		}
	}

}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}
}


