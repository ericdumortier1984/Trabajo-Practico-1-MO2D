#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#include <iostream>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#include <iostream>
#endif

using namespace std;

// Variables globales
int width = 800, height = 600; // tamaño inicial de la ventana
int nPoints = 0; // cantidad de puntos
int point[6]; // los puntos (hasta 3: x0,y0,x1,y1,x2,y2)
int indexPoint =- 1; // índice del punto a editar

// 1- Programar que la tecla <esc> (ASCII 27) borre toda la ventana.
void Keyboard_cb(unsigned char key, int x, int y)
{
	
	if(key == 27) // Tecla Esc (ASCII 27)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // Lo manda al monitor
		cout << "borra pantalla" << endl;
	}
}

bool IsNearVertex(int x, int y, int vertexX, int vertexY, int radius)
{
	
	int dx = x - vertexX;
	int dy = y - vertexY;
	return (dx * dx + dy * dy) <= (radius * radius);
}

// Seguimiento del cursor
void PassiveMotion_cb(int x, int y)
{
	y = height - y; // el 0 está arriba
	cout << x << "," << y << "\r" << flush;
}
	
// Drag (movimiento con algun boton apretado)
void Motion_cb(int x, int y)
{
	y = height - y; // el 0 está arriba
	point[2 * indexPoint] = x;
	point[2 * indexPoint + 1] = y; // fija el punto editado en x,y
	glutPostRedisplay(); // avisa que se debe redibujar
}

void Mouse_cb(int button, int state, int x, int y)
{
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // Botón izquierdo y clickeado
	{
// 2- Programar para que no pueda sacarse un punto fuera de la ventana.
		cout << "click izquierdo" << endl;
		if(x < 0 || x > glutGet(GLUT_WINDOW_WIDTH) || y < 0 || y > glutGet(GLUT_WINDOW_HEIGHT))
			return;
		// Código para dibujar un punto en la posición (x, y)
		
// 4- Implementar la métrica del rombo para la detección del click cerca de un vértice
// del triángulo.
		if (nPoints < 3 
			&& (IsNearVertex(x, y, 100, 100, 20) || // Vértice inferior izquierdo
			IsNearVertex(x, y, 500, 100, 20) || // Vértice inferior derecho
			IsNearVertex(x, y, 300, 400, 20)))   // Vértice superior
		{
			// Código para manejar el click cerca de un vértice del triángulo
			return;
		}
			// Agregar el punto al vector de puntos
			point[2 * nPoints] = x;
			point[2 * nPoints + 1] = height - y; // el 0 está arriba
			nPoints++;
			
			if (nPoints == 3)
				glutPostRedisplay(); // Avisar que se debe redibujar
	}
}
// 3- Modificar el programa para que el triángulo tenga líneas de borde, utilizando algún
// patrón de punteado.
void display_cb() 
{
	
	// Limpia el buffer de color
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (!nPoints)  // No hay nada
	{
		glutSwapBuffers(); 
		return; // Manda al monitor la pantalla vacía
	}
	
	if (nPoints == 3) // Si ya están los tres puntos
	{
		
		// Dibujar el triángulo relleno
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_TRIANGLES);
		glVertex2i(point[0], point[1]);
		glVertex2i(point[2], point[3]);
		glVertex2i(point[4], point[5]);
		glEnd();
		
		// Dibujar el triángulo con línea de borde punteada
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(2, 0x6666); // Patrón de punteado
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2i(point[0], point[1]);
		glVertex2i(point[2], point[3]);
		glVertex2i(point[4], point[5]);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	
	// Intercambia los buffers (doble buffer)
	glutSwapBuffers();
	}
}

/* 5- En términos de eficiencia, ¿qué aspectos cuestionaría respecto al callback de
teclado?
Al buscar eficiencia y no afectar el rendimiento en nuestro programa, 
se puede cuestionar el callback de teclado
si se necesita procesar una gran cantidad de entradas de teclado, 
lo cuál implica llamar a una función para cada evento de teclado.  
Una alternativa más eficiente sería procesar los eventos de teclado en un bucle principal del programa, 
en lugar de utilizar un callback específico.*/

void reshape_cb (int w, int h) 
{
	if (w == 0||h == 0) return;
	glViewport(0,0,w,h);
	
	// Establece la proyección ortográfica
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void initialize() 
{
	
	// Establece el modo de visualización
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	// Establece el tamaño de la ventana
	glutInitWindowSize (800, 600);
	// Establece la posición inicial de la ventana
	glutInitWindowPosition (100, 100);
	// Crea la ventana
	glutCreateWindow ("TRABAJO PRACTICO 1");
	// Establece la función de dibujo
	glutDisplayFunc (display_cb);
	// Establece la función de redimensionamiento
	glutReshapeFunc (reshape_cb);
	// Llamada a la función para no dibujar fuera de pantalla
	glutMouseFunc(Mouse_cb);
	// Lamada a función para seguir la posición del mouse
	glutPassiveMotionFunc(PassiveMotion_cb);
	// Llamada a función para borrar la ventana con la tecla ESC
	glutKeyboardFunc(Keyboard_cb);
	// Establecer el color de fondo
	glClearColor(0.f,0.f,0.f,0.f);
}

int main (int argc, char **argv) 
{
	
	// Inicializa GLUT
	glutInit (&argc, argv);
	// Llama a la función de inicialización
	initialize();
	// Entra en el bucle principal de GLUT
	glutMainLoop();
	
	return 0;
}
