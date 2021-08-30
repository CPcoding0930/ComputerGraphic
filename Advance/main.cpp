/*
*CS4182 Computer Graphics Course Project - Advanced Requirements
*Name: PANG Ching Christie ; SID: 55700892
*For the details of this program, please refer to the Advanced-readme.txt
*This is an educational application to let learners (teens) to get in touch with the basic operation of computer graphics.
*/
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <string>
#include <ctime>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glaux.h>
#include <GL/glut.h>

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

// all variables initialized to 1.0 = the triangle will initially be white
float red = 1.0f, blue = 1.0f, green = 1.0f;

// angle for rotating triangle
float angle = 0.0f;

void drawSnowMan()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
}

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy)
{
	float fraction = 0.1f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}
}

struct float2
{
    float2( float _x = 0.0f, float _y = 0.0f ) : x(_x), y(_y) {}

    float x;
    float y;
};

struct float3
{
    float3( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) : x(_x), y(_y), z(_z) {}
    
    float x;
    float y;
    float z;
};

int g_iWindowWidth = 512;
int g_iWindowHeight = 512;
int g_iGLUTWindowHandle = 0;
int g_iErrorCode = 0;
GLuint list;
float spinx = 0;
float spiny = 0;
float spinz = 0;
GLfloat tdist = 0.0;
GLfloat polyfactor = 1.0;
GLfloat polyunits = 1.0;
float delang = 5.0;
int xAbs, yAbs;


void InitGL( int argc, char* argv[] );
void DisplayGL();
void IdleGL();
void KeyboardGL( unsigned char c, int x, int y );
void MouseGL( int button, int state, int x, int y );
void MouseMove( int x, int y );
void ReshapeGL( int w, int h );
void gfxinit();

// 2D Primitives
void DrawRectangle( float width, float height );
void DrawCircle( float radius, int numSides = 8 );
void DrawTriangle( float2 p1, float2 p2, float2 p3 );

// 3D Primitives
void DrawCube( float width, float height, float depth );
void DrawSphere( float radius );
void DrawPyramid( float scale = 1.0f );

enum ESceneType
{
    ST_Scene1 = 0,
    ST_Scene2,
    ST_Scene3,
    ST_Scene4,
	ST_Scene5,
    ST_NumScenes
};
ESceneType g_eCurrentScene = ST_Scene1;

// Rotation parameters
float g_fRotate1 = 0.0f;
float g_fRotate2 = 0.0f;
float g_fRotate3 = 0.0f;

std::clock_t g_PreviousTicks;
std::clock_t g_CurrentTicks;

// Render a simple scene with 2D primitives
void RenderScene1();
// Render a slightly more complex scene using different colors
void RenderScene2(); 
// Render a scene with animated transformations
void RenderScene3();
// Render a scene with 3D objects that perform rotations on all 3 axis.
void RenderScene4();
// Render a scene with 3D objects that perform keyboard animation.
void RenderScene5();

// We're exiting, cleanup the allocated resources.
void Cleanup( int exitCode, bool bExit = true );

int main( int argc, char* argv[] )
{
    // Capture the previous time to calculate the delta time on the next frame
    g_PreviousTicks = std::clock();

    InitGL( argc, argv );
    glutMainLoop();

    Cleanup(g_iErrorCode);
}

void Cleanup( int errorCode, bool bExit )
{
    if ( g_iGLUTWindowHandle != 0 )
    {
        glutDestroyWindow( g_iGLUTWindowHandle );
        g_iGLUTWindowHandle = 0;
    }

    if ( bExit )
    {
        exit( errorCode );
    }
}


