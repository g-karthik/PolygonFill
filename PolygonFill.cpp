/*
Name: Karthik Gopalakrishnan
Roll: 1101CS21

Computer Graphics Lab Assignment 3
*/

#define GLUT_STATIC
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
using namespace std;

int noOfEdges = 0;
int colorCode = 1;

typedef struct Vertex
{
       float x, y;
}Vertex;

typedef struct Edge
{
        Vertex p, q;
}Edge;

Vertex *vertices;
Edge *edges;
Edge *activeEdges;

float yMin, yMax;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); //clear all pixels
    
    int j, k, ctr, tmp, swapped;
    float i;
    
    if(colorCode==1)                    //NOTE: This is for filling the polygon
    glColor3f(1.0, 0.0, 0.0);
    else if(colorCode==2)
    glColor3f(0.0, 1.0, 0.0);
    else
    glColor3f(0.0, 0.0, 1.0);
    
    glBegin(GL_POINTS);
    
    for(i=yMin; i<=yMax; i = i + 0.01)
    {
          ctr = 0;
          for(j=0;j<noOfEdges;j++)
          {
               if((edges[j].p.y <= i && edges[j].q.y > i) || (edges[j].p.y < i && edges[j].q.y >= i) || (edges[j].q.y <= i && edges[j].p.y > i) || (edges[j].q.y < i && edges[j].p.y >= i))
               {
                                activeEdges[ctr] = edges[j];
                                ctr++;
               }
          }
          
          Vertex *intersectionPoints;
          intersectionPoints = (Vertex*)malloc(ctr*sizeof(Vertex));
          
          for(k=0;k<ctr;k++)
          {
              intersectionPoints[k].y = i;
              intersectionPoints[k].x = activeEdges[k].p.x + (i - activeEdges[k].p.y)*(activeEdges[k].q.x - activeEdges[k].p.x)/(activeEdges[k].q.y - activeEdges[k].p.y);
          }
          
          tmp = ctr;
          do
          {
              swapped = 0;
              for(k=1; k<=tmp-1; k++)
              {
                       if(intersectionPoints[k-1].x > intersectionPoints[k].x)
                       {
                            swap(intersectionPoints[k-1], intersectionPoints[k]);
                            swap(activeEdges[k-1], activeEdges[k]);
                            swapped = 1;
                       }
              }
              tmp = tmp - 1;
          }
          while(swapped);
          
          //activeEdges and intersectionPoints must now be sorted in increasing order of 'x'
          
          int parity = 0;
          float x = intersectionPoints[0].x + 1;
          
          for(k=1;k<ctr;k++)
          {
                if(intersectionPoints[k-1].x == intersectionPoints[k].x)         //if both points are the same
                {
                      float x0 = activeEdges[k].p.x;
                      float y0 = activeEdges[k].p.y;
                      float x1 = activeEdges[k].q.x;
                      float y1 = activeEdges[k].q.y;
                      
                      float x2 = activeEdges[k+1].p.x;
                      float y2 = activeEdges[k+1].p.y;
                      float x3 = activeEdges[k+1].q.x;
                      float y3 = activeEdges[k+1].q.y;
                      
                      if((y0==i && y1 > i) || (y0 > i && y1==i))
                      {
                                parity = 1;
                                if((y2==i && y3 > i) || (y2 > i && y3==i))
                                {
                                          parity = 0;
                                }
                      }
                }
                else
                {
                    if(parity==1)
                    parity = 0;
                    else
                    parity = 1;
                }
                
                while(x<intersectionPoints[k].x)
                {
                      if(parity==1)
                      glVertex2f(x, i);
                      
                      x = x + 0.01;
                }
                x = x + 0.01;
          }
    }
    
    glEnd();
    
    if(colorCode==1)                      //NOTE: This is for the edges
    glColor3f(0.0, 1.0, 0.0);
    else if(colorCode==2)
    glColor3f(0.0, 0.0, 1.0);
    else
    glColor3f(1.0, 0.0, 0.0);
    
    glBegin(GL_LINES);
    
    for(j=0;j<noOfEdges;j++)
    {
         glVertex2f(edges[j].p.x, edges[j].p.y);
         glVertex2f(edges[j].q.x, edges[j].q.y);
    }
    
    glEnd();
    
    glFlush(); //start processing buffered OpenGL routines
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0); /* select clearing color */
    glMatrixMode(GL_PROJECTION); /* initialize viewing values */
    glLoadIdentity();
    glOrtho(-250.0, 250.0, -250.0, 250.0, -1.0, 1.0);
}

/* Declare initial window size, position, and display mode. Call initialization routines. Register callback function
to display graphics. Enter main loop and process events. */
int main(int argc, char** argv) {
    
    FILE *fp;
    char *line = (char*)malloc(100*sizeof(char));
    
    fp = fopen("input.txt", "r");
    if(fp == NULL)
    exit(EXIT_FAILURE);
    
    fgets(line, 100, fp);
    noOfEdges = atoi(line);
    //cout<<noOfEdges<<"\n";
    
    vertices = (Vertex*)malloc(noOfEdges*sizeof(Vertex));
    edges = (Edge*)malloc(noOfEdges*sizeof(Edge));
    activeEdges = (Edge*)malloc(noOfEdges*sizeof(Edge)); //maximum number of active edges = noOfEdges
    int i;
    char *pch;
    
    yMin = INT_MAX;
    yMax = INT_MIN;
    
    for(i=0;i<noOfEdges;i++)
    {
         fgets(line, 100, fp);
         pch = strtok(line, " ");
         vertices[i].x = atof(pch);
         pch = strtok(NULL, " ");
         vertices[i].y = atof(pch);
         //cout<<vertices[i].x<<" "<<vertices[i].y<<"\n";
         
         if(vertices[i].y < yMin)
         yMin = vertices[i].y;
         
         if(vertices[i].y > yMax)
         yMax = vertices[i].y;
    }
    
    fgets(line, 100, fp);
    colorCode = atoi(line);
    //cout<<colorCode;
    
    edges[0].p = vertices[0];
    
    for(i=1;i<noOfEdges;i++)
    {
          edges[i-1].q = vertices[i];
          edges[i].p = vertices[i];
    }
    
    edges[i-1].q = vertices[0];
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Polygon Fill");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
