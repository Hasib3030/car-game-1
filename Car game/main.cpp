#include<stdio.h>
//using namespace std;
//#include <bits/stdc++.h>
#include<windows.h>>
#include<GL/glut.h>
//#include<limits.h>
#include<iostream>
#define MAX_CARS 3

void drawCar();
void drawFootPath();
void drawDivider();
void drawSurroundings();
void stopGame();
void drawOtherCars(int);
void drawOther();
void setCars();
void moveOtherCars();
int detectCollision();
void drawString(std::string);
void drawMainMenu();
void drawExitMenu();
void drawTime();
void stopGame();
void resumeGame();
void drawScore(int);
void drawDistanceBar();
void drawEnd();
void moveEnd();
void drawFuelBar();
void fuelMessage();
void drawFuel();

int foot_y = 200;
int div_y = 220;
int end_y = 160;

int a = 0;
int speed = 1;
int steerSpeed = 1;
int temp = 0;
int seconds = 0;
int distance = 178;
int fuel = 178;

int game_state = 0;

void moveDivider();
void moveRoad();
void moveCar();
void periodicFunction(int);
void moveFuel();

int lane[MAX_CARS];
int pos[MAX_CARS];
int speeds[MAX_CARS];
int speedsTemp[MAX_CARS];

GLdouble width=1200, height=800;

bool carMoveLeft = false, carMoveRight = false, carMoveFast = false, carStopped = false, gameStopped = false, horn = false;
bool highlightStart = false, highlightExit = false;
bool reachedEnd= false, gameCompleted= false, fuelOver=false;
//car variables
int car_x=0;
int car_y = -100;

int fuel_x=0;
int fuel_y = -80;

void maindisp()
{

    glClear(GL_COLOR_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW);

    switch(game_state)
    {
            case 1 : glClearColor(0.5, 0.5, 0.5, 0.0);
                     drawFootPath();
                     drawSurroundings();
                     drawDivider();
                     //drawEnd();
                     drawCar();
                     drawOther();
                     drawTime();
                     drawDistanceBar();
                     drawFuelBar();
                     drawFuel();
                     break;

            case 0 : drawMainMenu();

                     break;

            case 2 : glClearColor(0.5, 0.5, 0.5, 0.0);
                     drawFootPath();
                     drawSurroundings();
                     drawDivider();
                     if(gameCompleted)
                        drawEnd();

                     if(fuelOver)
                        fuelMessage();

                     drawCar();
                     drawOther();
                     drawDistanceBar();
                     drawFuelBar();
                     drawExitMenu();
                     drawTime();
                     break;
    }

    glFlush();
    glutSwapBuffers();
}

void keyBoardFunc(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT: carMoveLeft = true, carMoveRight = false; break;
        case GLUT_KEY_RIGHT: carMoveRight = true, carMoveLeft = false; break;
        case GLUT_KEY_UP: carMoveFast = true; break;
    }
}

void keyboard_up_func(int k, int x, int y)
{
    switch(k)
    {
        case GLUT_KEY_LEFT: carMoveLeft = false;
                            break;
        case GLUT_KEY_RIGHT: carMoveRight = false;
                            break;

        case GLUT_KEY_UP: carMoveFast = false; break;
        case GLUT_KEY_DOWN: carStopped = false; break;
    }
}

void normalKeyBoardFunc (unsigned char key, int x, int y)
{
    if(game_state == 1)
    {
        switch(key)
        {
            //HORn
            case 'H':
            case 'h': horn = true; break;

        }
    }

    else if (game_state == 0)
    {
        if(key == 13)
        {
            setCars();
            game_state = 1;
        }
    }
}

void mouse_func(int button, int state, int x, int y)
{
    switch(game_state)
    {
        case 0 : if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                 {
                     if(x>= 790 && x <= 1150 && y >= 320 && y <= 487)
                     {
                         //gameStopped=false;
                         setCars();
                         game_state=1;
                     }

                    else if (x>=790 && x <= 1150 && y>=580 && y<=747)
                        exit(0);
                 }
                 break;

        case 2 : if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                 {
                     if(x>= 790 && x <= 1150 && y >= 320 && y <= 487)
                     {
                         gameStopped=false;
                         gameCompleted=false;
                         reachedEnd=false;
                         seconds=0;
                         distance=178;
                         fuel=178;
                         fuelOver=false;
                         end_y=160;
                         setCars();
                         game_state=1;
                         car_x = 0;
                     }

                     else if (x>=790 && x <= 1150 && y>=580 && y<=747)
                        exit(0);
                 }
                 break;



    }
}