void InitGL( int argc, char* argv[] )
{
    std::cout << "Initialise OpenGL..." << std::endl;

    glutInit(&argc, argv);
    int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitDisplayMode( GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH );

    glutInitWindowPosition( ( iScreenWidth - g_iWindowWidth ) / 2,
                            ( iScreenHeight - g_iWindowHeight ) / 2 );
    glutInitWindowSize( g_iWindowWidth, g_iWindowHeight );

    g_iGLUTWindowHandle = glutCreateWindow( "OpenGL-Advanced educational game" );

	std::cout << "Initialise OpenGL..." << std::endl;
	std::cout << "Welcome! I will be with you to guide your tour here. You  can do the operations:" << std::endl;
	std::cout << "1. Choose to click from 1-4 first, to get familiar with the changes of 2D and 3D world with animated transformation" << std::endl;
	std::cout << "2. Explore Key s/S, f/F and r/R " << std::endl;
	std::cout << "3. Click 5 for the question and quiz section." << std::endl;
	std::cout << "4. Click 6 for the five scene. Move the camera using arrow keys" << std::endl;
	std::cout << "5. Quit by q/Q, ESC, Enter" << std::endl;
    // Register GLUT callbacks
    glutDisplayFunc(DisplayGL);
    glutIdleFunc(IdleGL);
    glutMouseFunc(MouseGL);
    glutMotionFunc(MouseMove);
    glutKeyboardFunc(KeyboardGL);
    glutReshapeFunc(ReshapeGL);

    // Setup initial GL State
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClearDepth( 1.0f );

    glShadeModel( GL_SMOOTH );

    std::cout << "Initialise OpenGL: Success!" << std::endl;
}

void DisplayGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );                   // Clear the color buffer, and the depth buffer.

    switch ( g_eCurrentScene )
    {
    case ST_Scene1:
        {
            RenderScene1();
        }
        break;
    case ST_Scene2:
        {
            RenderScene2();
        }
        break;
    case ST_Scene3:
        {
            RenderScene3();
        }
        break;
    case ST_Scene4:
        {
            RenderScene4();
        }
        break;
	case ST_Scene5:
		{
			glutDisplayFunc(RenderScene5);
			glutReshapeFunc(changeSize);
			glutIdleFunc(RenderScene5);
			glutKeyboardFunc(processNormalKeys);
			glutSpecialFunc(processSpecialKeys);

			// OpenGL init
			glEnable(GL_DEPTH_TEST);

			// enter GLUT event processing cycle
			glutMainLoop();
		}
	break;
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void IdleGL()
{
    // Update our simulation
    g_CurrentTicks = std::clock();
    float deltaTicks = ( g_CurrentTicks - g_PreviousTicks );
    g_PreviousTicks = g_CurrentTicks;

    float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

    // Rate of rotation in (degrees) per second
    const float fRotationRate = 50.0f;

    // Update our rotation parameters
    g_fRotate1 += ( fRotationRate * fDeltaTime );
    g_fRotate1 = fmodf( g_fRotate1, 360.0f );

    g_fRotate2 += ( fRotationRate * fDeltaTime );
    g_fRotate2 = fmodf( g_fRotate2, 360.0f );

    g_fRotate3 += ( fRotationRate * fDeltaTime );
    g_fRotate3 = fmodf( g_fRotate3, 360.0f );

    glutPostRedisplay();
}

void KeyboardGL( unsigned char c, int x, int y )
{
    // Store the current scene so we can test if it has changed later.
    ESceneType currentScene = g_eCurrentScene;

    switch ( c )
    {
    case '1':
        {
            glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );                         // White background
            g_eCurrentScene = ST_Scene1;
        }
        break;
    case '2':
        {
            glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );                         // Black background
            g_eCurrentScene = ST_Scene2;
			
        }
        break;
    case '3':
        {
            glClearColor( 0.27f, 0.27f, 0.27f, 1.0f );                      // Dark-Gray background
            g_eCurrentScene = ST_Scene3;
        }
        break;
    case '4':
        {
            glClearColor( 0.73f, 0.73f, 0.73f, 1.0f );                      // glEnable(GL_LIGHTING);-Gray background
            g_eCurrentScene = ST_Scene4;
        }
        break;
	case '5':
	{
		std::string input1, input2, input3;
		int score = 0;
		std::cout << "Question 1: What is R stand for?" << std::endl;
		std::getline(std::cin, input1);
		if (input1 == "reset parameter") {
			std::cout << "Correct! Keep going!" << std::endl;
			score += 1;
		}
		else {
			std::cout << "Sorry, wrong answer... correct answer is reset parameter" << std::endl;
		}
		std::cout << "Question 2: What is F stand for?" << std::endl;
		std::getline(std::cin, input2);
		if (input2 == "flat shading") {
			std::cout << "Correct! You are smart!" << std::endl;
			score += 1;
		}
		else {
			std::cout << "Sorry, wrong answer...correct answer is flat shading" << std::endl;
		}
		std::cout << "Question 3: What is S stand for?" << std::endl;
		std::getline(std::cin, input3);
		if (input3 == "smooth shading") {
			std::cout << "Congratualation!" << std::endl;
			score += 1;
		}
		else {
			std::cout << "Sorry, wrong answer...correct answer is smooth shading" << std::endl;
		}
		std::cout << "Ask yourself: Do you feel interested? Do you want to explore more about OpenGL?" << std::endl;
		std::cout << "Your score is " << score << "/3" << std::endl;
		std::cout << "Now, explore interaction movement by clicking 6 to scene 5 and use arrow to explore the world of 3D computer graphics! Enjoy!" << std::endl;
	}
	break;
	case '6':
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// Black background
		g_eCurrentScene = ST_Scene5;
	}
	break;
	case 'l': //Lighting
	case 'L':
	{
		std::cout << "Lights On" << std::endl;
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	break;
	case 'd': //Lighting
	case 'D':
	{
		std::cout << "Lights  Off" << std::endl;
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	break;
	case 's':
    case 'S':
        {
            std::cout << "Shade Model: GL_SMOOTH" << std::endl;
            // Switch to smooth shading model
            glShadeModel( GL_SMOOTH );
        }
        break;
    case 'f':
    case 'F':
        {
            std::cout << "Shade Model: GL_FLAT" << std::endl;
            // Switch to flat shading model
            glShadeModel( GL_FLAT );
        }
        break;
    case 'r':
    case 'R':
        {
            std::cout << "Reset Parameters..." << std::endl;
            g_fRotate1 = g_fRotate2 = g_fRotate3 = 0.0f;
        }
        break;
    case '\033': // ESC  quits
    case '\015': // Enter quits    
    case 'Q':    // Q quits
    case 'q':    // q (or escape) quits
        {
            // Cleanup up and quit
            Cleanup(0);
        }
        break;
    }

    if ( currentScene != g_eCurrentScene )
    {
        std::cout << "Changed Render Scene: " << ( g_eCurrentScene + 1 ) << std::endl; 
    }

    glutPostRedisplay();
}

