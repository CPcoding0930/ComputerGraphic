/*
*CS4182 Computer Graphics Course Project - Basic Requirements
*Name: PANG Ching Christie ; SID: 55700892
*For the details of this program, please refer to the Basic-readme.txt
*As asked, this program I did not use the room, instead it contains 4 of the basic requirements
*/

#pragma comment(lib, "glaux.lib")
#include "GL\glaux.h"
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>

//Initialise the needed parameters and variables
GLuint list;
float spinx = 0;
float spiny = 0;
float spinz = 0;
GLfloat tdist = 0.0;
GLfloat polyfactor = 1.0;
GLfloat polyunits = 1.0;
float delang = 5.0;
int xAbs, yAbs;
float translate_z = -30.0;

//Setup  the window and the menu variables
static int window;
static int menu_id;
static int submenu_id;
static int value = 0;

float yRot = 0;
unsigned int nPre = 0;

//Setup all needed light
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_direction[] = { -1, -2, -1, 1 };
GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

//Setup all needed colors and light for the demonstration of lightings
GLfloat light_change[3] = { 3,3,-1 };
GLfloat light_param[] = { 1, 1, 1, 1 };
GLfloat gray[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat cyan[] = { 0.0, 1.0, 1.0, 0.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };

//Reset Position
void reset()
{
	translate_z = -30.0;
}

//Setup Menu
void menu(int num) {
	if (num == 0) { // when the user selects 'quit' (0)
		glutDestroyWindow(window);
		exit(0);
	}
	else {
		value = num;
	}
	glutPostRedisplay();
}

//Details of the Menu
/*
* The Menu includes: Clear, Light and Quit. Ambient, Diffuse, Specular, Position and Moving Light are all submenu in Light
*/
void createMenu(void) {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Ambient", 2);
	glutAddMenuEntry("Diffuse", 3);
	glutAddMenuEntry("Specular", 4);
	glutAddMenuEntry("Position", 5);
	glutAddMenuEntry("MovingLight", 6);
	glutAddMenuEntry("KeyboardMovingLight", 7);
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Clear", 1);
	glutAddSubMenu("Lights", submenu_id);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//Display Functions for lighting and the transformation
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0, 0.0, tdist);
	glRotatef((GLfloat)spinx, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)spiny, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)spinz, 0.0, 0.0, 1.0);

	glMaterialfv(GL_FRONT, GL_LIGHT_MODEL_AMBIENT, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gray);
	glMaterialfv(GL_FRONT, GL_POSITION, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 70);
	//	glMaterialfv(GL_FRONT, GL_SPECULAR, light_param);

	glEnable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);

	if (value == 2) { //Ambient
		glEnable(GL_LIGHT0);
		glPolygonOffset(polyfactor, polyunits);
		glCallList(list);
		glDisable(GL_LIGHT0);
	}
	else if (value == 3) { //Diffuse
		glEnable(GL_LIGHT1);
		glPolygonOffset(polyfactor, polyunits);
		glCallList(list);
		glDisable(GL_LIGHT1);

	}
	else if (value == 4) { //Specular
		glEnable(GL_LIGHT2);
		glPolygonOffset(polyfactor, polyunits);
		glCallList(list);
		glDisable(GL_LIGHT2);
	}
	else if (value == 5) { //Position
		glEnable(GL_LIGHT3);
		glPolygonOffset(polyfactor, polyunits);
		glCallList(list);
		glDisable(GL_LIGHT3);
	}
	else if (value == 6) { //Moving
		glColor3f(1.0, 1.0, 1.0);
		glLoadIdentity();
		glTranslated(0, -1, -5);    //Translate (0,0,-5), specifically, to -z translate 5
		glPushMatrix();        //Setthe moving light
		glDisable(GL_LIGHTING);
		glRotatef(yRot, 0, 1, 0);    //rotation and location of moving light
		glTranslatef(light_change[0], light_change[1], light_change[2]);
		glTranslatef(light_position[0], light_position[1], light_position[2]);

		glutSolidSphere(0.1, 4, 4);//A light source that radius as 1
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT4);
		glLightfv(GL_LIGHT4, GL_POSITION, light_position);
		glPopMatrix();

		light_direction[0] = -light_change[0];
		light_direction[1] = -light_change[1];
		light_direction[2] = -light_change[2];
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_direction);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
		glPushMatrix();
		glutSolidSphere(1, 30, 12);
		glPopMatrix();
		glDisable(GL_LIGHT4);
	}
	else if (value == 7) { //MovingKeyboard
		glColor3f(1.0, 1.0, 1.0);
		glLoadIdentity();
		glTranslated(0, -1, -5);    //Translate (0,0,-5), specifically, to -z translate 5
		glPushMatrix();        //Setthe moving light
		glDisable(GL_LIGHTING);
		glRotatef(translate_z, 0, 1, 0);    //rotation and location of moving light
		glTranslatef(light_change[0], light_change[1], light_change[2]);
		glTranslatef(light_position[0], light_position[1], light_position[2]);

		glutSolidSphere(0.1, 4, 4);//A light source that radius as 1
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT4);
		glLightfv(GL_LIGHT4, GL_POSITION, light_position);
		glPopMatrix();

		light_direction[0] = -light_change[0];
		light_direction[1] = -light_change[1];
		light_direction[2] = -light_change[2];
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_direction);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
		glPushMatrix();
		glutSolidSphere(1, 30, 12);
		glPopMatrix();
		glDisable(GL_LIGHT4);
	}

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(list);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
	glFlush();


}

//Initialise the light
void gfxinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0); //background Color

	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	glutSolidSphere(1.0, 30, 12);
	glEndList();

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	//Setup light models and lights settings
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 2.0);
}

// Reshape Function
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//Keyboard detection to move
void keyboard(unsigned char key, int a, int  b)
{
	if (key == 'd')
	{
		translate_z--;
		glutPostRedisplay();
	}
	else if (key == 'a')
	{
		translate_z++;
		glutPostRedisplay();
	}
}

//Dealing with the rotation
void CheckRot()
{
	if (spinx > 360.0f) {
		spinx = 0.0f;
	}
	if (spinx < 0.0f) {
		spinx = 360.0 - delang;
	}
	if (spiny > 360.0f) {
		spiny = 0.0f;
	}
	if (spiny < 0.0f) {
		spiny = 360.0f - delang;
	}
	if (spinz > 360.0f) {
		spinz = 0.0f;
	}
	if (spinz < 0.0f) {
		spinz = 360.0f - delang;
	}
}

// Dealing with the moving of mouse
void MouseMove(int x, int y)
{
	spinx += (y - yAbs);
	yAbs = y;
	spiny -= (x - xAbs);
	xAbs = x;
	CheckRot();
	glutPostRedisplay();
}

// Dealing with the mouse press, when the mouse is pressed, allowing the move
void MousePress(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		xAbs = x;
		yAbs = y;
	}
}

//When it is idled....
void MyIdle(void)
{
	unsigned int nNow = ::GetTickCount();
	float delt = nNow - nPre;
	if (delt >  100) {
		nPre = nNow;
		yRot += 0.1 * delt;
		glutPostRedisplay();
	}
}

//Main Function to group all the functions together
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //Setup initial settings of model (display)
	glutInitWindowSize(800, 800);// Window size
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	createMenu();
	gfxinit();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutReshapeFunc(reshape); // Reshape

	glutMotionFunc(MouseMove); // Mouse Move Action
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);// Display

	glutIdleFunc(MyIdle); //When Idle
	glutMainLoop();
	return EXIT_SUCCESS;
}