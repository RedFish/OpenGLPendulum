// Richard GUERCI
// CS426 - Computer Graphics
// Assignment 4

// Windows Library
#include "stdafx.h" 
// Glut Library
#include <freeglut.h>
// C Library
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI		3.14159265359
#define NUM		15000	// Number of iterations that will span width of display
#define FRAMETIMEMS 1	// One frame every one milliseconds
#define SPACEBAR 32		// ASCII code of space (in decimal)

// Handles to Windows
int   window1=0;

// global variables
float x[NUM],y[NUM];
float xx[NUM],yy[NUM],yyy[NUM];
float g, l, D, dt;
float v, a, t, b, c;
int i = 0, opt = 0, view = 1;

// preprocessors
void init(void);
void clear_plot();
void draw_bob(float);
void circle(float, float, float);
void gl_pnum(float,float,float,float);
void gl_pstring(float,float,float,char *);
void plot_attractor();
void info(void);
static void key(unsigned char, int, int);
static void special_key(int, int, int);
static void draw(void);
static void update(int);
static void resize(int width, int height);


// Initialize system variables
void init(void)
{
	g=9.81;
	l=0.25;
	D=-0.50;
	dt=0.001;
	v=10;
	a=0;
	t=0;
	b=0;
	c=0.0;
	clear_plot();
}

//clear plot
void clear_plot(void)
{
	i = 0;
}

// Draw pendulum
void draw_bob(float theta)
{
	float offset=PI/2.0f;
	// Draw line from center to bob
	glColor3f(0.4,0.4,0.4);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.5f,0.5f);
	glVertex2f(0.5f+(0.3f*cos(theta-offset)),0.5f+(0.3f*sin(theta-offset))); 
	glEnd();
	glColor3f(0.9,0.9,0.9);
	circle(0.5f+(0.3f*cos(theta-offset)),0.5f+(0.3f*sin(theta-offset)),0.045);
	
}

// Draw circle
void circle(float x, float y, float r)
{
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f(x, y);
    for( int n = 0; n <= 24; ++n )
    {
		float const t = 2*PI*(float)n/24.0f;
        glVertex2f(x + sin(t)*r, y + cos(t)*r);
	}
	glEnd();
}

// GLUT callback Handlers
static void draw(void)
{	
	// Clear screen (on left)
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw pendulum motion on left of screen
	glViewport(0,0,400,400);

	// Make the following region active;
	glScissor(0,0,400,400);
	glEnable(GL_SCISSOR_TEST);

	float theta=a;
	draw_bob(theta);
	info();

	// Set screen back to normal
	glScissor(0,0,800,400);
	// Set drawing colour to green and line style
	glColor3f(0.0,1.0,0.0);
	glDisable(GL_SCISSOR_TEST);
	glViewport(400,0,400,400);
	glLineWidth(0.5);
	
	plot_attractor();

    // Send message to cause re-draw (to return to Draw)
	glutPostRedisplay();

	glutSwapBuffers();
}

// The Physics: Eulers method applied to projectile motion 
static void update(int dummy)
{
    // Advance pendulum using Euler method
	a=a+(v*dt);
	v=v+((((-g/l)*sin(a))+(D*v)+(c*fabs(sin(b*t))))*dt);
	t=t+dt;

	//coordinates for view 0
	x[i]=a;
	y[i]=v;

	//coordinates for view 1
	xx[i]=t;
	yy[i]=v;
	yyy[i]=a;

	if (i<NUM)
	{
		i++;
	}
	else 
	{
		clear_plot();
	}

    // Tell OpenGL to redraw screen now you have updated the 'game play'
    glutTimerFunc(FRAMETIMEMS, update, 0);
}

