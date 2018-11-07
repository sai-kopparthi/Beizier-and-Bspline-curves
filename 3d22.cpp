/*
 * Simple glut demo that can be used as a template for
 * other projects by Garrett Aldrich
 */


#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;
int n;
float t;
float d[100][1000][1000],da[100][1000],das[100][2000];
float e[100][1000][1000],ea[100][1000],eas[100][2000];
int f[1000];
int c;
int ma[1000],ca[1000],mas[2000];
int km=4;
int cam[2000];
int u[2000][2000];
int pid,fa=0;
float res;
//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;
FILE *fp = fopen("in.txt", "r+");
FILE *fp1 = fopen("in.txt", "r+");
/*Window information*/
int win_height;
int win_width;
//void lineDDA (int , int , int , int )
void init();
void idle();
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void dda(float x1,float y1,float x2,float y2);
void draw_pix1(int x, int y);
void outputfile();

void lineDDA (int x0, int y0, int xEnd, int yEnd)
{
int dx = xEnd - x0, dy = yEnd - y0, steps, k;
float xIncrement, yIncrement, x = x0, y = y0;
if (fabs (dx) > fabs (dy))
steps = fabs (dx);
else
steps = fabs (dy);
xIncrement = float (dx) / float (steps);
yIncrement = float (dy) / float (steps);
draw_pix (round(x), round(y));
for (k = 0; k < steps; k++) {
x += xIncrement;
y += yIncrement;
draw_pix (round (x), round (y));
}
}
void lineDDA1 (int x0, int y0, int xEnd, int yEnd)
{
int dx = xEnd - x0, dy = yEnd - y0, steps, k;
float xIncrement, yIncrement, x = x0, y = y0;
if (fabs (dx) > fabs (dy))
steps = fabs (dx);
else
steps = fabs (dy);
xIncrement = float (dx) / float (steps);
yIncrement = float (dy) / float (steps);
draw_pix (round(x), round(y));
for (k = 0; k < steps; k++) {
x += xIncrement;
y += yIncrement;
draw_pix1 (round (x), round (y));
}
}
void poly1()
{
//clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();
if(fa==1)
{   
for(int i=0;i<n;i++)
{
 for(int j=0;j<f[i];j++)
	{
	draw_pix1(d[i][j][0],e[i][j][0]);
	}
}
}
for(int i=0;i<n;i++)
	{	
	for(int j=0;j<(f[i])-1;j++)
		{
			glLineWidth(2.5); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(1.0,0,0);
         glVertex2f(d[i][j][0],e[i][j][0]);
         glVertex2f(d[i][j+1][0],e[i][j+1][0]);
     glEnd();
			//lineDDA(d[i][j][0],e[i][j][0],d[i][j+1][0],e[i][j+1][0]);
	}
		//CSLCAD(d[i][j],e[i][j],d[i][0],e[i][0],20,50,20,50);
	}
}

