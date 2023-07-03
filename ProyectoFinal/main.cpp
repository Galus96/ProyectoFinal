#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "texturas/RgbImage.h"

using namespace std;

GLuint texturas[20];
GLUquadric* quad;

void dibujar();
void iniciarVentana(int ancho, int alto);
void inicializarLuces();
//movimiento del escenas y acciones teclado
void timer(int t);
void tecladoEspecial(int tecla, int x, int y);
void manejarTecla(unsigned char tecla, int x, int y);
void manejarTeclaSoltada(unsigned char tecla, int x, int y);
//escenario
void piso();
void pisoTextura();
void muros();
void dibujarEjes();
//Diseño vehiculo
void tanque();
void ruedas();
void cabina();
void cabinaTextura();
void lateralesTextura();
void costadoTextura();
void tambor();
void cañon();
void enemigo();
//Proyectil
void bala();
void estructuraBala();
//Avion
void avion();
void cabinaA();
void cabinaP();
void alas();
void ala();
void helices();
//Accesorio Escenario
void edificio();
void puertas();
void ventanasLaterales();
void fabrica();
void elementosFabrica();
void cilindroOxido();
void cilindroTextura();
void barril();
void torre();
void columna();
void caseta();
void casa();
void antena();
void roca();
void piedras();
void pilar();
void arboles();
//TERRENO
void limite();
//VEHICULO ANTIAEREO
void antiaereo();
void misiles();
void camion();
void llantas();
//LOGICA COLISION
bool detectarColision(float balaX, float balaY, float balaZ, float enemigoX, float enemigoY, float enemigoZ, float radioBala, float radioEnemigo);
//TEXTURAS
void loadTextureFromFile(const char* filename, int index);
void cargarImagenes();

//Medidas camara
//float camaraX = -20, camaraY = 39, camaraZ = 20;
//float angulo = 10, radio = 65, centroY = 0;

//float camaraX = -20, camaraY = 6.5, camaraZ = 20;
//float angulo = 3, radio = 45, centroY = -2;

float camaraX, camaraY = 3.6, camaraZ;
float angulo = 0, radio = 0, centroY = 3;

//float camaraX = -20, camaraY = 45, camaraZ = -20;
//float angulo = 7.8, radio = -46, centroY = 28;

const float M_PI = 3.1416;

float posicionZ = 0, deltaZ = 0, anguloRotacion = 0, direccionX = 0, direccionZ = 1;
float posicionX = 0, velocidadAvance = 0, rotacionCañon = 0;
float tanqueX = 1, tanqueZ = 1, posTanque = 0, posEnemigo = 0, velocidadTanque = 0.5, velocidadEnemigo = 0.3;
float posAvion = 200, posAvion2 = 210, posAvion3 = 210, avionZ = 1, velocidadAvion = 0.5;
float posTanque1 = 200, posTanque2 = 210, posTanque3 = 210;
bool girar = false, girarEnemigo = false;

bool moviendoArriba = false;
bool moviendoAbajo = false;
float velocidadVertical = 0.01;

bool moviendoDerecha = false;
bool moviendoIzquierda = false;

//Eventos Bala
float balaX = 0, balaY = 0, balaZ = 0;
float balaDireccionX = 0, balaDireccionZ = 0;
bool disparoActivo = false, colisionDetectadaTanque = false;
bool colisionDetectadaEnemigo = false, mostrarTanque = true, mostrarEnemigo = true;

int main(int argc, char* argv[]) {
	glutInit(&argc, argv); //iniciamos OpenGl

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //Uso para los canales de color
	glutInitWindowSize(1100, 700); //tamaño de la ventana
	glutInitWindowPosition(0, 0); //posicion inicial de la ventana
	glutCreateWindow("EVALUACION FINAL"); //creamos una ventana

	cargarImagenes();

	glutReshapeFunc(iniciarVentana);
	glutDisplayFunc(dibujar);//metodo para generar el dibujo
	glutSpecialFunc(tecladoEspecial);//habilitando el teclado

	glutKeyboardFunc(manejarTecla);
	glutKeyboardUpFunc(manejarTeclaSoltada);

	glutTimerFunc(0, timer, 0);//movimiento de camara
	glutMainLoop();
	return 0;
}

void inicializarLuces() {
	GLfloat luz_ambiente[] = { 0.4,0.4,0.4,1 };
	GLfloat luz_difusa[] = { 0.4,0.4,0.4,1 };
	GLfloat luz_especula[] = { 0.1,0.1,0.1,1 };
	float posicionLuz0[] = { 20,50,50,1 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especula);
	glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz0);
}