// Plot
void plot_attractor()
{
	float minx,maxx,miny,maxy;
	float scale = 0.0004;
	if(view)
	{
		maxx=x[0];
		minx=x[0];
		maxy=y[0];
		miny=y[0];
		for(int j=1;j<i;j++)
		{
			if(maxx<x[j])maxx=x[j];
			if(minx>x[j])minx=x[j];
			if(maxy<y[j])maxy=y[j];
			if(miny>y[j])miny=y[j];
		}
	}
	else
	{
		minx=xx[0];
		maxx=xx[0]+NUM*dt;
		maxy=miny=yy[0];
		for(int j=1;j<i;j++)
		{
			 if(maxy<yy[j])maxy=yy[j];
			 if(miny>yy[j])miny=yy[j];
			 if(maxy<yyy[j])maxy=yyy[j];
			 if(miny>yyy[j])miny=yyy[j];
		}
	}
	gl_pnum(0,-0.08,scale,minx);
	gl_pnum(0.9,-0.08,scale,maxx);
	if(view) gl_pstring(0.3,-0.08,scale,"Angle (Radian)");
	else gl_pstring(0.3,-0.08,scale,"Time (seconds)");
	gl_pnum(-0.20,0,scale,miny);
	gl_pnum(-0.20,0.95,scale,maxy);
	if(view)
	{
		gl_pstring(-0.20,0.5,scale,"Velocity");
	}
	else
	{
		glColor3f(1.0,0.0,0.0);
		gl_pstring(-0.20,0.5,scale,"Velocity");
		gl_pstring(-0.20,0.45,0.0003,"(R/s)");
		glColor3f(0.0,0.0,1.0);
		gl_pstring(-0.20,0.70,scale,"Angle");
		gl_pstring(-0.20,0.65,0.0003,"(Radian)");
	}
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.01,-0.01);
	glVertex2f(-0.01,1.01);
	glVertex2f(1.01,1.01);
	glVertex2f(1.01,-0.01);
	glEnd();
	glPointSize(1);
	if (view) 
	{
		glBegin(GL_POINTS);
		glColor3f(1.0,1.0,1.0);
		for(int j=0;j<i;j++)
		{
			glVertex2f((x[j]-minx)/(maxx-minx),(y[j]-miny)/(maxy-miny));
		}
		glEnd();
	}
	else
	{
		glBegin(GL_POINTS);
		for(int j=0;j<i;j++)
		{
			glColor3f(1.0,0.0,0.0);
			glVertex2f((xx[j]-minx)/(maxx-minx),(yy[j]-miny)/(maxy-miny));
			glColor3f(0.0,0.0,1.0);
			glVertex2f((xx[j]-minx)/(maxx-minx),(yyy[j]-miny)/(maxy-miny));
		}
		glEnd();
	}
	glColor3f(1.0,1.0,1.0);
	gl_pstring(-0.1,-0.15,scale,"v : change view");
	gl_pstring(0.6,-0.15,scale,"r : restart");
}

// Prints info
void info(void)
{
	glColor3f(1.0,1.0,1.0);
	float shift = 0.055;
	float scale = 0.00035;
	char text[20];
	gl_pstring(0.2,1.04,0.0004,"Pendulum OpenGL");
	
	gl_pstring(0.4,0.95-shift*0,0.0004,"Left/Right");
	gl_pstring(0.66,0.95-shift*0,0.0004,": change velocity");
	gl_pstring(0.4,0.95-shift*1,0.0004,"Up/Down");
	gl_pstring(0.66,0.95-shift*1,0.0004,": select");
	gl_pstring(0.4,0.95-shift*2,0.0004,"+/-");
	gl_pstring(0.66,0.95-shift*2,0.0004,": change value");

	gl_pstring(-0.10,0.95,scale,"G =    m");
	gl_pnum(0.02,0.95,scale,g);
	gl_pstring(-0.10,0.95-shift,scale,"L =    m");
	gl_pnum(0.02,0.95-shift,scale,l);
	gl_pstring(-0.10,0.95-shift*2,scale,"D =     R/s");
	gl_pnum(0.02,0.95-shift*2,scale,D);
	gl_pstring(-0.10,0.95-shift*3,scale,"dt =    s");
	sprintf(text,"%3.3f",dt);
	gl_pstring(0.02,0.95-shift*3,scale,text);

	gl_pstring(-0.15,0.95-shift*opt,scale,">");
	
	gl_pstring(-0.1,-0.05,scale,"time =      seconds");
	gl_pnum(0.1,-0.05,scale,t);
	gl_pstring(-0.1,-0.05+shift,scale,"Angle =      deg (    rad)");
	gl_pnum(0.1,-0.05+shift,scale,a*(180/PI));
	gl_pnum(0.43,-0.05+shift,scale,a);
	gl_pstring(-0.1,-0.15,0.0004,"q : quit   spacebar : reset");
}

