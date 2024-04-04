----------------------------------------------------------------
COMP 4300 - Assignment 4
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
- Entities in the game will be rendered using various Textures and Animations which we will be calling Assets (along with Fonts and Sounds)
- Assets are loaded once at the beginning of the program and stored in the Assets class, which is stored by the GameEngine class
- All Assets are defined in assets.txt, with the syntax defined below

Player:
- The player Entity in the game is represented by NotLink, which has several different Animations: RunDown, RunUp, RunRight, StandDown, StandUp, StandRight, AtkUp, AtkDown, AtkRight. You must determine which direction and state the player is currently in and assign the correct Animation.
- The Player is assigned the following animations in the direction facing.
  Please note that Left will be accomplished by mirroring Right (x scale = -1)
  StandDir  - When no input (or both opposite) is being given to the player
  RunDir    - When move input is given to the player in a direction
  AttackDir - When the player's sword is currently visible from an attack
- The player moves with the following controls:
  Left: A, Right: D, Up: W, Down: S, Attack: Space Key
- The player can move up, left, down, right, at any time during the game.
- The player can only move either horizontally or vertically at a given time
- If opposite directions are held, the player will be given a Stand animation
- The player can only attack once its current attack animation has finished.
- The player collides with 'Tile' entities in the level (see level syntax) whose bounding box blocks movements and cannot move through them.
- The player will be given a CBoundBox of a size specified in the level file
- The player will be given a CHeath component with a max health that is
  specified in the level file
- When the player's current health is <= 0, the player respawns, and the
  LinkDie sound should play
- If the player has a CInvincibility component, it takes no damage from
  enemies and its sprite is made transparent. The duration of the component
  is reduced each frame, and when the duration reaches 0, the component is
  removed, making it vulnerable to attack again.
- When the player collides with an enemy while not invincible:
  - player's current health is reduced by the enemy damage component
  - the LinkHit sound should player
  - player has a CInvincibility component which lasts for 30 frames

Other Entities:
- Each tile and NPC in the game specifies two boolean values of whether it blocks movements and whether it blocks vision. If a tile blocks movement, the player collides with it and cannot pass through. If an entity blocks vision then it affects enemy line of sight. See spec below.
- CBoundBox has two new booleans to reflect these changes
- When the player steps onto a tile with the "Black" animation, they
  are teleported to a random "Black" tile on the map

Attacking:
- When the player attacks, a sword appears for 10 frames and then disappears approximately 1 tile away from the player in the direction they are facing.
- The player's sword should be given a bounding box equal to the anim size
- When the sword collides with an enemy, it deals its damage component worth of damage to the health of the enemy that it collides with. The sword should only do damage for one frame, and then its damage component should be removed so that it does not deal damage for every frame it overlaps.
- When the sword is swung, the Slash sound should play
- When the sword collides with an enemy, the EnemyHit sound should play
- When an enemy's health is reduced to 0, it is destroyed
- When an enemy is destroyed, the EnemyDie sound should be play

NPC Entities:
- NPC entities will each be given an 'ai' behavior in the level file, either 'Follow' or 'Patrol'.
- 'Follow' means it will follow the player when the player is direct line of sight, or head back to its original position when not in line of sight. When an entity is at his home position, it should not oscillate.
- 'Patrol' means it will move toward a series of patrol positions, looping back around to the start when the final one is reached. An NPC has reached its patrol position if it is within a distance of 5 from it.

Tiles:
- Tiles are Entities that define the level geometry and interact with players
- Tiles can be given any Animation that is defined in the Assets file
- Tiles will be given a CBoundingBox equal to the size of the animation:
  tile->get<CAnimation>().animation.getSize()
- The current animation displayed for a tile can be retrieved with:
  tile->get<CAnimation>().animation.getName()
- If a tile is given the Heart animation, any NPC that collides with it
  should have its health filed to maximum, and the tile is destroyed.

Drawing:
- The game window is given a fixed size of 1280x768 pixels (20x12 tiles)
- Rendering of entities is provided for you, as well as a 'debug' rendering mode which can be toggled with the 'C' key, and the 'T' key toggling texture rendering.
- You are required to change the camera position (window.view()) of the game window to alternate between two modes based on the m_follow variable:
  true:  Follow Mode - the camera is always centered on NotLink
  false: Room Mode   - the camera is centered in the middle of the room

Rooms:
- Each level is split up into individual 'room' given a (rx,ry) coordinate
- Room (rx,ry) is defined by the rectangle (rx*w, ry*h, w, h) where w and h are the window width and height respectively.
- Each tile is given a (tx,ty) grid position within a room. By default, the grid cells and all tiles have size 64x64 pixels
- When NotLink moves to a new room, the camera should center to that room

GUI:
- You must implement the "Animations" and "EntityManager" tabs in ImGui
  so that they have the same functionality as the solution.

Misc:
- The 'P' key should pause the game
- The 'C' key toggles debugging view
- The 'T' key toggles textures rendering
- The 'G' key toggles drawing the grid
- The 'Y' key should toggle between follow camera or 'room' camera
- The 'ESC' key should go 'back' to the Main Menu, or quit if on the Main Menu

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