void mouse_hover(int x, int y)
{

        if(x>= 790 && x <= 1150 && y >= 320 && y <= 487)
        {
           highlightStart = true;
           highlightExit = false;
        }

        else if (x>=790 && x <= 1150 && y>=580 && y<=747)
        {
            highlightStart = false;
            highlightExit = true;
        }

        else
        {
           highlightExit = false;
           highlightStart = false;
        }
}

void sound()
{

    //PlaySound("a.wav", NULL, SND_ASYNC|SND_FILENAME);
    PlaySound("BMW+DRIVEBY.wav", NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);

}

int main (int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize((int) width, (int) height);
    glutCreateWindow("Project101");
    glutFullScreen();


    gluOrtho2D(-240.0, 240.0, -160, 160);

    glutDisplayFunc(maindisp);
    glutTimerFunc(100, periodicFunction, 0);

    glutSpecialFunc(keyBoardFunc);
    glutSpecialUpFunc(keyboard_up_func);
    glutKeyboardFunc(normalKeyBoardFunc);
    glutMouseFunc(mouse_func);
    glutPassiveMotionFunc(mouse_hover);
    sound();
    glutMainLoop();
    return 0;
}

//this function draws a car at (car_x, car_y)
void drawCar(void)
{
    glPushMatrix();

    glTranslated(car_x, car_y, 0.0);

    glColor3f(0.34, 1.0, 1.0);

    glRectd(2,10,-2,-10);
    glRectf(-8,12,8,13);
    glRectf(-10.0,9.0,-8.0,15.0);
    glRectf(8.0,9.0,10.0,15.0);

    glBegin(GL_LINES);
    glVertex2f(8.0,11.0);
    glVertex2f(6.0,7.0);
    glVertex2f(6.0,7.0);
    glVertex2f(6.0,-7.0);
    glVertex2f(6.0,-7.0);
    glVertex2f(10.0,-12.0);
    glVertex2f(-8.0,11.0);
    glVertex2f(-6.0,7.0);
    glVertex2f(-6.0,7.0);
    glVertex2f(-6.0,-7.0);
    glVertex2f(-6.0,-7.0);
    glVertex2f(-10.0,-12.0);
    glVertex2f(10.0,-12.0);
    glVertex2f(-10.0,-12.0);
    glEnd();

    glRectf(-11.0,-16.0,-9.0,-8.0);
    glRectf(9.0,-16.0,11.0,-8.0);
    glRectf(-7.0,-14.0,-5.0,-12.0);
    glRectf(5.0,-14.0,7.0,-12.0);



    glPopMatrix();
}

void drawOtherCars(int i)
{
    glPushMatrix();

    glTranslated((lane[i] - 1)*37, pos[i], 0.0);

    switch(speeds[i])
    {
        case 2: glColor3f(0.0,1.0,0.0); break;
        case 3: glColor3f(0.0,0.0,1.0); break;
        case 4:
        case 5:
        case 6:
        case 7: glColor3f(1.0,0.0,0.0); break;
        case 0: glColor3f(0.0,0.0,0.0); break;
    }

    glRectd(2,10,-2,-10);
    glRectf(-8,12,8,13);
    glRectf(-10.0,9.0,-8.0,15.0);
    glRectf(8.0,9.0,10.0,15.0);

    glBegin(GL_LINES);
        glVertex2f(8.0,11.0);
        glVertex2f(6.0,7.0);
        glVertex2f(6.0,7.0);
        glVertex2f(6.0,-7.0);
        glVertex2f(6.0,-7.0);
        glVertex2f(10.0,-12.0);
        glVertex2f(-8.0,11.0);
        glVertex2f(-6.0,7.0);
        glVertex2f(-6.0,7.0);
        glVertex2f(-6.0,-7.0);
        glVertex2f(-6.0,-7.0);
        glVertex2f(-10.0,-12.0);
        glVertex2f(10.0,-12.0);
        glVertex2f(-10.0,-12.0);
    glEnd();

    glRectf(-11.0,-16.0,-9.0,-8.0);
    glRectf(9.0,-16.0,11.0,-8.0);
    glRectf(-7.0,-14.0,-5.0,-12.0);
    glRectf(5.0,-14.0,7.0,-12.0);

    glPopMatrix();
}

