/*=============================================================================
*                           Balls and their admirers                            
*           https://c-programming.aydos.de/week09/balls-and-their-admirers.html
*gcc .\opgave-9\balls.c -o .\opgave-9\balls.exe  -I"opgave-9\raylib\include"  -L"opgave-9\raylib\lib"   -lraylib -lopengl32 -lgdi32 -lwinmm
* .\opgave-9\balls.exe
*==============================================================================*/

#include "raylib\include\raylib.h" 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h> 

/*=============================================================================
*                                     GUI                                      
*=============================================================================*/
#define WIDTH 400
#define HEIGHT 400
#define TITLE "Balls and their admirers"
#define BALL_COUNT 100
#define FPS 60
#define VEL_MAX 5
#define RADIUS_MAX 20
#define RADIUS_MIN 5

Color COLORS[] =
{
    LIGHTGRAY, GRAY,   DARKGRAY, YELLOW,     GOLD,      ORANGE,  PINK,
    RED,       MAROON, GREEN,    LIME,       DARKGREEN, SKYBLUE, BLUE,
    DARKBLUE,  PURPLE, VIOLET,   DARKPURPLE, BEIGE,     BROWN,   DARKBROWN,
};

typedef struct Ball {
    int posx, posy;
    int velx, vely;
    int radius;
    Color color;
    struct Ball *follows;
} Ball;

Ball balls[BALL_COUNT];

/*=============================================================================
*                         helper functions/definitions
*=============================================================================*/

/*-----------------------------------------------------------------------------
* Definition of Ball: randomly initiate state and properties (params)
*
*   @param posx & posy          ints for position
*   @param velx & vely          ints for velocity
*   @param radius               int for the size
*   @param Color                Color of the ball from array COLORS 
*   @param follows              pointer to other ball from balls array to follow. Cant be self
-----------------------------------------------------------------------------*/
Ball *init_ball_random(Ball *p)
{
    //_______________________________________________________ set color

    int color_index = rand() % (sizeof(COLORS) / sizeof(COLORS[0]));
    p->color = COLORS[color_index]; //give random color

    //_______________________________________________________ set position
    
    int start_posx = rand() % 401;
    p->posx = start_posx;

    int start_posy = rand() % 401;
    p->posy = start_posy;

    //_______________________________________________________ set velocity

    int start_velx = (rand() % 11) -5;
    p->velx = start_velx;

    int start_vely = (rand() % 11) -5;
    p->vely = start_vely;

    //_______________________________________________________ set radius

    int start_radius = (rand() % 16) + 5;
    p->radius = start_radius;

    //_______________________________________________________ find leader

    Ball *leader;
    do
    {
        int leader_index = rand() % BALL_COUNT;
        leader = &balls[leader_index];
    } while (leader == p); //if a ball is its own leader, rerun the leader code to find a new

    p->follows = leader;

    return p;
}

/*-----------------------------------------------------------------------------
* Initialize all balls
*
* Loop through BALL_COUNT entries in balls[]
* Calls init_ball_random(&balls[i]) for each index in balls[]
* Make sure balls[i] never points to self for variable follows
-----------------------------------------------------------------------------*/
void init_balls_random()
{
    for (int i = 0 ; i < BALL_COUNT ; i++)
    {
        init_ball_random(&balls[i]);
    }
}


/*-----------------------------------------------------------------------------
* Draw a single ball
*
*   @param p        Pointer to the ball structure to draw
*   Uses Raylib’s DrawCircle() to render the ball at its position with given color.
-----------------------------------------------------------------------------*/
Ball *draw_ball(Ball *p)
{
  DrawCircle(p->posx, p->posy, p->radius, p->color);
  return p;
}


/*-----------------------------------------------------------------------------
* update_pos: Update positions of balls according to velocity
*
*   @param p        Pointer to the ball
*   posx & posy += velx & vely
*   % WIDTH and % HEIGHT to ensure ball reappears on the other side if out of bounds
-----------------------------------------------------------------------------*/
Ball *update_pos(Ball *p)
{
  p->posx = (WIDTH + p->posx + p->velx) % WIDTH; // `WIDTH +` because C uses truncated division
  p->posy = (HEIGHT + p->posy + p->vely) % HEIGHT;
  return p;
}


/*-----------------------------------------------------------------------------
* update_vel_for_following: Update velocity for ball so it follows leading ball
*
*   @param p        Pointer to a Ball structure
*   Adjusts velocity (velx, vely) so that the ball moves toward its leader.
-----------------------------------------------------------------------------*/
Ball *update_vel_for_following(Ball *p)
{
  int errx = p->follows->posx - p->posx;
  int erry = p->follows->posy - p->posy;
  p->velx = errx > 0 ? 1 : -1;
  p->vely = erry > 0 ? 1 : -1;
  return p;
}


/*-----------------------------------------------------------------------------
* Update and draw all elements
*
*   Iterates through all balls, updates their velocity and position, then draws
*   them. Called once per frame to refresh the simulation.
-----------------------------------------------------------------------------*/
void update_elements()
{
    for (size_t i = 0; i < BALL_COUNT; ++i)
    {
        draw_ball(update_pos(update_vel_for_following(&balls[i])));
    }
}

/*=============================================================================
*                                     Main
*=============================================================================*/
int main()

{
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(FPS);

  srand(time(NULL));
  init_balls_random();

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    update_elements();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
  return 0;


}



/*=============================================================================
*                                 REQUIREMENTS
*==============================================================================
* A ball has a two-dimensional position (posx and posy), a velocity (velx and vely), a radius (radius), a color (Color color, where Color is defined by raylib) and a pointer to another leading ball (follows).

* Select meaningful types for these properties.

* Each ball starts at a random position, with a random velocity, random color and a random ball to follow. A ball should not follow itself.

* Use the template and only fill the missing parts.

* Include a flowchart.

* Do not include raylib library in your repository.

* Record a 5-10s video of your simulation.
*/