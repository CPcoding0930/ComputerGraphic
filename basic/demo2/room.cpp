/* Link the libraries code-wise. */
#ifdef _MSC_VER
#	pragma comment(lib, "OpenGL32.lib")
#	pragma comment(lib, "GLu32.lib")

#	pragma comment(lib, "SDL.lib")
#	pragma comment(lib, "SDLmain.lib")
#	pragma comment(lib, "SDL_image.lib")
#endif //_MSC_VER

#include <string>
#include <cmath>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#define PI 3.141592653589793

unsigned Textures[3];
unsigned BoxList(0);					//Added!

										/* These will define the player's position and view angle. */
double X(0.0), Y(0.0), Z(0.0);
double ViewAngleHor(0.0), ViewAngleVer(0.0);

/*
* DegreeToRadian
*	Converts a specified amount of degrees to radians.
*/
inline double DegreeToRadian(double degrees)
{
	return (degrees / 180.f * PI);
}

/*
* GrabTexObjFromFile
*	This function will use SDL to load the specified image, create an OpenGL
*	texture object from it and return the texture object number.
*/
GLuint GrabTexObjFromFile(const std::string& fileName)
{
	/* Use SDL_image to load the PNG image. */
	SDL_Surface *Image = IMG_Load(fileName.c_str());

	/* Image doesn't exist or failed loading? Return 0. */
	if (!Image)
		return 0;

	unsigned Object(0);

	/* Generate one texture (we're creating only one). */
	glGenTextures(1, &Object);

	/* Set that texture as current. */
	glBindTexture(GL_TEXTURE_2D, Object);

	/* You can use these values to specify mipmaps if you want to, such as 'GL_LINEAR_MIPMAP_LINEAR'. */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* We're setting textures to be repeated here. */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //NEW!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //NEW!

																  /* Create the actual texture object. */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->w, Image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->pixels);

	/* Free the surface, we are finished with it. */
	SDL_FreeSurface(Image);

	return Object;
}

/*
*	CompileLists
*		Compiles the display lists used by our application.
*/
void CompileLists()
{
	/* Let's generate a display list for a box. */
	BoxList = glGenLists(1);
	glNewList(BoxList, GL_COMPILE);

	/*
	* Render everything as you usually would, without texture binding. We're rendering the box from the
	* '3D Objects' tutorial here.
	*/
	glBegin(GL_QUADS);
	/* Front */
	glTexCoord2d(0, 0); glVertex3d(400, 125, 0.4);
	glTexCoord2d(1, 0); glVertex3d(750, 125, 0.4);
	glTexCoord2d(1, 1); glVertex3d(750, 475, 0.4);
	glTexCoord2d(0, 1); glVertex3d(400, 475, 0.4);

	/* Left side */
	glTexCoord2d(0, 0); glVertex3d(400, 125, -0.4);
	glTexCoord2d(1, 0); glVertex3d(400, 125, 0.4);
	glTexCoord2d(1, 1); glVertex3d(400, 475, 0.4);
	glTexCoord2d(0, 1); glVertex3d(400, 475, -0.4);

	/* Back */
	glTexCoord2d(0, 0); glVertex3d(750, 125, -0.4);
	glTexCoord2d(1, 0); glVertex3d(400, 125, -0.4);
	glTexCoord2d(1, 1); glVertex3d(400, 475, -0.4);
	glTexCoord2d(0, 1); glVertex3d(750, 475, -0.4);

	/* Right side */
	glTexCoord2d(0, 0); glVertex3d(750, 125, 0.4);
	glTexCoord2d(1, 0); glVertex3d(750, 125, -0.4);
	glTexCoord2d(1, 1); glVertex3d(750, 475, -0.4);
	glTexCoord2d(0, 1); glVertex3d(750, 475, 0.4);

	/* Top */
	glTexCoord2d(0, 0); glVertex3d(400, 125, -0.4);
	glTexCoord2d(1, 0); glVertex3d(750, 125, -0.4);
	glTexCoord2d(1, 1); glVertex3d(750, 125, 0.4);
	glTexCoord2d(0, 1); glVertex3d(400, 125, 0.4);

	/* Bottom */
	glTexCoord2d(0, 0); glVertex3d(400, 475, -0.4);
	glTexCoord2d(1, 0); glVertex3d(750, 475, -0.4);
	glTexCoord2d(1, 1); glVertex3d(750, 475, 0.4);
	glTexCoord2d(0, 1); glVertex3d(400, 475, 0.4);
	glEnd();
	glEndList();
}