Animation Asset Specification:
Animation N T F S
  Animation Name        N   std::string (it will have no spaces)
  Texture Name          T   std::string (refers to an existing texture)
  Frame Count           F   int (number of frames in the Animation)
  Animation Speed       S   int (number of game frames between animation frames)

Font Asset Specification:
Font N P
  Font Name         N       std::string (it will have no spaces)
  Font File Path    P       std::string (it will have no spaces)

Sound Asset Specification:
Sound N P
  Sound Name        N       std::string (it will have no spaces)
  Sound FilePath    P       std::string (it will have no spaces)

----------------------------------------------------------------
Level Specification File
----------------------------------------------------------------

Note: in this assignment (0,0) is in top-left.

Player Specification:
Player X Y BX BY S H
  Spawn Position       X Y      int, int (in pixel coordinates)
  Bounding Box Size    BX BY    int, int
  Speed                S        float
  Max Health           H        int

Tile Specification:
Tile Name RX RY TX TY BM BV
  Animation Name       Name     std::string
  Room Coordinate      RX RY    int, int
  Tile Position        TX TY    int, int
  Block Movement       BM       int (1 = true, 0 = false)
  Block Vision         BV       int (1 = true, 0 = false)

NPC Specification:
  NPC Name RX RY TX TY BM BV H D AI ...
  Animation Name       Name     std::string
  Room Coordinate      RX RY    int, int
  Tile Position        TX TY    int, int
  Block Movement       BM       int (1 = true, 0 = false)
  Block Vision         BV       int (1 = true, 0 = false)
  Max Health           H        int (> 0)
  Damage               D        int (> 0)
  AI Behavior Name     AI       std::string
  AI Parameters        ...      (see below)

AI = Follow
  ... = S
  Follow Speed         S        float (speed to follow player)

AI = Patrol
  ... = S N X1 Y1 X2 Y2 ... XN YN
  Patrol Speed         S        float
  Patrol Position      N        int (number of patrol positions)
  Position 1-N         Xi Yi    int, int (Tile Position of Patrol Position i)

For Example:
NPC Tektite 0 0 15 10 0 0 2 1 Patrol 2 4 15 10 15 7 17 7 17 10
  - Spawn an NPC with animation name Tektite in room (0,0) with tile pos (15,10)
  - This NPC has max health of 2 and damage of 1
  - This NPC does not block movement or vision
  - This NPC has a Patrol AI with speed 2 and 4 positions, each in room (0,0)
    Positions: (15,10) (15,7) (17,7) (17,10)

----------------------------------------------------------------
Misc Notes
----------------------------------------------------------------

1. The m_player variable has been removed and replaced with the player() function, which looks in the EntityManager for a player and returns it. This is slightly less efficient, but allows us to put in sanity checks for things like checking to see if a player object exists before using it, etc.

2. Entity getComponent, addComponent, etc. have benn shorted to get, add, etc.
Example: e->getComponent<CTransform> is now e->get<CTransform>()

3. sf::Sound objects can be played with .play() and stopped with .stop(). However in order to play, the Sound object must persist for at least the duration of the sound that you want to be played. If you simply allocate a local Sound object and then call sound.play(), it will stop playing as soon as the Sound object goes out of scope. Calling sound.play() on a Sound object that is already playing will restart the sound, not start playing another one.

----------------------------------------------------------------
Assignment Hints
----------------------------------------------------------------

You can use all of your collision code from Assignment 3 in this assignment.

I recommend approaching this assignment in the following order:

 0. Implement the GUI whenever you feel you'd like to have it for debugging
 1. Implement WASD movement for the player such that it moves u/l/d/r when those keys are pressed. This will allow you to test rectangular collisions as soon as possible
 2. Implement Scene_Zelda::loadLevel(), which will allow you to load the assets for the level given to you to test the rest of the assignment. To do this, you will need to implement getPosition()
 3. Implement spawnPlayer() so that it correctly uses the configuration specified by the level file, so you can start testing collisions
 4. Implement the camera changes described above
 5. Implement spawnSword() so that it spawns a sword in the correct position with the correct lifespan
 6. Implement player attacking / sword collision with NPCs / health / damage
 7. Implement player animation to correctly animate based on player state
 8. Implement NPC Patrol behavior
    Implement NPC Follow behavior without vision
 9. Implement Physics::LineIntersect() which will allow you to test whether two line segments AB and CD intersect, and where they intersect
10. Implement Physics::EntityIntersect() which will allow you to test whether line segment AB intersects with any lines of the bounding box of Entity
11. Implement NPC Follow behavior using 10 to check line of sight
12. Implement invincibility frames
13. Implement heart pickups
14. Implement black tile teleporting

----------------------------------------------------------------
Notes
----------------------------------------------------------------
We need to work on files:
Physics.h
Physics.cpp
Scene_Zelda.h
Scene_Zelda.cpp

void Scene_Zelda::update() {
    m_entityManager.update();

    // TODO: implement pause functionality
    if (!m_paused) {
        sDrag();
        sAI();
        sMovement();
        sStatus();
        sAnimation();
        sCollision();
        sCamera();
        m_currentFrame++;
    }

    sGUI();
    sRender();
}