// Resized new window, recreate
static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-0.2, 1.1, -0.2, 1.1);
}

// Callback to manage standard keys on the keyboard
static void key(unsigned char key, int a, int b)
{
    switch (key)
    {
		case SPACEBAR:
			init();
			break;
		case '+':
			switch(opt)
			{
				case 0://g
					g += 0.1;
					break;
				case 1://l
					l += 0.1;
					break;
				case 2://D
					D += 0.1;
					break;
			}
			break;
		case '-':
			switch(opt)
			{
				case 0://g
					g -= 0.1;
					break;
				case 1://l
					l -= 0.1;
					break;
				case 2://D
					D -= 0.1;
					break;
			}
			break;
		case 'r'://restart
			clear_plot();
			break;
		case 'v'://change view
			view++;
			view%=2;
			break;
		case 'Q':
		case 'q':
			glutLeaveMainLoop();
			break;  // (Stop!)
	    default: break;
    }
    glutPostRedisplay();
}

// Callback to manage special keys on the keyboard
static void special_key(int key, int a, int b)
{
    switch (key)
    {
		case GLUT_KEY_RIGHT:
			v+=5;
			break;
		case GLUT_KEY_LEFT:
			v-=5;
			break;
		case GLUT_KEY_UP:
			opt-=1;
			if(opt<0)opt=2;
			opt%=3;
			break;
		case GLUT_KEY_DOWN:
			opt+=1;
			opt%=3;
			break;
	    default: break;
    }
}

// Print the value of a variable on screen
void gl_pnum(float x,float y,float scale,float n)
{
	char text[20];
	sprintf(text,"%3.2f",n);
	gl_pstring(x,y,scale,text);
}

// Print a string on screen
void gl_pstring(float x,float y, float scale,char text[])
{
	glPushMatrix();
	glTranslatef(x,y,1);
	glScalef(scale,scale,1);
	glutStrokeString(GLUT_STROKE_ROMAN,(unsigned char *)text);
	glPopMatrix();
}


// Assumes plotting into a region [0,0][1,1] with
// some space around edges for lables
int main(int argc, char *argv[])
{
	// Create a Window called "FreeGlut One" at 100,100 on main screen, sized 800x400 pixels
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800, 400);
	glutInit(&argc, argv);
	// Create a double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Window1 stores id used to allow switch between active windows.
	window1 = glutCreateWindow("Two Viewport: Pendulum Animation and Path");

	// Set clearscreen colour to white
	glClearColor(0.0,0.0,0.0,0.0);

    // Load location of ReShapeFunction into mainloop so it can be called if required
    // "resize" is both the name of the function and when referenced the memory address of the function
    glutReshapeFunc(resize);
	
	// Callback function used when mainloop decides/graphics card to re-draw the scene
	// Sending a glutPostRedisplay(); message to mainloop will cause display to be called
    glutDisplayFunc(draw);

    // Callback functions to manage keyboard input
    glutKeyboardFunc(key);
    glutSpecialFunc(special_key);

    // Called during idle time (like Update in XNA)
    // Doublebuffer at frame rate
    glutTimerFunc(FRAMETIMEMS, update, 0);

	//Init variables
	init();

    // Start mainloop running
    glutMainLoop();

    return EXIT_SUCCESS;
}