void beizer()
{
for(int k=0;k<n;k++)
{
ma[k]=0;
}
t=0.01;
while(t<=1)
{
	for(int k=0;k<n;k++)
	{//ma[k]=0;
		for(int j=1;j<(f[k]);j++)
		{
			for(int i=0;i<=(f[k]-j);i++)
			{
				d[k][i][j]=((1-t)*d[k][i][j-1])+(t*d[k][i+1][j-1]);
				e[k][i][j]=((1-t)*e[k][i][j-1])+(t*e[k][i+1][j-1]);
			}
			
		}

//printf("%d",c);
	    ma[k]=ma[k]+1;
		da[k][ma[k]]=d[k][0][f[k]-1];
		ea[k][ma[k]]=e[k][0][f[k]-1];

	}

//c=0;
t=t+(1.0/res);	
}
}
void bspline()
{

for(int k=0;k<n;k++)
{
mas[k]=0;
ca[k]=0;
cam[k]=0;
for(int i=0;i<f[k];i++)
{
int im=i+km-1;
if(im<f[k]-1)
ca[k]=ca[k]+1;
else 
break;
}
}
for(int k=0;k<n;k++)
{
for(int i=0;i<(f[k]-1+km);i++)
{
u[k][i]=cam[k];
cam[k]=cam[k]+1;
}
}
for(int k=0;k<n;k++)
{
for(float ux=u[k][km-1];ux<=u[k][ca[k]+km];ux+=(1.0/res))
{
int apple=ux;
//printf("%f",ux);
int m=floor(apple);
//printf("k: %d  m:%d \n",k,m);
for(int j=1;j<=km-1;j++)
{
for(int i=(m-(km-1));i<=m-j;i++)
{
d[k][i][j]=((u[k][i+km]-ux)/(u[k][i+km]-u[k][i+j]))*d[k][i][j-1]+((ux-u[k][i+j])/(u[k][i+km]-u[k][i+j]))*d[k][i+1][j-1];
e[k][i][j]=((u[k][i+km]-ux)/(u[k][i+km]-u[k][i+j]))*e[k][i][j-1]+((ux-u[k][i+j])/(u[k][i+km]-u[k][i+j]))*e[k][i+1][j-1];
//printf("%f \t %f \t",d[k][i][j],e[k][i][j]);
}
}
mas[k]=mas[k]+1;
		das[k][mas[k]]=d[k][m-(km-1)][km-1];
		eas[k][mas[k]]=e[k][m-(km-1)][km-1];
//printf("x %f k %d c  %d \n",das[k][ma[k]],k,mas[k]);
//printf("y %f k %d c  %d  \n",eas[k][ma[k]],k,mas[k]);
}

}
}
void replace()
{
printf("enter the polygon number where you want to add the point ");
scanf("%d",&pid);
int pos;
printf("enter the position you want to enter the point");
scanf("%d",&pos);
pos=pos-1;
float x,y;
printf("enter the value of x cooridinate");
scanf("%f",&x);
printf("enter the value of y coordinate");
scanf("%f",&y);
d[pid][pos][0]=x;
e[pid][pos][0]=y;
}
void insertpoint()
{
printf("enter the polygon number where you want to add the point ");
scanf("%d",&pid);
pid=pid-1;
int m=f[pid];
f[pid]=f[pid]+1;
printf("enter the position you want to enter the point");
int pos;
scanf("%d",&pos);
 for (int c = m - 1; c >= pos - 1; c--)
      {
	d[pid][c+1][0]=d[pid][c][0];
	e[pid][c+1][0]=e[pid][c][0];
      }
 float x,y;
printf("enter the value of x cooridinate");
scanf("%f",&x);
printf("enter the value of y coordinate");
scanf("%f",&y);
d[pid][pos-1][0]=x;
e[pid][pos-1][0]=y;
}
void deletepoint()
{
printf("enter the polygon number where you want to remove the point ");
scanf("%d",&pid);
pid=pid-1;
int m=f[pid];
f[pid]=f[pid]-1;
printf("enter the position you want to enter the point");
int pos;
scanf("%d",&pos);
if ( pos >= m+1 )
      printf("Deletion not possible.\n");
   else
   {
      for (int c = pos - 1 ; c < m - 1 ; c++ )
{
        d[pid][c][0]=d[pid][c+1][0];
	e[pid][c][0]=e[pid][c+1][0];
	  //array[c] = array[c+1];
 
 }    
   }
}

void outputfile()
{
	
	fprintf(fp1,"%d\n\n",n);
int j;
	for(int i=0;i<n;i++)
	{
	fprintf(fp1,"%d\n",f[i]);
	  for( j=0;j<f[i];j++)
	{
	    // printf("Enter the %d point x and y coordinate respectively\n",j+1);
	    GLfloat xcord,ycord;
	     fprintf(fp1,"%f \t",d[i][j][0]);
	     fprintf(fp1,"%f \n",e[i][j][0]);
	}
	}
	fclose(fp1);
}
/*void addpolygon()
{
n=n+1;
printf("enter number vertices");
int a;
scanf("%d",&a);
for(int j=0;j<a;j++)
{
float ab,ac;
printf("enter x%d coordinate",j);
scanf("%f",&ab);
d[n][j][0]=ab;
printf("enter y%d coordinate",j);
scanf("%f",&ac);
e[n][j][0]=ac;
}
}*/
void drawpoly()
{

for(int i=0;i<n;i++)
{
for(int j=0;j<ma[i];j++)
{
glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);
         glVertex2f(da[i][j],ea[i][j]);
         glVertex2f(da[i][j+1],ea[i][j+1]);
     glEnd();
//lineDDA(da[i][j],ea[i][j],da[i][j+1],ea[i][j+1]);

}
}
}
void drawpoly1a()
{
for(int i=0;i<n;i++)
lineDDA1(das[i][mas[i]],eas[i][mas[i]],das[i+1][0],eas[i+1][0]);
}