void MouseGL( int button, int state, int x, int y ){}

void CheckRot()
{
	if (spinx > 360.0f)
		spinx = 0.0f;
	if (spinx < 0.0f)
		spinx = 360.0 - delang;

	if (spiny > 360.0f)
		spiny = 0.0f;
	if (spiny < 0.0f)
		spiny = 360.0f - delang;

	if (spinz > 360.0f)
		spinz = 0.0f;
	if (spinz < 0.0f)
		spinz = 360.0f - delang;
}

void MouseMove( int x, int y )
{
	spinx += (y - yAbs);
	yAbs = y;
	spiny -= (x - xAbs);
	xAbs = x;
	CheckRot();
	glutPostRedisplay();
}

void ReshapeGL( int w, int h )
{
    std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

    if ( h == 0)										// Prevent A Divide By Zero error
    {
        h = 1;										// Making Height Equal One
    }

    g_iWindowWidth = w;
    g_iWindowHeight = h;

    glViewport( 0, 0, g_iWindowWidth, g_iWindowHeight );

    // Setup the projection matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, (GLdouble)g_iWindowWidth/(GLdouble)g_iWindowHeight, 0.1, 100.0 );

    glutPostRedisplay();
}

// Draw 2D Prmitives
void DrawTriangle( float2 p1, float2 p2, float2 p3 )
{
    glBegin( GL_TRIANGLES );
        glVertex2f( p1.x, p1.y );
        glVertex2f( p2.x, p2.y );
        glVertex2f( p3.x, p3.y );
    glEnd();
}

void DrawRectangle( float width, float height )
{
    const float w = width / 2.0f;
    const float h = height / 2.0f;

    glBegin( GL_QUADS );
        glVertex2f( -w,  h );   // Top-Left
        glVertex2f(  w,  h );   // Top-Right
        glVertex2f(  w, -h );   // Bottom-Right
        glVertex2f( -w, -h );   // Bottom-Left
    glEnd();
        
}

