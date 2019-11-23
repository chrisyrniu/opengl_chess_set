/*
Author: <Yaru Niu> 
Class: ECE6122 
Last Date Modified: <2019-11-22>
Description: Solution to Homework 6: the main program and functions to create the 3D chess set
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include "objloader.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
using namespace std;

#define ESC 27

// Eye position
float eyePosX = 4.0;
float eyePosY = -5.0;
float eyePosZ = 10.0;

// Light flag
int lightFlag0 = 0;
int lightFlag1 = 0;

// Rotation angle of the chess set
float rotation = 0;

// Team and position of the piece (initialized)
int pawn[16][3] = {{0,0,1}, {0,1,1}, {0,2,1}, {0,3,1}, {0,4,1}, {0,5,1}, {0,6,1}, {0,7,1},
                   {1,0,6}, {1,1,6}, {1,2,6}, {1,3,6}, {1,4,6}, {1,5,6}, {1,6,6}, {1,7,6}};
int rook[4][3] = {{0,0,0}, {0,7,0}, {1,0,7}, {1,7,7}};
int knight[4][3] = {{0,1,0}, {0,6,0}, {1,1,7}, {1,6,7}};
int bishop[4][3] = {{0,2,0}, {0,5,0}, {1,2,7}, {1,5,7}};
int queen[2][3] = {{0,3,0}, {1,3,7}};
int king[2][3] = {{0,4,0},{1,4,7}};

// Random number of choosing which piece to move and choosing how to move (initialized)
int pawnRand = 0;
int knightRand = 0;
int pawnMoveRand = 0;
int knightMoveRand = 0;

// Map of the chessboard (initialized)
int chessBoard[8][8] = {{1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}};

// Load obj files
// Pawn
std::vector< vec3 > verticesPawn;
std::vector< vec2 > uvsPawn;
std::vector< vec3 > normalsPawn;
bool resPawn = loadOBJ("pawn.obj", verticesPawn, uvsPawn, normalsPawn);

// Rook
std::vector< vec3 > verticesRook;
std::vector< vec2 > uvsRook;
std::vector< vec3 > normalsRook;
bool resRook = loadOBJ("rook.obj", verticesRook, uvsRook, normalsRook);

// Knight
std::vector< vec3 > verticesKnight;
std::vector< vec2 > uvsKnight;
std::vector< vec3 > normalsKnight;
bool resKnight = loadOBJ("knight.obj", verticesKnight, uvsKnight, normalsKnight);

// Bishop
std::vector< vec3 > verticesBishop;
std::vector< vec2 > uvsBishop;
std::vector< vec3 > normalsBishop;
bool resBishop = loadOBJ("bishop.obj", verticesBishop, uvsBishop, normalsBishop);

// Queen
std::vector< vec3 > verticesQueen;
std::vector< vec2 > uvsQueen;
std::vector< vec3 > normalsQueen;
bool resQueen = loadOBJ("queen.obj", verticesQueen, uvsQueen, normalsQueen);

// King
std::vector< vec3 > verticesKing;
std::vector< vec2 > uvsKing;
std::vector< vec3 > normalsKing;
bool resKing = loadOBJ("king.obj", verticesKing, uvsKing, normalsKing);

// Enhanced mode bool
bool enhanced = false;

// Reshape callback
void changeSize(int w, int h)
{
    // Window aspect ratio
    float ratio = ((float)w) / ((float)h); 
    // Projection matrix is active
    glMatrixMode(GL_PROJECTION); 
    // Reset the projection
    glLoadIdentity(); 
    // Perspective transformation
    gluPerspective(45.0, ratio, 0.1, 100.0); 
    // Return to modelview mode
    glMatrixMode(GL_MODELVIEW); 
    // Set viewport (drawing area) to entire window
    glViewport(0, 0, w, h); 
}

// Function to draw the chessboard
void drawChessboard()
{
    // Loop to draw 64 1*1 squares
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            glPushMatrix();
            glTranslatef(4.0, 4.0, 0.0);
            glRotatef(rotation, 0.0, 0.0, 1.0);
            glTranslatef(-4.0, -4.0, 0.0);
            glTranslatef(i*1, j*1, 0);
            if ((i%2 == 0 && j%2 == 0) || (i%2 == 1 && j%2 == 1))
            {
                glColor3f(0.0, 0.0, 0.0);
            }
            else
            {
                glColor3f(1.0, 1.0, 1.0);
            }
            glBegin(GL_POLYGON);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 1.0, 0.0);
            glVertex3f(1.0, 1.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);
            glEnd();
            glPopMatrix();
        }
    }
}

// Function to draw pawns (team, position x and y)
void drawPawn(int team, float x, float y)
{
    glPushMatrix();

    // Rotate if press 'r' or 'R'
    glTranslatef(4.0, 4.0, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glTranslatef(-4.0, -4.0, 0.0);

    // Enhanced mode has a different start to draw
    if (enhanced == true) glTranslatef(0.5, 0.5, 0.0);
    else glTranslatef(0.5, 0.5, 0.5);

    glTranslatef(x, y, 0.0);
    if (team == 0)
        glColor3f(0.5490, 0.5490, 0.5294);
    if (team == 1)
    {
        glColor3f(0.5882, 0.2941, 0);
        glRotatef(180, 0, 0, 1);
    }

    // Enhanced mode
    if (enhanced == true)
    {
        glRotatef(90, 1.0, 0, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < verticesPawn.size(); ++i)
        {
            glVertex3f(verticesPawn[i].x, verticesPawn[i].y, verticesPawn[i].z);
            glNormal3f(normalsPawn[i].x, normalsPawn[i].y, normalsPawn[i].z);
        }
        glEnd();
    }
    // Normal mode
    else
    {
        glScalef(0.75, 0.75, 1.0);
        glutSolidSphere(0.5, 20, 20);
    }
    glPopMatrix();
}

// Function to draw rooks (team, position x and y)
void drawRook(int team, float x, float y)
{
    glPushMatrix();

    // Rotate if press 'r' or 'R'
    glTranslatef(4.0, 4.0, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glTranslatef(-4.0, -4.0, 0.0);

    // Enhanced mode has a different start to draw
    if (enhanced == true) glTranslatef(0.5, 0.5, 0.0);
    else glTranslatef(0.5, 0.5, 0.5);

    glTranslatef(x, y, 0.0);
    if (team == 0)
        glColor3f(0.5490, 0.5490, 0.5294);
    if (team == 1)
    {
        glColor3f(0.5882, 0.2941, 0);
        glRotatef(180, 0, 0, 1);
    }

    // Enhanced mode
    if (enhanced == true)
    {
        glRotatef(90, 1.0, 0, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < verticesRook.size(); ++i)
        {
            glVertex3f(verticesRook[i].x, verticesRook[i].y, verticesRook[i].z);
            glNormal3f(normalsRook[i].x, normalsRook[i].y, normalsRook[i].z);
        }
        glEnd();
    }
    // Normal mode
    else
    {
        glScalef(0.75, 0.75, 1.0);
        glutSolidCube(1);
    }
    glPopMatrix();
}

// Function to draw knights
void drawKnight(int team, float x, float y)
{
    glPushMatrix();

    // Rotate if press 'r' or 'R'
    glTranslatef(4.0, 4.0, 0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glTranslatef(-4.0, -4.0, 0.0);

    // Enhanced mode has a different start to draw
    if (enhanced == true) glTranslatef(0.5, 0.5, 0.0);
    else glTranslatef(0.5, 0.5, 0.5);

    glTranslatef(x, y, 0.0);
    if (team == 0)
        glColor3f(0.5490, 0.5490, 0.5294);
    if (team == 1)
    {
        glColor3f(0.5882, 0.2941, 0);
        glRotatef(180, 0, 0, 1);
    }
    glRotatef(90, 1.0, 0, 0);

    // Enhanced mode 
    if (enhanced == true)
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < verticesKnight.size(); ++i)
        {
            glVertex3f(verticesKnight[i].x, verticesKnight[i].y, verticesKnight[i].z);
            glNormal3f(normalsKnight[i].x, normalsKnight[i].y, normalsKnight[i].z);
        }
        glEnd();
    }
    // Normal mode
    else
    {
        glScalef(0.60, 1.33, 0.60);
        glutSolidTeapot(0.5);
    }

    glPopMatrix();
}

// Function to draw bishops
void drawBishop(int team, float x, float y)
{
    glPushMatrix();

    // Rotate if press 'r' or 'R'
    glTranslatef(4.0, 4.0, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glTranslatef(-4.0, -4.0, 0.0);

    // Enhanced mode has a different start to draw
    if (enhanced == true) glTranslatef(0.5, 0.5, 0.0);
    else glTranslatef(0.5, 0.5, 0.0);


    glTranslatef(x, y, 0.0);
    if (team == 0)
        glColor3f(0.5490, 0.5490, 0.5294);
    if (team == 1)
        glColor3f(0.5882, 0.2941, 0);

    // Enhanced mode 
    if (enhanced == true)
    {
        glRotatef(90, 1.0, 0, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < verticesBishop.size(); ++i)
        {
            glVertex3f(verticesBishop[i].x, verticesBishop[i].y, verticesBishop[i].z);
            glNormal3f(normalsBishop[i].x, normalsBishop[i].y, normalsBishop[i].z);
        }
        glEnd();
    }
    // Normal mode
    else
    {
        glScalef(0.75, 0.75, 1.0);
        glutSolidCone(0.5, 1, 20, 20);
    }
    glPopMatrix();
}

// Function to draw queens
void drawQueen(int team, float x, float y)
{
    glPushMatrix();

    // Rotate if press 'r' or 'R'
    glTranslatef(4.0, 4.0, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glTranslatef(-4.0, -4.0, 0.0);

    // Enhanced mode has a different start to draw
    if (enhanced == true) glTranslatef(0.5, 0.5, 0.0);
    else glTranslatef(0.5, 0.5, 0.5);

    glTranslatef(x, y, 0.0);
    if (team == 0)
        glColor3f(0.5490, 0.5490, 0.5294);
    if (team == 1)
    {
        glColor3f(0.5882, 0.2941, 0);
        glRotatef(180, 0, 0, 1);
    }

    // Enhanced mode
    if (enhanced == true)
    {
        glRotatef(90, 1.0, 0, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < verticesQueen.size(); ++i)
        {
            glVertex3f(verticesQueen[i].x, verticesQueen[i].y, verticesQueen[i].z);
            glNormal3f(normalsQueen[i].x, normalsQueen[i].y, normalsQueen[i].z);
        }
        glEnd();
    }
    // Normal mode
    else
    {
        glScalef(0.43, 0.43, 0.86);
        glutSolidTetrahedron();
    }
    glPopMatrix();
}

// Function to draw kings
void drawKing(int team, float x, float y)
{
    glPushMatrix();

    // Rotate if press 'r' or 'R'
    glTranslatef(4.0, 4.0, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glTranslatef(-4.0, -4.0, 0.0);

    // Enhanced mode has a different start to draw
    if (enhanced == true) glTranslatef(0.5, 0.5, 0.0);
    else glTranslatef(0.5, 0.5, 0.5);

    glTranslatef(x, y, 0.0);
    if (team == 0)
        glColor3f(0.5490, 0.5490, 0.5294);
    if (team == 1)
    {
        glColor3f(0.5882, 0.2941, 0);
        glRotatef(180, 0, 0, 1);
    }

    // Enhanced mode
    if (enhanced == true)
    {
        glRotatef(90, 1.0, 0, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < verticesKing.size(); ++i)
        {
            glVertex3f(verticesKing[i].x, verticesKing[i].y, verticesKing[i].z);
            glNormal3f(normalsKing[i].x, normalsKing[i].y, normalsKing[i].z);
        }
        glEnd();
    }
    // Normal mode
    else
    {
        glScalef(0.43, 0.43, 0.50);
        glutSolidOctahedron();
    }
    glPopMatrix();
}


// Function to update with each idle event
void update(void)
{
    glutPostRedisplay(); // redisplay everything
}

// Function to draw the entire scene
void renderScene(void)
{
    // Set up the background color
    glClearColor(0.36, 0.36, 0.36, 1.0); 
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Enable lighting and color 
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Turn on and off light 0, light 1
    if (lightFlag0 % 2 == 0)
    {
        glEnable(GL_LIGHT0);
    }
    if (lightFlag0 % 2 == 1)
    {
        glDisable(GL_LIGHT0);
    }

    if (lightFlag1 % 2 == 0)
    {
        glEnable(GL_LIGHT1);
    }
    if (lightFlag1 % 2 == 1)
    {
        glDisable(GL_LIGHT1);
    }

    // Define light 0 and 1
    GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light1_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light1_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position[] = { 5.0, 5.0, 8.0, 0.0 };

    // Set up light 0 and 1
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    glShadeModel(GL_SMOOTH);

    // Define material properties
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    // Set up material properties
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set the camera centered at (eyePosX, eyePosY, eyePosZ) and looking along directional
    // vector (4, 4, 0), with the z-axis pointing up
    gluLookAt(
            eyePosX, eyePosY, eyePosZ,
            4.0, 4.0, 0.0,
            0.0, 0.0, 1.0);

    // Draw the chessboard
    drawChessboard();

    // Draw pawns
    for (int i = 0; i < 8; ++i)
    {
        drawPawn(0, pawn[i][1], pawn[i][2]);
    }
    for (int i = 8; i < 16; ++i)
    {
        drawPawn(1, pawn[i][1], pawn[i][2]);
    }

    // Draw rooks
    drawRook(0, rook[0][1], rook[0][2]);
    drawRook(0, rook[1][1], rook[1][2]);
    drawRook(1, rook[2][1], rook[2][2]);
    drawRook(1, rook[3][1], rook[3][2]);

    // Draw knights
    drawKnight(0, knight[0][1], knight[0][2]);
    drawKnight(0, knight[1][1], knight[1][2]);
    drawKnight(1, knight[2][1], knight[2][2]);
    drawKnight(1, knight[3][1], knight[3][2]);

    // Draw bishops
    drawBishop(0, bishop[0][1], bishop[0][2]);
    drawBishop(0, bishop[1][1], bishop[1][2]);
    drawBishop(1, bishop[2][1], bishop[2][2]);
    drawBishop(1, bishop[3][1], bishop[3][2]);

    // Draw queens
    drawQueen(0, queen[0][1], queen[0][2]);
    drawQueen(1, queen[1][1], queen[1][2]);

    // Draw kings
    drawKing(0, king[0][1], king[0][2]);
    drawKing(1, king[1][1], king[1][2]);

    // Make it all visible
    glutSwapBuffers(); 
}

//----------------------------------------------------------------------
// User-input callbacks
// ESC, q, and Q cause program to exit
// r, R rotate the chess set
// d, D move the eye location down z-axis
// u, U move the eye location up z-axis
// 0 turns the light 0 on and off
// 1 turns the light 1 on and off
// p, P move a random available pawn with a random move (forward and backward)
// k, K move a random available knight with a random move (8 kind of moves)
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q')
    {
        exit(0);
    }
    // Rotate
    if (key == 'r' || key == 'R')
    {
        rotation += 10;
    }
    // Moves the eye down
    if (key == 'd' || key == 'D')
    {
        eyePosZ -= 0.25;
    }
    // Moves the eye up
    if (key == 'u' || key == 'U')
    {
        eyePosZ += 0.25;
    }
    // Turn light0 on and off
    if (key == '0')
    {
        lightFlag0++;
    }
    // Turn light1 on and off
    if (key == '1')
    {
        lightFlag1++;
    }
    // Move the pawn
    if (key == 'p' || key == 'P')
    {
        while(1)
        {
            // Randomly choose the available piece and move
            pawnRand = rand() % 16;
            pawnMoveRand = rand() % 2;
            int temp;
            if (pawnMoveRand == 0) temp = 1;
            else temp = -1;
            if (pawn[pawnRand][2]+temp >= 0 && pawn[pawnRand][2]+temp <= 7)
            {
                if (chessBoard[pawn[pawnRand][2]+temp][pawn[pawnRand][1]] == 0)
                {
                    // Update the position of the piece and the chessboard map
                    chessBoard[pawn[pawnRand][2]][pawn[pawnRand][1]] = 0;
                    chessBoard[pawn[pawnRand][2]+temp][pawn[pawnRand][1]] = 1;
                    pawn[pawnRand][2] = pawn[pawnRand][2]+temp;
                    break;
                }
            }
        }

    }

    // Move the knight
    if (key == 'k' || key == 'K')
    {
        while(1)
        {
            // Randomly choose the available piece and move
            knightRand = rand() % 4;
            knightMoveRand = rand() % 8;
            int tempX, tempY;
            if (knightMoveRand == 0){tempX = 1; tempY = 2;}
            if (knightMoveRand == 1){tempX = 2; tempY = 1;}
            if (knightMoveRand == 2){tempX = -1; tempY = 2;}
            if (knightMoveRand == 3){tempX = -2; tempY = 1;}
            if (knightMoveRand == 4){tempX = 1; tempY = -2;}
            if (knightMoveRand == 5){tempX = 2; tempY = -1;}
            if (knightMoveRand == 6){tempX = -1; tempY = -2;}
            if (knightMoveRand == 7){tempX = -2; tempY = -1;}
            if (knight[knightRand][1]+tempX >= 0 && knight[knightRand][1]+tempX <= 7 && knight[knightRand][2]+tempY >= 0 && knight[knightRand][2]+tempY <= 7)
            {
                if (chessBoard[knight[knightRand][2]+tempY][knight[knightRand][1]+tempX] == 0)
                {
                    // Update the position of the piece and the chessboard map
                    chessBoard[knight[knightRand][2]][knight[knightRand][1]] = 0;
                    chessBoard[knight[knightRand][2]+tempY][knight[knightRand][1]+tempX] = 1;
                    knight[knightRand][1] = knight[knightRand][1]+tempX;
                    knight[knightRand][2] = knight[knightRand][2]+tempY;
                    break;
                }
            }
        }

    }
    if (key == 'e' || key == 'E')
    {
        if (enhanced == false) enhanced = true;
        else enhanced = false;
    }
}

// Main program  - standard GLUT initializations and callbacks
int main(int argc, char **argv)
{
    // General initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Enable the double buffering
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Chess");
    glEnable(GL_DEPTH_TEST);
    
    // Register callbacks
    glutReshapeFunc(changeSize); // window reshape callback
    glutDisplayFunc(renderScene); // (re)display callback
    glutIdleFunc(update); // incremental update
    glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
    glutKeyboardFunc(processNormalKeys); // process standard key clicks

    // Enter GLUT event processing cycle
    glutMainLoop();

    return 0; 
}