/*gcc main.c -o main -lm*/


#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


float cubeWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char charBuffer[160 * 44];
int background =  ' ';
int distanceFromCam = 100;
float incrementSpeed = 0.6;
float horizontalOffset;

float A, B, C, x, y, z, ooz, K1= 40;
int xp, yp, idx;

float calcX(int i, int j, int k)
{
    return j* sin(A) * sin(B) * cos(C)
           - k * cos(A) * sin(B) * cos(C)
           + j * cos(A) * sin(C) 
           + k * sin(A) * sin(C)
           + i * cos(B) * cos(C);  
}


float calcY(int i, int j, int k)
{
    return j* cos(A) * cos(C)
           + k * sin(A) * cos(C)
           - j * sin(A) * sin(C) * sin(B) 
           + k * cos(A) * sin(C) * sin(B)
           - i * cos(B) * sin(C);  
}


float calcZ(int i, int j, int k)
{
    return k* cos(A) * cos(B)
           - j * sin(A) * cos(B) 
           + i * sin(B);
}

void calculateSurface(float cubeX, float cubeY, float cubeZ, int ch)
{
    x = calcX(cubeX, cubeY, cubeZ);
    y = calcY(cubeX, cubeY, cubeZ);
    z = calcZ(cubeX, cubeY, cubeZ) + distanceFromCam;


    ooz = 1/z;

    xp = (int) (width / 2 + K1 * ooz * x * 2);  
    yp = (int) (height  / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if( idx >= 0 && idx < width * height)
    {
        if(ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            charBuffer[idx] = ch;
        }
    }
}



int main()
{
    printf("\x1b[2J");// limpa o terminal
    while(1)
    {
        memset(charBuffer, background, width * height);
        memset(zBuffer, 0, width * height * 4);


        for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
        {
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                calculateSurface(cubeX, cubeY, -cubeWidth, '#');
                //calculateSurface(cubeWidth, cubeY, cubeX, '.');
                //calculateSurface(-cubeWidth, cubeY, -cubeX, '$');
                //calculateSurface(-cubeX, cubeY, cubeWidth, '%');
                //calculateSurface(cubeX, -cubeWidth, -cubeY, '@');
                //calculateSurface(cubeX, cubeWidth, cubeY, '+');


            }
        }
        printf("\x1b[H"); // desloca o cursor
        for(int k = 0; k < width * height;k++)
        {
            //printf("\033[1;31m");// troca cor
            putchar(k % width ? charBuffer[k] : 10);
        }
        A += 0.005;
        B += 0.005;
        usleep(1000);

    }

    return 0;
}