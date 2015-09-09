
#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include <iostream>
#include <OpenGl/gl.h>
#include <OpenGl/glu.h>
int v,nFPS=60;
float s=0.001;
float sealevel;
float polysize;
float x=0.0,y=0.0,z=0.0;
float up[3]={0,0,1};
float eye[3]={-1.8,0,0.6};
float wing[3]={0.0,0,0};
float head[3]={1.0,0,0};
float rot[3][3];
int yaw=0,roll=0;
double angle = 0.0,angle2=0.0,angle3=0.0,angle4=0.0;
int pitch=0;

void crossProduct(float v1[], float v2[], float vR[]) { //Computes cross product of v1 and v2

    vR[0] =   ( (v1[1] * v2[2]) - (v1[2] * v2[1]) );
    vR[1] = - ( (v1[0] * v2[2]) - (v1[2] * v2[0]) );
    vR[2] =   ( (v1[0] * v2[1]) - (v1[1] * v2[0]) );
}
void rotate(float about[],float vec[], float angle,float res[])     //Rotates vec about arbitrary with given angle
{ float p,q,r;
    p=vec[0];q=vec[1];r=vec[2];
    rot[0][0]=cos(angle)+about[0]*about[0]*(1-cos(angle));
    rot[0][1]=about[0]*about[1]*(1-cos(angle))-about[2]*sin(angle);
    rot[0][2]=about[2]*about[0]*(1-cos(angle))+about[1]*sin(angle);
    rot[1][0]=about[0]*about[1]*(1-cos(angle))+about[2]*sin(angle);
    rot[1][1]=cos(angle)+about[1]*about[1]*(1-cos(angle));
    rot[1][2]=about[1]*about[2]*(1-cos(angle))-about[0]*sin(angle);
    rot[2][0]=about[0]*about[2]*(1-cos(angle))-about[1]*sin(angle);
    rot[2][1]=about[2]*about[1]*(1-cos(angle))+about[0]*sin(angle);
    rot[2][2]=cos(angle)+about[2]*about[2]*(1-cos(angle));
    res[0]=rot[0][0]*p+rot[0][1]*q+rot[0][2]*r;
    res[1]=rot[1][0]*p+rot[1][1]*q+rot[1][2]*r;
    res[2]=rot[2][0]*p+rot[2][1]*q+rot[2][2]*r;

}

int seed(float x, float y) {
    static int a = 1588635695, b = 1117695901;
    int xi = *(int *)&x;
    int yi = *(int *)&y;
    return ((xi * a) % b) - ((yi * b) % a);
}
void normalize(float v1[], float vR[]) {            //Normalizes vector
    float fMag;
    float p,q,r;
    p=v1[0];
    q=v1[1];
    r=v1[2];
    fMag = sqrt( pow(v1[0], 2) +
                pow(v1[1], 2) +
                pow(v1[2], 2)
                );
    
    vR[0] = p / fMag;
    vR[1] = q / fMag;
    vR[2] = r/ fMag;
}
void mountain(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float s)
{
    float x01,y01,z01,x12,y12,z12,x20,y20,z20;
    
    if (s < polysize) {
        x01 = x1 - x0;
        y01 = y1 - y0;
        z01 = z1 - z0;
        
        x12 = x2 - x1;
        y12 = y2 - y1;
        z12 = z2 - z1;
        
        x20 = x0 - x2;
        y20 = y0 - y2;
        z20 = z0 - z2;
        
        float nx = y01*(-z20) - (-y20)*z01;
        float ny = z01*(-x20) - (-z20)*x01;
        float nz = x01*(-y20) - (-x20)*y01;
        
        float den = sqrt(nx*nx + ny*ny + nz*nz);
        
        if (den > 0.0) {
            nx /= den;
            ny /= den;
            nz /= den;
        }
        
        glNormal3f(nx,ny,nz);
        glBegin(GL_TRIANGLES);
        glVertex3f(x0,y0,z0);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
        glEnd();
        
        return;
    }
    
    x01 = 0.5*(x0 + x1);
    y01 = 0.5*(y0 + y1);
    z01 = 0.5*(z0 + z1);
    
    x12 = 0.5*(x1 + x2);
    y12 = 0.5*(y1 + y2);
    z12 = 0.5*(z1 + z2);
    
    x20 = 0.5*(x2 + x0);
    y20 = 0.5*(y2 + y0);
    z20 = 0.5*(z2 + z0);
    
    s *= 0.5;
    
    srand(seed(x01,y01));
    z01 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x12,y12));
    z12 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x20,y20));
    z20 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    
    mountain(x0,y0,z0,x01,y01,z01,x20,y20,z20,s);
    mountain(x1,y1,z1,x12,y12,z12,x01,y01,z01,s);
    mountain(x2,y2,z2,x20,y20,z20,x12,y12,z12,s);
    mountain(x01,y01,z01,x12,y12,z12,x20,y20,z20,s);

    
}