void dibujar() {
	inicializarLuces();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	glClearColor(0 / 255.0, 255 / 255.0, 0 / 255.0, 1); //color de fondo

	//Movimiento de la camara vertical sin interrumpir la rotacion
	/*if (moviendoIzquierda) {
		angulo += velocidadVertical;
	}
	else if (moviendoDerecha) {
		angulo -= velocidadVertical;
	}*/

	//Calculamos la posicion de la camara
	camaraX = posicionX - 5 * sin(anguloRotacion * M_PI / 180);
	camaraZ = posicionZ - 5 * cos(anguloRotacion * M_PI / 180);

	//Configuracion de la camara
	gluLookAt(camaraX, camaraY, camaraZ,	//Posicion de la camara
		posicionX, centroY, posicionZ,		//Posicion de enfoque(Posicion del vehiculo)
		0, 1, 0);							//Vector de orientacion
	
	
	//gluLookAt(radio * cos(angulo), camaraY, radio * sin(angulo), //posicion de la camara
	//	0, centroY, 0,										 //punto de mira
	//	0, 1, 0);											//vector de arriba

	piso();
	//pisoTextura();
	//dibujarEjes();

	//TORRE
	glPushMatrix();
		glTranslated(25, 0, 9);
		torre();
	glPopMatrix();

	//EDIFICIOS
	glPushMatrix();
		glTranslated(-3, 0, 15);
		edificio();

		glTranslated(7, 0, 0);
		//glRotated(90, 0, 1, 0);
		edificio();

		glTranslated(7, 0, -40);
		edificio();

		glTranslated(-7, 0, 0);
		edificio();
	glPopMatrix();

	//FABRICA
	if (disparoActivo) {
		if (detectarColision(balaX, balaY, balaZ, -100, 0, 100, 2, 20)) {
			//colisionDetectadaTanque = true;
			disparoActivo = false;
		}
	}

	glPushMatrix();
		glTranslated(-30, 0, 30);
		glRotated(-50, 0, 1, 0);
		glScaled(2, 2, 2);
		fabrica();
	glPopMatrix();

	//BARRILES
	glPushMatrix();
		glTranslated(26, 0, -2);
		barril();

		glTranslated(0, 0, -5);
		barril();
	glPopMatrix();

	//CASAS
	glPushMatrix();
		glTranslated(21, 0, 10);
		casa();

		glTranslated(5, 0, -5);
		glRotated(-90, 0, 1, 0);
		casa();

		glTranslated(-18, 0, 0);
		casa();
	glPopMatrix();

	//ROCAS
	glPushMatrix();
		glTranslated(0, 0, -20);
		roca();
	glPopMatrix();

	glPushMatrix();
		glTranslated(10, 0, 30);
		roca();
	glPopMatrix();

	glPushMatrix();
		glTranslated(30, 0, -40);
		glScaled(2, 1, 2);
	roca();

	glTranslated(0, 0, 10);
		glRotated(90, 0, 1, 0);
		glScaled(0.6, 1, 0.6);
		roca();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-30, 0, -40);
		glScaled(3, 1, 3);
		roca();

		glTranslated(-2, 0, 6);
		glRotated(90, 0, 1, 0);
		glScaled(0.5, 1, 0.5);
		roca();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-20, 0, 10);
		glScaled(0.6, 0.4, 0.6);
		roca();
	glPopMatrix();

	//PIEDRAS
	glPushMatrix();
		glTranslated(0, 0, -18);
		piedras();
	glPopMatrix();

	glPushMatrix();
		glTranslated(30, 0, -33);
		piedras();

		glTranslated(0, 0, 2);
		glRotated(60, 0, 1, 0);
		piedras();

		glScaled(2, 1, 2);
		piedras();
	glPopMatrix();

	glPushMatrix();
		glTranslated(30, 0, 30);
		piedras();

		glTranslated(0, 0, -1);
		piedras();

		glTranslated(-2, 0, 0);
		piedras();

		glTranslated(-15, 0, -1);
		glScaled(2, 1, 2);
		piedras();

		glTranslated(-1, 0, -0.5);
		piedras();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 30);
		piedras();

		glTranslated(0, 0, -1.5);
		glScaled(2, 2, 2);
		piedras();

		glTranslated(0, 0, 0);
		glRotated(50, 0, 1, 0);
		piedras();

		glTranslated(-1, 0, 0);
		piedras();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, -40);
		piedras();

		glTranslated(0, 0, 0.8);
		piedras();

		glTranslated(1, 0, 1);
		glRotated(60, 0, 1, 0);
		piedras();

		glTranslated(0, 0, 0);
		glScaled(1.5, 1, 1.5);
		piedras();
	glPopMatrix();

	//PILARES PIEDRA
	glPushMatrix();
		glTranslated(-40, 0, 0);
		glRotated(90, 0, 1, 0);
		pilar();
	glPopMatrix();

	//ARBOLES
	glPushMatrix();
		glTranslated(31, 0, -32);
		arboles();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, -19);
		arboles();

		glTranslated(2, 0, 1);
		glRotated(55, 0, 1, 0);
		arboles();
	glPopMatrix();

	glPushMatrix();
		glTranslated(30, 0, 30);
		arboles();

		glTranslated(-15, 0, -1);
		glRotated(30, 0, 1, 0);
		arboles();

		glTranslated(-1, 0, -2);
		arboles();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-20, 0, -36);
		glRotated(-90, 0, 1, 0);
		arboles();

		glTranslated(0.7, 0, 4);
		arboles();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-20, 0, 10);
		arboles();
	glPopMatrix();

	//LIMITES TERRENO
	glPushMatrix();
		glTranslated(50, 0, 0);
		glScaled(1, 1, 2);
		limite();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-50, 0, 0);
		glScaled(1, 1, 2);
		limite();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 50);
		glRotated(90, 0, 1, 0);
		glScaled(1, 1, 2);
		limite();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, -50);
		glRotated(90, 0, 1, 0);
		glScaled(1, 1, 2);
		limite();
	glPopMatrix();

	//VEHICULO ANTIAEREO
	glPushMatrix();
		glTranslated(25, 0, 20);
		glScaled(1.5, 1.5, 1.5);
		glRotated(-90, 0, 1, 0);
		antiaereo();
	glPopMatrix();

	//TANQUE
	glPushMatrix();
		
		glTranslated(posicionX, 0, posicionZ);
		glRotated(anguloRotacion, 0, 1, 0);
		glScaled(0.3, 0.3, 0.3);
		tanque();
	glPopMatrix();

	if (disparoActivo) {
		glPushMatrix();
			glTranslated(balaX, balaY, balaZ);
			glRotated(anguloRotacion, 0, 1, 0);
			glScaled(0.3, 0.3, 0.3);
			bala();
		glPopMatrix();
	}

	if (posTanque > 130) {
		tanqueX = +1;
		girar = false;
	}

	if (posTanque < -130) {
		tanqueX = -1;
		girar = true;
	}

	posTanque -= velocidadTanque * tanqueX;

	if (disparoActivo) {
		if (detectarColision(balaX, balaY, balaZ, posTanque, 0, 130, 2, 8)) {
			colisionDetectadaTanque = true;
			disparoActivo = false;
		}
	}

	if (!colisionDetectadaTanque) {
		glPushMatrix();
			glScaled(0.3, 0.3, 0.3);
			glTranslated(posTanque, 0, 130);
			//glRotated(-90, 0, 1, 0);
			if (girar) {
				glRotated(90, 0, 1, 0);
			}
			else {
				glRotated(-90, 0, 1, 0);
			}

			if (mostrarTanque) {
				tanque();
			}
		glPopMatrix();
	}
	else {
		mostrarTanque = false;
	}

	//movimiento enemigo
	if (posEnemigo > 40) {
		tanqueZ = +1;
		girarEnemigo = false;
	}

	if (posEnemigo < -35) {
		tanqueZ = -1;
		girarEnemigo = true;
	}

	posEnemigo -= velocidadEnemigo * tanqueZ;

	if (disparoActivo) {
		if (detectarColision(balaX, balaY, balaZ, 40, 0, posEnemigo, 2, 8)) {
			colisionDetectadaEnemigo = true;
			disparoActivo = false;
		}
	}

	if (!colisionDetectadaEnemigo) {
		glPushMatrix();
			glTranslated(-5, 0, posEnemigo);
			glScaled(0.3, 0.3, 0.3);

			if (girarEnemigo) {
				glRotated(0, 0, 1, 0);
			}
			else {
				glRotated(-180, 0, 1, 0);
			}

			if (mostrarEnemigo) {
				tanque();
			}
		glPopMatrix();
	}
	else {
		mostrarEnemigo = false;
	}

	//Inicio tanque
	//if (posTanque1) {
	//	tanqueZ = +1;
	//}

	//posTanque1 -= velocidadTanque * tanqueZ;

	//glPushMatrix();
	//	glTranslated(0, 0, posTanque1);
	//	glScaled(0.3, 0.3, 0.3);
	//	glRotated(180, 0, 1, 0);
	//	tanque();
	//glPopMatrix();

	//if (posTanque2) {
	//	tanqueZ = +1;
	//}

	//posTanque2 -= velocidadTanque * tanqueZ;

	//glPushMatrix();
	//	glTranslated(8, 0, posTanque2);
	//	glScaled(0.3, 0.3, 0.3);
	//	glRotated(180, 0, 1, 0);
	//	tanque();
	//glPopMatrix();

	//if (posTanque3) {
	//	tanqueZ = +1;
	//}

	//posTanque3 -= velocidadTanque * tanqueZ;

	//glPushMatrix();
	//	glTranslated(-8, 0, posTanque3);
	//	glScaled(0.3, 0.3, 0.3);
	//	glRotated(180, 0, 1, 0);
	//	tanque();
	//glPopMatrix();

	////movimiento avion
	//if (posAvion) {
	//	avionZ = +1;
	//}

	//posAvion -= velocidadAvion * avionZ;

	//glPushMatrix();
	//	glTranslated(0, 30, posAvion);
	//	avion();
	//glPopMatrix();

	//if (posAvion2) {
	//	avionZ = +1;
	//}

	//posAvion2 -= velocidadAvion * avionZ;

	//glPushMatrix();
	//	glTranslated(8, 30, posAvion2);
	//	avion();
	//glPopMatrix();

	//if (posAvion3) {
	//	avionZ = +1;
	//}

	//posAvion3 -= velocidadAvion * avionZ;

	//glPushMatrix();
	//	glTranslated(-8, 30, posAvion3);
	//	avion();
	//glPopMatrix();

	//posicionZ += deltaZ;
	posicionX += direccionX * deltaZ;
	posicionZ += direccionZ * deltaZ;

	glutSwapBuffers();
}

