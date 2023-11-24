#include <stdio.h>
#include <stdlib.h>
#include "graphics.c"
#include <math.h>
#include <time.h>

const int NUMOFSQUARES = 8;
const int WINDOWWIDTH = 640;
const int SQUAREWIDTH = (WINDOWWIDTH * 0.9) / NUMOFSQUARES;
const int PAUSETIME = 100;

typedef struct listItem
{
    int x;
    int y;
    struct listItem *next;
} list;

typedef struct square
{
    int x;
    int y;
    int type;
    int visited;
} square;

typedef struct robot
{
    int x;
    int y;
    int direction;
    int marker;
} robot;

square homePos = {0, 0, 1, 1};
square board[NUMOFSQUARES][NUMOFSQUARES];

int atHome(robot *r)
{
    if (r->x == homePos.x && r->y == homePos.y)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

double findDistance(int x1, int y1, int x2, int y2)
{
    double distance = sqrt((x1-x2)^2 + (y1-y2)^2);
    return distance;
}

void createBoard(square home)
{
    int x = 0;
    int y = 0;
    //squares stored in board[x][y]
    for (x=0; x<NUMOFSQUARES; x++)
    {
        for (y=0; y<NUMOFSQUARES; y++)
        {
            board[x][y].x = x;
            board[x][y].y = y;
            board[x][y].type = 0;
            board[x][y].visited = 0;
        }
    }
    board[home.x][home.y] = home;
}

void drawMarker(square s)
{
    //DRAWS THE MARKER AT THE SPECIFIED LOCATION
    if (s.type == 0) //this is no marker - normal empty square
    {
        setColour(white);
        fillRect(0.05*WINDOWWIDTH + (s.x)*SQUAREWIDTH, 0.05*WINDOWWIDTH + (s.y)*SQUAREWIDTH, SQUAREWIDTH, SQUAREWIDTH);
    }
    if (s.visited == 1)
    {
        setColour(red);
        fillRect(0.05*WINDOWWIDTH + (s.x)*SQUAREWIDTH, 0.05*WINDOWWIDTH + (s.y)*SQUAREWIDTH, SQUAREWIDTH, SQUAREWIDTH);
    }
    if (s.type == 1) //this is the home marker
    {
        setColour(blue);
        fillRect(0.05*WINDOWWIDTH + (s.x)*SQUAREWIDTH, 0.05*WINDOWWIDTH + (s.y)*SQUAREWIDTH, SQUAREWIDTH, SQUAREWIDTH);
    }
    if (s.type == 2) //this is the collectible marker
    {
        setColour(gray);
        fillRect(0.05*WINDOWWIDTH + (s.x)*SQUAREWIDTH, 0.05*WINDOWWIDTH + (s.y)*SQUAREWIDTH, SQUAREWIDTH, SQUAREWIDTH);        
    }
    if (s.type == 3)
    {//immovable obstacle
        setColour(black);
        fillRect(0.05*WINDOWWIDTH + (s.x)*SQUAREWIDTH, 0.05*WINDOWWIDTH + (s.y)*SQUAREWIDTH, SQUAREWIDTH, SQUAREWIDTH);
    }
    if (s.type > 3 && s.type%2==0) //multiple collectible markers
    {
        setColour(gray);
        fillRect(0.05*WINDOWWIDTH + (s.x)*SQUAREWIDTH, 0.05*WINDOWWIDTH + (s.y)*SQUAREWIDTH, SQUAREWIDTH, SQUAREWIDTH);
    }
}

void drawBoard()
{
    setColour(black);
    //DRAWS THE GRID LINES FOR INITIAL BOARD
    double i = 0.05;
    for (i=0.05; i<0.95; i+=0.9/NUMOFSQUARES)
    {
        drawLine(i*WINDOWWIDTH, 0.05*WINDOWWIDTH, i*WINDOWWIDTH, 0.95*WINDOWWIDTH);
        drawLine(0.05*WINDOWWIDTH, i*WINDOWWIDTH, 0.95*WINDOWWIDTH, i*WINDOWWIDTH);
    }
    drawLine(0.05*WINDOWWIDTH, 0.95*WINDOWWIDTH, 0.95*WINDOWWIDTH, 0.95*WINDOWWIDTH);
    drawLine(0.95*WINDOWWIDTH, 0.05*WINDOWWIDTH, 0.95*WINDOWWIDTH, 0.95*WINDOWWIDTH);
    //DRAWS THE HOME SQUARE
    drawMarker(homePos);
}

void drawRobot(robot r)
{
    drawMarker(board[r.x][r.y]);
    if (r.direction < 0)
    {
        r.direction += 360;
    }
    if (r.direction > 360)
    {
        r.direction -= 360;
    }
    setColour(green);
    int* xs; 
    int* ys;
    xs = (int*) malloc (3 * sizeof(int));
    ys = (int*) malloc (3 * sizeof(int));
    //DRAWS THE ROBOT AT THE SPECIFIED LOCATION - angle from north
    if ((r.direction%360) == 0)
    {
        xs[0] = 0.05*WINDOWWIDTH + ((r.x))*SQUAREWIDTH;
        xs[1] = 0.05*WINDOWWIDTH + (r.x)*SQUAREWIDTH + SQUAREWIDTH/2;
        xs[2] = 0.05*WINDOWWIDTH + ((r.x)+1)*SQUAREWIDTH;
        ys[0] = 0.05*WINDOWWIDTH + ((r.y)+1)*SQUAREWIDTH;
        ys[1] = 0.05*WINDOWWIDTH + (r.y)*SQUAREWIDTH;
        ys[2] = 0.05*WINDOWWIDTH + ((r.y)+1)*SQUAREWIDTH;
    };
    if (r.direction == 90)
    {
        xs[0] = 0.05*WINDOWWIDTH + (r.x)*SQUAREWIDTH;
        xs[1] = 0.05*WINDOWWIDTH + (r.x)*SQUAREWIDTH;
        xs[2] = 0.05*WINDOWWIDTH + ((r.x)+1)*SQUAREWIDTH;
        ys[0] = 0.05*WINDOWWIDTH + (r.y)*SQUAREWIDTH;
        ys[1] = 0.05*WINDOWWIDTH + ((r.y)+1)*SQUAREWIDTH;
        ys[2] = 0.05*WINDOWWIDTH + ((r.y))*SQUAREWIDTH + SQUAREWIDTH/2;
    };
    if (r.direction == 180)
    {
        xs[0] = 0.05*WINDOWWIDTH + ((r.x))*SQUAREWIDTH;
        xs[1] = 0.05*WINDOWWIDTH + ((r.x)+1)*SQUAREWIDTH;
        xs[2] = 0.05*WINDOWWIDTH + (r.x)*SQUAREWIDTH + SQUAREWIDTH/2;
        ys[0] = 0.05*WINDOWWIDTH + (r.y)*SQUAREWIDTH;
        ys[1] = 0.05*WINDOWWIDTH + (r.y)*SQUAREWIDTH;
        ys[2] = 0.05*WINDOWWIDTH + ((r.y)+1)*SQUAREWIDTH;
    };
    if (r.direction == 270)
    {
        xs[0] = 0.05*WINDOWWIDTH + ((r.x)+1)*SQUAREWIDTH;
        xs[1] = 0.05*WINDOWWIDTH + ((r.x)+1)*SQUAREWIDTH;
        xs[2] = 0.05*WINDOWWIDTH + (r.x)*SQUAREWIDTH;
        ys[0] = 0.05*WINDOWWIDTH + (r.y)*SQUAREWIDTH;
        ys[1] = 0.05*WINDOWWIDTH + ((r.y)+1)*SQUAREWIDTH;
        ys[2] = 0.05*WINDOWWIDTH + (r.y)*SQUAREWIDTH + SQUAREWIDTH/2;
    };
    
    fillPolygon(3,xs,ys);
    free(xs);
    free(ys);
}

void right(robot *r)
{
    r->direction += 90;
    if (r->direction >= 360)
    {
        r->direction -= 360;
    }
    sleep(50);
    drawRobot(*r);
}

void left(robot *r)
{
    r->direction -= 90;
    if (r->direction < 0)
    {
        r->direction += 360;
    }
    sleep(50);
    drawRobot(*r);
}

int canMoveForward(robot *r)
{
    if (r->direction == 0 && (r->y > 0) && (board[r->x][r->y-1].type != 3))
    {
        return 1;
    }
    if (r->direction == 90 && (r->x < NUMOFSQUARES-1) && (board[r->x+1][r->y].type != 3))
    {
        return 1;
    }
    if (r->direction == 180 && (r->y < NUMOFSQUARES-1) && (board[r->x][r->y+1].type != 3))
    {
        return 1;
    }
    if (r->direction == 270 && (r->x > 0) && (board[r->x-1][r->y].type != 3))
    {
        return 1;
    }
    return 0;
}

void move(robot *r)
{
    if (canMoveForward(r) == 1)
    {
        if (r->direction == 0 && (r->y > 0) && (board[r->x][r->y-1].type != 3))
        {
            r->y -= 1;
        }
        if (r->direction == 90 && (r->x < NUMOFSQUARES-1) && (board[r->x+1][r->y].type != 3))
        {
            r->x += 1;
        }
        if (r->direction == 180 && (r->y < NUMOFSQUARES-1) && (board[r->x][r->y+1].type != 3))
        {
            r->y += 1;
        }
        if (r->direction == 270 && (r->x > 0) && (board[r->x-1][r->y].type != 3))
        {
            r->x -= 1;
        }
    }
}

void forward(robot *r)
{
    drawMarker(board[r->x][r->y]);
    move(r);
    for (int i=0; i<NUMOFSQUARES; i++)
    {
        for (int j=0; j<NUMOFSQUARES; j++)
        {
            drawMarker(board[i][j]);
        }
    }
    drawRobot(*r);
    drawBoard();
    board[r->x][r->y].visited = 1;
}

void dropMarker(robot *r)
{
    if (r->marker > 0)
    {
        r->marker -= 1;
        board[r->x][r->y].type += 2;
        drawMarker(board[r->x][r->y]);
        drawRobot(*r);
    }
}

list* addToList(list *l, int x, int y)
{
    //get next allocated address in list and add the new item, then return new item address
    list* new = l+1;
    new->x = x;
    new->y = y;
    new->next = NULL;
    l->next = new;
    return new;
}

int isBoardfullyVisited(void)
{
    //check if all squares on board are visited
    int i = 0;
    int j = 0;
    for (i=0; i<NUMOFSQUARES; i++)
    {
        for (j=0; j<NUMOFSQUARES; j++)
        {
            if (board[i][j].visited == 0)
            {
                printf("square (%d, %d) is not visited\n", i, j);
                return 0;
            }
        }
    }
    return 1;
}

square findNearestUnvisitedSquare(int x, int y)
{
    //find the nearest unvisited square to the current position
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    double *distance = malloc(sizeof(double));
    *distance = NUMOFSQUARES^2;
    double *checkDistance = malloc(sizeof(double));
    *checkDistance = 0.0;
    square nearestSquare;
    for (*i=0; *i<NUMOFSQUARES; (*i)++)
    {
        for (*j=0; *j<NUMOFSQUARES; (*j)++)
        {
            if (board[*i][*j].visited == 0)
            {
                *checkDistance = findDistance(x,y,*i,*j);
                if (*checkDistance <= *distance)
                {
                    *distance = *checkDistance;
                    nearestSquare = board[*i][*j];
                }
            }
        }
    }
    free(i);
    free(j);
    free(distance);
    free(checkDistance);
    return nearestSquare;
}

int atMarker(square s)
{
    //check if robot is at marker
    if (s.type == 2 || (s.type > 2 && s.type % 2 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

list* pickUpMarker(int currentX, int currentY, robot *r, list *listNext)
{
    list* listReturn = listNext;
    if (atMarker(board[currentX][currentY]) == 1)
    {
        //if the square has a marker, add to list
        if (board[currentX][currentY].type == 2)
        {
            listReturn = addToList(listNext, r->x, r->y);
            board[currentX][currentY].type = 0;
            drawMarker(board[currentX][currentY]);
            drawBoard();
            drawRobot(*r);
            r->marker += 1;
            printf("\nmarker found at x: %d, y: %d\n", r->x, r->y);
        }
        //if square has multiple markers
        else if (board[currentX][currentY].type > 2 && board[currentX][currentY].type % 2 == 0)
        {
            int i;
            for (i=(board[currentX][currentY].type/2); i>0; i--)
            {
                listReturn = addToList(listNext, r->x, r->y);
                board[currentX][currentY].type -= 2;
                r->marker += 1;
                printf("\nmarker found at x: %d, y: %d\n", r->x, r->y);
            }
            drawMarker(board[currentX][currentY]);
            drawBoard();
            drawRobot(*r);
        }
    }
    return listReturn;
}

list* checkIfMarkerAtSquare(int currentX, int currentY, robot *r, list *listOfMarkers)
{
    list* listNext = listOfMarkers;
    if (atMarker(board[currentX][currentY]) == 1)
    {
        listNext = pickUpMarker(currentX, currentY, r, listOfMarkers);
    }
    return listNext;
}

list* moveToSquare(square s, robot *r, list *listOfMarkers)
{
    list *listReturn = listOfMarkers;
    list *temp = malloc(sizeof(list));
    int *counter = malloc(sizeof(int));
    *counter = 0;
    //move from current pos to square s, while avoiding obstacles
    while ((r->x!=s.x || r->y!=s.y) && s.x < NUMOFSQUARES && s.y < NUMOFSQUARES)
    {
        printf("\nmoving to square (%d, %d) from (%d, %d)\n", s.x, s.y, r->x, r->y);
        if (r->x < s.x)
        {
            while (r->direction != 90)
            {
                right(r);
            }
            while (r->x != s.x)
            {
                if (board[r->x+1][r->y].type == 3)
                {
                    if (r->y > 0 && *counter < (NUMOFSQUARES-1))
                    {
                        left(r);
                        forward(r);
                        sleep(PAUSETIME);
                        right(r);
                        (*counter)++;
                    }
                    else
                    {
                        right(r);
                        forward(r);
                        sleep(PAUSETIME);
                        left(r);
                    }  
                }
                else
                {
                    forward(r);
                    sleep(PAUSETIME);
                }
                temp = checkIfMarkerAtSquare(r->x, r->y, r, listReturn);
                if (temp != NULL)
                {
                    listReturn = temp;
                }
            }
            
        }

        if (r->x > s.x)
        {
            while (r->direction != 270)
            {
                left(r);
            }
            while (r->x != s.x)
            {
                if (board[r->x-1][r->y].type == 3)
                {
                    if (r->y > 0 && *counter < (NUMOFSQUARES-1))
                    {
                        left(r);
                        forward(r);
                        sleep(PAUSETIME);
                        right(r);
                        (*counter)++;
                    }
                    else
                    {
                        right(r);
                        forward(r);
                        sleep(PAUSETIME);
                        left(r);
                    }  
                }
                else
                {
                    forward(r);
                    sleep(PAUSETIME);
                }
                temp = checkIfMarkerAtSquare(r->x, r->y, r, listReturn);
                if (temp != NULL)
                {
                    listReturn = temp;
                }
            }
        }

        if (r->y < s.y)
        {
            while (r->direction != 180)
            {
                right(r);
            }
            while (r->y != s.y)
            {
                if (board[r->x][r->y+1].type == 3)
                {
                    if (r->y > 0 && *counter < (NUMOFSQUARES-1))
                    {
                        left(r);
                        forward(r);
                        sleep(PAUSETIME);
                        right(r);
                        (*counter)++;
                    }
                    else
                    {
                        right(r);
                        forward(r);
                        sleep(PAUSETIME);
                        left(r);
                    }  
                }
                else
                {
                    forward(r);
                    sleep(PAUSETIME);
                }
                temp = checkIfMarkerAtSquare(r->x, r->y, r, listReturn);
                if (temp != NULL)
                {
                    listReturn = temp;
                }
            }
        }

        if (r->y > s.y)
        {
            while (r->direction != 0)
            {
                left(r);
            }
            while (r->y != s.y)
            {
                if (board[r->x][r->y-1].type == 3)
                {
                    if (r->y > 0 && (*counter) < (NUMOFSQUARES-1))
                    {
                        left(r);
                        forward(r);
                        sleep(PAUSETIME);
                        right(r);
                        (*counter)++;
                    }
                    else
                    {
                        right(r);
                        forward(r);
                        sleep(PAUSETIME);
                        left(r);
                    }  
                }
                else
                {
                    forward(r);
                    sleep(PAUSETIME);
                }
                temp = checkIfMarkerAtSquare(r->x, r->y, r, listReturn);
                if (temp != NULL)
                {
                    listReturn = temp;
                }
            }
        }
    }
    free(counter);
    return listReturn;
}

list* markerFind(robot *r, list *listStart, int depth)
{
    //mark current square as visited and check if all on board are visited
    int *currentX = malloc(sizeof(int));
    int *currentY = malloc(sizeof(int));
    *currentX = r->x;
    *currentY = r->y;
    board[*currentX][*currentY].visited = 1;

    //check if marker is at current square
    list *listNext = listStart;
    int i = 0;
    int j = 0;
    list *temp1 = checkIfMarkerAtSquare(*currentX, *currentY, r, listStart);
    if (temp1 != NULL)
    {
        listNext = temp1;
    }

    int *count = malloc(sizeof(int));
    *count = 0;
    //checks for unvisited adjacent squares
    //printf("call %d\n",depth);
    do
    {
        if ((r->direction == 270 && *currentX > 0 && board[*currentX-1][*currentY].visited == 0 ||
        r->direction == 90 && *currentX < NUMOFSQUARES-1 && board[*currentX+1][*currentY].visited == 0 ||
        r->direction == 0 && *currentY > 0 && board[*currentX][*currentY-1].visited == 0 ||
        r->direction == 180 && *currentY < NUMOFSQUARES-1 && board[*currentX][*currentY+1].visited == 0) && canMoveForward(r) == 1)
        {
            printf("\ncurrent position: (%d,%d)\n", r->x, r->y);
            sleep(PAUSETIME);
            forward(r);
            listNext = markerFind(r, listNext, depth+1);
        }
        if (*count <4)
        {
            left(r);
        }
        else if (isBoardfullyVisited() == 0)
        {
            right(r);
        }
        else
        {
            break;
        }
        (*count)++;
    } while ((*count) < 8);
    //check for unvisited squares nearby
    free(count);
    printf("\ncall %d", depth);
    //if board is not fully visited, move to nearest unvisited square
    if (isBoardfullyVisited() == 0)
    {
        double distance = NUMOFSQUARES^2;
        double checkDistance = 0.0;
        square nearestSquare;
        for (i=0; i<NUMOFSQUARES; i++)
        {
            for (j=0; j<NUMOFSQUARES; j++)
            {
                if (board[i][j].visited == 0)
                {
                    checkDistance = findDistance(*currentX,*currentY,i,j);
                    if (checkDistance <= distance)
                    {
                        distance = checkDistance;
                        nearestSquare = board[i][j];
                    }
                }
            }
        }
        //move to nearest square
        listNext = moveToSquare(nearestSquare, r, listNext);
        //set currentX and *currentY to new position
        *currentX = r->x;
        *currentY = r->y;
        board[*currentX][*currentY].visited = 1;
        printf("moved to nearest square at (%d, %d).\n", *currentX, *currentY);
    }
    else
    {
        //if board is fully visited, return to home, else keep searching for squares at original call
        square *nearestAdd = malloc(sizeof(square));
        if (depth == 0 && (atHome(r) == 0))
        {
            while (isBoardfullyVisited() == 0)
            {
                printf("visiting next nearest square\n");
                *nearestAdd = findNearestUnvisitedSquare(r->x, r->y);
                listNext = moveToSquare(*nearestAdd, r, listNext);
                sleep(2000);
            }
            free(nearestAdd);
            printf("returning to home\n");
            listNext = moveToSquare(homePos, r, listNext);
            sleep(2000);
        }
    }
    free(currentX);
    free(currentY);
    drawRobot(*r);
    return listNext;
}

int main (int argc, char **argv)
{
    // The default values if the command line arguments 
    // are not given. 
    int initialX = 0; 
    int initialY = 0; 
    int initialDirection = 0; //in degrees
    
    if (argc == 4) // Four arguments were typed 
    { 
        initialX = atoi(argv[1]); // Get x value 
        initialY = atoi(argv[2]); // Get y value 
        initialDirection = atoi(argv[3]); // Get direction 
        
        homePos.x = initialX;
        homePos.y = initialY;
    } 
    // Then continue with the rest of the code 
    //initialise the list
    list *l = malloc(32*sizeof(list));
    int list_length = 0;

    //initialise the board and robot
    robot robot = {homePos.x, homePos.y, initialDirection, 0};
    createBoard(homePos);
    setWindowSize(WINDOWWIDTH, WINDOWWIDTH+100);
    setColour(white);
    fillRect(0, 0, WINDOWWIDTH, WINDOWWIDTH);
    drawBoard();
    drawRobot(robot);

    //draw collectibles
    int xh[5] = {1,2,3,4,7};
    int yh[5] = {1,2,3,4,4};

    for (int i = 0; i < 5; i++)
    {
        board[xh[i]][yh[i]].type = 2;
        drawMarker(board[xh[i]][yh[i]]);
        printf("collectible at (%d, %d)\n", xh[i], yh[i]);
    }

    //draw the immovable block
    int num_of_immovable = 4;
    int xi[4] = {5,3,2,0};
    int yi[4] = {0,2,5,7};
    for (int i = 0; i < num_of_immovable; i++)
    {
        board[xi[i]][yi[i]].type = 3;
        board[xi[i]][yi[i]].visited = 1;
        drawMarker(board[xi[i]][yi[i]]);
        printf("immovable block at (%d, %d)\n", xi[i], yi[i]);
    }
    int i = 0;
    list *listEnd = markerFind(&robot, l, 0); // address of final marker in list
    drawRobot(robot);
    printf("\nthe robot collected %d markers.\n", robot.marker);
    printf("list of markers: \n");
    //print the markers in said list
    for (list *i = l->next; i != NULL; i = i->next)
    {
        printf("%d %d\n", i->x, i->y);
    }
    return 0;
} 