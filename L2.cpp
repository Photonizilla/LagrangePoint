#include <GL/glut.h>
#include <cmath>

double V[4];
double E_0 = 0.5 * 9.0 * 1e4;
double E_1 = 0.5 * 3.0 * 1e4;
double T = 0.01;
double a = 1.0 / 10.0;

double Abs(double x) {
    return (x >= 0) ? x : -x;
}

struct Vector{
    double x;
    double y;
    double z;
};

struct Particle {
    Vector p;
    Vector q;
    Vector F;
};

GLdouble Red(double E) {
    double t = 0;
    if(E >= -E_0 / 2.0)
        t = M_PI * (E + E_0 / 2.0) / (E_1 + E_0 / 2.0);
    if(E < -E_0 / 2.0)
        t = M_PI * (E / E_0 + 0.5);
    return GLdouble(0.5 - 0.5 * cos(t));
}

GLdouble Green(double E) {
    double t = M_PI * E / E_0;
    return GLdouble(0.5 + 0.5 * cos(t));
}

GLdouble Blue(double E) {
    double t = 0;
    if(E < 0)
        t = M_PI * E / E_0;
    return GLdouble(0.5 - 0.5 * cos(t));
}

void trajectory() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 0.5 * 1e2, 10.0 * 1e2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0 * 1e2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glBegin(GL_POINTS);
    for(int i=1;i<=800;i++) {
        for(int j=1;j<=800;j++) {
            double x = double(i-400) / 4.0;
            double y = double(j-400) / 4.0;
            double E = 0.5 * V[1] * x * x + 0.5 * V[2] * y * y;
            glColor3f(Red(E), Green(E), Blue(E));
            glVertex2d(GLdouble(x), GLdouble(y));
        }
    }
    glEnd();

    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.0f);
    
    for(int k=0;k<=20;k++) {
        double h = 0.08 * double(k) * 1e4;
        glBegin(GL_LINE_STRIP);
        for(int i=1;i<=800;i++) {
            double x = double(i-400) / 4.0;
            double y = sqrt((h - 0.5 * V[1] * x * x) * 2.0 / V[2]);
            glVertex2d(GLdouble(x), GLdouble(y));
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for(int i=1;i<=800;i++) {
            double x = double(i-400) / 4.0;
            double y = sqrt((h - 0.5 * V[1] * x * x) * 2.0 / V[2]);
            glVertex2d(GLdouble(x), GLdouble(-y));
        }
        glEnd();
    }

    for(int k=1;k<=60;k++) {
        double h = -0.1 * double(k) * 1e4;
        glBegin(GL_LINE_STRIP);
        for(int i=1;i<=800;i++) {
            double y = double(i-400) / 4.0;
            double x = sqrt((h - 0.5 * V[2] * y * y) * 2.0 / V[1]);
            glVertex2d(GLdouble(x), GLdouble(y));
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for(int i=1;i<=800;i++) {
            double y = double(i-400) / 4.0;
            double x = sqrt((h - 0.5 * V[2] * y * y) * 2.0 / V[1]);
            glVertex2d(GLdouble(-x), GLdouble(y));
        }
        glEnd();
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    
    glLineWidth(4.0f);
    glColor3d(1.0f, 0.0f, 0.0f);
    Particle m;
    for(int k=0;k<=16;k++) {
        glBegin(GL_LINE_STRIP);
        int t = 300;
        m.p = {-100.0 * cos(M_PI / 4.0 + double(k) / 16.0 * M_PI / 6.0), 100.0 * sin(M_PI / 4.0 + double(k) / 16.0 * M_PI / 6.0), 0.0};
        m.q = {0.0,0.0,0.0};
        while(t--) {
            glVertex3d(GLdouble(m.q.x), GLdouble(m.q.y), GLdouble(100.0 - double(t) / 3.0));
            m.q.x += m.p.x * T;
            m.q.y += m.p.y * T;
            m.F.x = 2.0 * m.p.y - V[1] * m.q.x;
            m.F.y = -2.0 * m.p.x - V[2] * m.q.y;
            m.p.x += m.F.x * T;
            m.p.y += m.F.y * T;
        }
        glEnd();
    }
    

    glFlush();
}

double potential(double x, double y) {
    double v = 0.0;
    v -= 60.0 * 60.0 * 60.0 * 1.0 / (1.0 + a) / sqrt((x + 13.0) * (x + 13.0) + y * y);
    v -= 60.0 * 60.0 * 60.0 * a / (1.0 + a) / sqrt((x - 47.0) * (x - 47.0) + y * y);
    double cm = 47.0 * a / (1 + a) - 13.0 / (1 + a);
    v -= 0.5 * ((x - cm) * (x - cm) + y * y);
    return v;
}

void LagrangePoints() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 0.5 * 1e2, 10.0 * 1e2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0 * 1e2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    glBegin(GL_POINTS);
    for(int i=1;i<=800;i++) {
        for(int j=1;j<=800;j++) {
            double x = double(i-400) / 4.0;
            double y = double(j-400) / 4.0;
            double E = potential(x, y) * 7.0 + 44000.0;
            glColor3f(Red(E), Green(E), Blue(E));
            glVertex2d(GLdouble(x), GLdouble(y));
        }
    }
    glEnd();
    
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POINTS);
    for(int h = potential(100.0, 100.0) * 5.0 + 30000.0; h <= potential(17.0, 52.0) * 5.0 + 963.0 + 30000.0; h += 963.0) {
        for(int i=1;i<=800;i++) {
            for(int j=1;j<=800;j++) {
                double x = double(i-400) / 4.0;
                double y = double(j-400) / 4.0;
                double E = potential(x, y) * 5.0 + 30000.0;
                if(Abs(E - h) <= 100.0)
                    glVertex2d(GLdouble(x), GLdouble(y));
            }
        }
    }
    glEnd();
    
    glTranslated(-13.0, 0.0, 0.0);
    double E = potential(3.5, 0.0) * 7.0 + 44000.0;
    double b_0 = 11.0 * 11.0;
    for(double r=16.5; r>=5.5; r -= 0.1) {
        double b = (r - 5.5) * (r - 5.5);
        b /= b_0;
        glColor3f(Red(E) * b, Green(E) * b, Blue(E) * b);
        glutSolidSphere(r, 50, 50);
    }
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(7.5, 50, 50);

    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslated(60.0, 0.0, 0.0);
    glutSolidSphere(2.5, 30, 30);
    
    glColor3f(0.2f, 0.2f, 1.0f);
    
    glTranslated(-cbrt(a / 3.0) * 60.0, 0.0, 0.0);
    glRasterPos2f(-1.0, -1.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
    glRasterPos2d(3.0, -1.8);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '1');
    
    glTranslated(cbrt(a / 3.0) * 120.0, 0.0, 0.0);
    glRasterPos2f(-1.0, -1.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
    glRasterPos2d(3.0, -1.8);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '2');
    
    glTranslated(-cbrt(a / 3.0) * 60.0 - 120.0, 0.0, 0.0);
    glRasterPos2f(-1.0, -1.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
    glRasterPos2d(3.0, -1.8);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '3');
    
    glTranslated(90.0, 51.9615, 0.0);
    glRasterPos2f(-1.0, -1.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
    glRasterPos2d(3.0, -1.8);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '4');
    
    glTranslated(0.0, -103.9230, 0.0);
    glRasterPos2f(-1.0, -1.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
    glRasterPos2d(3.0, -1.8);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '5');

    glFlush();
}

int main(int argc, char** argv) {
    V[1] = -9.0;
    V[2] = 3.0;
    V[3] = 4.0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(400, 50);
    glutCreateWindow("Trajectory");
    glutDisplayFunc(&trajectory);
    glutInitWindowPosition(450, 100);
    glutCreateWindow("LagrangePoints");
    glutDisplayFunc(&LagrangePoints);
    glutMainLoop();
    return 0;
}

