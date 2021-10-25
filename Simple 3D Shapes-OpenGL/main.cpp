#include <windows.h>
#include <random>
#include <gl/glut.h>
#include <cstdlib>

struct GLPoint {
	GLfloat x, y, z;
};

GLuint index = 3;
GLfloat camx = 0, camy = 0, camz = 1, sScale = 1, cScale = 1, angle = 0,
xrot = 0, yrot = 0, zrot = 0, xOrigin = -1, lx = 0, lz = 0, deltaAngle = 0,
sxrot = 0, syrot = 0, szrot = 0,
cxrot = 0, cyrot = 0, czrot = 0;

void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:camy += 0.1; break;
	case GLUT_KEY_DOWN:camy -= 0.1; break;
	case GLUT_KEY_LEFT:camx -= 0.1; break;
	case GLUT_KEY_RIGHT:camx += 0.1; break;
	}
	glutPostRedisplay();
}

void handleKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'x':
		switch (index) {
		case 1:sxrot--; break;
		case 2:cxrot--; break;
		case 0:xrot--; break;
		}
		break;
	case 'X':
		switch (index) {
		case 1:sxrot++; break;
		case 2:cxrot++; break;
		case 0:xrot++; break;
		}
		break;
	case 'y':
		switch (index) {
		case 1:syrot--; break;
		case 2:cyrot--; break;
		case 0:yrot--; break;
		}
		break;
	case 'Y':
		switch (index) {
		case 1:syrot++; break;
		case 2:cyrot++; break;
		case 0:yrot++; break;
		}
		break;
	case 'z':
		switch (index) {
		case 1:szrot--; break;
		case 2:czrot--; break;
		case 0:zrot--; break;
		}
		break;
	case 'Z':
		switch (index) {
		case 1:szrot++; break;
		case 2:czrot++; break;
		case 0:zrot++; break;
		}
		break;
	case 'S':
		if (index == 1 && sScale < 3)
			sScale++;
		else if (index == 2 && cScale < 3)
			cScale++;
		break;
	case 's':
		if (index == 1 && sScale > 1)
			sScale--;
		else if (index == 2 && cScale > 1)
			cScale--;
		break;
	}
	glLoadIdentity();
	glutPostRedisplay();
}

void onMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == 0) {

		GLfloat window_width = glutGet(GLUT_WINDOW_WIDTH);
		GLfloat window_height = glutGet(GLUT_WINDOW_HEIGHT);

		glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
	}
	if (button == 2) {
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
		}
	}

}

void drawTriangle(GLPoint point1, GLPoint point2, GLPoint point3) {
	glBegin(GL_TRIANGLES);
	glVertex3f(point1.x, point1.y, point1.z);
	glVertex3f(point2.x, point2.y, point2.z);
	glVertex3f(point3.x, point3.y, point3.z);
	glEnd();

	glColor3f(0, 0, 0);
	glPointSize(5.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(point1.x, point1.y, point1.z);
	glVertex3f(point2.x, point2.y, point2.z);
	glVertex3f(point3.x, point3.y, point3.z);
	glEnd();
}

void Face(GLPoint point1, GLPoint point2, GLPoint point3, GLPoint point4) {
	glColor3f(1.0, 0.3, 0.5);
	drawTriangle(point1, point4, point3);

	glColor3f(0.5, 0.3, 1.0);
	drawTriangle(point1, point2, point3);

	/*glColor3f(0.0, 0.9, 0.0);
	drawTriangle(point3, point2, midPoint);

	glColor3f(0.9, 0.85, 0.0);
	drawTriangle(point1, point2, midPoint);*/
}

void cuboid() {
	GLPoint points[8] = {
		{-0.2,0.12,0.2},
		{0.2,0.12,0.2},
		{0.2,-0.12,0.2},
		{-0.2,-0.12,0.2},
		{-0.2,0.12,-0.2},
		{-0.2,-0.12,-0.2},
		{0.2,0.12,-0.2},
		{0.2,-0.12,-0.2}
	};


	Face(points[0], points[1], points[2], points[3]); //Front Face
	Face(points[0], points[4], points[5], points[3]); //Left Face
	Face(points[4], points[6], points[7], points[5]); //Rear face
	Face(points[1], points[6], points[7], points[2]); //Right face
	Face(points[4], points[6], points[1], points[0]); //Top face
	Face(points[5], points[7], points[2], points[3]); //Bottom face
}

void squarePyramid() {
	GLPoint points[5] = {
		{0,0.2,0},
		{-0.2,-0.2,0.2},
		{-0.2,-0.2,-0.2},
		{0.2,-0.2,-0.2},
		{0.2,-0.2,0.2}
	};

	glColor3f(1.0, 0.5, 0.5);
	drawTriangle(points[1], points[2], points[4]);

	glColor3f(0.8, 0.3, 1.0);
	drawTriangle(points[2], points[3], points[4]);

	glColor3f(1.0, 0.3, 0.5);
	drawTriangle(points[0], points[1], points[2]);

	glColor3f(0.5, 0.3, 1.0);
	drawTriangle(points[0], points[2], points[3]);

	glColor3f(1.0, 0.3, 0.5);
	drawTriangle(points[0], points[3], points[4]);

	glColor3f(0.5, 0.3, 1.0);
	drawTriangle(points[0], points[1], points[4]);

}

void display() {
	// Clear your Window
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	gluLookAt(camx, camy, camz, 0, 0, 0, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
	glRotatef(zrot, 0, 0, 1);
	glPushMatrix();
	glScalef(sScale, sScale, sScale);
	glRotatef(sxrot, 1, 0, 0);
	glRotatef(syrot, 0, 1, 0);
	glRotatef(szrot, 0, 0, 1);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	squarePyramid();
	glPopMatrix();
	glPushMatrix();
	glScalef(cScale, cScale, cScale);
	glRotatef(90, 0, 1, 0);
	glRotatef(cxrot, 1, 0, 0);
	glRotatef(cyrot, 0, 1, 0);
	glRotatef(czrot, 0, 0, 1);
	glStencilFunc(GL_ALWAYS, 2, 0xFF);
	cuboid();
	glPopMatrix();
	glColor3f(1, 0, 0);
	glColor3f(1, 1, 1);
	glStencilFunc(GL_ALWAYS, 3, 0xFF);
	glutWireSphere(1.5, 13, 10);

	//gluLookAt(camX * 2, 0.5, camZ, 0, 0, 0, 0, 1, 0);
	glPopMatrix();

	glutSwapBuffers();
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		camx = sin(angle + deltaAngle);
		camz = -cos(angle + deltaAngle);
		glutPostRedisplay();
	}
}


void reshapeFunc(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);
	glLoadIdentity();//Use the whole window for rendering
}

int main(int argc, char* argv[])
{
	// Step1:  initialize GLUT using the command line parameters
	glutInit(&argc, argv);


	glutInitWindowSize(1024, 720);
	glutInitWindowPosition(250, 50);
	glClearStencil(0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GL_STENCIL_BUFFER_BIT);
	glColor3f(0.5, 0.5, 0.5);
	glClearColor(0, 0, 0, 1);
	glutCreateWindow("Hello World! Display a Point");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);



	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeys);
	glutReshapeFunc(reshapeFunc);
	glutMouseFunc(onMouse);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(specialKeys);


	//Setup projection window
	glMatrixMode(GL_PROJECTION); // Select Projection Matrix
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPointSize(1.0);

	glutMainLoop();
}