void drawFootPath()
{
    int i, y;
    glPushMatrix();

    //Draw right side foot path
    glTranslated(60,0,0);
    y=foot_y + 20;

    for(i=0;i<20;i++)
    {
        if(a==0)
        {
            if(i%2==0)
                glColor3f(1.0,1.0,1.0);
            else
                glColor3f(1.0,1.0,0.0);
        }

        else
        {
            if(i%2==1)
                glColor3f(1.0,1.0,1.0);
            else
                glColor3f(1.0,1.0,0.0);
        }


        y-=20;
        glRectd(5, y, -5, y-20);

        //1st tree
        glColor3ub(225,131,36);
        glRectd(1, 5, -1, -5);
        glColor3ub(44,225,36);
        glRectd(4, 6, -4, 3);
        glRectd(3, 8, -3, 6);
        glRectd(2,10,-2,8);
        glRectd(1,12,-1,10);

        //2nd Tree
        glColor3ub(225,131,36);
        glRectd(1, 85, -1, 75);
        glColor3ub(44,225,36);
        glRectd(4, 86, -4, 83);
        glRectd(3, 88, -3, 86);
        glRectd(2,90,-2,88);
        glRectd(1,92,-1,90);

        //3rd tree
        glColor3ub(225,131,36);
        glRectd(1, -85, -1, -75);
        glColor3ub(44,225,36);
        glRectd(1, -71, -1, -69);
        glRectd(2, -73, -2, -71);
        glRectd(3,-75,-3,-73);
        glRectd(4,-78,-4,-75);
    }
    glPopMatrix();

    //Draw left side footpath
    glPushMatrix();
    glTranslated(-60,0,0);

    y=foot_y + 20;
    for(i=0;i<20;i++)
    {
        if(a==0)
        {
            if(i%2==0)
                glColor3f(1.0,1.0,1.0);
            else
                glColor3f(1.0,1.0,0.0);
        }

        else
        {
            if(i%2==1)
                glColor3f(1.0,1.0,1.0);
            else
                glColor3f(1.0,1.0,0.0);
        }


        y-=20;
        glRectd(5, y, -5, y-20);

        //1st tree
        glColor3ub(225,131,36);
        glRectd(1, 5, -1, -5);
        glColor3ub(44,225,36);
        glRectd(4, 6, -4, 3);
        glRectd(3, 8, -3, 6);
        glRectd(2,10,-2,8);
        glRectd(1,12,-1,10);

        //2nd Tree
        glColor3ub(225,131,36);
        glRectd(1, 85, -1, 75);
        glColor3ub(44,225,36);
        glRectd(4, 86, -4, 83);
        glRectd(3, 88, -3, 86);
        glRectd(2,90,-2,88);
        glRectd(1,92,-1,90);

        //3rd tree
        glColor3ub(225,131,36);
        glRectd(1, -85, -1, -75);
        glColor3ub(44,225,36);
        glRectd(1, -71, -1, -69);
        glRectd(2, -73, -2, -71);
        glRectd(3,-75,-3,-73);
        glRectd(4,-78,-4,-75);

    }
    glPopMatrix();

}

void drawDivider()
{
    int i;
    int y=div_y + 80;
    glColor3f(1.0,1.0,1.0);

    for(i=0;i<8;i++)
    {
        y-=80;
        glRectd(22, y, 18, y-40);
        glRectd(-18, y, -22, y-40);
    }

}

void drawSurroundings()
{
    glColor3f(0.0,1.0,0.0);
    glRectd(240,160,65,-160);
    glRectd(-240,160,-65,-160);
}

void stopGame()
{
    speed=0;
    steerSpeed=0;
}

void resumeGame()
{
    speed = 2;
    steerSpeed = 1;
}

void drawOther()
{
    int i;
    for(i=0;i< MAX_CARS; i++)
    {
        drawOtherCars(i);
    }
}

void setCars()
{
    int i;
    for(i=0;i<MAX_CARS; i++)
    {
        lane[i] = i;
        pos[i] =110+ rand()%100;
        //pos[i] =rand();
        speeds[i] = 1+i+rand()%4;
        //speeds[i] = 1+i+rand();
    }
}

void moveDivider()
{
    div_y -= speed;
    if(div_y <30 && distance >0)
    //if(distance >0)
    {
        div_y = 200;

        if(carMoveFast)
        {
            distance -= 1.5;
            fuel -=4;
        }
        else
        {

            distance -=0.5;
            fuel -=2;
        }

        if(fuel < 0)
            fuelOver = true;
        if(distance < 5)
            reachedEnd = true;


    }
}