//funcion para ajustar la escena 3D al nuevo tamaño de la ventana
void iniciarVentana(int ancho, int alto) {
	glViewport(0, 0, ancho, alto); //ajusta el viewport
	glMatrixMode(GL_PROJECTION); //selecciona la matriz de proyeccion
	glLoadIdentity(); //carga la matriz identidad
	//gluPerspective(fovy,aspect,zNear,zFar);
	//fovy(angulo de apertura visual),aspect(relacion de aspecto entre el ancho y alto),
	//zNear(distancia mas cercana en la cual se puede ver el objeto), zFar(distancia mas lejana para ver el objeto)
	gluPerspective(60, (float)ancho / (float)alto, 1, 200); //ajusta la perspectiva
}

void piso() {
	glPushMatrix();
		glColor3ub(140, 103, 79);
		glTranslated(0, -0.8, 0); //-3
		glScaled(25, 0.4, 25);
		glutSolidCube(4);
	glPopMatrix();
}

void pisoTextura() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[0]);
	
	glPushMatrix();
		glBegin(GL_QUADS);
		//glColor3ub(0, 255, 0);
		glTexCoord2d(0, 60); glVertex3d(-50, 0.01, -50);
		glTexCoord2d(60, 60); glVertex3d(50, 0.01, -50);
		glTexCoord2d(60, 0); glVertex3d(50, 0.01, 50);
		glTexCoord2d(0, 0); glVertex3d(-50, 0.01, 50);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glFlush();
}

void muros() {
	glBegin(GL_QUADS);
		glColor3ub(162, 192, 203);
		glVertex3d(20, 0, -20);
		glVertex3d(20, 0, 20);

		glColor3ub(78, 130, 167);
		glVertex3d(20, 40, 20);
		glVertex3d(20, 40, -20);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3d(20, 0, -20);
		glVertex3d(20, 40, -20);
		glColor3ub(162, 192, 203);
		glVertex3d(-20, 40, -20);
		glVertex3d(-20, 0, -20);
	glEnd();
}

void tanque() {
	glPushMatrix();
		ruedas();
		cabina();
		tambor();
	glPopMatrix();
}

void ruedas() {
	//lado izquierdo
	glPushMatrix();
		glColor3ub(117, 119, 121);
		glRotated(90, 0, 1, 0);
		glTranslated(0, 1.5, 4);
		glScaled(5, 1, 3);
		glutSolidTorus(0.5, 1, 50, 50);
	glPopMatrix();

	//lado derecho
	glPushMatrix();
		glRotated(90, 0, 1, 0);
		glTranslated(0, 1.5, -4);
		glScaled(5, 1, 3);
		glutSolidTorus(0.5, 1, 50, 50);
	glPopMatrix();
}

void cabina() {
	glPushMatrix();
		glColor3ub(49, 110, 0);
		glTranslated(0, 2.5, 0);
		glScaled(2, 0.65, 2.5);
		glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 1.81, 0);
		cabinaTextura();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 0.01);
		lateralesTextura();

		glTranslated(0, 0, -10.02);
		lateralesTextura();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0.01, 0, 0);
		costadoTextura();

		glTranslated(-8.02, 0, 0);
		costadoTextura();
	glPopMatrix();

	glPushMatrix();
		glColor3ub(0, 0, 0);
		glTranslated(0, 3, 4.7);
		glScaled(4, 1, 1);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(0, 0, 0);
		glTranslated(-3, 3.5, -4);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.1, 0.1, 8, 50, 50);
	glPopMatrix();
}

