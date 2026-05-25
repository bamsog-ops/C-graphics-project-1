#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Camera parameters
float cameraAngleX = 30.0f;
float cameraAngleY = 45.0f;
float cameraDistance = 60.0f;

// Mouse control
int lastMouseX = 0, lastMouseY = 0;
bool mouseLeftDown = false;

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
// Function to draw a rectangular box (generic building block)
void drawBox(float x, float y, float z, float width, float height, float depth,
             float r=0.69, float g=0.67, float b=0.61) {
    glColor3f(r, g, b);

    glPushMatrix();
    glTranslatef(x, y, z);

    // Front face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, depth);
    glVertex3f(width, 0.0f, depth);
    glVertex3f(width, height, depth);
    glVertex3f(0.0f, height, depth);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(width, height, 0.0f);
    glVertex3f(width, 0.0f, 0.0f);
    glEnd();

    // Left face
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, depth);
    glVertex3f(0.0f, height, depth);
    glVertex3f(0.0f, height, 0.0f);
    glEnd();

    // Right face
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(width, 0.0f, 0.0f);
    glVertex3f(width, height, 0.0f);
    glVertex3f(width, height, depth);
    glVertex3f(width, 0.0f, depth);
    glEnd();

    // Top face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(0.0f, height, depth);
    glVertex3f(width, height, depth);
    glVertex3f(width, height, 0.0f);
    glEnd();

    // Bottom face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(width, 0.0f, 0.0f);
    glVertex3f(width, 0.0f, depth);
    glVertex3f(0.0f, 0.0f, depth);
    glEnd();

    glPopMatrix();
}

// Draw a window (dark with orange glow)
void drawWindow(float x, float y, float z, float width, float height, float depth, bool lit = false) {
    // Window frame (dark)
    glColor3f(0.15f, 0.15f, 0.15f);
    drawBox(x, y, z, width, height, depth, 0.15f, 0.15f, 0.15f);

    // Glass/interior (orange if lit, dark if not)
    if (lit) {
        glColor3f(0.8f, 0.5f, 0.2f);
        drawBox(x + 0.05f, y + 0.05f, z - 0.01f, width - 0.1f, height - 0.1f, depth + 0.02f,
                0.8f, 0.5f, 0.2f);
    } else {
        glColor3f(0.25f, 0.3f, 0.35f);
        drawBox(x + 0.05f, y + 0.05f, z - 0.01f, width - 0.1f, height - 0.1f, depth + 0.02f,
                0.25f, 0.3f, 0.35f);
    }
}

// Draw a column
void drawColumn(float x, float y, float z, float width, float height, float depth) {
    glColor3f(0.7f, 0.7f, 0.68f);
    drawBox(x, y, z, width, height, depth, 0.7f, 0.7f, 0.68f);
}

// Draw a balcony with railing
void drawBalcony(float x, float y, float z, float width, float depth, float height) {
    // Balcony floor slab
    glColor3f(0.7f, 0.68f, 0.62f);
    drawBox(x, y, z, width, 0.3f, depth, 0.7f, 0.68f, 0.62f);


    // Side railings
    drawBox(x + width - 0.1f, y + 0.3f, z, 0.1f, 1.0f, depth, 0.75f, 0.75f, 0.73f);
        drawBox(x, y + 0.3f, z, 0.1f, 1.0f, depth, 0.75f, 0.75f, 0.73f);

}
// Draw a landing platform
void drawLanding(float x, float y, float z, float width, float depth) {
    float landingHeight = 0.1f; // Thin platform
    glColor3f(0.65f, 0.63f, 0.58f);
    drawBox(x, y, z, width, landingHeight, depth, 0.65f, 0.63f, 0.58f);
}

// Draw a single flight of stairs
void drawStairFlight(float x, float y, float z, float width, int numSteps,
                     float stepHeight, float stepDepth, bool reverseDirection) {
    glColor3f(0.65f, 0.63f, 0.58f);

    for (int i = 0; i < numSteps; i++) {
        float currentY = y + i * stepHeight;
        float currentZ;

        if (reverseDirection) {
            // Stairs going in positive Z direction
            currentZ = z + i * stepDepth;
            drawBox(x, currentY, currentZ, width/2.0f, stepHeight, stepDepth,
                0.65f, 0.63f, 0.58f);
        } else {
            // Stairs going in negative Z direction
            currentZ = z - i * stepDepth;
            drawBox(x+width/2.0f, currentY, currentZ, width/2.0f, stepHeight, stepDepth,
                0.65f, 0.63f, 0.58f);
        }

    }
}