/*
* DrawRoom
*	This will render the entire scene (in other words, draw the room).
*/
void DrawRoom()
{
	/* You also could do this at front by using the SDL surface's values or in an array. */
	static float WallTexWidth(0.f);
	static float WallTexHeight(0.f);

	static float FloorTexWidth(0.f);
	static float FloorTexHeight(0.f);

	static bool Once(false);

	/* Perform this check only once. */
	if (!Once)
	{
		/* Bind the wall texture. */
		glBindTexture(GL_TEXTURE_2D, Textures[0]);

		/* Retrieve the width and height of the current texture (can also be done up front with SDL and saved somewhere). */
		glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &WallTexWidth);
		glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &WallTexHeight);

		/* Bind the floor texture. */
		glBindTexture(GL_TEXTURE_2D, Textures[1]);

		/* Retrieve the width and height of the current texture (can also be done up front with SDL and saved somewhere). */
		glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &FloorTexWidth);
		glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &FloorTexHeight);

		Once = true;
	}

	glPushMatrix();

	/* Move the world and rotate the view. */
	glRotated(ViewAngleVer, 1, 0, 0);
	glRotated(ViewAngleHor, 0, 1, 0);

	glTranslated(-X, -Y, -Z);

	/* Set the coordinate system. */
	glOrtho(0, 800, 600, 0, -1, 1);

	/* Draw walls. */
	glBindTexture(GL_TEXTURE_2D, Textures[0]);

	glBegin(GL_QUADS);
	/* Wall in front of you when the app starts. */
	glTexCoord2f(0, 0);
	glVertex3d(-200, 0, 4.0);

	glTexCoord2f(1200.f / WallTexWidth, 0);
	glVertex3d(1000, 0, 4.0);

	glTexCoord2f(1200.f / WallTexWidth, 400.f / WallTexHeight);
	glVertex3d(1000, 500, 4.0);

	glTexCoord2f(0, 400.f / WallTexHeight);
	glVertex3d(-200, 500, 4.0);

	/* Wall left of you. */
	glTexCoord2f(0, 0);
	glVertex3d(-200, 0, -4.0);

	glTexCoord2f(1200.f / WallTexWidth, 0);
	glVertex3d(-200, 0, 4.0);

	glTexCoord2f(1200.f / WallTexWidth, 400.f / WallTexHeight);
	glVertex3d(-200, 500, 4.0);

	glTexCoord2f(0, 400.f / WallTexHeight);
	glVertex3d(-200, 500, -4.0);

	/* Wall right of you. */
	glTexCoord2f(0, 0);
	glVertex3d(1000, 0, 4.0);

	glTexCoord2f(1200.f / WallTexWidth, 0);
	glVertex3d(1000, 0, -4.0);

	glTexCoord2f(1200.f / WallTexWidth, 400.f / WallTexHeight);
	glVertex3d(1000, 500, -4.0);

	glTexCoord2f(0, 400.f / WallTexHeight);
	glVertex3d(1000, 500, 4.0);

	/* Wall behind you (you won't be able to see this just yet, but you will later). */
	glTexCoord2f(0, 0);
	glVertex3d(1000, 0, -4.0);

	glTexCoord2f(1200.f / WallTexWidth, 0);
	glVertex3d(-200, 0, -4.0);

	glTexCoord2f(1200.f / WallTexWidth, 400.f / WallTexHeight);
	glVertex3d(-200, 500, -4.0);

	glTexCoord2f(0, 400.f / WallTexHeight);
	glVertex3d(1000, 500, -4.0);
	glEnd();

	/* Draw the floor and the ceiling, this is done separatly because glBindTexture isn't allowed inside glBegin. */
	glBindTexture(GL_TEXTURE_2D, Textures[1]);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3d(-200, 500, 4.0);

	glTexCoord2f(1200.f / FloorTexWidth, 0);
	glVertex3d(1000, 500, 4.0);

	glTexCoord2f(1200.f / FloorTexWidth, (8.f / 2.f * 600.f) / FloorTexHeight);
	glVertex3d(1000, 500, -4.0);

	glTexCoord2f(0, (8.f / 2.f * 600.f) / FloorTexHeight);
	glVertex3d(-200, 500, -4.0);

	/* Ceiling. */
	glTexCoord2f(0, 0);
	glVertex3d(-200, 0, 4.0);

	glTexCoord2f(1200.f / FloorTexWidth, 0);
	glVertex3d(1000, 0, 4.0);

	glTexCoord2f(1200.f / FloorTexWidth, (8.f / 2.f * 600.f) / FloorTexHeight);
	glVertex3d(1000, 0, -4.0);

	glTexCoord2f(0, (8.f / 2.f * 600.f) / FloorTexHeight);
	glVertex3d(-200, 0, -4.0);
	glEnd();

	/* Now we're going to render some boxes using display lists. */
	glPushMatrix();
	/* Let's make it a bit smaller... */
	glScaled(0.5, 0.4, 0.5);

	/* Can't bind textures while generating a display list, but we can give it texture coordinates and bind it now. */
	glBindTexture(GL_TEXTURE_2D, Textures[2]);

	/*
	* Because display lists have preset coordinates, we'll need to translate it to move it around. Note that we're
	* moving the small version of the cube around, not the big version (because we scaled *before* translating).
	*/
	glTranslated(-700, 750, 6);

	/*
	* Let's draw a whole lot of boxes. Note that because we're not pushing and popping matrices, translations
	* and changes will 'accumulate' and add to the previous translation.
	*/
	for (short i(0); i < 12; ++i)
	{
		glTranslated(350, 0, 0);

		/* These make sure that every once in a while, a new row is started. */
		if (i == 5)		glTranslated(-1575, -350, 0);
		if (i == 9)		glTranslated(-1225, -350, 0);

		/*
		* glCallList is all that is really needed to execute the display list. Remember to try the 'K' button
		* to turn on wireframe mode, with these extra polygons, it looks pretty neat!
		*/
		glCallList(BoxList);
	}

	glPopMatrix();

	glPopMatrix();
}

