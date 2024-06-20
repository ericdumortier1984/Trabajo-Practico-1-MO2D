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
int width = 800, height = 600; // Tama�o inicial de la ventana
int nPoints = 0; // Cantidad de puntos
int point[6]; // Los puntos (hasta 3: x0,y0,x1,y1,x2,y2)
int indexPoint =- 1; // �ndice del punto a editar

int draggedPoint = -1; // �ndice del punto que se est� arrastrando

    // Caracteristicas del tri�ngulo
float fillColor[3] = {0.5f, 0.5f, 0.5f}; // color del interior del tri�ngulo

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
	
	// Promocional: Agregar una nueva tecla para cambiar las características del triangulo
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
		if (key == 8) indexPoint--; // �ndice pasa a ser el anterior
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
	glRasterPos2i(10, 580); // Posici�n del texto (x, y)
	static const char* message1 = "* Clic izquierdo (3 puntos distintos) para crear triangulo, luego arrastar si se desea";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message1);
	// Mensaje 2
	glRasterPos2i(10, 560); // Posici�n del texto (x, y)
	static const char* message2 = "* Tecla 'c' cambia color";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message2);
	// Mensaje 3
	glRasterPos2i(10, 540); // Posici�n del texto (x, y)
	static const char* message3 = "* Tecla 'Esc' para borrar";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message3);
	// Mensaje 4
	glRasterPos2i(10, 520); // Posici�n del texto (x, y)
	static const char* message4 = "* Tecla 'Backspace' borra un punto";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message4);
	// Mensaje 5
	glRasterPos2i(10, 500); // Posici�n del texto (x, y)
	static const char* message5 = "* Alt + F4 sale del programa";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message5);
	
	if (!nPoints)  // No hay nada
	{
		glutSwapBuffers(); 
		return; // Manda al monitor la pantalla vac�a
	}
	
	// Dibuja los puntos conforme se van agregando
	glColor3f(1.f, 0.f, 0.f);
	glPointSize(5.0); // Tama�o del punto
	glBegin(GL_POINTS);
	for (int i = 0; i < nPoints; i++) 
	{
		glVertex2i(point[2 * i], point[2 * i + 1]);
	}
	glEnd();
	
	if (nPoints == 3) // Si ya est�n los tres puntos
	{ 
	// Dibuja el tri�ngulo relleno
	glColor3fv(fillColor);
	glBegin(GL_TRIANGLES);
	glVertex2i(point[0], point[1]);
	glVertex2i(point[2], point[3]);
	glVertex2i(point[4], point[5]);
	glEnd();
		
	// Puntos (despu�s del tri�ngulo, para que se vean encima)
	glColor3f(1.f, 0.f, 0.f);
	glPointSize(5.0); // Punto grueso
	glBegin(GL_POINTS);
	for (int i = 0; i < nPoints; i++)
		glVertex2i(point[2 * i], point[2 * i + 1]);
	glEnd();
		
	// 3- Modificar el programa para que el triangulo tenga l�neas de borde, utilizando alg�n patr�n de punteado.
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0x6666); // Patr�n de punteado
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
	// 4- Implementar la metrica del rombo para la detecci�n del click cerca de un vertice del triangulo.
	int dx = abs(x - vertexX);
	int dy = abs(y - vertexY);
	return (dx + dy) <= (2 * radius);
}

// Mouse-----------------------------------------------------------------------

    // Seguimiento del cursor
void PassiveMotion_cb(int x, int y)
{
	y = height - y; // El 0 est� arriba
	cout << x << "," << y << " \r" << flush;
}

void Motion_cb(int x, int y) 
{
	y = height - y; // El 0 est� arriba
	cout << x << "," << y << "," << draggedPoint << endl;
	
	if (draggedPoint >= 0 && draggedPoint < nPoints) 
	{
		// Mantener el punto dentro de los l�mites de la ventana
		point[2 * draggedPoint] = max(0, min(x, glutGet(GLUT_WINDOW_WIDTH)));
		point[2 * draggedPoint + 1] = max(0, min(y, glutGet(GLUT_WINDOW_HEIGHT)));
		Display_cb(); // Redibuja
	}
}