// Draw railings for a flight
void drawRailings(float x, float y, float z, float width, int numSteps,
                  float stepHeight, float stepDepth, bool reverseDirection) {
    glColor3f(0.7f, 0.7f, 0.68f);
    float railHeight = numSteps * stepHeight +0.0f;
    float railDepth = numSteps * stepDepth;

    if (reverseDirection) {
        // Railings for stairs going in positive Z direction
        drawBox(x - 0.1f, y, z, 0.1f, railHeight, railDepth + 0.5f,
                0.7f, 0.7f, 0.68f);
        drawBox(x + width, y, z, 0.1f, railHeight, railDepth + 0.5f,
                0.7f, 0.7f, 0.68f);
    } else {
        // Railings for stairs going in negative Z direction
        drawBox(x - 0.1f, y, z - railDepth, 0.1f, railHeight, railDepth + 0.5f,
                0.7f, 0.7f, 0.68f);
        drawBox(x + width, y, z - railDepth, 0.1f, railHeight, railDepth + 0.5f,
                0.7f, 0.7f, 0.68f);
    }
}



// Draw switchback staircase with 2 landings (3 if ground is counted)
void drawSwitchbackStaircase(float x, float y, float z, float width,
                             int numSteps, float totalHeight) {
    float stepHeight = totalHeight / numSteps;
    float stepDepth = 0.5f;
    float landingDepth = 2.0f; // Depth of landing platform

    // Divide steps into two flights with landings between
    int stepsPerFlight = numSteps / 2;

    // GROUND LEVEL (can be considered landing 1)

    // FIRST FLIGHT - going in negative Z direction
    float flight1Y = y;
    float flight1Z = z;
    drawStairFlight(x, flight1Y, flight1Z, width, stepsPerFlight,
                   stepHeight, stepDepth, false);
    drawRailings(x, flight1Y, flight1Z, width, stepsPerFlight,
                stepHeight, stepDepth, false);

    // FIRST LANDING (landing 2)
    float landing1Y = flight1Y + stepsPerFlight * stepHeight;
    float landing1Z = flight1Z - stepsPerFlight * stepDepth;
    drawLanding(x, landing1Y-0.1f, landing1Z - landingDepth, width, landingDepth);

    // SECOND FLIGHT - going in positive Z direction (switchback/reversal)
    float flight2Y = landing1Y;
    float flight2Z = landing1Z - landingDepth;
    drawStairFlight(x, flight2Y, flight2Z, width, stepsPerFlight,
                   stepHeight, stepDepth, true);
    drawRailings(x, flight2Y, flight2Z, width, stepsPerFlight,
                stepHeight, stepDepth, true);

    // SECOND LANDING / TOP FLOOR (landing 3)
    float landing2Y = flight2Y + stepsPerFlight * stepHeight;
    float landing2Z = flight2Z + stepsPerFlight * stepDepth;
    drawLanding(x, landing2Y, landing2Z, width, landingDepth);
}

