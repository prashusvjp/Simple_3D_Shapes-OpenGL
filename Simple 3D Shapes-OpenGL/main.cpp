#include <windows.h>
#include <gl/glut.h>
#include <cstdlib>

struct GLPoint {
	GLfloat x, y, z;
};

GLuint index = 0,cxs=0,cys=0,czs=1,sxs=1,sys=0,szs=0,cstate=1,sstate=1;
GLfloat window_width = 1024;
GLfloat window_height = 720;
GLfloat camx = 0, camy = 0, camz = 1, sScale = 1, cScale = 1, angle = 0, bscale = 1.0,
xrot = 0, yrot = 0, zrot = 0, xOrigin = -1, lx = 0, lz = 0, deltaAngle = 0,
sxrot = 0, syrot = 0, szrot = 0, sxt=0.3, syt=0.4, szt=0.3, sxrots = 1, szrots = 1,
cxrot = 0, cyrot = 0, czrot = 0,ctheta=0,stheta=0,cxrots=1,cyrots=1,cxt = 0.5, cyt = -0.4, czt = 0;

void specialKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:camy += 0.1; break;
		case GLUT_KEY_DOWN:camy -= 0.1; break;
		case GLUT_KEY_LEFT:camx -= 0.1; break;
		case GLUT_KEY_RIGHT:camx += 0.1; break;
	}
	glutPostRedisplay();
}

void incOrDecValues(int index,int op,GLfloat *p1, GLfloat *p2,GLfloat *p3) {
	switch (index) {
		case 0:*p1+=op; break;
		case 1:*p2+=op; break;
		case 2:*p3+=op; break;
	}
}


void handleKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'x':
		incOrDecValues(index, -1, &xrot, &sxrot, &cxrot);
		break;
	case 'X':
		incOrDecValues(index,1,&xrot, &sxrot, &cxrot);
		break;
	case 'y':
		incOrDecValues(index, -1, &yrot, &syrot, &cyrot);
		break;
	case 'Y':
		incOrDecValues(index,1, &yrot, &syrot, &cyrot);
	case 'z':
		incOrDecValues(index, -1, &zrot, &szrot, &czrot);
		break;
	case 'Z':
		incOrDecValues(index, 1, &zrot, &szrot, &czrot);
	case 'S':
		if (index == 1 && sScale < 3)
			sScale += 0.5;
		else if (index == 2 && cScale < 3)
			cScale += 0.5;
		else if(index == 0 && bscale < 2)
			bscale += 0.25;
		break;
	case 's':
		if (index == 1 && sScale > 1)
			sScale-=0.5;
		else if (index == 2 && cScale > 1)
			cScale-=0.5;
		else if (index == 0 && bscale > 1)
			bscale -= 0.25;
		break;
	}
	glutPostRedisplay();
}

void onMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
		if (index == 1) 
			sstate = !sstate;

		if (index == 2) 
			cstate = !cstate;
	}
	if (button == 2) {
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else {
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

void cuboidFace(GLPoint point1, GLPoint point2, GLPoint point3, GLPoint point4) {
	glColor3f(1.0, 0.3, 0.3);
	drawTriangle(point1, point4, point3);

	glColor3f(0.9, 0.2, 1.0);
	drawTriangle(point1, point2, point3);
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


	cuboidFace(points[0], points[1], points[2], points[3]); //Front Face
	cuboidFace(points[0], points[4], points[5], points[3]); //Left Face
	cuboidFace(points[4], points[6], points[7], points[5]); //Rear face
	cuboidFace(points[1], points[6], points[7], points[2]); //Right face
	cuboidFace(points[4], points[6], points[1], points[0]); //Top face
	cuboidFace(points[5], points[7], points[2], points[3]); //Bottom face
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

GLfloat getDirections(GLfloat p,int index, int axis) {
	switch (index) {
	case 1:
		if (axis == 1)
			if (sxs)
				return p + 0.001;
			else
				return p - 0.001;
		else if(axis == 2)
			if (sys)
				return p + 0.001;
			else
				return p - 0.001;
		else
			if (szs)
				return p + 0.001;
			else
				return p - 0.001;
		break;
	case 2:
		if (axis == 1)
			if (cxs)
				return p + 0.001;
			else
				return p - 0.001;
		else if (axis == 2)
			if (cys)
				return p + 0.001;
			else
				return p - 0.001;
		else
			if (czs)
				return p + 0.0001;
			else
				return p - 0.0001;
		break;
	}
}

GLfloat getOppositeDirections(GLfloat p,int index,int axis) {
	switch (index) {
	case 1:
		if (axis == 1)
			sxs = !sxs;
		else if (axis == 2)
			sys = !sys;
		else
			szs = !szs;
		break;
	case 2:
		if (axis == 1)
			cxs = !cxs;
		else if (axis == 2)
			cys = !cys;
		else
			czs = !czs;
		break;
	}
	if (p <= 0)
		return p + 0.001;
	else
		return p - 0.001;
}

void mouseMove(int x, int y) {
	if (xOrigin >= 0) {
		deltaAngle = (x - xOrigin) * 0.001f;
		camx = sin(angle + deltaAngle);
		camz = -cos(angle + deltaAngle);
		glutPostRedisplay();
	}
}

GLint isInsideSphere(int x, int y, int z) {
	GLint x1 = pow((x - 0), 2);
	GLint y1 = pow((y - 0), 2);
	GLint z1 = pow((z - 0), 2);
	return x1 + y1 + z1;
}

void computeForCuboid() {
	if (cstate) {
		ctheta += 0.05;
		if (ctheta > 360)
			ctheta = 0;
		if (isInsideSphere(cxt,cyt,czt) < bscale*bscale) {
			cxt = getDirections(cxt, 2, 1);
			cyt = getDirections(cxt, 2, 2);
			czt = getDirections(czt, 2, 3);
		}
		else {
			cxt = getOppositeDirections(cxt, 2, 1);
			cyt = getOppositeDirections(cyt, 2, 2);
			czt = getOppositeDirections(czt, 2, 3);
		}
	}
}

void computeForSquarePyramid() {
	if (sstate) {
		stheta += 0.05;
		if (stheta > 360)
			stheta = 0;
		if (isInsideSphere(sxt,syt,szt) < bscale*bscale) {
			sxt = getDirections(sxt, 1, 1);
			syt = getDirections(syt, 1, 2);
			szt = getDirections(szt, 1, 3);
		}
		else {
			sxt = getOppositeDirections(sxt, 1, 1);
			syt = getOppositeDirections(syt, 1, 2);
			szt = getOppositeDirections(szt, 1, 3);
		}
	}
}

void idleCallback() {
	computeForCuboid();
	computeForSquarePyramid();
	glutPostRedisplay();
}

void reshapeFunc(int x, int y)
{
	if (y == 0 || x == 0) return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);
	glLoadIdentity();
}

void display() {
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glPushMatrix();
		glTranslatef(0.0, 0.0, -5.0);
		gluLookAt(camx, camy, camz, 0, 0, 0, 0, 1, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(xrot, 1, 0, 0);
		glRotatef(yrot, 0, 1, 0);
		glRotatef(zrot, 0, 0, 1);
		glScalef(bscale, bscale, bscale);
		glColor3f(1, 1, 1);
		glStencilFunc(GL_ALWAYS, 3, 0xFF);
		glutWireSphere(1.5, 13, 10);
		glPushMatrix();
			glTranslatef(sxt, syt, szt);
			glScalef(sScale, sScale, sScale);
			glRotatef(stheta, sxrots, 0, szrots);
			glRotatef(sxrot, 1, 0, 0);
			glRotatef(syrot, 0, 1, 0);
			glRotatef(szrot, 0, 0, 1);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			squarePyramid();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(cxt, cyt, czt);
			glScalef(cScale, cScale, cScale);
			glRotatef(ctheta, cxrots, cyrots, 0);
			glRotatef(cxrot, 1, 0, 0);
			glRotatef(cyrot, 0, 1, 0);
			glRotatef(czrot, 0, 0, 1);
			glStencilFunc(GL_ALWAYS, 2, 0xFF);
			cuboid();
		glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();

}

int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 720);
	glutInitWindowPosition(250, 50);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GL_STENCIL_BUFFER_BIT);
	glColor3f(0.5, 0.5, 0.5);
	glClearColor(0, 0, 0, 1);
	glutCreateWindow("Simple 3D shapes using OpenGL");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);



	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeys);
	glutReshapeFunc(reshapeFunc);
	glutMouseFunc(onMouse);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(idleCallback);

	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
}
