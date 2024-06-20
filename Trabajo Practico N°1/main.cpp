#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#include <iostream>
#include <cmath>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#include <iostream>
#include <cmath>
#endif

#include <GL/freeglut.h> // Inclusion para reconocer los textos en pantalla

using namespace std;

    // Variables globales
int width = 800, height = 600; // Tamaño inicial de la ventana
int nPoints = 0; // Cantidad de puntos
int point[6]; // Los puntos (hasta 3: x0,y0,x1,y1,x2,y2)
int indexPoint =- 1; // Índice del punto a editar

int draggedPoint = -1; // Índice del punto que se está arrastrando

    // Caracteristicas del triángulo
float fillColor[3] = {0.5f, 0.5f, 0.5f}; // color del interior del triángulo

// Teclado---------------------------------------------------------------------

void Keyboard_cb(unsigned char key, int x, int y)
{
    // 1- Programar que la tecla <esc> (ASCII 27) borre toda la ventana.
	if(key == 27) // Tecla Esc (ASCII 27)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		nPoints = 0; // Reiniciamos la variable para redibujar
		glutSwapBuffers(); // Lo manda al monitor
		cout << "borra pantalla" << endl;
	}
	
	// Promocional: Agregar una nueva tecla para cambiar las caracterÃ­sticas del triangulo
	if (key == 99) // Tecla 'c' (ASCII 99)
	{
		// Cambiar el color del interior del triangulo
		// Utilizamos static_cast para convertir a float el valor int devuelto por rand()
		fillColor[0] = static_cast<float>(rand()) / RAND_MAX;
		fillColor[1] = static_cast<float>(rand()) / RAND_MAX;
		fillColor[2] = static_cast<float>(rand()) / RAND_MAX;
	
		glutPostRedisplay(); // Avisa que se debe redibujar
	}
	
	if (key == 127||key == 8) // DEL o Backspace ==> borra el punto ep (el editable)
	{
		if (indexPoint == -1||indexPoint == nPoints) return;
		// Corre los siguientes hacia atras
		for(int i=indexPoint; i < nPoints-1; i++) 
		{
			point[2 * i] = point[2 * i+2];
			point[2 * i+1] = point[2 * i+3];
		}
		nPoints--;
		
		glutPostRedisplay(); // Avisa que se debe redibujar
		
		// Backspace borra para atras, DEL borra para adelante
		if (key == 8) indexPoint--; // Índice pasa a ser el anterior
	}
}

void Special_cb(int key, int xm = 0, int ym = 0)
{
	if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT) // alt+f4 => exit
		exit(EXIT_SUCCESS);
}


void Display_cb() 
{
	glClear(GL_COLOR_BUFFER_BIT); // Limpia el buffer de color
	
	// Escribir texto en la pantalla
	glColor3f(1.0f, 1.0f, 1.0f); // Color del texto (blanco)
	// Mensaje 1
	glRasterPos2i(10, 580); // Posición del texto (x, y)
	static const char* message1 = "* Clic izquierdo (3 puntos distintos) para crear triangulo, luego arrastar si se desea";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message1);
	// Mensaje 2
	glRasterPos2i(10, 560); // Posición del texto (x, y)
	static const char* message2 = "* Tecla 'c' cambia color";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message2);
	// Mensaje 3
	glRasterPos2i(10, 540); // Posición del texto (x, y)
	static const char* message3 = "* Tecla 'Esc' para borrar";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message3);
	// Mensaje 4
	glRasterPos2i(10, 520); // Posición del texto (x, y)
	static const char* message4 = "* Tecla 'Backspace' borra un punto";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message4);
	// Mensaje 5
	glRasterPos2i(10, 500); // Posición del texto (x, y)
	static const char* message5 = "* Alt + F4 sale del programa";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message5);
	
	if (!nPoints)  // No hay nada
	{
		glutSwapBuffers(); 
		return; // Manda al monitor la pantalla vacía
	}
	
	// Dibuja los puntos conforme se van agregando
	glColor3f(1.f, 0.f, 0.f);
	glPointSize(5.0); // Tamaño del punto
	glBegin(GL_POINTS);
	for (int i = 0; i < nPoints; i++) 
	{
		glVertex2i(point[2 * i], point[2 * i + 1]);
	}
	glEnd();
	
	if (nPoints == 3) // Si ya están los tres puntos
	{ 
	// Dibuja el triángulo relleno
	glColor3fv(fillColor);
	glBegin(GL_TRIANGLES);
	glVertex2i(point[0], point[1]);
	glVertex2i(point[2], point[3]);
	glVertex2i(point[4], point[5]);
	glEnd();
		
	// Puntos (después del triángulo, para que se vean encima)
	glColor3f(1.f, 0.f, 0.f);
	glPointSize(5.0); // Punto grueso
	glBegin(GL_POINTS);
	for (int i = 0; i < nPoints; i++)
		glVertex2i(point[2 * i], point[2 * i + 1]);
	glEnd();
		
	// 3- Modificar el programa para que el triangulo tenga líneas de borde, utilizando algún patrón de punteado.
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0x6666); // Patrón de punteado
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(point[0], point[1]);
	glVertex2i(point[2], point[3]);
	glVertex2i(point[4], point[5]);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	}
		
	glutSwapBuffers();
}

