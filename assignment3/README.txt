----------------------------------------------------------------
COMP 4300 - Assignment 3
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

Assets:
- Entities in the game will be rendered using various Textures and Animations which we will be calling Assets (along with Fonts)
- Assets are loaded once at the beginning of the program and stored in the Assets class, which is stored by the GameEngine class
- All Assets are defined in assets.txt, with the syntax defined below

Note:
- All entity positions denote the center of their rectangular sprite. It also denotes the center of the bounding box, if it has one. This is set via sprite.setOrigin() in the Animation class constructor

Player:
- The player Entity in the game is represented by Megaman, which has several different Animations: Stand, Run, and Air. You must determine which state the player is currently in and assign the correct Animation.
- The player moves with the following controls:
  Left: A key, Right: D key, Jump: W key, Shoot: Space key
- The player can move left, move right, or shoot at any time during the game. This means the player can move left/right while in the air.
- The player can only jump if it is currently standing on a tile
- If the jump key is held, the player should not continuously jump, but instead it should only jump once per button press. If the player lets go of the jump key mid-jump, it should start falling back down immediately
- If the player moves left/right, the player's sprite will face in that direction until the other direction has been pressed
- bullets shot by the player travel in the direction the player is facing.
- The player collides with 'Tile' entities in the level (see level syntax) and cannot move through them, The player land on a Tile entity and stand in place if it falls on it from above.
- The player does not collide with 'Dec' (decoration) entities in the level
- If the player falls below the bottom of the screen, they respawn at the start of a level
- The player should have a Gravity component which constantly accelerates it downward on the screen until it collides with a tile
- The player has a maximum speed specified in the Level file (see below) which it should not exceed in either x or y direction.
- The player will be given a CBoundingBox of a size specified in the level file.
- The player's sprite and bounding box are centered on the player's position

Animations:
- See below for Animation asset specification
- Animations are implemented by storing multiple frames inside a texture
- The Animation class handles frame advancement based on animation speed
- You need to implement Animation::update() to properly progress animations
- You need to implement Animation:hasEnded() which returns true if an animation has finished its last frame, false otherwise.
- Animations can be repeating (loop forever) or non-repeating (play once)
- Any entity with a non-repeating animation should be destroyed once its Animation's hasEnded() returns true (has finished one cycle)

Decoration Entities:
- Decoration entities ('Dec' in a level file) are simply drawn to the screen, and do not interact with any other entities in the game in any way
- Decorations can be given any Animation in the game, but intuitively they should be reserved for things like clouds, bushes, etc.

Tiles:
- Tiles are Entities that define the level geometry and interact with players
_ Tiles can be given any Animation that is defined in the assets file
- Tiles will be given a CBoundingBox equal to the size of the animation tile->getComponent<CAnimation>().animation.getSize()
- The current animation displayed for a tile can be retrieved with: tile->getComponent<CAnimation>().animation.getName()
- Tiles have different behavior depending on which Animation they are given

  Brick Tiles:
  - Brick tiles are given the 'Brick' Animation
  - When a brick tile collides with a bullet, or is hit by a player from below:
    - Its animation should change to 'Explosion' (non-repeat)
    - Non-repeating animation entities are destroyed when hasEnded() is true
    - Its CBoundingBox component should be removed

  Question Tiles:
  - Question tiles are given the 'Question' Animation when created
  - When a Question tile is hit by a player from below, 2 things happen:
  - A temporary lifespan entity with the 'Coin' animation should appear for 30 frames, 64 pixels above the location of the Question entity

Drawing:
- Entity rendering has been implemented for you, no need to change that system

Bonus:
- Any special effects which do not alter game play can be added for up to 5% bonus marks on the assignment. Note that assignments cannot go above 100% total marks, but the 5% bonus can overwrite any marks lost in other areas of the assignment.
- You may develop a 'special weapon' that has special effects which can also contribute to the 5% bonus marks available on the assignment.
- I will show off some of the more impressive bonus submissions in class

Misc:
- The 'P' key should pause the game
- Pressing the 'T' key toggles drawing textures
- Pressing the 'C' key toggles drawing bounding boxes of entities
- Pressing the 'G' key toggles drawing of the grid. This should be very helpful for debugging
- The 'ESC' key should go 'back' to the Main Menu, or quit if on the Main Menu

----------------------------------------------------------------
Level Creation
----------------------------------------------------------------
For this assignment you are also required to create your own level. This level should include some interesting gameplay. Include this level in the zip file as level.txt, and I will show off some of the more interesting levels in class after the assignment is due.

----------------------------------------------------------------
Config Files
----------------------------------------------------------------
There will be two configuration files in this assignment. The Assets config file, and the Level configuration file.

----------------------------------------------------------------
Assets File Specification
----------------------------------------------------------------
There will be three different line types in the Assets file, each of which correspond to a different type of Asset. They are as follows:

Texture Asset Specification:
Texture N P
  Texture Name      N       std::string (it will have no spaces)
  Texture FilePath  P       std::string (it will have no spaces)

Animation Asset Specification
Animation N T F S
  Animation Name        N   std::string (it will have no spaces)
  Texture Name          T   std::string (refers to an existing texture)
  Frame Count           F   int (number of frames in the Animation)
  Animation Speed       S   int (number of game frames between animation frames)

Font Asset Specification
Font N P
  Font Name         N       std::string (it will have no spaces)
  Font File Path    P       std::string (it will have no spaces)