void Mouse_cb(int button, int state, int x, int y) 
{
	cout << x << "," << y << "," << button << endl;
	if (button == GLUT_LEFT_BUTTON) // Bot�n izquierdo
	{ 
		if (state == GLUT_DOWN)  // Clickeado
		{
			y = height - y; // El 0 est� arriba
			indexPoint = -1;
			draggedPoint = -1;
			
			// Detecci�n de cercan�a a los vertices del triangulo
			if (nPoints < 3 && (IsNearVertex(x, y, 100, 100, 20) || // V�rtice inferior izquierdo
								IsNearVertex(x, y, 500, 100, 20) || // V�rtice inferior derecho
								IsNearVertex(x, y, 300, 400, 20))) { // V�rtice superior
				return;
			}
			
			// Verifica si pic� a menos de 5 pixeles de alg�n punto previo
			for (int i = 0; i < nPoints; i++)
			{
				int dx = abs(x - point[2 * i]);
				int dy = abs(y - point[2 * i + 1]);
				if (max(dx, dy) > 5) continue; // Lejos
				indexPoint = i; // Edita el punto i
				draggedPoint = i; // Punto que se est� arrastrando
				glutMotionFunc(Motion_cb); // Define el callback para los drags
				return;
			}
			
			// No pic� cerca de otro
			if (nPoints == 3) return; // Ya hay 3 ==> no hace nada
			
			// Agrega un punto
			point[2 * nPoints] = x;
			point[2 * nPoints + 1] = y;
			nPoints++;
			Display_cb(); // Redibuja
			
			// Queda listo para editar el �ltimo punto hasta que suelte el bot�n izquierdo
			indexPoint = nPoints - 1; // Edita el �ltimo punto
			draggedPoint = nPoints - 1; // Punto que se est� arrastrando
			glutMotionFunc(Motion_cb); // Define el callback para los drags
		} 
		else if (state == GLUT_UP) // Soltado
		{ 
			draggedPoint = -1; // Reinicia el punto que se est� arrastrando
			glutMotionFunc(0); // Anula el callback para los drags
		}
	}
}
	
/* 5- En t�rminos de eficiencia, �que aspectos cuestionar�a respecto al callback de
teclado?
Al buscar eficiencia y no afectar el rendimiento en nuestro programa, 
se puede cuestionar el callback de teclado
si se necesita procesar una gran cantidad de entradas de teclado, 
lo cu�l implica llamar a una funci�n para cada evento de teclado.  
Una alternativa m�s eficiente ser�a procesar los eventos de teclado en un bucle principal del programa, 
en lugar de utilizar un callback espec�fico.*/

void Reshape_cb (int w, int h) 
{
	width = w;
	height = h;
	if (w == 0||h == 0) return;
	glViewport(0,0,w,h);
	
	// Establece la proyecci�n ortogr�fica
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void Initialize() 
{
	
	// Establece el modo de visualizaci�n
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	// Establece el tamaa�o de la ventana
	glutInitWindowSize (width, height);
	// Establece la posici�n inicial de la ventana
	glutInitWindowPosition (50, 50);
	// Crea la ventana
	glutCreateWindow ("TRABAJO PRACTICO 1");
	// Establece la funci�n de dibujo
	glutDisplayFunc (Display_cb);
	// Establece la funci�n de redimensionamiento
	glutReshapeFunc (Reshape_cb);
	// Llamada a la funci�n para no dibujar fuera de pantalla
	glutMotionFunc(Motion_cb);
	// Llamada a funciones del mouse
	glutMouseFunc(Mouse_cb);
	// LLamada a funci�n para seguir la posici�n del mouse
	glutPassiveMotionFunc(PassiveMotion_cb);
	// Llamada a funci�n para callbacks del teclado
	glutKeyboardFunc(Keyboard_cb);
	// Lamada a funci�n para teclas especiales (no-ASCII)
	glutSpecialFunc(Special_cb);
	// Establecer el color de fondo
	glClearColor(0.8f,0.5f,0.3f,0.f);
}

int main (int argc, char **argv) 
{
	
	// Inicializa GLUT
	glutInit (&argc, argv);
	// Llama a la funci�n de inicializaci�n
	Initialize();
	// Entra en el bucle principal de GLUT
	glutMainLoop();
	
	return 0;
}