void DrawCircle(  float radius, int numSides) /* = 8 */
{
    const float step = M_PI / numSides;
    glBegin( GL_TRIANGLE_FAN );
        glVertex2f(0.0f, 0.0f);
        for ( float angle = 0.0f; angle < ( 2.0f * M_PI ); angle += step )
        {
            glVertex2f( radius * sinf(angle), radius * cosf(angle) );
        }
        glVertex2f( 0.0f, radius ); // One more vertex to close the circle
    glEnd();
}

void gfxinit(void)
{
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glClearColor(0.0, 0.0, 0.0, 1.0);

	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	glutSolidSphere(1.0, 20, 12);
	glEndList();

	glEnable(GL_DEPTH_TEST);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 70);
}


void RenderScene1()
{
    glMatrixMode( GL_MODELVIEW );                                           // Switch to modelview matrix mode
    glLoadIdentity();                                                       // Load the identity matrix

    glTranslatef( -1.5f, 1.0f, -6.0f );                                     // Translate our view matrix back and a bit to the left.
    glColor3f( 1.0f, 0.0f, 0.0f );                                          // Set Color to red
    DrawTriangle( float2(0.0f, 1.0f), float2(-1.0f, -1.0f), float2(1.0f, -1.0f ) );

    glTranslatef( 3.0f, 0.0f, 0.0f );                                       // Shift view 3 units to the right
    glColor3f( 0.0f, 0.0f, 1.0f );                                          // Set Color to blue
    DrawRectangle( 2.0f, 2.0f );

    glTranslatef( -1.5f, -3.0f, 0.0f );                                     // Back to center and lower screen
    glColor3f( 1.0f, 1.0f, 0.0f );                                          // Set color to yellow
    DrawCircle( 1.0f, 16 );
}

void RenderScene2()
{
    glMatrixMode( GL_MODELVIEW );                                           // Switch to modelview matrix mode
    glLoadIdentity();                                                       // Load the identity matrix

    glTranslatef( -1.5f, 1.0f, -6.0f );                                     // Translate back and to the left
    // Draw a triangle with different colors on each vertex
    glBegin( GL_TRIANGLES );
        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex2f( 0.0f, 1.0f );                                           // Top-Center

        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex2f( -1.0f, -1.0f );                                         // Bottom-Left

        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex2f( 1.0f, -1.0f );                                          // Bottom-Right
    glEnd();

    glTranslatef( 3.0f, 0.0f, 0.0f );                                       // Translate right
    // Draw a rectangle with different colors on each vertex
    glBegin( GL_QUADS );
        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex2f( -1.0f, 1.0f );                                          // Top-Left

        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex2f( 1.0f, 1.0f );                                           // Top-Right

        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex2f( 1.0f, -1.0f );                                          // Bottom-Right

        glColor3f( 1.0f, 1.0f, 1.0f );                                      // White
        glVertex2f( -1.0f, -1.0f );                                         // Bottom-Left
    glEnd();

    glTranslatef( -1.5f, -3.0f, 0.0f );                                     // Back to center and lower screen

    // Draw a circle with blended red/blue vertices.
    const float step = M_PI / 16;
    const float radius = 1.0f;

    glBegin( GL_TRIANGLE_FAN );
        glColor3f( 1.0f, 1.0f, 1.0f );    
        glVertex2f(0.0f, 0.0f);
        for ( float angle = 0.0f; angle < ( 2.0f * M_PI ); angle += step )
        {
            float fSin = sinf(angle);
            float fCos = cosf(angle);

            glColor3f( ( fCos + 1.0f ) * 0.5f, ( fSin + 1.0f ) * 0.5f , 0.0f);
            glVertex2f( radius * fSin, radius * fCos );
        }
        glColor3f( 1.0f, 0.5f, 0.0f );
        glVertex2f( 0.0f, radius ); // One more vertex to close the circle
    glEnd();
}

