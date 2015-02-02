////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "Header.h"
#define WINDOWTITLE	"Attach Skin to Skeleton"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;
static int joint_index = 0;
Light blue(GL_LIGHT0, 90); // Set id and angle respectively
Light red(GL_LIGHT1, 90);

int main(int argc, char **argv) {
	// To input command line args, go to properties -> debugging
	//std::cout << "you input " << argc << " arguments"<< std::endl;
	glutInit(&argc, argv);



	TESTER = new Tester(argc,argv);
	glutMainLoop();
	glDeleteProgram(TESTER->the_shader.ps_id);
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
	glutSetWindow(WindowHandle);

	// Add Choose & Select pixelformat code here

	glewInit();
	// Set up Shaders
	char* vertexShaderSourceCode = the_shader.readFile("do_nothing.vsh");  //gaurd shader
	char* fragmentShaderSourceCode = the_shader.readFile("simple_frag.fsh");
	the_shader.makeVertexShader(vertexShaderSourceCode);
	the_shader.makeFragmentShader(fragmentShaderSourceCode);
	the_shader.makeShaderProgram();
	

	// Background color
	glClearColor( 0., 0., 0., 1. );


	// Lighting
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the lights
	blue.setColor(0.15, 0.15, 0.25, 0.5, 0.5, 1.0, 1.0);
	blue.setPosition(-2.0, 1.0, -1.0, 0.0);
	blue.on();

	red.setColor(0.25, 0.15, 0.15, 1.0, 0.5, 0.5, 1.0);
	red.setPosition(2, 1.0, -1.0, 1);
	red.on();
	
	// Set the material
	mat.setDiffandAmb(0.25, 0.25, 0.25, 1.0);
	mat.setSpec(1.0, 1.0, 1.0, 1.0, 5.0);
	mat.on();
	
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
		jack.load("wasp.skel");
	else                                  // Skel file specified
		jack.load(argv[1]);

	jack.update();  // Prepare the skeleton for rendering

	jack.init_all_joints(jack.root);   // Create a list of all joints in this skeleton to easily traverse thru joints later
	std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
	
	if (argc <= 2)
		the_skin.load("wasp.skin");  // Skin file not specified
	else
		the_skin.load(argv[2]);
	the_skin.update(jack);
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	glMatrixMode(GL_MODELVIEW);

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
	
	glUseProgram(the_shader.ps_id);
	the_skin.draw();

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
		//case 'd':
		//	jack.all_joints.clear(); // clear all previous joints from previous skeleton
		//	jack.load("dragon.skel");
		//	jack.update();
		//	jack.init_all_joints(jack.root);
		//	if (joint_index >= jack.all_joints.size())  // Make sure index is within range
		//		joint_index = 0;
		//	std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
		//	break;
		//case 't':
		//	jack.all_joints.clear();
		//	jack.load("test.skel");
		//	jack.update();
		//	jack.init_all_joints(jack.root);
		//	if (joint_index >= jack.all_joints.size())
		//		joint_index = 0;
		//	std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
		//	break;
		case 'w':
			jack.all_joints.clear();
			the_skin.reset();
			jack.load("wasp.skel");
			jack.update();
			jack.init_all_joints(jack.root);
			the_skin.load("wasp.skin");
			the_skin.update(jack);
			if (joint_index >= jack.all_joints.size())
				joint_index = 0;
			std::cout << "Joint chosen: " << jack.all_joints[joint_index]->getName() << std::endl;
			break;
		case 'u':
			jack.all_joints.clear();
			the_skin.reset();
			jack.load("tube.skel");
			jack.update();
			jack.init_all_joints(jack.root);
			the_skin.load("tube_smooth.skin");
			the_skin.update(jack);
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
			the_skin.update(jack);
			break;
		case 'x':
			(current_joint->getDofX())->setPose(((current_joint->getDofX())->getPose()) - 0.1); // Subtract 0.01 to the current pose
			jack.update();
			the_skin.update(jack);
			break;
		case 'Y':
			(current_joint->getDofY())->setPose(((current_joint->getDofY())->getPose()) + 0.1); // Add 0.01 to the current pose
			jack.update();
			the_skin.update(jack);
			break;
		case 'y':
			(current_joint->getDofY())->setPose(((current_joint->getDofY())->getPose()) - 0.1); // Subtract 0.01 to the current pose
			jack.update();
			the_skin.update(jack);
			break;
		case 'Z':
			(current_joint->getDofZ())->setPose(((current_joint->getDofZ())->getPose()) + 0.1); // Add 0.01 to the current pose
			jack.update();
			the_skin.update(jack);
			break;
		case 'z':
			(current_joint->getDofZ())->setPose(((current_joint->getDofZ())->getPose()) - 0.1); // Subtract 0.01 to the current pose
			jack.update();
			the_skin.update(jack);
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