void cabinaTextura() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);

	glPushMatrix();
		glBegin(GL_QUADS);
			glColor3ub(255, 255, 255);
			glTexCoord2d(0, 1); glVertex3d(-4, 2, -5);
			glTexCoord2d(1, 1); glVertex3d(4, 2, -5);
			glTexCoord2d(1, 0); glVertex3d(4, 2, 5);
			glTexCoord2d(0, 0); glVertex3d(-4, 2, 5);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void lateralesTextura() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);

	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3d(-4, 1.2, 5);
			glTexCoord2d(1, 1); glVertex3d(4, 1.2, 5);
			glTexCoord2d(1, 0); glVertex3d(4, 3.81, 5);
			glTexCoord2d(0, 0); glVertex3d(-4, 3.81, 5);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void costadoTextura() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);

	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3d(4, 1.2, -5);
			glTexCoord2d(1, 1); glVertex3d(4, 1.2, 5);
			glTexCoord2d(1, 0); glVertex3d(4, 3.81, 5);
			glTexCoord2d(0, 0); glVertex3d(4, 3.81, -5);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void tambor() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);

	glPushMatrix();
		glColor3ub(255, 255, 255);
		glTranslated(0, 3.5, 0);
		glRotated(-90, 1, 0, 0);
		quad = gluNewQuadric();
		gluQuadricTexture(quad, 2);
		gluCylinder(quad, 3.5, 3.5, 2.5, 50, 50);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	/*glPushMatrix();
		glColor3ub(49, 110, 0);
		glTranslated(0, 3.5, 0);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 3.5, 3.5, 2.5, 50, 50);
	glPopMatrix();*/

	glPushMatrix();
		glColor3ub(62, 58, 46);
		glTranslated(0, 6, 0);
		glRotated(-90, 1, 0, 0);
		gluDisk(gluNewQuadric(), 0, 3.5, 50, 50);
	glPopMatrix();

	/*glPushMatrix();
		glColor3ub(32, 73, 0);
		glTranslated(0, 6, 0);
		glScaled(2, 1, 2);
		glutSolidSphere(1, 50, 50);
	glPopMatrix();*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);

	glPushMatrix();
		//glColor3ub(32, 73, 0);
		glTranslated(0, 6, 0);
		glScaled(2, 1, 2);
		quad = gluNewQuadric();
		gluQuadricTexture(quad,1);
		gluSphere(quad, 1, 50, 50);
	glPopMatrix();

	glPushMatrix();
		cañon();
	glPopMatrix();
}

void cañon() {
	/*glPushMatrix();
		glColor3ub(49, 110, 0);
		glTranslated(0, 5, 0);
		gluCylinder(gluNewQuadric(), 0.8, 0.8, 10, 50, 50);
	glPopMatrix();*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);

	glPushMatrix();
		glColor3ub(255, 255, 255);
		glTranslated(0, 5, 0);
		quad = gluNewQuadric();
		gluQuadricTexture(quad, 2);
		gluCylinder(quad, 0.8, 0.8, 10, 50, 50);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void edificio() {
	glPushMatrix();
		glColor3ub(226, 160, 105);
		glTranslated(10, 1.2, 0);
		glScaled(2.5, 1.2, 6);
		glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
		//glColor3ub(228, 202, 175);
		glTranslated(10, 2.4, -6);
		glScaled(2.5, 0.8, 2);
		gluCylinder(gluNewQuadric(), 1, 1, 6, 50, 50);
	glPopMatrix();

	glPushMatrix();
		//glColor3ub(255, 0, 0);
		glTranslated(10, 2.4, -6);
		glScaled(2.5, 0.8, 0);
		gluPartialDisk(gluNewQuadric(), 0, 1, 50, 50, -90, 180);
	glPopMatrix();

	glPushMatrix();
		glTranslated(10, 2.4, 6);
		glScaled(2.5, 0.8, 0);
		gluPartialDisk(gluNewQuadric(), 0, 1, 50, 50, -90, 180);
	glPopMatrix();

	//Ventanas
	puertas();
	glPushMatrix();
		glTranslated(0, 0, -12.02);
		puertas();
	glPopMatrix();

	//Ventanas
	ventanasLaterales();
	glPushMatrix();
		glTranslated(5.02, 0, 0);
		ventanasLaterales();
	glPopMatrix();

	//barriles
	cilindroOxido();
	glPushMatrix();
		glTranslated(0, 0, 0.6);
		cilindroOxido();
		//cilindroTextura();

		glTranslated(-0.8, 0, 0);
		cilindroOxido();
	glPopMatrix();
}

void puertas() {
	//Puerta y ventanas frentes
	glBegin(GL_QUADS);
		//glColor3ub(197, 143, 98);
		glColor3ub(182, 130, 87);
		glVertex3d(9.5, 0, 6.01);
		glVertex3d(9.5, 2, 6.01);
		glVertex3d(10.5, 2, 6.01);
		glVertex3d(10.5, 0, 6.01);

		glVertex3d(8, 1, 6.01);
		glVertex3d(8, 2, 6.01);
		glVertex3d(9, 2, 6.01);
		glVertex3d(9, 1, 6.01);

		glVertex3d(11, 1, 6.01);
		glVertex3d(11, 2, 6.01);
		glVertex3d(12, 2, 6.01);
		glVertex3d(12, 1, 6.01);
	glEnd();
}

void ventanasLaterales() {
	//Costados 
	glBegin(GL_QUADS);
		glVertex3d(7.49, 1, 1);
		glVertex3d(7.49, 2, 1);
		glVertex3d(7.49, 2, 5);
		glVertex3d(7.49, 1, 5);

		glVertex3d(7.49, 1, -1);
		glVertex3d(7.49, 2, -1);
		glVertex3d(7.49, 2, -5);
		glVertex3d(7.49, 1, -5);
	glEnd();
}

void fabrica() {
	glPushMatrix();
		elementosFabrica();
	glPopMatrix();
}

void elementosFabrica() {
	glPushMatrix();
		glColor3ub(137, 130, 155);
		glTranslated(0, 1.5, 0);
		glScaled(2.5, 3, 5);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-2.15, 1, 1);
		glScaled(1.8, 2, 3);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(176, 171, 188);
		glTranslated(-2.1, 0, -1.4);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.8, 0.8, 1.5, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-2.1, 1.5, -1.4);
		glRotated(-90, 1, 0, 0);
		gluDisk(gluNewQuadric(), 0, 0.8, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(176, 171, 188);
		glTranslated(-2.1, 1.5, -1.4);
		gluSphere(gluNewQuadric(), 0.6, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(200, 255, 255);
		glTranslated(-1.3, 1.8, -1.4);
		glutSolidTorus(0.2, 0.8, 50, 50);

		glTranslated(0, 0.3, 1.2);
		glutSolidTorus(0.1, 0.4, 50, 50);

		glTranslated(0, 0, 0.3);
		glutSolidTorus(0.1, 0.4, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 3, 1.5);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.6, 0.3, 4, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(220, 220, 255);
		glTranslated(0, 5, 1.5);
		glRotated(-90, 1, 0, 0);
		glutSolidTorus(0.1, 0.4, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			glColor3ub(176, 171, 188);
			glVertex3d(1.1, 2.5, 2.51);
			glVertex3d(1.1, 2.7, 2.51);
			glVertex3d(-1.1, 2.7, 2.51);
			glVertex3d(-1.1, 2.5, 2.51);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(1.3, 2.5, 2.3);
			glVertex3d(1.3, 2.7, 2.3);
			glVertex3d(1.3, 2.7, -2.3);
			glVertex3d(1.3, 2.5, -2.3);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(1, 0, -2.51);
			glVertex3d(1, 2, -2.51);
			glVertex3d(-1, 2, -2.51);
			glVertex3d(-1, 0, -2.51);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(-1.8, 0, 2.51);
			glVertex3d(-1.8, 1.2, 2.51);
			glVertex3d(-2.8, 1.2, 2.51);
			glVertex3d(-2.8, 0, 2.51);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(-2.6, 1.4, 2.51);
			glVertex3d(-2.6, 1.8, 2.51);
			glVertex3d(-3, 1.8, 2.51);
			glVertex3d(-3, 1.4, 2.51);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(-1.9, 1.4, 2.51);
			glVertex3d(-1.9, 1.8, 2.51);
			glVertex3d(-2.4, 1.8, 2.51);
			glVertex3d(-2.4, 1.4, 2.51);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(-1.3, 1.4, 2.51);
			glVertex3d(-1.3, 1.8, 2.51);
			glVertex3d(-1.7, 1.8, 2.51);
			glVertex3d(-1.7, 1.4, 2.51);
		glEnd();
	glPopMatrix();
}

void cilindroOxido() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[1]);

	glPushMatrix();
		glTranslated(7.2, 0, 0);
		glRotated(-90, 1, 0, 0);
		quad = gluNewQuadric();
		gluQuadricTexture(quad, 2);
		gluCylinder(quad, 0.3, 0.3, 1, 50, 50);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	/*glPushMatrix();
		glColor3ub(184, 93, 11);
		glTranslated(7.2, 0, 0);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.3, 0.3, 1, 50, 50);
	glPopMatrix();*/

	glPushMatrix();
		//glColor3ub(255, 0, 0);
		glTranslated(7.2, 1, 0);
		glRotated(-90, 1, 0, 0);
		gluDisk(gluNewQuadric(), 0, 0.3, 50, 50);
	glPopMatrix();
}

