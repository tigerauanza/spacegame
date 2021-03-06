#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>


#define PI 3.14159
#define GAME_SCREEN 0			//Constant to identify background color
#define MENU_SCREEN 4			
#define MAX_STONES  100      
#define MAX_STONE_TYPES 5
#define stoneRotationSpeed 10
#define SPACESHIP_SPEED 40
int stoneTranslationSpeed = 10;

GLint m_viewport[4];
GLint CI = 0;
int x, y;
int i;
int randomStoneIndices[100];
int index;
int Score = 0;
int alienLife = 100;
int GameLvl = 1;
float mouseX, mouseY;				//Cursor coordinates;
float LaserAngle = 0, stoneAngle = 0, lineWidth = 1;
float xOne = 500, yOne = 0;				//Spaceship coordinates
float xStone[MAX_STONES], yStone[MAX_STONES];//coordinates of stones
float xHealthBarStart = 1200;				//Health bar starting coodinate
GLint stoneAlive[MAX_STONES];		//check to see if stone is killed

bool mButtonPressed = false, startGame = false, gameOver = false;		//boolean values to check state of the game
bool startScreen = true, nextScreen = false, previousScreen = false;
bool gameQuit = false, instructionsGame = false, optionsGame = false;


char highScore[100], ch;
void display();
void StoneGenerate();
void SetDisplayMode(int modeToDisplay) {
	switch (modeToDisplay) {
	case GAME_SCREEN: glClearColor(0, 0, 0.11, 1); break;
	case MENU_SCREEN: glClearColor(0, 0, 0, 1); break;
	}
}
void initializeStoneArray() {
	//random stones index

	for (int i = 0; i < MAX_STONES; i++) {
		randomStoneIndices[i] = rand() % MAX_STONE_TYPES;
		stoneAlive[i] = true;
	}

	xStone[0] = -(200 * MAX_STONES) - 600;             //START LINE for stone appearance

	for (int i = 0; i<MAX_STONES; i++) {				//ramdom appearance yIndex for each stone
		yStone[i] = rand() % 600;
		if (int(yStone[i]) % 2)
			yStone[i] *= -1;
		xStone[i + 1] = xStone[i] + 200;				//xIndex of stone aligned with 200 units gap
	}
}
void DrawSpaceshipBody()
{
	glColor3f(0, 1, 0);				//BASE

	glPushMatrix();
	glScalef(100, 30, 1);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 0, 0);				//COLLAR
}
void DrawSpaceshipDoom()
{
	glColor4f(0.7, 1, 1, 0.0011);
	glPushMatrix();
	glTranslated(0, 30, 0);
	glScalef(35, 20, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
}
void DrawSpaceShipLazer() {

	glColor3f(0.3, 0.5, 1);
	glPushMatrix();
	glBegin(GL_POLYGON);           //Lazer stem
	glVertex2f(-55, 10);
	glVertex2f(-55, 30);
	glVertex2f(-50, 30);
	glVertex2f(-50, 10);
	glEnd();

	float xMid = 0, yMid = 0;
	//Mid point of the lazer horizontal 	
	xMid = (55 + 50) / 2.0;
	yMid = (25 + 35) / 2.0;

	//Rotating about the point ,20
	glTranslated(-xMid, yMid, 0);
	glRotated(LaserAngle, 0, 0, 1);
	glTranslated(xMid, -yMid, 0);

	//find mid point of top of lazer stem
	float midPoint = -(55 + 50) / 2.0;

	glBegin(GL_POLYGON);           //Lazer horizontal stem
	glVertex2f(midPoint + 10, 25);
	glVertex2f(midPoint + 10, 35);
	glVertex2f(midPoint - 10, 35);
	glVertex2f(midPoint - 10, 25);
	glEnd();

	glPopMatrix();
}
void DrawLazerBeam() {

	float xMid = -(55 + 50) / 2.0;
	float yMid = (25 + 35) / 2.0;

	float mouseXEnd = -((-mouseX) + xOne);
	float mouseYEnd = -((-mouseY) + yOne);
	glLineWidth(5);  

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(xMid, yMid);
	glVertex2f(mouseXEnd, mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch (StoneIndex)                           //CHANGE INDEX VALUE FOR DIFFERENT STONE VARIETY;
	{
	case 0:

		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glColor3f(0.4f, 0.0f, 0.0f);
		glScalef(35, 35, 1);
		glutSolidSphere(1, 9, 50);

		glLoadIdentity();
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60, 10, 1);
		glutSolidSphere(1, 5, 50);

		glLoadIdentity();
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10, 60, 1);
		glutSolidSphere(1, 5, 50);
		break;

	case 1:
		glColor3f(1.0f, 0.8f, 0.8f);
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(15, 20, 1);
		glutSolidSphere(1, 9, 50);

		glLoadIdentity();
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(40, 5, 1);
		glutSolidSphere(1, 5, 50);
		break;

	case 2:
		glColor3f(0.2f, 0.2f, 0.0f);
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60, 25, 1);
		glutSolidSphere(1, 9, 50);

		glLoadIdentity();
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25, 60, 1);
		glutSolidSphere(1, 9, 50);

		break;

	case 3:
		glColor3f(0.8f, 0.8f, 0.1f);
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(35, 10, 1);
		glutSolidSphere(1, 10, 7);

		glLoadIdentity();
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(50, 20, 1);
		glutSolidSphere(1, 5, 50);
		break;
	case 4:
		glColor3f(0.26f, 0.26f, 0.26f);
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10, 55, 1);
		glutSolidSphere(1, 9, 50);

		glLoadIdentity();
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(20, 10, 1);
		glutSolidSphere(1, 9, 50);
		glLoadIdentity();

		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle + 45, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25, 10, 1);
		glutSolidSphere(1, 9, 50);
		break;
	}
	glPopMatrix();
}
bool checkIfSpaceShipIsSafe() {
	for (int i = 0; i<MAX_STONES; i++) {
		if (stoneAlive[i] & ((xOne >= (xStone[i] / 2 - 70) && xOne <= (xStone[i] / 2 + 70) && yOne >= (yStone[i] / 2 - 18) && yOne <= (yStone[i] / 2 + 53)) || (yOne <= (yStone[i] / 2 - 20) && yOne >= (yStone[i] / 2 - 90) && xOne >= (xStone[i] / 2 - 40) && xOne <= (xStone[i] / 2 + 40))))
		{
			stoneAlive[i] = 0;
			return false;
		}
	}
	return true;
}
void SpaceshipCreate() {
	glPushMatrix();
	glTranslated(xOne, yOne, 0);
	if (!checkIfSpaceShipIsSafe() && alienLife) {
		alienLife -= 10;
		xHealthBarStart -= 230;
	}
	DrawSpaceshipDoom();
	glPushMatrix();
	glTranslated(4, 19, 0);
	
	glPopMatrix();
	DrawSpaceshipBody();
	DrawSpaceShipLazer();
	if (mButtonPressed) {
		DrawLazerBeam();
	}
	glEnd();
	glPopMatrix();
}
void DisplayHealthBar() {

	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-xHealthBarStart, 720);
	glVertex2f(1200, 720);
	glVertex2f(1200, 670);
	glVertex2f(-xHealthBarStart, 670);
	glEnd();
	
	
}
void startScreenDisplay()
{
	glLineWidth(50);
	SetDisplayMode(MENU_SCREEN);


	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLES);				//START GAME PLOYGON
	glVertex3f(-130, 100, 0.5);
	glVertex3f(-130, 400, 0.5);
	glVertex3f(130, 250, 0.5);
	glEnd();

	
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);				//QUIT POLYGON
	glVertex3f(-200, -200, 0.5);
	glVertex3f(-200, -100, 0.5);
	glVertex3f(200, -100, 0.5);
	glVertex3f(200, -200, 0.5);
	glEnd();

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 150 && mouseY <= 200) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			startGame = true;
			gameOver = false;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);

	

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 30 && mouseY <= 80) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			instructionsGame = true;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);
	

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= -90 && mouseY <= -40) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			gameQuit = true;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);


}
void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);
	DisplayHealthBar();
	glScalef(2, 2, 0);
	if (alienLife) {
		SpaceshipCreate();
	}
	else {
		gameOver = true;
		startScreen = false;
	}								//<----------------------gameover screen

	StoneGenerate();

}
void GameOverScreen()
{
	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLES);				
	glVertex3f(-130, 100, 0.5);
	glVertex3f(-130, 400, 0.5);
	glVertex3f(130, 250, 0.5);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);				
	glVertex3f(-200, -200, 0.5);
	glVertex3f(-200, -100, 0.5);
	glVertex3f(200, -100, 0.5);
	glVertex3f(200, -200, 0.5);
	glEnd();

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 25 && mouseY <= 75) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {                                                       //Reset game default values
			startGame = true;
			gameOver = false;
			mButtonPressed = false;
			initializeStoneArray();
			alienLife = 100;
			xHealthBarStart = 1200;
			Score = 0;
			GameLvl = 1;
			GameScreenDisplay();
		}
	}
	else
		glColor3f(0, 0, 0);
	

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= -100 && mouseY <= -50) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			exit(0);
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);
	

}
void StoneGenerate() {

	if (xStone[0] >= 1200) {      //If the last screen hits the end of screen then go to Nxt lvl
		GameLvl++;
		stoneTranslationSpeed += 3;
		Score += 50;
		initializeStoneArray();
		GameScreenDisplay();
	}

	for (int i = 0; i<MAX_STONES; i++) {
		index = i;

		if (mouseX <= (xStone[i] / 2 + 20) && mouseX >= (xStone[i] / 2 - 20) && mouseY >= (yStone[i] / 2 - 20) && mouseY <= (yStone[i] / 2 + 20) && mButtonPressed) {
			if (stoneAlive[i]) {   // IF ALIVE KILL STONE
				stoneAlive[i] = 0;
				Score++;
				if (Score % 1 == 0) {
					stoneTranslationSpeed += 1;			//<--------------Rate of increase of game speed
				}
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if (stoneAlive[i])             //stone alive
			DrawStone(randomStoneIndices[i]);
	}
	stoneAngle += stoneRotationSpeed;
	if (stoneAngle > 360) stoneAngle = 0;
}
void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1280, 720);

	if (startGame && !gameOver)
		GameScreenDisplay();

	else if (gameOver)
		GameOverScreen();

	//Make spaceship bigger
	else if (startScreen) {

		startScreenDisplay();
		if (gameQuit || startGame || optionsGame || instructionsGame) {
			//startScreen = false;

			if (startGame) {
				SetDisplayMode(GAME_SCREEN);
				startScreen = false;

			}
			else if (gameQuit)
				exit(0);

		}
		
		
	}

	//Reset Scaling values
	glScalef(1 / 2, 1 / 2, 0);
	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}