void moveCar()
{
    if(carMoveLeft)
        car_x -=steerSpeed;


    else if(carMoveRight)
        car_x +=steerSpeed;

    if(car_x >45 || car_x <-45)
    {
        game_state =2;
        gameStopped =true;
    }
}

void moveRoad()
{
    foot_y -= speed;
    if(foot_y < 160)
    {
        foot_y = 180;
        if(a==0)
            a=1;
        else
            a=0;
    }
}

void periodicFunction(int v)
{
    if(gameStopped)
        stopGame();

    else
        resumeGame();

    if(speed !=0)
    {
        if(carMoveFast)
            speed = 6;

        else
            speed = 2;

    }

    if(fuelOver)
    {
        gameStopped = true;
        game_state = 2;

    }

    moveRoad();
    moveDivider();
    moveCar();
    moveOtherCars();
    moveEnd();
    moveFuel();

    if(!detectCollision() && !gameStopped && game_state ==1)
        temp += 15;

    if(temp > 1000)
    {
        temp =0;
        seconds++;
    }

    glutPostRedisplay();
    glutTimerFunc(10, periodicFunction, v);
}

void moveOtherCars()
{
    int i;
    for(i =0;i<MAX_CARS;i++)
    {
        pos[i] += -speed +speeds[i];
        //pos[i] += rand();

        if(pos[i] <-200 || pos[i] > 200)
        {
            pos[i] = 200+ rand()%100;
            speeds[i] =2+rand()%4;
        }
    }

    if(horn)
    {
        speeds[(car_x + 60)/40]++;
        if(speeds[(car_x + 60)/40]>7)
            speeds[(car_x + 60)/40] = 7;
        horn = false;
    }
}

int detectCollision()
{
  //  cout<<game_state;
    if(game_state != 1)
        return 0;

    int i, limit;
    for(i=0;i<MAX_CARS;i++)
    {
        if(pos[i] <-70 && pos[i]>-130)
        {
            limit = (i - 1)*40;
           // cout<<i << ":" << car_x;
            if(car_x < limit + 22 && car_x >limit - 22)
            {
                speeds[i] = 0;
                gameStopped = true;
                game_state = 2;
                return 1;
            }
        }
    }

    if((fuel_x > car_x && fuel_x - car_x < 20) || (fuel_x < car_x && car_x - fuel_x < 20))
    {
        if(fuel_y < -80 && fuel_y > -120)
        {
            fuel+=40;
            if(fuel > 178)
                fuel = 178;
            fuel_y = 600 + rand()%150;
            fuel_x = (rand()%3 - 1)*37;
        }
    }

    return 0;

}

void draw_string(std::string str)
{
    for (unsigned int i=0;i<str.length();i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *(str.begin()+i));
    }
}

void drawMainMenu()
{
    //draw start button
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    drawFootPath();
    drawSurroundings();
    drawDivider();
    drawCar();

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
       glVertex2f(30, 15);
       glVertex2f(30, -15);
       glVertex2f(-30, -15);
       glVertex2d(-30, 15);
       glEnd();
       glPopMatrix();

       glPushMatrix();
       glTranslated(-20, 30, 0);
       glScalef(0.1, 0.1, 0.1);
       glColor3f(1.0, 1.0, 1.0);
       draw_string("START");

       glPopMatrix();

       //draw exit button
       glColor3f(1.0, 1.0, 1.0);

       glPushMatrix();
       glTranslated(0, -30, 0);
       glBegin(GL_LINE_LOOP);
         glVertex2f(30, 15);
         glVertex2f(30, -15);
         glVertex2f(-30, -15);
         glVertex2d(-30, 15);
       glEnd();
       glPopMatrix();

       glPushMatrix();
       glTranslated(-15, -30, 0);
       glScalef(0.1, 0.1, 0.1);
       draw_string("EXIT");

       glPopMatrix();

       if(highlightStart)
       {
           glColor3f(1.0, 0.0, 0.0);

           glPushMatrix();
           glTranslated(0, 30, 0);
           glBegin(GL_LINE_LOOP);
              glVertex2f(35, 20);
              glVertex2f(35, -20);
              glVertex2f(-35, -20);
              glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();

       }

       if(highlightExit)
       {
           glColor3f(1.0, 0.0, 0.0);

           glPushMatrix();
           glTranslated(0, -30, 0);
           glBegin(GL_LINE_LOOP);
              glVertex2f(35, 20);
              glVertex2f(35, -20);
              glVertex2f(-35, -20);
              glVertex2d(-35, 20);
          glEnd();
          glPopMatrix();

       }
}

