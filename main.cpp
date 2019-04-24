#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>

static int girar = 0;
static int inicio;
static int girar2 = 0;
static int inicio2;
double x=0, y=0, bx=0.0,by=0.4,bz=0,s=0;
double fy=0;
double gravidade=-0.0005, vx=0.0001, vz = 0.0002;

void moverLuz(int button, int state, int x, int y)
{
if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
inicio = x;
inicio2 = y;
}

void tempoOcioso(void)
{
s += 0.0001;
//gravidade
if(by<=0.025)
    fy = (fy*-1);
if(bz<=-1.5 || bz >= 1.5)
    vz = vz*-1;
if(bx<=-1.5 || bx >= 1.5)
    vx = vx*-1;
fy += gravidade/10000;
by += fy;
bz += vz;
bx += vx;
//bx += 0.0001; //y
//by += 0.0001; //z
//bz += 0.0001; //x
glutPostRedisplay();
}

void visivel(int vis)
{
if (vis == GLUT_VISIBLE)
glutIdleFunc(tempoOcioso);
else
glutIdleFunc(NULL);
}

void movimento(int x, int y)
{
girar = (girar + (x - inicio)) % 360;
inicio = x;
girar2 = (girar2 + (y - inicio2)) % 360;
inicio2 = y;
glutPostRedisplay();
}

void specialKeys( int key, int xi, int yi ) {
if (key == GLUT_KEY_RIGHT) y += 5;
else if (key == GLUT_KEY_LEFT) y -= 5;
else if (key == GLUT_KEY_UP ) x += 5;
else if (key == GLUT_KEY_DOWN) x -= 5;
glutPostRedisplay();
}

void focoLuz(void)
{
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glDepthFunc(GL_LESS);
glEnable(GL_DEPTH_TEST);
}

void estrutura(int w, int h)
{
glViewport(0, 0, w, h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(40.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
glMatrixMode(GL_MODELVIEW);
}

void campo(){
    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex3f( -1.5, 0, 2 );
    glVertex3f( -1.5, 0, -2 );
    glVertex3f( 1.5, 0, -2 );
    glVertex3f( 1.5, 0, 2 );
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,0,0);
    glVertex3f( 1.5, 0, -2 );
    glVertex3f( -1.5, 0, -2 );
    glVertex3f( -1.5, 1, -2 );
    glVertex3f( 1.5, 1, -2 );
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex3f( -1.5, 0, 2 );
    glVertex3f( -1.5, 0, -2 );
    glVertex3f( -1.5, 1, -2 );
    glVertex3f( -1.5, 1, 2 );
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex3f( 1.5, 0, 2 );
    glVertex3f( 1.5, 0, -2 );
    glVertex3f( 1.5, 1, -2 );
    glVertex3f( 1.5, 1, 2 );
    glEnd();
/*
    glBegin(GL_LINE);
    glColor3f(1,1,1);
    glVertex3f( 1.5, 0, 2 );
    glVertex3f( -1.5, 0, 2 );
    glVertex3f( -1.5, 0.5, 2 );
    glVertex3f( 1.5, 0.5, 2 );
    glEnd();*/
}

void bola(){
    glTranslated(bx,by,bz);
    glutSolidSphere(0.05,20,20);
}

void player(){
    glScalef(1.0,1.0,1.0);
    glutSolidTetrahedron();
}

void display(void)
{
GLfloat position[] = {0.0, 0.0, 1.5, 1.0};
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glTranslatef(0.0, 0.0, -5.0);
glPushMatrix();
glRotated((GLdouble) girar, 0.0, 1.0, 0.0);
glRotated(0.0, 1.0, 0.0, 0.0);
glRotated((GLdouble) girar2, 1.0, 0.0, 0.0);
glRotated(0.0, 1.0, 0.0, 0.0);
glLightfv(GL_LIGHT0, GL_POSITION, position);
glTranslated(0.0, 0.0, 1.5);
glDisable(GL_LIGHTING);
glColor3f(1.0, 1.0, 1.0);
glutWireCube(0.2);
glEnable(GL_LIGHTING);
glPopMatrix();
glRotatef(20,1.0,0.0,0.0);
campo();
player();
bola();
glPopMatrix();
glutSwapBuffers();
}

int main(int argc, char **argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(500, 500);
glutCreateWindow(argv[0]);
focoLuz();
glutSpecialFunc(specialKeys);
glutMouseFunc(moverLuz);
glutMotionFunc(movimento);
glutReshapeFunc(estrutura);
glutDisplayFunc(display);
glutVisibilityFunc(visivel);
glutMainLoop();
return 0;
}