void somethingMovedRecalculateLaserAngle() {

	float mouseXForTan = (-50 - mouseX) + xOne;
	float mouseYForTan = (35 - mouseY) + yOne;
	float LaserAngleInRadian = atan(mouseYForTan / mouseXForTan);
	LaserAngle = (180 / PI) * LaserAngleInRadian;
}
void keys(unsigned char key, int x, int y)
{
	
	if (key == 'd') xOne += SPACESHIP_SPEED;
	if (key == 'a') xOne -= SPACESHIP_SPEED;
	if (key == 'w') { yOne += SPACESHIP_SPEED; }
	if (key == 's') { yOne -= SPACESHIP_SPEED; }
	if (key == 'd' || key == 'a' || key == 'w' || key == 's')
		somethingMovedRecalculateLaserAngle();

	display();

}
void myinit()
{
	glClearColor(1, 1, 1, 0);
	glColor3f(0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-1280, 1280, -720, 720);                   
														  
	glMatrixMode(GL_MODELVIEW);
}
void passiveMotionFunc(int x, int y) {

	mouseX = float(x) / (m_viewport[2] / 1280.0) - 600.0;  
	mouseY = -(float(y) / (m_viewport[3] / 720.0) - 350.0);

	
	somethingMovedRecalculateLaserAngle();
	display();
}
void mouseClick(int buttonPressed, int state, int x, int y) {

	if (buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else
		mButtonPressed = false;
	display();
}
void idleCallBack() {			//when no mouse or keybord pressed
	display();
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(90, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Space Shooting CMM");
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutPassiveMotionFunc(passiveMotionFunc);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutIdleFunc(idleCallBack);
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();
}