void cilindroTextura() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[1]);

	glPushMatrix();
		glTranslated(7.2, 0, 0);
		glRotated(-90, 1, 0, 0);
		quad = gluNewQuadric();
		gluQuadricTexture(quad, 2);
		gluCylinder(quad, 0.3, 0.3, 1, 50, 50);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void barril() {
	glPushMatrix();
		glColor3ub(151, 136, 128);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 2, 2, 3, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 3, 0);
		glRotated(-90, 1, 0, 0);
		gluDisk(gluNewQuadric(), 0, 2, 50, 50);
	glPopMatrix();
}

void torre() {
	glPushMatrix();
		glTranslated(0, 0, 0);
		caseta();

		glTranslated(0, 0, 0);
		columna();

		glTranslated(0, 0, 1);
		columna();
	glPopMatrix();
}

void columna() {
	glPushMatrix();
		glColor3ub(133, 65, 25);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.1, 0.1, 6, 50, 50);
	glPopMatrix();

	glPushMatrix();
		//glColor3ub(255, 255, 0);
		glTranslated(1, 0, 0);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.1, 0.1, 6, 50, 50);
	glPopMatrix();
}

void caseta() {
	//Plataforma
	glPushMatrix();
		glColor3ub(173, 128, 101);
		glTranslated(0.5, 6, 0.5);
		glScaled(2, 0.1, 2);
		glutSolidCube(1);
	glPopMatrix();

	//Caseta
	glPushMatrix();
		glColor3ub(133, 65, 25);
		glTranslated(0.5, 6.5, 0.5);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(173, 128, 101);
		glTranslated(0.5, 7, 0.5);
		glRotated(-90, 0.5, 0, 0);
		glutSolidCone(1, 0.5, 8, 8);
	glPopMatrix();
}

