#include <windows.h>
#include <GL/glut.h>

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

const int x_max = 10;
const int y_max = 8;
const int x_min = 4;
const int y_min = 4;

int computeCode(double x, double y)
{
    int code = INSIDE;

    if (x < x_min)       // a la izquiera del rectangulo
        code |= LEFT;
    else if (x > x_max)  // a la derecha del rectangulo
        code |= RIGHT;
    if (y < y_min)       // debajo del rectangulo
        code |= BOTTOM;
    else if (y > y_max)  // encima del rectangulo
        code |= TOP;

    return code;
}

// Implementación del algoritmo de Cohen-Sutherland
// Recorte de una línea de P1 = (x2, y2) a P2 = (x2, y2)
void cohenSutherlandClip(double x1, double y1, double x2, double y2)
{
    // Calcular los códigos de región para P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Inicializar la línea como fuera de la ventana rectangular
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // Si ambos extremos se encuentran dentro del rectángulo
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // Si ambos extremos están fuera del rectángulo,
            // en la misma región
            break;
        }
        else
        {
            // Algún segmento de línea se encuentra dentro del rectángulo
            int code_out;
            double x, y;

            // Al menos un extremo está fuera del rectángulo
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Encuentra el punto de intersección;
            // usando fórmulas y = y1 + pendiente * (x - x1),
            // x = x1 + (1 / pendiente) * (y - y1)
            if (code_out & TOP)
            {
                // esta por encima del rectangulo
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM)
            {
                // está por debajo del rectángulo
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT)
            {
                // el punto esta a la derecha del rectangulo
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT)
            {
                // el punto está a la izquierda del rectángulo
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            // Ahora se encuentra el punto de intersección x,y
            // Reemplazamos el punto fuera del rectángulo
            // por el punto de intersección
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

        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2f(vx0, vy0);
        glVertex2f(vx1, vy1);
        glEnd();
    }
}

void display()
{
    //dibujar rectangulo
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_min, y_min);
    glVertex2f(x_max, y_min);
    glVertex2f(x_max, y_max);
    glVertex2f(x_min, y_max);
    glEnd();

    //Dibujar ejes
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(-20.0, 0.0);
    glVertex2f(20.0, 0.0);
    glVertex2f(0.0, -20.0);
    glVertex2f(0.0, 20.0);
    glEnd();

    //
    cohenSutherlandClip(-20, -10, 20, 10);
    cohenSutherlandClip(-20, 10, 20, -10);
    cohenSutherlandClip(-10, -20, 10, 20);
    cohenSutherlandClip(10, -20, -10, 20);

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    //Obtiene el tamaño de la pantalla
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Obtener el tamaño de la ventana
    int windowSizeX = 500;
    int windowSizeY = 500;
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // Calcular la posición de la ventana para centrarla en la pantalla
    int windowPosX = (screenWidth - windowWidth - windowSizeX) / 2;
    int windowPosY = (screenHeight - windowHeight - windowSizeY) / 2;
    //Inicializar el modo de despliegue
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(windowSizeX, windowSizeY);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow("Algoritmo de recorte de linea de Cohen–Sutherland");
    gluOrtho2D(-20, 20, -20, 20);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}