int main(int argc, char **argv)
{
    
    //the number of pixels in the grid
    grid_width = 100;
    grid_height = 100;
    
    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 5;
    
    /*Window information*/
    win_height = grid_height*pixel_size;
    win_width = grid_width*pixel_size;
    GLint nump;
	fscanf(fp,"%d",&nump);
	n=nump;
int j;
	for(int i=0;i<n;i++)
	{
	//printf("Enter the number of vertices in a %d polygon\n",i+1);
	GLint vertex;
	fscanf(fp,"%d",&vertex);
	f[i]=vertex;
	//scanf("%d",&v[i]);
	  for( j=0;j<f[i];j++)
	{
	    // printf("Enter the %d point x and y coordinate respectively\n",j+1);
	    GLfloat xcord,ycord;
	     fscanf(fp,"%f",&xcord);
	     fscanf(fp,"%f",&ycord);
	      d[i][j][0]=xcord;
	      e[i][j][0]=ycord;
	     printf("\n");
	}
	}
	fclose(fp);
	/*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/
   
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
	glutCreateWindow("glut demo");
    
	/*defined glut callback functions*/
	glutDisplayFunc(display); //rendering calls here
	glutReshapeFunc(reshape); //update GL on window size change
	glutMouseFunc(mouse);     //mouse button events
	glutMotionFunc(motion);   //mouse movement events
	glutKeyboardFunc(key);    //Keyboard events
	glutIdleFunc(idle);       //Function called while program is sitting "idle"
    
    //initialize opengl variables
    init();
    //start glut event loop
	glutMainLoop();
	return 0;
}
void init()
{
    //set clear color (Default background to white)
	glClearColor(1.0,1.0,1.0,1.0);
    //checks for OpenGL errors
	check();
}

//called repeatedly when glut isn't doing anything else
void idle()
{
    //redraw the scene over and over again
	glutPostRedisplay();	
}

//this is where we render the screen
void display()
{
    //clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();
	
     
       if(fa==0)
    {
	printf("enter the resolution for beizer\n");
        scanf("%f",&res);
	beizer();
        poly1();
	//drawpoly();
	
      for(int i=0;i<n;i++) 
      {
         for(int j=1;j<ma[i];j++)
         {
		glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);
         glVertex2f(da[i][j],ea[i][j]);
         glVertex2f(da[i][j+1],ea[i][j+1]);
     glEnd();
          //lineDDA(da[i][j],ea[i][j],da[i][j+1],ea[i][j+1]);
         }
       }

    }
    if(fa==1)
      { 
	printf("enter the resolution for b spline\n");
scanf("%f",&res);
	int gh;
      printf("Enter the order for the B-Spline curve you want\n");
      scanf("%d",&km);
	
	//drawpolydis();
	bspline();
	poly1();
	for(int i=0;i<n;i++)
{
for(int j=1;j<mas[i];j++)
{
glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);
         glVertex2f(das[i][j],eas[i][j]);
         glVertex2f(das[i][j+1],eas[i][j+1]);
     glEnd();
//lineDDA(das[i][j],eas[i][j],das[i][j+1],eas[i][j+1]);
}
}
 
	//drawpoly1();
//drawpoly1a();
	
     
    }
    //drawScene();
    //blits the current opengl framebuffer on the screen
   glutSwapBuffers();
 	
   printf("Enter 1 to add vertice to polygon\n");
   printf("Enter 2 to delete vertice from the polygon\n");
   printf("enter 3 to replace value at any point in a polygon\n");	
   printf("Enter 4 to display Bspline\n");
   printf("Enter 5 to display Beizer curve\n"); 
   printf("Enter -1 if you want to exit\n");
   int p;
   scanf("%d",&p);
   switch(p)
   {
   /*case 1:
     addpolygon();
    break;*/
   case 1:
    insertpoint();
   break;
   case 2:
    deletepoint();
   break;
   case 3:
     replace();
	break;	
   case 4:
   fa=1;
   break;
   case 5:
   fa=0;
   break;
   case -1:
   outputfile();
   exit(0);
    }
   
    //checks for opengl errors
	check();
}


//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(x+.01,y+.01,0);
    glEnd();
}
void draw_pix1(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(0,1.0,0);
    glVertex3f(x+.001,y+.001,0);
    glEnd();
}


/*Gets called when display size changes, including initial craetion of the display*/
void reshape(int width, int height)
{
	/*set up projection matrix to define the view port*/
    //update the ne window width and height
	win_width = width;
	win_height = height;
    
    //creates a rendering area across the window
	glViewport(0,0,width,height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0,grid_width,0,grid_height,-10,10);
    
    //clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //set pixel size based on width, if the aspect ratio
    //changes this hack won't work as well
    pixel_size = width/(float)grid_width;
    
    //set pixel size relative to the grid cell size
    glPointSize(pixel_size);
    //check for opengl errors
	check();
}

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
	switch(ch)
	{
		default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n",ch);
			break;
	}
    //redraw the scene after keyboard input
	glutPostRedisplay();
}


//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf ("MOUSE AT PIXEL: %d %d, GRID: %d %d\n",x,y,(int)(x/pixel_size),(int)(y/pixel_size));
	switch(button)
	{
		case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
		case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
		default:
            printf("UNKNOWN "); //any other mouse button
			break;
	}
    if(state !=GLUT_DOWN)  //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n");  //button clicked
    
    //redraw the scene after mouse click
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    //redraw the scene after mouse movement
	glutPostRedisplay();
}

//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		printf("GLERROR: There was an error %s\n",gluErrorString(err) );
		exit(1);
	}
}
