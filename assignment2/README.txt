----------------------------------------------------------------
COMP 4300 - Assignment 2
----------------------------------------------------------------
Assignment Notes:
----------------------------------------------------------------

Submission:

For this assignment you will zip and submit the entire 'src' folder of the assignment. You are *NOT* allowed to add any files in addition to the ones already in the src folder from the assignment zip file. This means that you will not be submitting ANY changes you have made to the visual studio project, if you used it for development.

YOU MUST INCLUDE THE FOLLOWING INFO IN A COMMENT AT THE TO OF main.cpp:
- ALL students' full names, cs usernames, and student numbers
- If you did not get a specific feature to work (such as text centering) please explain what you tried to do to get it to work, and partial marks may be given for effort.

----------------------------------------------------------------
Program Specification
----------------------------------------------------------------

In this assignment you will be writing the game that was presented in class.
This game must have the following features:

Player:
- The player is represented by a shape which is defined in the config file
- The player must spawn in the center of the screen at the beginning of the game, and after it dies (collides with an enemy)
- The player moves by a speed read from the config gile in these directions:
  Up: W key, LEft: A key, Down: S key, Right: D key
- The player is confined to move only within the bounds of the window
- The player will shoot a bullet toward the mouse pointer when the left mouse button is clicked. The speed, size, and lifespan of the bullets are read from the config file.

Special Ability
- You are free to come up with your own 'special move' which is fired by the player when the right mouse button is clicked. This special ability must:
- Mulitple entities (bullets etc) spawned by special weapon
- Entities have some unique graphic associate with them
- A unique game mechanic is introduced via a new component
- A 'cooldown timer' must be implemented for the special weapon
The properties of the special move are not in the config file.

Enemy(s):
- Enemies will spawn in a random location on the screen every X frames, where X is defined in the configuration file.
- Enemies must not overlap the sides of the screen at the time of spawn.
- Enemies shapes have random number of vertices, between a given minimum and maximum number, which is specified in the config file.
- Enemy shape radius will be specified in the configuration file.
- Enemies will be given a random color upon spawning.
- Enemies will be given a random speed upon spawning, between a minimum and maximum value specified in the config file.
- When an enemy reaches the edge of the window, it should bounce off in the opposite direction at the same speed.
- When (lagre) enemies collide with a bullet or player, they are destroyed, and N small enemies spawn in its place, where N is the mumber of vertices of the original enemy. Each small enemy must have the same nubmer of vetices and color of the original enemy. These small entities travel outward at angles at a fixed intervals equal to (360 / vertices). For example, if the original enemy had 6 sides, the 6 smaller enemies will travel outward in intervals of (360/6) = 60 degrees.

Score:
- Each time an enemy spawns, it is given a score component of N*100, where N is the number of vertices it has. Small enemies het double this value.
- If a player bullet kills an enemy, the game score is increased by the score component of the enemy killed.
- The score should be displayed with the font specified by the config file in the top-left corner of the screen

Drawing:
- In the renter system, all entities should be given a slow rotation, which makes the game look a little nicer.
- Any special effects which do not alter game play can be added for up to 5% bonus marks on the assignment. Note thea assignments cannot go above 100% total marks, but the 5% bonus can overwrite any marks lost in other areas of the assignment.
- Any Entity with a lifespan is currently alive, it should have its Color alpha channel set to a ration depending on how long it has left to live. For example, if an Entity has a 80 frame life span, and 25 frames remaining, its alpha value should be set to (float)25/80*255. The alpha sould go from 255 when it is first spawned, to 0 on the last frame it is alive.

GUI:
- You must construct a GUI using ImGui which has the following functionality:
- The GUI must display options to turn off each system independently
- For each entity in the game, the GUI must list the ID, tag, and position of that entity. You must display a list of all entities, as well as lists of entities by their tag. You must also have some way of destroying a given entity by interacting with the UI element associated with it.
- You must be able to change the enemy spawn interval through the GUI
- You must be able to manually spawn enemies in some way through the GUI
- You may develop the GUI in any way that contains this functionality, but it must be easily usable and clearly presented to get full marks.

Misc:
- The 'P' key should pause the game
- The 'ESC' key should close the game

Configuration File:

The configuration file will have one line each specifying the window size, font format, player, bullet specification, and enemy specifications. Lines will be given in that order, woth the following syntax:

Window W H FL FS
- This line declares that the SFML Window must be constructed with width W and height H, each of which will be integers. FL is the frame limit that the window should be set to, and FS will be an integer which specifies whether to display the applicatiop in full-screen mode (1) or not (0).

Font F S R G B
- This lines defines the font which is to be used to draw text for this program. The format of the line is as follows:
Font File   F       std::string (it will have no spaces)
Font Size   S       integers
RGB color   (R,G,B) int, int, int

Player Specification:
Player SR CR S FR FG FB OR OG OB OT V
Shape Radius      SR        int
Collision Radius  CR        int
Speed             S         float
Fill Color        FR,FG,FB  int,int,int
Outline Color     OR,OG,OB  int,int,int
Outline Thickness OT        int
Shape vertices    V         int

Enemy Specification:
Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
Shape Radius      SR        int
Collision Radius  CR        int
Min/Max Speed     SMIN,SMAX float,float
Outline Color     OR,OG,OB  int,int,int
Outline Thickness OT        int
Min/Max vertices  VMIN,VMAX int,int
Small Lifespan    L         int
Spawn interval    SI        int

Bullet Specification:
Player SR CR S FR FG FB OR OG OB OT V L
Shape Radius      SR        int
Collision Radius  CR        int
Speed             S         float
Fill Color        FR,FG,FB  int,int,int
Outline Color     OR,OG,OB  int,int,int
Outline Thickness OT        int
Shape vertices    V         int
Lifespan          L         int

----------------------------------------------------------------
Assignment Hints
----------------------------------------------------------------
I recommend approaching this assignment in the following order:

0. Save the configuration file reading until later, after Entities implemented
1. Implement the Vec2 class, which you will use for all Components
2. Implement the basic functionality in the EntityManager class. Implement the EntityManager's addEntity() and update() functions so you can start testing the Game class quickly. Don't worry about the update() function which deletes dead entities until later when you get more game mechanics working.
3. Implement basics of the Game class:
  a. Construct a player Entity using the spawn Player() function
  b. Implement basic drawing of entities using the Game::sRender() function
  c. Construct some enemies using the spawnEnemy() function
  d. Construct a bullet using the spawnBullet() function
4. Implement Player Movement in Game::sUserInput and Game::sMovement
5. Implement the EntityManager::update() function so it deletes dead entities
6. Implement the EntityManager::getEntities(tag) functionality
7. Implement collisions in sCollision and entity.destroy() if it's dead
8. Implement the rest of the game's functionality, including config file reading
9. Implement the GUI functionality. You may however want to implement the GUI functionality earlier to help you debug your program, it's up to you.
