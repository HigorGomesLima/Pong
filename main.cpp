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
double x=0, y=0, bx=0.0,by=0.4,bz=0,s=0,jx=0,ax=0;
double fy=0;
int pontosP = 0,pontosA = 0;
double gravidade=-0.005, vx=0.001, vz = 0.0005, inercia = 0,ia=0.0008;

void colisao_bola()
{
    if(bz>=1.3 && bz<=1.5)
    {
        if(bx<=(jx+0.2) && bx>=(jx-0.2))
        {
            vz = vz*-1;
            vx += inercia/50000;
        }
    }
    else if(bz<=-1.3 && bz>=-1.5)
    {
        if(bx<=(ax+0.2) && bx>=(ax-0.2))
        {
            vz = vz*-1;
        }
    }
}

void tempoOcioso(void)
{
    s += 0.0001;
    colisao_bola();
    if(by<=0.025)
        fy = (fy*-1);
    if(bz<=-1.7)
    {
        bz = 0.5;
        vx=0.001;
        ia +=0.00005;
        pontosP++;
    }
    if(bz>=1.7)
    {
        bz = -0.5;
        vx=0.001;
        pontosA++;
    }
    if(bx<=-1.4 || bx >= 1.4)
        vx = vx*-1;
    fy += gravidade/10000;
    by += fy;
    bz += vz;
    bx += vx;
    if(bx>ax && ax<=1.3)
    {
        ax+= ia;
    }
    else if(bx<ax && ax>=-1.3)
    {
        ax-= ia;
    }
    if(inercia > 0)
    {
        inercia -= ia;
    }
    else if(inercia < 0)
    {
        inercia += ia;
    }
    glutPostRedisplay();
}

void output()
{
    char pp[10];
    char pa[10];
    itoa(pontosA,pa,10);
    itoa(pontosP,pp,10);
    char *p;
    glPushMatrix();
    glTranslatef(1400, 1200, 0);
    for (p = pa; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, 0);
    for (p = pp; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}

void fim(char *texto)
{
    char *p;
    glPushMatrix();
    glTranslatef(500, 500, 0);
    for (p = texto; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}

void visivel(int vis)
{
    if (vis == GLUT_VISIBLE)
        glutIdleFunc(tempoOcioso);
    else
        glutIdleFunc(NULL);
}

void specialKeys( int key, int xi, int yi )
{
    if (key == GLUT_KEY_RIGHT)
    {
        if(jx<= 1.3)
        {
            jx += inercia + 0.05;
            inercia += 0.02;
        }
        else
            inercia = 0;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        if(jx>= -1.3)
        {
            jx += inercia - 0.05;
            inercia -= 0.02;
        }
        else
            inercia = 0;
    }
    else
        inercia = 0;
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
}

void campo()
{
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
}

void bola()
{
    glTranslated(bx,by,bz);
    glutSolidSphere(0.05,20,20);
}

void player()
{
    glScalef(1.0,1.0,0.1);
    glTranslatef(jx, 0.0, 0.0);
    glutSolidCube(0.4);
}

void oponente()
{
    glScalef(1.0,1.0,0.1);
    glTranslatef(ax, 0.0, 0.0);
    glutSolidCube(0.4);
}

void display(void)
{
    GLfloat position[] = {0.0, 0.0, -3.0, 1.0};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0.0, 0.0, -5.0);
    glPushMatrix();
    glRotated(45.0, 1.0, 0.0, 0.0);
    glRotated(0.0, 1.0, 0.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glTranslated(0.0, 0.0, 1.5);
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    glutWireCube(0.001);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glRotatef(20,1.0,0.0,0.0);
    campo();
    bola();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotated(20, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -1.4, -3.0);
    player();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotated(20, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -1.4, -5.5);
    oponente();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3, 0.3, 0.0);
    glCallList(1);
    glPopMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1500, 0, 1500);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    if(pontosA >= 1)
        glColor3f(1,0,0);
    else if (pontosP >= 1)
        glColor3f(0,0,1);
    if(pontosA >= 2 || pontosP >= 2)
    {
        vx = 0;
        vz = 0;
        if(pontosA >= 2)
            fim("Voce perdeu!");
        else
            fim("Voce Ganhou!");
    }
    else
    {
        output();
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);

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
    glutReshapeFunc(estrutura);
    glutDisplayFunc(display);
    glutVisibilityFunc(visivel);
    glutMainLoop();
    return 0;
}