// Draw one floor of apartments with balconies along the side (perpendicular to depth)
void drawSideApartments(float x, float y, float z, float depth, int numUnits, bool leftSide) {
    float unitDepth = depth / numUnits;

    for (int i = 0; i < numUnits; i++) {
        float unitZ = z + i * unitDepth;

        // Balcony extending into courtyard
        if (leftSide) {
            // Left side - balconies extend to the right (positive X)
            if (i!=1){
            drawBalcony(x, y, unitZ + 0.3f, 2.7f, unitDepth - 0.6f, 1.2f);
            }
            else {
            drawBalcony(x, y, unitZ + 3.3f,12.6f, unitDepth - 3.6f, 1.2f);
}
            if (i==4){
            drawBox(x+0.46, y+0.1, unitZ + 6.3f, 1.2, 1.5, 0.4f,0.69,0.67,0.61);
            }
            //else {
            //drawBox(x+1.3, y+0.1, unitZ + 6.3f, 1.0, 1.4, 0.4f,0.69,0.67,0.61);
            //}
            drawBox(0.1+x, y + 0.3f, z, 0.1f, 1.0f, depth, 0.3, 0.3, 0.4);
            drawBox(x+0.1, y+0.1, z, 2.0f, 0.3f, depth,0.69,0.67,0.61);


            // Window behind balcony
            bool isLit = ((i + (int)y) % 3 == 0);
            drawWindow(x +0.1f, y + 0.5f, unitZ + unitDepth * 0.3f,
                      0.15f, 2.0f, unitDepth * 0.4f, isLit);
        } else {
            // Right side - balconies extend to the left (negative X)
            if (i!=1){
            drawBalcony(x - 2.5f, y, unitZ + 0.3f, 2.5f, unitDepth - 0.6f, 1.2f);
            }
            if (i==4){
            drawBox(x-1.46, y+0.1, unitZ + 6.3f, 1.2, 1.5, 0.4f,0.69,0.67,0.61);
            }
            if (i==1){
            drawBox(x-8.3, y+0.3, unitZ + 6.3f,  5.0 , 1.4, 0.4f,0.69,0.67,0.61);
            }
            drawBox(x -0.2f, y + 0.3f, z, 0.1f, 1.0f, depth, 0.3, 0.3, 0.4);
            drawBox(x-2.0f, y+0.1, z, 2.0f, 0.3f, depth, 0.7f, 0.68f, 0.62f);

            // Window behind balcony
            bool isLit = ((i + (int)y) % 3 == 0);
            drawWindow(x - 0.25f, y + 0.5f, unitZ + unitDepth * 0.3f,
                      0.15f, 2.0f, unitDepth * 0.4f, isLit);
        }
    }
}
// Draw a rectangular prism roof (pitched/gabled roof)
void drawRooftop(float x, float y, float z, float width, float depth, float peakHeight) {
    // Roof color (darker reddish-brown for tiles)
    glColor3f(0.7f, 0.68f, 0.62f);

    // Calculate peak center position
    float centerX = x + width / 2.0f;

    // Left slope (triangle face at front)
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x, y, z);
    glVertex3f(x + width, y, z);
    glVertex3f(centerX, y + peakHeight, z);
    glEnd();

    // Right slope (triangle face at back)
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x, y, z + depth);
    glVertex3f(centerX, y + peakHeight, z + depth);
    glVertex3f(x + width, y, z + depth);
    glEnd();

    // Left sloped side
    glBegin(GL_QUADS);
    float nx = -peakHeight;
    float ny = width / 2.0f;
    float len = sqrt(nx*nx + ny*ny);
    nx /= len; ny /= len;
    glNormal3f(nx, ny, 0.0f);
    glVertex3f(x, y, z);
    glVertex3f(centerX, y + peakHeight, z);
    glVertex3f(centerX, y + peakHeight, z + depth);
    glVertex3f(x, y, z + depth);
    glEnd();

    // Right sloped side
    glBegin(GL_QUADS);
    nx = peakHeight;
    ny = width / 2.0f;
    len = sqrt(nx*nx + ny*ny);
    nx /= len; ny /= len;
    glNormal3f(nx, ny, 0.0f);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y, z + depth);
    glVertex3f(centerX, y + peakHeight, z + depth);
    glVertex3f(centerX, y + peakHeight, z);
    glEnd();
}

