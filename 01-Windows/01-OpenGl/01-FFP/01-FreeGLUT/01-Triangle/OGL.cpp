//header files
#include<GL/freeglut.h>

//global variables
bool bFullScreen = false;

//Entry-Point Function
int main(int argc,char *argv[])
{
	//Local Function Declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	//code
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Sahil A. Buwa");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();
	return 0;

}


void initialize(void)
{
	//code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}


void resize(int width, int height)
{
	//code
	glViewport(0, 0, width, height);

}


void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	
	glEnd();
	glutSwapBuffers();

}


void keyboard(unsigned char key,int x,int y )
{
	//code
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bFullScreen == false)
		{
			glutFullScreen();
			bFullScreen = true;

		}
		else
		{
			glutLeaveFullScreen();
			bFullScreen = false;

		}
		break;
	default:
		break;

	}

}

void mouse(int button, int state, int x, int y)
{
	//code
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

}

void uninitialize(void)
{
	//code
}
