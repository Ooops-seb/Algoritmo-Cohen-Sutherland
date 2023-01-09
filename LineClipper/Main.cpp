#include <windows.h>
#include <GL/glut.h>

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

int x_max = 10;
int y_max = 15;
int x_min = -10;
int y_min = -5;

int computeCode(double x, double y)
{
    int code = INSIDE;

    if (x < x_min)       // to the left of rectangle
        code |= LEFT;
    else if (x > x_max)  // to the right of rectangle
        code |= RIGHT;
    if (y < y_min)       // below the rectangle
        code |= BOTTOM;
    else if (y > y_max)  // above the rectangle
        code |= TOP;

    return code;
}

// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
void cohenSutherlandClip(double x1, double y1, double x2, double y2)
{
    // Compute region codes for P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Initialize line as outside the rectangular window
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else
        {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            double x, y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP)
            {
                // point is above the clip rectangle
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM)
            {
                // point is below the rectangle
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT)
            {
                // point is to the right of rectangle
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT)
            {
                // point is to the left of rectangle
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept)
    {
        double sx = (x_max - x_min) / (double)(x_max - x_min);
        double sy = (y_max - y_min) / (double)(y_max - y_min);

        double vx0 = x_min + (x1 - x_min) * sx;
        double vy0 = y_min + (y1 - y_min) * sy;
        double vx1 = x_min + (x2 - x_min) * sx;
        double vy1 = y_min + (y2 - y_min) * sy;

        // Draw the original line in blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();

        // Draw the clipped line in red
        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2f(vx0, vy0);
        glVertex2f(vx1, vy1);
        glEnd();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_min, y_min);
    glVertex2f(x_max, y_min);
    glVertex2f(x_max, y_max);
    glVertex2f(x_min, y_max);
    glEnd();

    glColor3f(211.0, 211.0, 211.0);
    glBegin(GL_LINES);
    glVertex2f(-20.0, 0.0);
    glVertex2f(20.0, 0.0);
    glVertex2f(0.0, -20.0);
    glVertex2f(0.0, 20.0);
    glEnd();

    // Test different lines with the clipping algorithm
    cohenSutherlandClip(-15, -5, 15, 5);
    cohenSutherlandClip(-15, 5, 15, -5);
    cohenSutherlandClip(-5, -15, 5, 15);
    cohenSutherlandClip(5, -15, -5, 15);

    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(-15, -5);
    glVertex2f(15, 5);
    glVertex2f(-15, 5);
    glVertex2f(15, -5);
    glVertex2f(-5, -15);
    glVertex2f(5, 15);
    glVertex2f(5, -15);
    glVertex2f(-5, 15);
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Cohen-Sutherland Line Clipping Algorithm");
    gluOrtho2D(-20, 20, -20, 20);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
