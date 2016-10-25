#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glext.h"

int mouseoldx, mouseoldy; // For mouse motion
GLdouble eyeloc = 2.0; // Where to look from; initially 0 -2 2

const GLfloat florverts [4][3]={
{0.5,0.5,0.0},{-0.5,0.5,0.0},{-0.5,-0.5,0.0},{0.5,-0.5,0.0}};
const GLfloat floorcol[4][3]={
{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,1.0}};
const GLubyte floorinds[1][4]={ {0,1,2,3}};
const GLfloat floorverts2[4][3]={
{0.5,0.5,1.0},{-0.5,0.5,1.0},{-0.5,-0.5,1.0},{0.5,-0.5,1.0}};
const GLfloat floorcol2[4][3]={
{1.0,0.0,0.0},{1.0,0.0,0.0},{1.0,0.0,0.0},{1.0,0.0,0.0}};
const GLubyte floorinds2[1][4]= {{0,1,2,3}};
const int numobjects = 2; // number of objects for buffer
const int numperobj = 3; // vertices, colors, indices
GLuint buffers [numperobj] ; // list of buffers for geometric data
GLuint objects [numobjects]; // for each object
GLenum PrimType[numobjects]; // Primitive Type (quads, polygons)
GLsizei NumElems[numobjects]; // number of geometric elements

// Floor Geometry is specified with a vertex array
// The Buffer Offset Macro is from Red Book, page 103, 106
// Note for more complex objects the indices must be integers, not bytes.
#define BUFFER_OFFSET(bytes) ((GLubyte *) NULL + (bytes))
#define NumberOf(array) (sizeof(array)/sizeof(array[0]))
enum {Vertices, Colors, Elements}; // For arrays for objects
enum {FLOOR, FLOOR2}; // For objects, for the floor

void initobject (GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type)
{
	int offset = object * numperobj;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]);
	glBufferData(GL_ARRAY_BUFFER, sizevert, vert, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT,0,BUFFER_OFFSET(0));
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors+offset]);
	glBufferData(GL_ARRAY_BUFFER, sizecol, col,GL_STATIC_DRAW);
	glColorPointer(3,GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds, GL_STATIC_DRAW);
	PrimType[object] = type;
	NumElems[object] = sizeind;
}

void init (void)
{
	/* select clearing color */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	/* initializeviewing values */
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0,-eyeloc,eyeloc,0,0,0,0,1,1);
}

/* display function - code from:
     http://fly.cc.fer.hr/~unreal/theredbook/chapter01.html
This is the actual usage of the OpenGL library. 
The following code is the same for any platform */
void display()
{
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //glColor3f(1.0, 1.0, 1.0);
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
	glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
	glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-0.5, -0.5, 0.0);
	glColor3f(1.0, 1.0, 1.0);
        glVertex3f(0.5, -0.5, 0.0);
    glEnd();
    glFlush();
}

/* keyboard function*/

void keyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: // escape to quit
			exit(0);
		break;
		default:
		break;
	}
}

/* Reshape function: reshapes the window appropriately */
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (GLdouble)w / (GLdouble)h, 1.0 ,10.0);
}

/* Mouse function */

void mousedrag (int x, int y)
{
	int yloc = y - mouseoldy ; 
	eyeloc += 0.005*yloc;
	if (eyeloc < 0) eyeloc = 0.0;
	mouseoldy = y;
	/* Set the eye location */
	glMatrixMode (GL_MODELVIEW) ; 
	glLoadIdentity();
	gluLookAt(0,-eyeloc,eyeloc,0,0,0,0,1,1);
	glutPostRedisplay();
}

void mouse (int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP) { // Do nothing ; 
		}
		else if (state == GLUT_DOWN) {
		//mousedrag(x,y);
		mouseoldx = x; mouseoldy= y; 
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{// Reset gluLookAt
		eyeloc = 2.0;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0,-eyeloc,eyeloc,0,0,0,0,1,1);
		glutPostRedisplay();
	}
}
/* Main method - main entry point of application
the freeglut library does the window creation work for us, 
regardless of the platform. */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL - First window demo");
    init();

    display();

// Define callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mousedrag);
// start the main code
    glutMainLoop();    
    return 0;
}