void init(void)
{
    GLfloat white[] = {1.0,1.0,1.0,1.0};
    GLfloat lpos[] = {0.0,1.0,0.0,0.0};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glClearColor (0.5, 0.5, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    
    sealevel = 0.0;
    polysize = 0.01;
}

void display(void)
{
    
    GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
    GLfloat tandiff[] = {0.4,0.3,0.2,1.0};
    
    GLfloat seaamb[] = {0.0,0.0,0.2,1.0};
    GLfloat seadiff[] = {0.0,0.0,0.8,1.0};
    GLfloat seaspec[] = {0.5,0.5,1.0,1.0};
    
    
    
   
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glLoadIdentity ();             /* clear the matrix */
    /* viewing transformation  */
    
    gluLookAt (eye[0], eye[1], eye[2],eye[0]+head[0],eye[1]+head[1],eye[2]+head[2],up[0], up[1], up[2]);
   
       /* modeling transformation */
    normalize(up,up);
    normalize(wing,wing);
    normalize(up,up);
    glTranslatef(x,y-0.6,z);
    
    //Define direction of translation
  
    if(head[0]>0)
        x-=s;
    if(head[0]<0)
        x+=s;
    if(head[1]>0)
        y-=s;
    if(head[1]<0)
        y+=s;
    if(head[2]>0)
        z-=s;
    if(head[2]<0)
        z+=s;
        
    crossProduct(head,up,wing);
    normalize(wing,wing);
  

    
    
    //Roll left
    if(roll==1)
    {rotate(head,up,-angle2,up);
        normalize(up,up);
        rotate(head,wing,-angle2,wing);
        normalize(wing,wing);
        crossProduct(up,wing,head);
        normalize(head,head);
        
    }
    
    //Roll right
    
    if(roll==2)
    {rotate(head,up,angle2,up);
        normalize(up,up);
        rotate(head,wing,angle2,wing);
        normalize(wing,wing);
        crossProduct(up,wing,head);
        normalize(head,head);
    }
    
    //Pitch up
    if(pitch==1)
    {
        rotate(wing,head,angle3,head);
        normalize(head,head);
 
        
        
        rotate(wing,up,angle3,up);
        normalize(up,up);
        
        
    }
    
    //Pitch down
    if(pitch==2)
    {
        rotate(wing,head,-angle4,head);
        normalize(head,head);
        rotate(wing,up,-angle4,up);
        normalize(up,up);
        
        
    }
    
    //Yaw right
    if(yaw==1){
        
        
        rotate(up,head,-angle,head);
        normalize(head,head);
        
        rotate(up,wing,-angle,wing);
      
        normalize(wing,wing);
        crossProduct(wing,head,up);
        normalize(up,up);
        
    }
    
    //Yaw left
    
    if(yaw==2){
        
        rotate(up,head,angle,head);
        normalize(head,head);
        rotate(up,wing,angle,wing);
        normalize(wing,wing);
        crossProduct(wing,head,up);
        normalize(up,up);
        
    }    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tandiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
    
    mountain(0.0,0.0,0.0, 1.0,0.0,0.0, 0.0,1.0,0.0, 1.0);
    mountain(1.0,1.0,0.0, 0.0,1.0,0.0, 1.0,0.0,0.0, 1.0);
    mountain(2.0,2.0,0.0, 0.0,1.0,0.0, 1.0,0.0,0.0, 1.0);
    mountain(2.0,0.0,0.0, 2,2,0.0, 0.0,2,0.0, 1.0);
    mountain(-2,2,0.0,-2,-1,0 , 0.0,-1,0.0, 1.0);
    mountain(2.0,0.0,0.0, 2,2,0.0, 0.0,1,0.0, 1.0);
    mountain( 2,2,0.0,0.0,2.0,0.0, 0.0,1,0.0, 1.0);
    mountain(1.0,2.0,0.0, 2,-2,0.0,  2.0,2,0.0, 1.0);
    

    
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
    
  
    
    
    
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(-2.0,-2.0,sealevel);
    glVertex3f(-2.0,2.0,sealevel);
    glVertex3f(2.0,2.0,sealevel);
    glVertex3f(2.0,-2.0,sealevel);
    glEnd();
    
    glutSwapBuffers();
    glFlush ();
   
    

    
}

void timer(int v)
{
  
    glutTimerFunc(1000/nFPS,timer,v);
      glutPostRedisplay();
}


void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(90.0,1.0,0.01,10.0);
    glMatrixMode (GL_MODELVIEW);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND); /* (which requires blend mode to be enabled*/
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3.0); /* Set the line width to 2 for thicker lines*/
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    /* And use the best available line hinting*/

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case '-':
            sealevel -= 0.01;
            break;
        case '+':
        case '=':
            sealevel += 0.01;
            break;
        case 'f':
            polysize *= 0.5;
            break;
        case 'c':
            polysize *= 2.0;
            break;
        case 27:
            exit(0);
            break;
        case 'r':               //Yaw Right
            angle+=0.01;
            yaw=1;
            break;               //Yaw Left
        case 'l':
            angle+=0.01;
            yaw=2;
            break;
                    
    }
}
void spkey(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:         //Roll left
            roll=1;
            angle2+=0.001;
            break;
        case GLUT_KEY_RIGHT:        //Roll right
            roll=2;
            angle2+=0.001;
            break;
   
        case GLUT_KEY_UP:           //Pitch up
            pitch=1;
            angle3+=0.001;
            break;
        case GLUT_KEY_DOWN:         //Pitch down
            pitch=2;
            angle4+=0.001;
            break;

    }
}
void release(unsigned char key,int x,int y)
{
    switch(key)
    {
        case 'r':
            yaw=0;
            angle=0;
            break;
        case 'l':
            yaw=0;
            angle=0;
            break;
      
    }
}
void sprel(int key,int x,int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            roll=0;
            angle2=0;
            break;
            
        case GLUT_KEY_RIGHT:
            roll=0;
            angle2=0;
            break;
        case GLUT_KEY_UP:
            pitch=0;
            angle3=0;
            break;
        case GLUT_KEY_DOWN:
            pitch=0;
            angle4=0;
            break;
    }
    
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(spkey);
    glutKeyboardUpFunc(release);
    glutSpecialUpFunc(sprel);
    glutTimerFunc(1000/60,timer,v);
    glutMainLoop();
    return 0;
}