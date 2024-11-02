#include <GL/glut.h>
#include <cmath>

// Cube vertices
GLfloat vertices[][3] = {
    {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
    {-0.5, -0.5, 0.5},  {0.5, -0.5, 0.5},  {0.5, 0.5, 0.5},  {-0.5, 0.5, 0.5}
};

// Cube faces
GLint faces[][4] = {
    {0, 1, 2, 3}, {4, 5, 6, 7}, {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}
};

// Hole parameters
float hole_diameter = 0.8f;
float hole_depth = 0.6f;
float external_depth = 0.25f;
float hole_radius = hole_diameter / 2;
int hole_steps = 30;

void drawCube() {
    glColor3f(0.7, 0.7, 0.9); // Cube color
    for (int i = 0; i < 6; ++i) {
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; ++j) {
            glVertex3fv(vertices[faces[i][j]]);
        }
        glEnd();
    }
}

void drawCylinder(float x, float y, float z, float radius, float height) {
    float theta;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < hole_steps; ++i) {
        theta = 2.0f * M_PI * float(i) / float(hole_steps); 
        float x_offset = radius * cosf(theta);
        float y_offset = radius * sinf(theta);
        glVertex3f(x + x_offset, y + y_offset, z);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < hole_steps; ++i) {
        theta = 2.0f * M_PI * float(i) / float(hole_steps); 
        float x_offset = radius * cosf(theta);
        float y_offset = radius * sinf(theta);
        glVertex3f(x + x_offset, y + y_offset, z - height);
    }
    glEnd();

    glBegin(GL_LINES);
    for (int i = 0; i < hole_steps; ++i) {
        theta = 2.0f * M_PI * float(i) / float(hole_steps);
        float x_offset = radius * cosf(theta);
        float y_offset = radius * sinf(theta);
        glVertex3f(x + x_offset, y + y_offset, z);
        glVertex3f(x + x_offset, y + y_offset, z - height);
    }
    glEnd();
}

void drawHiddenHole() {
    glColor3f(0.0, 0.0, 0.0); // Set hidden lines color

    // Set polygon mode to line for hidden effect
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float x = 0.0f, y = 0.0f, z = 0.5f + external_depth; // Starting point of the hole
    drawCylinder(x, y, z, hole_radius, hole_depth);

    // Reset polygon mode to fill for the cube
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);
    glRotatef(25, 1.0, 1.0, 0.0);

    drawCube();        // Draw the cube
    drawHiddenHole();  // Draw hidden lines representing the external hole

    glFlush();
    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Background color
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Opaque Cube with External Hole and Hidden Lines");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