void RenderScene3()
{
    glMatrixMode( GL_MODELVIEW );                                           // Switch to modelview matrix mode
    glLoadIdentity();                                                       // Load the identity matrix

    glTranslatef( -1.5f, 1.0f, -6.0f );                                     // Translate back and to the left
    glPushMatrix();                                                         // Push the current transformation onto the matrix stack
    glRotatef( g_fRotate1, 0.0f, 0.0f, 1.0f );
    // Draw a triangle with different colors on each vertex
    glBegin( GL_TRIANGLES );
        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex2f( 0.0f, 1.0f );                                           // Top-Center

        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex2f( -1.0f, -1.0f );                                         // Bottom-Left

        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex2f( 1.0f, -1.0f );                                          // Bottom-Right
    glEnd();
    glPopMatrix();

    glTranslatef( 3.0f, 0.0f, 0.0f );                                       // Translate right
    glPushMatrix();
    glRotatef( g_fRotate2, 0.0f, 0.0f, 1.0f );
    // Draw a rectangle with different colors on each vertex
    glBegin( GL_QUADS );
        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex2f( -1.0f, 1.0f );                                          // Top-Left

        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex2f( 1.0f, 1.0f );                                           // Top-Right

        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex2f( 1.0f, -1.0f );                                          // Bottom-Right

        glColor3f( 1.0f, 1.0f, 1.0f );                                      // White
        glVertex2f( -1.0f, -1.0f );                                         // Bottom-Left
    glEnd();
    glPopMatrix();

    glTranslatef( -1.5f, -3.0f, 0.0f );                                     // Back to center and lower screen
    glPushMatrix();
    glRotatef( g_fRotate3, 0.0f, 0.0f, 1.0f );
    // Draw a circle with blended red/blue vertecies
    const float step = M_PI / 16;
    const float radius = 1.0f;

    glBegin( GL_TRIANGLE_FAN );
        glColor3f( 1.0f, 1.0f, 1.0f );    
        glVertex2f(0.0f, 0.0f);
        for ( float angle = 0.0f; angle < ( 2.0f * M_PI ); angle += step )
        {
            float fSin = sinf(angle);
            float fCos = cosf(angle);

            glColor3f( ( fCos + 1.0f ) * 0.5f, ( fSin + 1.0f ) * 0.5f , 0.0f);
            glVertex2f( radius * fSin, radius * fCos );
        }
        glColor3f( 1.0f, 0.5f, 0.0f );
        glVertex2f( 0.0f, radius ); // One more vertex to close the circle
    glEnd();
    glPopMatrix();
}