int main(int argc, char **argv)
{
	/* Initialize SDL and set up a window. */
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_WM_SetCaption("OpenGL - Display Lists", 0);
	SDL_WM_GrabInput(SDL_GRAB_ON);

	SDL_ShowCursor(SDL_DISABLE);

	SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

	/* Basic OpenGL initialization, handled in 'The Screen'. */
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 1);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(80.0, 800.0 / 600.0, 0.1, 100.0);

	/* We now switch to the modelview matrix. */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/* Enable 2D texturing. */
	glEnable(GL_TEXTURE_2D);

	/* Set up alpha blending. */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4d(1, 1, 1, 1);

	Textures[0] = GrabTexObjFromFile("Data/Wall.png");
	Textures[1] = GrabTexObjFromFile("Data/Floor.png");
	Textures[2] = GrabTexObjFromFile("Data/Box.png");			//Added!

																//Replaced this with a loop that immediately checks the entire array.
																//sizeof(Textures) is the size of the entire array in bytes (unsigned int = 4 bytes)
																//so sizeof(Textures) would give 3 * 4 = 12 bytes, divide this by 4 bytes and you
																//have 3.
	for (unsigned i(0); i < sizeof(Textures) / sizeof(unsigned); ++i)
	{
		if (Textures[i] == 0)
		{
#ifdef _WIN32
			MessageBoxA(0, "Something went seriously wrong!", "Fatal Error!", MB_OK | MB_ICONERROR);
#endif //_WIN32

			return 1;
		}
	}

	/* Compile the display lists. */
	CompileLists();

	SDL_Event event;

	int RelX(0), RelY(0);
	int MovementDelay(SDL_GetTicks());

	bool Wireframe(false);
	bool Keys[4] =
	{
		false, /* Up arrow down? */
		false, /* Down arrow down? */
		false, /* Left arrow down? */
		false  /* Right arrow down? */
	};

	/* Application loop. */
	for (;;)
	{
		/* Handle events with SDL. */
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break;

			/* Mouse events? */
			else if (event.type == SDL_MOUSEMOTION)
			{
				/* Get the relative mouse movement of the mouse (based on CurMouseCoord - PrevMouseCoord). */
				SDL_GetRelativeMouseState(&RelX, &RelY);

				ViewAngleHor += RelX / 4;
				ViewAngleVer += RelY / 4;

				/* Prevent the horizontal angle from going over 360 degrees or below 0 degrees. */
				if (ViewAngleHor >= 360.0)		ViewAngleHor = 0.0;
				else if (ViewAngleHor < 0.0)		ViewAngleHor = 360.0;

				/* Prevent the vertical view from moving too far (comment this out to get a funny effect). */
				if (ViewAngleVer > 60.0)			ViewAngleVer = 60.0; /* 60 degrees is when you're looking down. */
				else if (ViewAngleVer < -60.0)	ViewAngleVer = -60.0; /* This is when you're looking up. */

																	  /* This delay might seem strange, but it helps smoothing out the mouse if you're experiencing jittering. */
				SDL_Delay(5);
			}

			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
					break;

				if (event.key.keysym.sym == SDLK_k)
					glPolygonMode(GL_FRONT_AND_BACK, ((Wireframe = !Wireframe) ? GL_LINE : GL_FILL));

				if (event.key.keysym.sym == SDLK_UP)			Keys[0] = true;
				if (event.key.keysym.sym == SDLK_DOWN)		Keys[1] = true;
				if (event.key.keysym.sym == SDLK_LEFT)		Keys[2] = true;
				if (event.key.keysym.sym == SDLK_RIGHT)		Keys[3] = true;
			}

			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_UP)			Keys[0] = false;
				if (event.key.keysym.sym == SDLK_DOWN)		Keys[1] = false;
				if (event.key.keysym.sym == SDLK_LEFT)		Keys[2] = false;
				if (event.key.keysym.sym == SDLK_RIGHT)		Keys[3] = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix();
		DrawRoom();
		glPopMatrix();

		/* Move if the keys are pressed, this is explained in the tutorial. */
		if (Keys[0])
		{
			X -= cos(DegreeToRadian(ViewAngleHor + 90.0)) * 0.05;
			Z -= sin(DegreeToRadian(ViewAngleHor + 90.0)) * 0.05;
		}

		if (Keys[1])
		{
			X += cos(DegreeToRadian(ViewAngleHor + 90.0)) * 0.05;
			Z += sin(DegreeToRadian(ViewAngleHor + 90.0)) * 0.05;
		}

		if (Keys[2])
		{
			X += cos(DegreeToRadian(ViewAngleHor + 180.0)) * 0.05;
			Z += sin(DegreeToRadian(ViewAngleHor + 180.0)) * 0.05;
		}

		if (Keys[3])
		{
			X -= cos(DegreeToRadian(ViewAngleHor + 180.0)) * 0.05;
			Z -= sin(DegreeToRadian(ViewAngleHor + 180.0)) * 0.05;
		}

		/* Swap the display buffers. */
		SDL_GL_SwapBuffers();
	}

	/* Delete the created textures. */
	glDeleteTextures(3, Textures);		//Changed to 3.
	glDeleteLists(BoxList, 1);

	/* Clean up. */
	SDL_Quit();

	return 0;
}