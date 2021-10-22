#include <random>
#include <gl/glut.h>
#include <cstdlib>

struct GLPoint {
	GLfloat x, y;
};


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	
	glMatrixMode(GL_MODELVIEW);

	
	glLoadIdentity();

	
	glFlush();
}


int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);

	glutInitWindowSize(640, 640);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB);

	glutCreateWindow("3D shapes");

	glClearColor(1, 1, 1, 0);

	glutDisplayFunc(display);

	//define a timer callback function
	//glutIdleFunc(timerCallBack);

	//Setup projection window
	glMatrixMode(GL_PROJECTION); // Select Projection Matrix

	glLoadIdentity();

	//ortho graphic view for 3D
	glOrtho(-2, 2, -2, 2, -2, 4);

	//defines size of the point
	glPointSize(3.0);

	// Step5: Telling the GLUT to loop into the callback
	glutMainLoop();
}