bool IsNearVertex(int x, int y, int vertexX, int vertexY, int radius)
{
	// 4- Implementar la metrica del rombo para la detección del click cerca de un vertice del triangulo.
	int dx = abs(x - vertexX);
	int dy = abs(y - vertexY);
	return (dx + dy) <= (2 * radius);
}

// Mouse-----------------------------------------------------------------------

    // Seguimiento del cursor
void PassiveMotion_cb(int x, int y)
{
	y = height - y; // El 0 está arriba
	cout << x << "," << y << " \r" << flush;
}

void Motion_cb(int x, int y) 
{
	y = height - y; // El 0 está arriba
	cout << x << "," << y << "," << draggedPoint << endl;
	
	if (draggedPoint >= 0 && draggedPoint < nPoints) 
	{
		// Mantener el punto dentro de los límites de la ventana
		point[2 * draggedPoint] = max(0, min(x, glutGet(GLUT_WINDOW_WIDTH)));
		point[2 * draggedPoint + 1] = max(0, min(y, glutGet(GLUT_WINDOW_HEIGHT)));
		Display_cb(); // Redibuja
	}
}

void Mouse_cb(int button, int state, int x, int y) 
{
	cout << x << "," << y << "," << button << endl;
	if (button == GLUT_LEFT_BUTTON) // Botón izquierdo
	{ 
		if (state == GLUT_DOWN)  // Clickeado
		{
			y = height - y; // El 0 está arriba
			indexPoint = -1;
			draggedPoint = -1;
			
			// Detección de cercanía a los vertices del triangulo
			if (nPoints < 3 && (IsNearVertex(x, y, 100, 100, 20) || // Vértice inferior izquierdo
								IsNearVertex(x, y, 500, 100, 20) || // Vértice inferior derecho
								IsNearVertex(x, y, 300, 400, 20))) { // Vértice superior
				return;
			}
			
			// Verifica si picó a menos de 5 pixeles de algún punto previo
			for (int i = 0; i < nPoints; i++)
			{
				int dx = abs(x - point[2 * i]);
				int dy = abs(y - point[2 * i + 1]);
				if (max(dx, dy) > 5) continue; // Lejos
				indexPoint = i; // Edita el punto i
				draggedPoint = i; // Punto que se está arrastrando
				glutMotionFunc(Motion_cb); // Define el callback para los drags
				return;
			}
			
			// No picó cerca de otro
			if (nPoints == 3) return; // Ya hay 3 ==> no hace nada
			
			// Agrega un punto
			point[2 * nPoints] = x;
			point[2 * nPoints + 1] = y;
			nPoints++;
			Display_cb(); // Redibuja
			
			// Queda listo para editar el último punto hasta que suelte el botón izquierdo
			indexPoint = nPoints - 1; // Edita el último punto
			draggedPoint = nPoints - 1; // Punto que se está arrastrando
			glutMotionFunc(Motion_cb); // Define el callback para los drags
		} 
		else if (state == GLUT_UP) // Soltado
		{ 
			draggedPoint = -1; // Reinicia el punto que se está arrastrando
			glutMotionFunc(0); // Anula el callback para los drags
		}
	}
}
	
/* 5- En términos de eficiencia, ¿que aspectos cuestionaría respecto al callback de
teclado?
Al buscar eficiencia y no afectar el rendimiento en nuestro programa, 
se puede cuestionar el callback de teclado
si se necesita procesar una gran cantidad de entradas de teclado, 
lo cuál implica llamar a una función para cada evento de teclado.  
Una alternativa más eficiente sería procesar los eventos de teclado en un bucle principal del programa, 
en lugar de utilizar un callback específico.*/

void Reshape_cb (int w, int h) 
{
	width = w;
	height = h;
	if (w == 0||h == 0) return;
	glViewport(0,0,w,h);
	
	// Establece la proyección ortográfica
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void Initialize() 
{
	
	// Establece el modo de visualización
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	// Establece el tamaaño de la ventana
	glutInitWindowSize (width, height);
	// Establece la posición inicial de la ventana
	glutInitWindowPosition (50, 50);
	// Crea la ventana
	glutCreateWindow ("TRABAJO PRACTICO 1");
	// Establece la función de dibujo
	glutDisplayFunc (Display_cb);
	// Establece la función de redimensionamiento
	glutReshapeFunc (Reshape_cb);
	// Llamada a la función para no dibujar fuera de pantalla
	glutMotionFunc(Motion_cb);
	// Llamada a funciones del mouse
	glutMouseFunc(Mouse_cb);
	// LLamada a función para seguir la posición del mouse
	glutPassiveMotionFunc(PassiveMotion_cb);
	// Llamada a función para callbacks del teclado
	glutKeyboardFunc(Keyboard_cb);
	// Lamada a función para teclas especiales (no-ASCII)
	glutSpecialFunc(Special_cb);
	// Establecer el color de fondo
	glClearColor(0.8f,0.5f,0.3f,0.f);
}

int main (int argc, char **argv) 
{
	
	// Inicializa GLUT
	glutInit (&argc, argv);
	// Llama a la función de inicialización
	Initialize();
	// Entra en el bucle principal de GLUT
	glutMainLoop();
	
	return 0;
}