void drawExitMenu()
{
    //draw start button
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
      glVertex2f(30, 15);
      glVertex2f(30, -15);
      glVertex2f(-30,-15);
      glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-25, 30, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("RESTART");

    glPopMatrix();

    //Draw exit button
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(0, -30, 0);
    glBegin(GL_LINE_LOOP);
       glVertex2f(30, 15);
       glVertex2f(30, -15);
       glVertex2f(-30, -15);
       glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-15, -30, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("EXIT");

    glPopMatrix();

    if(highlightStart)
    {
        glColor3f(1.0, 1.0, 1.0);

        glPushMatrix();
        glTranslated(0, 30, 0);
        glBegin(GL_LINE_LOOP);
          glVertex2f(35, 20);
          glVertex2f(35, -20);
          glVertex2f(-35, -20);
          glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();

    }

    if(highlightExit)
    {
        glColor3f(1.0, 1.0, 1.0);

        glPushMatrix();
        glTranslated(0, -30, 0);
        glBegin(GL_LINE_LOOP);
          glVertex2f(35, 20);
          glVertex2f(35, -20);
          glVertex2f(-35, -20);
          glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();

    }
}

void drawTime()
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(-200, 90, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(50,15);
        glVertex2f(50,-15);
        glVertex2f(-30,-15);
        glVertex2f(-30,15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-220,85 , 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("Time: ");
    glPopMatrix();

    glPushMatrix();
    glTranslated(-180,85 , 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 0.0, 0.0);
    //glutStrokeCharacter(GLUT_STROKE_ROMAN, '4');
    drawScore(seconds);
    glPopMatrix();
}

void drawScore(int score)
{
    int temp=score;
    int str[20], i=0;

    while(temp>0)
    {
        str[i++] = (temp%10);
        temp/=10;
    }
    i--;
    while(i>=0)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i--]+'0');
    }
    if(score == 0) glutStrokeCharacter(GLUT_STROKE_ROMAN, '0');
}

void drawDistanceBar()
{
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);

    glTranslated(-75, 40, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 90);
        glVertex2f(5, -90);
        glVertex2f(-5, -90);
        glVertex2f(-5, 90);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-75, -49, 0);
    glColor3ub(153, 0.0, 0.0);
    glRectd(4, 0, -4, distance);
    glPopMatrix();

}

void drawFuelBar()
{
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);

    glTranslated(75, 40, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 90);
        glVertex2f(5, -90);
        glVertex2f(-5, -90);
        glVertex2f(-5, 90);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(75, -49, 0);
    glColor3ub(0.6, 255.0, 255.0);
    glRectd(4, 0, -4, fuel);
    glPopMatrix();

}

void drawEnd()
{
    int i,j;
    for(i=0; i<5; i++)
    {
        if(i % 2 == 0)
            glColor3f(0.0, 0.0, 0.0);
        else
            glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();

        glTranslated(-55, end_y + 10*i, 0);

        for(j=0; j<11; j++)
        {
            if(i%2)
            {
                if(j%2 == 0)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);

            }

            else
            {
                if (j % 2)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);
            }

            glRectd(10 * j, 0, 10*(j+1), 10);
        }

        glPopMatrix();
    }
}

void fuelMessage()
{
    glPushMatrix();
    glTranslated(75, -70, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 0.0, 0.0);
    draw_string("Fuel Exhausted\n");
    glPopMatrix();
}

void moveEnd()
{
    if(reachedEnd)
        end_y-=speed;

    if(end_y < -125)
    {
        gameStopped=true;
        gameCompleted=true;
        game_state=2;
    }
}

void drawFuel()
{
    glPushMatrix();
    glTranslated(fuel_x, fuel_y, 0);
    glColor3f(0.0, 1.0, 0.0);
    glRectf(5, 5, -5, -5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(fuel_x, fuel_y+5, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 0);
        glVertex2f(5, 5);
        glVertex2f(-3, 5);
        glVertex2f(-5, 0);
    glEnd();
    glPopMatrix();

}

void moveFuel()
{
    fuel_y-=speed;

    if(fuel_y < -200)
    {
        fuel_y = 600 + rand()%150;
        fuel_x = (rand()%3 -1)*37;
    }
}