----------------------------------------------------------------
Level Specification File
----------------------------------------------------------------
Game levels will be specified by a Level file, which will contain a list of entity specifications, one per line. It will also contain a single line which specifies properties of the player in that level. In this way, you can define an entire level in the data file, rather than in programming code. The syntax of lines of the Level file are as follows.

IMPORTANT NOTE:
All (GX, GY) positions given in the level specification file are given in 'grid' coordinates. The 'grid' cells are of size 64 by 64 pixels, and the entity should be positioned such that the bottom left corner of its texture is aligned with the bottom left corner of the given grid coordinate. The grid starts an (0,0) in the bottom-left of the screen, and can be seen by pressing the 'G' key while the game is running.

Tile Entity Specification:
Tile N GX GY
  Animation Name    N       std::string (Animation asset name for this tile)
  GX Grid X Pos     GX      float
  GY Grid Y Pos     GY      float

Decoration Entity Specification:
Dec N X Y
  Animation Name     N      std::string (Animation asset name for this tile)
  X Grid X Pos       X      float
  Y Grid Y Pos       Y      float

Player Specification
Player GX GY CW CH SX SY SM GY B
  GX, GY Grid Pos       X, Y    float, float (starting position of player)
  BoundingBox W/H       CW, CH  float, float (in pixels)
  Left/Right Speed      SX      float
  Jump Speed            SY      float
  Max Speed             SM      float
  Gravity               GY      float
  Bullet Animation      B       std::string (Animation asset to use for bullets)

----------------------------------------------------------------
Assignment Hints
----------------------------------------------------------------
I recommend approaching this assignment in the following order, which will help you debug your program along the way with minimal errors. Remember to do one step at a time and test whether what you have just implemented is working properly before moving on to any additional steps.

- Rendering system has already been set up for you, to help you debug:
  You can press the T key to toggle drawing textures
  You can press the C key to toggle drawing bounding boxes
  You can press the G key to toggle drawing the grid

- You can implement Animation::update() and Animation::hasEnded() at any time, it will not affect the gameplay mechanics whatsoever, just animation

- Implement Scene_Play::loadLevel()
  Since rendering is already completed, once you correctly read in the different types of entities, add them to the EntityManager and they should automatically be drawn to the screen. Add the correct bounding boxes to Tile entities, and no bounding boxes to the Dec entities. Remember you can toggle debug viewing of bounding boxes with the T and C keys
  As part of this step, implement the Scene_Play::gridToMidPixel() function, which takes in as parameters a grid x,y position and an Entity, and returns the Vec2 position of the CENTER of that Entity. You must use the Animation size of the Entity to determine where its center point should be. Keep in mind that this means your Entity MUST have its CAnimation component added first, so that it can be used to calculate the midpoint in this function.

- Implement Scene_Play::spawnPlayer()
  Read the player configuration from the level file and spawn the player. This is where the player should restart when they die.

- Implement some basic WASD u/l/d/r movement for the player entity so that you can use this to help test collisions in the future. Remember that you must use registerAction to register a new action for the scene. See the actions already registered for you, and the sDoAction() function for syntax on how to perform actions.

- Implement Scene_Play::spawnBullet()
  Bullet should shoot when Space is pressed in same direction player facing. Holding down the space button should not continuously fire bullets. A new bullet can only be fired after the space key has been released. Use the entity's CInput.canShoot variable to implement this

- Implement Physics::GetOverlap()
  This function should return the overlap dimensions between the bounding boxes of two entities. This is the same as the purple rectangle in notes. GetPreviousOverlap should be a copy/paste of this solution except using the previous positions instead of the current positions of th entity. If either input entity has no bounding box, then return Vec2(0,0).

- Implement collision checking with bullets / brick tiles such that the brick is destroyed when a bullet collides with it. Remember, a collision occurs when the overlap is non-zero in both the X and Y component. Bullets should always be destroyed when they collide with any non-decorative tile.

- Implement collision resolution such that when the player collides with a non-decorative tile, the player cannot enter it over overlap it. When the player collides with a tile from below, its y-velocity should be set to zero so that it falls back downward and doesn't 'hover' below the tile.

- Implement a way of detecting which side the player collided with the tile

- Change the controls such that they are the proper left/right/jump style.
  Note: All movement logic should be in the movement system. The sDoAction system is ONLY used to set the proper CInput variables. If you modify the player's speed or position anywhere inside the sDoAction system, you will lose marks as this is potentially unsafe / produces unwanted effects.

- Implement gravity such that the player falls toward the bottom of the screen and lands on tiles when it collides with a tile from above. Note that when the player lands on a tile from above, you should set its vertical speed to zero so that gravity does not continue to accelerate the player downward.

----------------------------------------------------------------
Notes
----------------------------------------------------------------
folder structure explained in 5:50
bin
  fonts
  images
    mario
    megaman
    misc
  solution.exe
  assets.txt
  level1.txt
  level2.txt
  level3.txt

We need to work on files marked with '->':
+ Action.cpp
+ Action.h
+ -> Animation.cpp
+ Animation.h
+ Assets.cpp
+ Assets.h
+ Components.h
+ Entity.cpp (?consturctor)
+ Entity.h
+ EntityManager.cpp
+ EntityManager.h
+ GameEngine.cpp
+ GameEngine.h
+ main.cpp
+ -> Physics.cpp
+ Physics.h
+ Scene.cpp
+ Scene.h
+ Scene_Menu.cpp
+ Scene_Menu.h
+ -> Scene_Play.cpp
+ Scene_Play.h
+ vec2.cpp
+ vec2.h
