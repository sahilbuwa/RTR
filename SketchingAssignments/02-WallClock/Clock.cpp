// Header Files
#include "GL\freeglut.h"
#include <math.h>

// variable declarations
bool bFullScreen = false;

// entry-point function
int main(int argc, char* argv[]) {

	// local function declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	// code
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sahil Buwa");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return 0;
}

void initialize(void) {

	// code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height) {

	// code
	glViewport(0, 0, width, height);
}

void display(void) {

	// code
	glClear(GL_COLOR_BUFFER_BIT);

    // Background
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
    glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f,1.0f,0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f,1.0f,0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
    glEnd();

    // Outer Boundary
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) 
    {
		// Find the angle
		float angle_theta = i * 3.142 / 180;
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.5f * cos(angle_theta),
				   0.5f * sin(angle_theta), 0.0f);
	}
    glEnd();

    // Aatli Boundary
    glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) 
    {
		// Find the angle
		float angle_theta = i * 3.142 / 180;
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.47f * cos(angle_theta),
				   0.47f * sin(angle_theta), 0.0f);
	}
    glEnd();

	for (int i = 0; i < 360; i+=30) 
    {
		// Find the angle
		float angle_theta_big = i * 3.142 / 180;
        if(i%90==0)
        {
            glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.43f * cos(angle_theta_big)+0.02f,0.43f * sin(angle_theta_big)-0.02f,0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.43f * cos(angle_theta_big)+0.02f,0.43f * sin(angle_theta_big)+0.02f,0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.43f * cos(angle_theta_big)-0.02f,0.43f * sin(angle_theta_big)+0.02f,0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.43f * cos(angle_theta_big)-0.02f,0.43f * sin(angle_theta_big)-0.02f,0.0f);
            glEnd();
        }
        else
        {
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i+=30) 
            {
                //Taasanche thipke
                // Find the angle
                float angle_theta_small = i * 3.142 / 180;
                glColor3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.43f * cos(angle_theta_big)+0.015f * cos(angle_theta_small),
                        0.43f * sin(angle_theta_big)+0.015f * sin(angle_theta_small), 0.0f);
            }
            glEnd();
        }
	}
    // Centre bindu
    glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) 
    {
		// Find the angle
		float angle_theta = i * 3.142 / 180;
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.03f * cos(angle_theta),
				   0.03f * sin(angle_theta), 0.0f);
	}
    glEnd();
    // Hour Hand
    float hour_angle=205 * 3.142 / 180;
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f,0.025f,0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.4f * cos(hour_angle),0.4f * sin(hour_angle),0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f,-0.025f,0.0f);
    glEnd();

    //Minute Hand
    float minute_angle=30 * 3.142 / 180;
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.03f,0.0f,0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.4f * cos(minute_angle),0.4f * sin(minute_angle),0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.03f,0.0f,0.0f);
    glEnd();


    //Second Hand
    
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f,0.36f,0.0f);
    glEnd();

	
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {

	// code
	switch (key) {
	case 27:	// escape ASCII value
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bFullScreen == false) {
			glutFullScreen();
			bFullScreen = true;
		} else {
			glutLeaveFullScreen();
			bFullScreen = false;
		}
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y) {

	// code
	switch (button) {
	case GLUT_LEFT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void uninitialize(void) {

	// code

}
 