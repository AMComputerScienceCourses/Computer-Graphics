#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

// Cube position
float cubeX = 0.0f, cubeY = 0.25f, cubeZ = 0.0f;
bool gameOver = false;

// Lighting setup
void setupLighting()
{
    GLfloat light_position[] = {1.0f, 2.0f, 3.0f, 1.0f}; // Light source position
    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Ambient light
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};  // Diffuse light

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

// Initialize OpenGL settings
void init()
{
    glEnable(GL_DEPTH_TEST); // Enable depth buffering
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1, 0.1, 0.1, 1.0); // Dark gray background
    setupLighting();
}

// Function to display text
void drawText(const char *text, float x, float y)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10); // Set up a 2D orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0); // White text
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up the camera
    gluLookAt(0.0, 3.0, 6.0,  // Camera position (higher and further back)
              0.0, 0.0, 0.0,  // Look at the center
              0.0, 1.0, 0.0); // Up direction

    // Draw the ground plane (road)
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glColor3f(0.3f, 0.3f, 0.3f); // Gray road
    glBegin(GL_QUADS);
    glVertex3f(-3.0f, 0.0f, -2.0f); // Adjusted plane size (width: 6, height: 4)
    glVertex3f(3.0f, 0.0f, -2.0f);
    glVertex3f(3.0f, 0.0f, 2.0f);
    glVertex3f(-3.0f, 0.0f, 2.0f);
    glEnd();
    glPopMatrix();

    // Draw the cube
    glPushMatrix();
    glTranslatef(cubeX, cubeY, cubeZ);
    glColor3f(1.0f, 0.0f, 0.0f); // Red cube
    glutSolidCube(0.5);
    glPopMatrix();

    // Draw game over text
    if (gameOver)
    {
        drawText("Quit (Q)", -1.8f, -0.8f);  // Adjusted x-coordinate for spacing
        drawText("Continue (C)", 0.2f, -0.8f); // Adjusted x-coordinate for spacing
    }

    glutSwapBuffers();
}

// Handle cube movement
void specialKeys(int key, int x, int y)
{
    if (!gameOver)
    {
        switch (key)
        {
        case GLUT_KEY_LEFT:
            cubeX -= 0.2f;
            break;
        case GLUT_KEY_RIGHT:
            cubeX += 0.2f;
            break;
        case GLUT_KEY_UP:
            cubeZ -= 0.2f;
            break;
        case GLUT_KEY_DOWN:
            cubeZ += 0.2f;
            break;
        }
    }

    // Check if the cube falls off the plane
    if (cubeX > 2.5 || cubeX < -2.5 || cubeZ > 1.5 || cubeZ < -1.5)
    {
        gameOver = true;
    }
    glutPostRedisplay();
}

// Handle quitting and continuing
void keyboard(unsigned char key, int x, int y)
{
    if (gameOver)
    {
        if (key == 'q' || key == 'Q')
        {
            exit(0); // Quit
        }
        else if (key == 'c' || key == 'C')
        {
            cubeX = 0.0f; // Reset cube position
            cubeZ = 0.0f;
            gameOver = false;
        }
    }
    glutPostRedisplay();
}

// Reshape function to handle window resizing
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Cube on Road");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}