void casa() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3d(-2.25, 3.01, -1.5);
			glTexCoord2d(1, 1); glVertex3d(2.25, 3.01, -1.5);
			glTexCoord2d(1, 0); glVertex3d(2.25, 3.01, 1.5);
			glTexCoord2d(0, 0); glVertex3d(-2.25, 3.01, 1.5);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3d(-2.25, 3.01, 1.51);
			glTexCoord2d(1, 1); glVertex3d(2.25, 3.01, 1.51);
			glTexCoord2d(1, 0); glVertex3d(2.25, 0, 1.51);
			glTexCoord2d(0, 0); glVertex3d(-2.25, 0, 1.51);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3d(-2.25, 3.01, -1.51);
			glTexCoord2d(1, 1); glVertex3d(2.25, 3.01, -1.51);
			glTexCoord2d(1, 0); glVertex3d(2.25, 0, -1.51);
			glTexCoord2d(0, 0); glVertex3d(-2.25, 0, -1.51);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3d(2.26, 3.01, 1.5);
			glTexCoord2d(1, 1); glVertex3d(2.26, 3.01, -1.5);
			glTexCoord2d(1, 0); glVertex3d(2.26, 0, -1.5);
			glTexCoord2d(0, 0); glVertex3d(2.26, 0, 1.5);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3d(-2.26, 3.01, 1.5);
			glTexCoord2d(1, 1); glVertex3d(-2.26, 3.01, -1.5);
			glTexCoord2d(1, 0); glVertex3d(-2.26, 0, -1.5);
			glTexCoord2d(0, 0); glVertex3d(-2.26, 0, 1.5);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glFlush();

	glPushMatrix();
		glColor3ub(155, 120, 79);
		glTranslated(0, 1.5, 0);
		glScaled(1.5, 1, 1);
		glutSolidCube(3);
	glPopMatrix();

	glPushMatrix();
		//glColor3ub(222, 122, 122);
		glTranslated(0, 2, -2);
		glScaled(3, 0.2, 1);
		glutSolidCube(1);
	glPopMatrix();

	//Escalera
	glPushMatrix();
		glBegin(GL_TRIANGLES);
			//glColor3ub(200, 255, 120);
			glVertex3d(-3, 0, -1.2);
			glVertex3d(-3, 3, -1.2);
			glVertex3d(-3, 0, 1.5);
		glEnd();

		glBegin(GL_TRIANGLES);
			glVertex3d(-2.26, 0, -1.2);
			glVertex3d(-2.26, 3, -1.2);
			glVertex3d(-2.26, 0, 1.5);
		glEnd();

		glBegin(GL_QUADS);
			//glColor3ub(200, 220, 180);
			glVertex3d(-2.26, 3, -1.2);
			glVertex3d(-2.26, 0, 1.5);
			glVertex3d(-3, 0, 1.5);
			glVertex3d(-3, 3, -1.2);
		glEnd();

		glBegin(GL_QUADS);
			//glColor3ub(200, 220, 180);
			glVertex3d(-2.26, 3, -1.2);
			glVertex3d(-2.26, 0, -1.2);
			glVertex3d(-3, 0, -1.2);
			glVertex3d(-3, 3, -1.2);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			//glColor3ub(226, 160, 105);
			glColor3ub(125, 82, 65);
			glVertex3d(-1.8, 0, 1.52);
			glVertex3d(-1.8, 2, 1.52);
			glVertex3d(-0.8, 2, 1.52);
			glVertex3d(-0.8, 0, 1.52);
		glEnd();

		//ventana
		glBegin(GL_QUADS);
			glVertex3d(0, 1, 1.52);
			glVertex3d(0, 2, 1.52);
			glVertex3d(2, 2, 1.52);
			glVertex3d(2, 1, 1.52);
		glEnd();

		//puerta
		glBegin(GL_QUADS);
			glVertex3d(-1.2, 0, -1.52);
			glVertex3d(-1.2, 2, -1.52);
			glVertex3d(1.2, 2, -1.52);
			glVertex3d(1.2, 0, -1.52);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslated(1, 3.5, 0);
		antena();
	glPopMatrix();
}

void antena() {
	glPushMatrix();
		glColor3ub(255, 250, 250);
		glRotated(50, 1, 0, 0);
		glutSolidCone(0.8, 0.3, 40, 40);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 0.2);
		glRotated(90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.1, 0.1, 1, 50, 50);
	glPopMatrix();
}

void roca() {
	glPushMatrix();
		glColor3ub(125, 82, 65);
		glTranslated(2, 0.5, 0);
	glutSolidDodecahedron();

	glTranslated(2, 2, 0);
		glScaled(0.8, 3, 1);
		glutSolidDodecahedron();
	glPopMatrix();

	glPushMatrix();
		glutSolidIcosahedron();
		glTranslated(1, 0, -1);
		glScaled(1, 3, 1);
		glutSolidIcosahedron();
	glPopMatrix();
}

void piedras() {
	glPushMatrix();
		glScaled(0.5, 0.5, 0.5);
		glutSolidIcosahedron();

		glTranslated(2, 0, 2);
		//glScaled(0.5, 0.5, 0.5);
		glutSolidIcosahedron();

		glTranslated(0, 0, -3);
		glutSolidIcosahedron();
	glPopMatrix();
}

void pilar() {
	glPushMatrix();
		glTranslated(0, 0, 0);
		glRotated(-90, 1, 0, 0);
		//gluCylinder(gluNewQuadric(), 1.2, 0.6, 3, 50, 50);
		glScaled(1, 1.2, 1);
		glutSolidDodecahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 2, 0);
		glScaled(0.6, 1.5, 0.6);
		glutSolidDodecahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(1, 0, 1);
		piedras();

		glTranslated(-3, 0, 0);
		piedras();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 2);
		glRotated(90, 0, 1, 0);
		arboles();
	glPopMatrix();
}

void arboles() {
	glPushMatrix();
		glColor3ub(0, 0, 0);
		glTranslated(0, 0, 0);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.2, 0.05, 4, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 1.5, 0);
		glRotated(-50, 1, 0, 0);
		glutSolidCone(0.1, 2, 50, 50);

		glTranslated(0, 0.3, 0.2);
		glRotated(-90, 1, 0, 0);
		glutSolidCone(0.1, 2, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 3, 0);
		glRotated(-50, 1, 0, 0);
		glutSolidCone(0.1, 1, 50, 50);

		glRotated(-60, 0, 1, 0);
		glutSolidCone(0.1, 1, 50, 50);
	glPopMatrix();
}

void limite() {
	glPushMatrix();
		glColor3ub(186, 140, 117);
		glTranslated(0, 4, 0);
		glScaled(6, 7, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, 6);
		glScaled(6, 6, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, 12);
		glScaled(6, 6, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, 18);
		glScaled(6, 7, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, 22);
		glScaled(6, 6, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, -6);
		glScaled(6, 7, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, -12);
		glScaled(6, 6, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, -18);
		glScaled(6, 7, 6);
		glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 4, -22);
		glScaled(6, 6, 6);
		glutSolidIcosahedron();
	glPopMatrix();
}