// Draw the complete courtyard building complex
void drawBuilding() {
    float buildingWidth = 20.0f;
    float buildingDepth = 40.0f;
    float wallThickness = 4.0f;
    float floorHeight = 4.0f;
    float numFloors = 3;
    float totalHeight = numFloors * floorHeight;
    float roofSteepness = 2.0f; // Height of the roof peak (adjustable)
    // Ground plane
    glColor3f(0.45f, 0.45f, 0.45f);
    drawBox(-30.0f, -0.1f, -30.0f, 60.0f, 0.1f, 60.0f, 0.45f, 0.45f, 0.45f);

    // Courtyard floor (tiled)
    glColor3f(0.6f, 0.6f, 0.58f);
    drawBox(-buildingWidth/2 + wallThickness, 0.0f, -buildingDepth/2 + wallThickness,
            buildingWidth - 2*wallThickness, 0.05f, buildingDepth - 2*wallThickness,
            0.6f, 0.6f, 0.58f);

    // Left wing
    glColor3f(0.78f, 0.76f, 0.7f);
    drawBox(-buildingWidth/2-5.0, 0.0f, -buildingDepth/2,
            wallThickness+5.0, totalHeight, buildingDepth-3.9f,
            0.78f, 0.76f, 0.7f);

    // Right wing
    drawBox(buildingWidth/2 - wallThickness, 0.0f, -buildingDepth/2,
            wallThickness+5, totalHeight, buildingDepth-3.9f,
            0.78f, 0.76f, 0.7f);

    // Back wall
    drawBox(-buildingWidth/2, 0.0f, -buildingDepth/2,
            buildingWidth, totalHeight, wallThickness,
            0.78f, 0.76f, 0.7f);

    // Apartment floors with balconies on side walls only
    for (int floor = 0; floor < numFloors; floor++) {
        float floorY = floor * floorHeight;

        // Left wing apartments (balconies extending into courtyard)
        drawSideApartments(-buildingWidth/2 + wallThickness, floorY,
                          -buildingDepth/2 + wallThickness,
                          buildingDepth - 2*wallThickness, 5, true);

        // Right wing apartments (balconies extending into courtyard)
        drawSideApartments(buildingWidth/2 - wallThickness, floorY,
                          -buildingDepth/2 + wallThickness,
                          buildingDepth - 2*wallThickness, 5, false);
    }
    // Add rooftop with adjustable steepness
    drawRooftop(-buildingWidth/2 - 5.0f, totalHeight, -buildingDepth/2,
                buildingWidth/2 +2.0 , -3.9f+buildingDepth, roofSteepness);
    drawRooftop(-buildingWidth/2 +18.0f- 5.0f, totalHeight, -buildingDepth/2,
                buildingWidth/2+2.0f, -3.9f+buildingDepth, roofSteepness);

    // Central staircase on back wall
for (int floor = 0; floor < numFloors; floor++) {
    float stairY = floor * floorHeight * 2/3;
    drawSwitchbackStaircase(-3.0f, stairY, -buildingDepth/2 + wallThickness + 11.0f,
                           6.0f, 8, floorHeight * 2/3);
}

    // Columns along corridors (vertical pillars)
    for (int floor = 0; floor < numFloors; floor++) {
        float colY = floor * floorHeight;


//x,-buildingWidth/2 + wallThickness + 0.5f + i * 3.0f
//y,-buildingDepth/2 + wallThickness + 0.5f
        // Left corridor columns
        for (int i = 0; i < 4; i++) {
            drawColumn(-buildingDepth/2 + wallThickness +11.6,
                      colY, -buildingWidth/2 + wallThickness + 3.0 + i * 6.2f,
                      0.5f, floorHeight, 0.5f);
        }

        // Right corridor columns
        for (int i = 0; i < 4; i++) {
            drawColumn(-buildingDepth/2 + wallThickness + 20.0f,
                      colY, buildingWidth/2 - wallThickness - 9.0f + i * 6.2f,
                      0.5f, floorHeight, 0.5f);
        }
    }


}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Position camera
    float camX = cameraDistance * sin(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);
    float camY = cameraDistance * sin(cameraAngleX * M_PI / 180.0f);
    float camZ = cameraDistance * cos(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);

    gluLookAt(camX, camY, camZ,  // Camera position
              0.0, 6.0, 0.0,      // Look at point (center of courtyard)
              0.0, 1.0, 0.0);     // Up vector

    // Draw building
    drawBuilding();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case 'r':
        case 'R':
            // Reset view
            cameraAngleX = 30.0f;
            cameraAngleY = 45.0f;
            cameraDistance = 60.0f;
            break;
        case '+':
        case '=':
            // Zoom in
            cameraDistance -= 2.0f;
            if (cameraDistance < 10.0f) cameraDistance = 10.0f;
            break;
        case '-':
        case '_':
            // Zoom out
            cameraDistance += 2.0f;
            if (cameraDistance > 150.0f) cameraDistance = 150.0f;
            break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngleY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleY += 5.0f;
            break;
        case GLUT_KEY_UP:
            cameraAngleX += 5.0f;
            if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
            break;
        case GLUT_KEY_DOWN:
            cameraAngleX -= 5.0f;
            if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mouseLeftDown = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (mouseLeftDown) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        cameraAngleY += deltaX * 0.5f;
        cameraAngleX += deltaY * 0.5f;

        // Clamp vertical angle
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void initGL() {
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // Sky blue background
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Set up light
    GLfloat lightAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPosition[] = {30.0f, 50.0f, 30.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Material properties
    GLfloat matSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat matShininess[] = {20.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // Enable smooth shading
    glEnable(GL_NORMALIZE);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Building Model - Courtyard Apartment Complex");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);


    std::cout << "Controls:\n";
    std::cout << "  Arrow Keys: Rotate camera\n";
    std::cout << "  Mouse Drag: Rotate view\n";
    std::cout << "  +/- : Zoom in/out\n";
    std::cout << "  R : Reset view\n";
    std::cout << "  ESC : Exit\n";

    glutMainLoop();
    return 0;
}
