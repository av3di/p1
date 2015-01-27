////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "Header.h"
#define WINDOWTITLE	"Attach Skin to Skeleton"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;
static int joint_index = 0;

int main(int argc, char **argv) {
	// To input command line args, go to properties -> debugging
	//std::cout << "you input " << argc << " arguments"<< std::endl;
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );



	// Initialize components
	
	Cam.SetAspect(float(WinX)/float(WinY));

	if (argc <= 1)	                      // Skel file not specified
		jack.load("test.skel");
	else                                  // Skel file specified
		jack.load(argv[1]);
	jack.update();
	jack.init_all_joints(jack.root);
	std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	//Cube.Update();
	

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));

	//Cube.Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	//Cube.Draw();
	jack.draw();
	//getchar();
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	Joint* current_joint = jack.all_joints[joint_index];
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		case 'd':
			jack.all_joints.clear(); // clear all previous joints from previous skeleton
			jack.load("dragon.skel");
			jack.update();
			jack.init_all_joints(jack.root);
			if (joint_index >= jack.all_joints.size())  // Make sure index is within range
				joint_index = 0;
			std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
			break;
		case 't':
			jack.all_joints.clear();
			jack.load("test.skel");
			jack.update();
			jack.init_all_joints(jack.root);
			if (joint_index >= jack.all_joints.size())
				joint_index = 0;
			std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
			break;
		case 'w':
			jack.all_joints.clear(); 
			jack.load("wasp.skel");
			jack.update();
			jack.init_all_joints(jack.root);
			if (joint_index >= jack.all_joints.size())
				joint_index = 0;
			std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
			break;
		case 'u':
			jack.all_joints.clear();
			jack.load("tube.skel");
			jack.update();
			jack.init_all_joints(jack.root);
			if (joint_index >= jack.all_joints.size())
				joint_index = 0;
			std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
			break;
		case 'n':
			joint_index++;
			if (joint_index >= jack.all_joints.size())
				joint_index = 0;
			std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
			break;
		case 'X':
			(current_joint->getDofX())->setPose(((current_joint->getDofX())->getPose()) + 0.1); // Add 0.01 to the current pose
			jack.update();
			break;
		case 'x':
			(current_joint->getDofX())->setPose(((current_joint->getDofX())->getPose()) - 0.1); // Subtract 0.01 to the current pose
			jack.update();
			break;
		case 'Y':
			(current_joint->getDofY())->setPose(((current_joint->getDofY())->getPose()) + 0.1); // Add 0.01 to the current pose
			jack.update();
			break;
		case 'y':
			(current_joint->getDofY())->setPose(((current_joint->getDofY())->getPose()) - 0.1); // Subtract 0.01 to the current pose
			jack.update();
			break;
		case 'Z':
			(current_joint->getDofZ())->setPose(((current_joint->getDofZ())->getPose()) + 0.1); // Add 0.01 to the current pose
			jack.update();
			break;
		case 'z':
			(current_joint->getDofZ())->setPose(((current_joint->getDofZ())->getPose()) - 0.1); // Subtract 0.01 to the current pose
			jack.update();
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