void antiaereo() {
	glPushMatrix();
		camion();
	glPopMatrix();
}

void camion() {
	glPushMatrix();
		glColor3ub(194, 160, 114);
		glTranslated(0, 0.7, 0);
		glutSolidCube(1);
	glPopMatrix();

	//VENTANAS
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor3ub(178, 199, 221);
			glVertex3d(-0.45, 1.15, 0.51);
			glVertex3d(-0.02, 1.15, 0.51);
			glVertex3d(-0.02, 0.8, 0.51);
			glVertex3d(-0.45, 0.8, 0.51);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(0.45, 1.15, 0.51);
			glVertex3d(0.02, 1.15, 0.51);
			glVertex3d(0.02, 0.8, 0.51);
			glVertex3d(0.45, 0.8, 0.51);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(0.51, 1.15, 0.45);
			glVertex3d(0.51, 1.15, 0.02);
			glVertex3d(0.51, 0.8, 0.02);
			glVertex3d(0.51, 0.8, 0.45);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(0.51, 1.15, -0.2);
			glVertex3d(0.51, 1.15, -0.02);
			glVertex3d(0.51, 0.8, -0.02);
			glVertex3d(0.51, 0.8, -0.2);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(-0.51, 1.15, 0.45);
			glVertex3d(-0.51, 1.15, 0.02);
			glVertex3d(-0.51, 0.8, 0.02);
			glVertex3d(-0.51, 0.8, 0.45);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3d(-0.51, 1.15, -0.2);
			glVertex3d(-0.51, 1.15, -0.02);
			glVertex3d(-0.51, 0.8, -0.02);
			glVertex3d(-0.51, 0.8, -0.2);
		glEnd();
	glPopMatrix();

	//Plataforma
	glPushMatrix();
		glColor3ub(194, 160, 114);
		glTranslated(0, 0.4, -2);
		glScaled(1, 0.2, 3);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 0);
		llantas();

		glTranslated(0, 0, -0.7);
		llantas();

		glTranslated(0, 0, -0.5);
		llantas();

		glTranslated(0, 0, -1.3);
		llantas();

		glTranslated(0, 0, -0.5);
		llantas();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 1.1, 0.2);
		glRotated(-20, 1, 0, 0);
		misiles();
	glPopMatrix();
}

void llantas() {
	glPushMatrix();
		glColor3ub(0, 0, 0);
		glTranslated(-0.6, 0.2, 0);
		glRotated(90, 0, 1, 0);
		gluCylinder(gluNewQuadric(), 0.2, 0.2, 1.2, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-0.6, 0.2, 0);
		glRotated(90, 0, 1, 0);
		gluDisk(gluNewQuadric(), 0, 0.2, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0.6, 0.2, 0);
		glRotated(90, 0, 1, 0);
		gluDisk(gluNewQuadric(), 0, 0.2, 50, 50);
	glPopMatrix();
}


void misiles() {
	glPushMatrix();
		glColor3ub(194, 160, 114);
		glTranslated(0, 1, -2);
		glScaled(1, 0.8, 2.6);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(0, 0, 0);
		glTranslated(0, 1.2, -0.69);
		gluDisk(gluNewQuadric(), 0, 0.15, 50, 50);

		glTranslated(0, -0.4, 0);
		gluDisk(gluNewQuadric(), 0, 0.15, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-0.32, 1.2, -0.69);
		gluDisk(gluNewQuadric(), 0, 0.15, 50, 50);

		glTranslated(0, -0.4, 0);
		gluDisk(gluNewQuadric(), 0, 0.15, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0.32, 1.2, -0.69);
		gluDisk(gluNewQuadric(), 0, 0.15, 50, 50);

		glTranslated(0, -0.4, 0);
		gluDisk(gluNewQuadric(), 0, 0.15, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0.1, -2);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.1, 0.1, 1.2, 50, 50);
	glPopMatrix();
}

void enemigo() {
	glPushMatrix();
		glColor3ub(230, 230, 120);
		glTranslated(5, 1.5, 2);
		gluSphere(gluNewQuadric(), 1, 50, 50);
	glPopMatrix();
}

void bala() {
	glPushMatrix();
		glTranslated(0, 0, 0);
		//glTranslated(-1.5, 3.5, 3);
		glRotated(-90, 0, 1, 0);
		estructuraBala();
	glPopMatrix();
}

void estructuraBala() {
	glPushMatrix();
		glColor3ub(0, 0, 0);
		glTranslated(0, 0, 0);
		gluSphere(gluNewQuadric(), 0.5, 50, 50);
	glPopMatrix();

	glPushMatrix();
		//glColor3ub(255, 0, 0);
		glTranslated(0, 0, 0);
		glRotated(-90, 0, 1, 0);
		gluCylinder(gluNewQuadric(), 0.5, 0.5, 1, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-1, 0, 0);
		glRotated(-90, 0, 1, 0);
		gluDisk(gluNewQuadric(), 0, 0.5, 50, 50);
	glPopMatrix();
}

void avion() {
	cabinaA();
	cabinaP();
	alas();
	ala();
	helices();
}

void cabinaA() {
	glPushMatrix();
		glColor3ub(148, 148, 148);
		glTranslated(0, 2, 0);
		gluCylinder(gluNewQuadric(), 0.6, 0.6, 3, 50, 50);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 2, 3);
		gluCylinder(gluNewQuadric(), 0.6, 0.2, 3, 50, 50);
	glPopMatrix();
}

void cabinaP() {
	glPushMatrix();
		glColor3ub(76, 110, 144);
		glTranslated(0, 2.5, 2);
		glScaled(1, 1, 2);
		gluSphere(gluNewQuadric(), 0.5, 50, 50);
	glPopMatrix();
}

void alas() {
	glPushMatrix();
		glColor3ub(255, 0, 0);
		glTranslated(0, 1.7, 1.2);
		glScaled(6, 0.1, 1.2);
		glutSolidCube(1);
	glPopMatrix();
}