void RenderScene4()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glEnable( GL_DEPTH_TEST );

    glTranslatef( -1.5f, 1.0f, -6.0f );                                     // Translate back and to the left

    glPushMatrix();                                                         // Push the current modelview matrix on the matrix stack
    glRotatef(g_fRotate1, 1.0f, 1.0f, 1.0f );                               // Rotate on all 3 axis
    glBegin( GL_TRIANGLES );                                                // Draw a pyramid        
        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f );                                     // Top of front face
        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex3f( -1.0f, -1.0f, 1.0f );                                   // Left of front face
        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex3f( 1.0f, -1.0f, 1.0f );                                    // Right of front face

        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f );                                     // Top of right face
        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex3f( 1.0f, -1.0f, 1.0f );                                    // Left of right face
        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex3f( 1.0f, -1.0f, -1.0f );                                   // Right of right face

        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f );                                     // Top of back face
        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex3f( 1.0f, -1.0f, -1.0f );                                   // Left of back face
        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex3f( -1.0f, -1.0f, -1.0f );                                  // Right of back face

        glColor3f( 1.0f, 0.0f, 0.0f );                                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f );                                     // Top of left face
        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex3f( -1.0f, -1.0f, -1.0f );                                  // Left of left face
        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex3f( -1.0f, -1.0f, 1.0f );                                   // Right of left face
    glEnd();

    // Render a quad for the bottom of our pyramid
    glBegin( GL_QUADS );
        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex3f( -1.0f, -1.0f, 1.0f );                                   // Left/right of front/left face
        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex3f( 1.0f, -1.0f, 1.0f );                                    // Right/left of front/right face
        glColor3f( 0.0f, 1.0f, 0.0f );                                      // Green
        glVertex3f( 1.0f, -1.0f, -1.0f );                                   // Right/left of right/back face
        glColor3f( 0.0f, 0.0f, 1.0f );                                      // Blue
        glVertex3f( -1.0f, -1.0f, -1.0f );                                  // Left/right of right/back face
    glEnd();
    glPopMatrix();

    glTranslatef( 3.0f, 0.0f, 0.0f );                                        // Translate right
    glPushMatrix();                                                         // Push the current modelview matrix on the matrix stack
    glRotatef( g_fRotate2, 1.0f, 1.0f, 1.0f );                              // Rotate the primitive on all 3 axis
    glBegin( GL_QUADS );
        // Top face
        glColor3f(   0.0f, 1.0f,  0.0f );                                   // Green
        glVertex3f(  1.0f, 1.0f, -1.0f );                                   // Top-right of top face
        glVertex3f( -1.0f, 1.0f, -1.0f );                                   // Top-left of top face
        glVertex3f( -1.0f, 1.0f,  1.0f );                                   // Bottom-left of top face
        glVertex3f(  1.0f, 1.0f,  1.0f );                                   // Bottom-right of top face

        // Bottom face
        glColor3f(   1.0f,  0.5f,  0.0f );                                  // Orange
        glVertex3f(  1.0f, -1.0f, -1.0f );                                  // Top-right of bottom face
        glVertex3f( -1.0f, -1.0f, -1.0f );                                  // Top-left of bottom face
        glVertex3f( -1.0f, -1.0f,  1.0f );                                  // Bottom-left of bottom face
        glVertex3f(  1.0f, -1.0f,  1.0f );                                  // Bottom-right of bottom face

        // Front face
        glColor3f(   1.0f,  0.0f, 0.0f );                                  // Red
        glVertex3f(  1.0f,  1.0f, 1.0f );                                  // Top-Right of front face
        glVertex3f( -1.0f,  1.0f, 1.0f );                                  // Top-left of front face
        glVertex3f( -1.0f, -1.0f, 1.0f );                                  // Bottom-left of front face
        glVertex3f(  1.0f, -1.0f, 1.0f );                                  // Bottom-right of front face

        // Back face
        glColor3f(   1.0f,  1.0f,  0.0f );                                 // Yellow
        glVertex3f(  1.0f, -1.0f, -1.0f );                                 // Bottom-Left of back face
        glVertex3f( -1.0f, -1.0f, -1.0f );                                 // Bottom-Right of back face
        glVertex3f( -1.0f,  1.0f, -1.0f );                                 // Top-Right of back face
        glVertex3f(  1.0f,  1.0f, -1.0f );                                 // Top-Left of back face

        // Left face
        glColor3f(   0.0f,  0.0f,  1.0f);                                   // Blue
        glVertex3f( -1.0f,  1.0f,  1.0f);                                   // Top-Right of left face
        glVertex3f( -1.0f,  1.0f, -1.0f);                                   // Top-Left of left face
        glVertex3f( -1.0f, -1.0f, -1.0f);                                   // Bottom-Left of left face
        glVertex3f( -1.0f, -1.0f,  1.0f);                                   // Bottom-Right of left face

        // Right face
        glColor3f(   1.0f,  0.0f,  1.0f);                                   // Violet
        glVertex3f(  1.0f,  1.0f,  1.0f);                                   // Top-Right of left face
        glVertex3f(  1.0f,  1.0f, -1.0f);                                   // Top-Left of left face
        glVertex3f(  1.0f, -1.0f, -1.0f);                                   // Bottom-Left of left face
        glVertex3f(  1.0f, -1.0f,  1.0f);                                   // Bottom-Right of left face
    glEnd();
    glPopMatrix();

    glTranslatef( -1.5f, -3.0f, 0.0f );                                     // Back to center and lower screen
    glPushMatrix();
    glRotatef( g_fRotate3, 1.0f, 1.0f, 1.0f );   
    glColor3f( 1.0f, 1.0f, 0.0f );                                          // Yellow
    glutSolidSphere( 1.0f, 16, 16 );                                        // Use GLUT to draw a solid sphere
    glScalef( 1.01f, 1.01f, 1.01f );                                        
    glColor3f( 1.0f, 0.0f, 0.0f );                                          // Red
    glutWireSphere( 1.0f, 16, 16 );                                         // Use GLUT to draw a wireframe sphere
    glPopMatrix();
}

void RenderScene5()
{
	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	// Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	// Draw 36 SnowMen
	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0, 0, j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}

	glutSwapBuffers();
}