void ala() {
	glPushMatrix();
		glColor3ub(255, 0, 0);
		glTranslated(0, 2, 5.7);
		glScaled(2, 0.1, 0.6);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glColor3ub(255, 0, 0);
		glTranslated(0, 2.5, 5.7);
		glScaled(0.1, 1, 0.6);
		glutSolidCube(1);
	glPopMatrix();
}

void helices() {
	glPushMatrix();
		glColor3ub(148, 148, 148);
		glTranslated(0, 2, 0);
		glScaled(1, 1, 1.2);
		glutSolidTorus(0.25, 0.36, 50, 50);
	glPopMatrix();
}

void dibujarEjes() {
	glPushMatrix();
		glLineWidth(3);
		glBegin(GL_LINES);
			//eje X rojo
			glColor3ub(255, 0, 0);
			glVertex3d(0, 0, 0);
			glVertex3d(50, 0, 0);

			//eje Y verde
			glColor3ub(0, 255, 0);
			glVertex3d(0, 0, 0);
			glVertex3d(0, 50, 0);

			//eje Z azul
			glColor3ub(0, 0, 255);
			glVertex3d(0, 0, 0);
			glVertex3d(0, 0, 50);
		glEnd();
	glPopMatrix();
}

void timer(int t) {
	if (disparoActivo) {
		// Actualiza la posición de la bala según la dirección y velocidad deseada
		float velocidadBala = 5; // Ajusta la velocidad como desees
		balaX += balaDireccionX * velocidadBala;
		balaZ += balaDireccionZ * velocidadBala;

		// Comprueba si la bala ha alcanzado una posición final o está fuera de la escena
		float distanciaMaxima = 150, distanciaMinima = -150; // Ajusta la distancia máxima de la bala como desees
		if (balaX >= distanciaMaxima || balaX <= distanciaMinima || balaZ >= distanciaMaxima || balaZ <= distanciaMinima) {
			disparoActivo = false; // Desactiva el disparo cuando la bala alcanza una posición final o está fuera de la escena
		}
	}

	glutPostRedisplay(); //redibujo
	glutTimerFunc(20, timer, 0);
}

void tecladoEspecial(int tecla, int x, int y) {
	cout << "Angulo: " << angulo << ", Radio: " << radio << ", Centro Y: " << centroY << ", Camara X: " << camaraX << ", Camara Y: " << camaraY << ", Camara Z: " << camaraZ << endl;
	cout << tecla << endl;
	switch (tecla)
	{
	case 101:
		camaraY += 0.2;
		break;
	case 103:
		camaraY -= 0.2;
		break;
	case 100:
		//angulo += 0.5;
		break;
	case 102:
		//angulo -= 0.5;
		break;
	case 105://Re Pag
		radio += 2;
		break;
	case 104://Av Pag
		radio -= 2;
		break;
	case 106://Inicio
		centroY += 1;
		break;
	case 107://Fin
		centroY -= 1;
		break;
	}
}

bool detectarColision(float balaX, float balaY, float balaZ, float enemigoX, float enemigoY, float enemigoZ, float radioBala, float radioEnemigo) {
	float distancia = sqrt((balaX - enemigoX) * (balaX - enemigoX) +
		(balaY - enemigoY) * (balaY - enemigoY) +
		(balaZ - enemigoZ) * (balaZ - enemigoZ));
	return distancia < (radioBala + radioEnemigo);
}

void cargarImagenes() {
	loadTextureFromFile("texturas/piso.bmp", 0);
	loadTextureFromFile("texturas/oxidoMetal.bmp", 1);
	loadTextureFromFile("texturas/camuflajeTanque.bmp", 2);
	loadTextureFromFile("texturas/muroTextura.bmp", 3);
}

void loadTextureFromFile(const char* filename, int index) {
	RgbImage theTexMap(filename);
	glGenTextures(1, &texturas[index]);
	glBindTexture(GL_TEXTURE_2D, texturas[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
}

void manejarTecla(unsigned char tecla, int x, int y) {
	//cout << tecla << endl;
	//cout << "Direccion X: " << direccionX << ", Direccion Z: " << direccionZ << endl;
	switch (tecla) {
	case 'w':
	case 'W':
		deltaZ = 0.2; // Velocidad hacia adelante
		break;
	case 's':
	case 'S':
		deltaZ = -0.2; // Velocidad hacia atrás
		break;
	case 'd':
	case 'D':
		anguloRotacion -= 5;
		direccionX = sin(anguloRotacion * M_PI / 180.0f);
		direccionZ = cos(anguloRotacion * M_PI / 180.0f);
		break;
	case 'a':
	case 'A':
		anguloRotacion += 5;
		direccionX = sin(anguloRotacion * M_PI / 180.0f);
		direccionZ = cos(anguloRotacion * M_PI / 180.0f);
		break;
	case 'q':
	case 'Q':
		//angulo += 0.02;
		moviendoIzquierda = true;
		break;
	case 'e':
	case 'E':
		//angulo -= 0.02;
		moviendoDerecha = true;
		break;
	//case 'p':
	//case 'P':
	//	camaraY += 0.5;
	//	//moviendoArriba = true;
	//	break;
	//case 'l':
	//case 'L':
	//	camaraY -= 0.5;
	//	//moviendoAbajo = true;
	//	break;
	case ' ':
		disparoActivo = true;

		balaX = posicionX;
		balaY = 1;
		balaZ = posicionZ;

		/*balaDireccionX = sin(anguloRotacion * PI / 180);
		balaDireccionZ = sin(anguloRotacion * PI / 180);*/

		float direccionBalaX = sin(anguloRotacion * M_PI / 180);
		float direccionBalaZ = cos(anguloRotacion * M_PI / 180);

		// Normaliza el vector de dirección de la bala
		float magnitud = sqrt(direccionBalaX * direccionBalaX + direccionBalaZ * direccionBalaZ);
		balaDireccionX = direccionBalaX / magnitud;
		balaDireccionZ = direccionBalaZ / magnitud;
		break;
	}
}

void manejarTeclaSoltada(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'w':
	case 'W':
	case 's':
	case 'S':
		deltaZ = 0.0; // Detener el movimiento
		break;
	//case 'q':
	//case 'Q':
	//	moviendoIzquierda = false;
	//	break;
	//case 'e':
	//case 'E':
	//	moviendoDerecha = false;
	//	break;
	}
}