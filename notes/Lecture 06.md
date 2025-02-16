[COMP4300 - Game Programming - Lecture 06 - EntityManager + 2D Game Math](https://youtu.be/QFTsQPV9heA?si=gFfPErqoUQsUOKsA)

Data Management
* One of the most important concepts in all of programming: separate logic and data
* Create data structures which manage data so your logic code doesn't worry about it
* Complex algorithm shouldn't have to know whether set, vector, map implementation
* Let's make an Entity Manager!

Entity Manager
* Uses the Factory design pattern
* Entity Manager will handle the creation, storage, and lifetime of ALL Entity objects
* EntityManager functions:
	* addEntity, removeEntity, storage, bookkeeping
* Factory should somehow ensure that user cannot create its own Entities

Entity Tags
* We may want to group Entities by functionality
* Entity is 'tagged' by string
	* Tile, Player, Bullet, Enemy
* EntityManager should have functionality for quickly getting Entities of a given tag
* Example: loop over all bullets

Entity Implementation
```
using ComponentTuple = std::tuple<
  CTransform,
  CLifeSpan,
  CInput,
  CBoundingBox,
  CAnimation,
  CGravity,
  CState
>;

class Entity
{
  ComponentTuple     m_components;
  bool               m_alive = true;    // if false, will delete entity
  std::string        m_tag = "default"; // type of entity
  size_t             m_id = 0;          // unique integer id
public:
  Entity() {}
  void               add<T>(args);
  T&                 get<T>();
  size_t             id() const;
  bool               isAlive() const;
  void               destroy();
  const std::string& tag() const;
```

Entity Manager
* Will trade some memory for functionality
* Will store all Entity objects in a vector
* Will also store separate vectors of Entity objects by their tag for quick retrieval
	* `std::map<std::string, EntityVec>`
* We store double the number of pointers to save on computation in our engine

Entity Storage Options
Option 1. Store Entity objects directly in vector
	`using EntityVec = std::vector<Entity>`
Option 2. Store Smart Pointers in vector
	`using EntityVec = std::vector<std::shared_ptr<Entity>>`
Option 3. Use a Memory Pool to store Entity Data (have a future lecture on this)

Option `std::vector<Entity>`
* + Avoids overhead of smart pointer
* + Data stored contiguously in memory
* + Simplifies the design overall
*  - Ownership, references, and lifetime of Entity object are tied to the container
* - Removing Entity can be slower (more data)
* - Reallocation of owning vector can invalidate existing references to Entity

Option `std::vector<std::shared_ptr<Entity>>`
* + Shared ownership - can safely refer to Entity in multiple systems without lifetime worries
* + Automatic memory management
* + Add/Delete from vector is faster (less data)
* + Can modify vector while still referring to Entities in other parts of the code
* - Overhead of smart pointer is slower
* - Entities not stored contiguously in memory
* - Slightly more complex overall design
* Note: can't use `unique_ptr` for even more complex reasons

Entity Storage Options
Option 1. Store Entity objects directly in vector
	Can have memory issues we want to avoid
Option 2, Store Smart Pointers in vector
	More robust, slightly slower, Will use this for now.
Option 3. Use a Memory Pool to store Entity Data
	Entity class basically becomes an integer index
	This is what we should actually do (but later)

Entity Manager
```
typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map   <std::string, EntityVec>  EntityMap;
class EntityManager
{
  EntityVec m_entities;
  EntityMap m_entityMap;
  size_t    m_totalEntities = 0;
public:
  EntityManager();
  std::shared_ptr<Entity> addEntity(const std::string& tag);
  EntityVec& getEntities();
  EntityVec& getEntities(const std::string& tag);
};
```

Adding Entities
```
std::shared_ptr<Entity> addEntity(const std::string& tag)
{
  // create a new Entity object
  // store it in the vector of all entities  
  // store it in the map of tag->entityvector
  // return the shared pointer pointing to that entity
}

// with sample code
std::shared_ptr<Entity> addEntity(const std::string& tag)
{
  auto e = std::make_Shared<Entity>(tag, m_totalEntities++);
  m_entities.push_back(e);
  m_entityMAp[tag].push_back(e);
  return e;
}
```

Entity Manager Usage
```
EntityManager m_entities;

void spawnEnemy()
{
  auto e = m_entities.addEntity("enemy");
  e->add<CTransform>(args);
  e->add<CShape>(args);
}
```

Iterator Invalidation
* Iterator invalidation is very important
* Caused by modifying the contents of a collection as we are iterating through it
* This happens all the time in games
	* Adding: While iterating through all entities to check for collisions, an explosion happens and we spawn an explosion entity
	* Removing: Iterate through entities to check collisions, one dies, so we remove it

C++ Iterator Invalidation
* Vector function may cause reallocation
	* Adding to vector: push_back, insert, etc...
	* Vector may resize, recreating internal array
	* Invalidates all pointers and iterators
* Erasing elements may invalidate too
	* Remove from vector: erase, pop_back
	* Invalidates iterators and references at or after the point of erasing in the internal array

Iterator Invalidation Example
```
void sCollision()
{
  EntityVec bullets;
  EntityVec tiles;
  for (auto& b : bullets)
    for (auto& t : tiles)
      if (Physics::IsCollision(b, t))
        bullets.erase(b);
}
// all iterators/values of b after erase may be invalid
```

Delayed Effects
* One way to avoid iterator invalidation is to delay the effects of actions that modify collections until it is safe to do so
* With the EntityManager handling Entity creation and destruction, this becomes easy
* Idea: Only add or remove entities at the beginning of a frame when it is safe

Entity Manager
```
typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map   <std::string, EntityVec>  EntityMap;
class EntityManager
{
  EntityVec m_entities;
  EntityVec m_toAdd;     // <- add this
  EntityMap m_entityMap;
  size_t    m_totalEntities = 0;
public:
  EntityManager();
  void update();         // <- add this
  std::shared_ptr<Entity> addEntity(const std::string& tag);
  EntityVec& getEntities();
  EntityVec& getEntities(const std::string& tag);
};
```

Adding Entities
```
std::shared_ptr<Entity> addEntity(const std::string& tag)
{
  // give each entity an increasing integer id
  auto e = std::make_shared<Entity>(tag, m_totalEntities++);
  m_toAdd.push_back(e);
  return e;
}
```

Entity Death/Removal
* With this system, we can easily implement entity death/removal
* Entity has `m_alive` bool variable
* Set this to false when we want it to die
* EntityManager update() will read this variable and remove entities if false
* Prevents removal iterator invalidation

Entity Manager Update
```
// called at beginning of each frame by game engine
// entities added will now be available to use this frame
void EntityManager::update()
{
  for (auto e: m_toAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }
  for (auto e : m_entities)
  {
    // if e is dead, remove it from m_entities
    // if e is dead, remove it from m_entityMap[e->tag()]
  }
  m_toAdd.clear();
}
```

Entity Manager Usage
```
EntityManager m_entities;

void collisions()
{
  for (auto b : m_entities.getEntities("bullet"))
    for (auto e: m_entities.getEntities("enemy"))
      if (Physics::CheckCollision(b, e))
      {
        b->destroy(); e->destroy();
	  }
}
```

Entity Implementation
```
class Entity
{
  ComponentTuple     m_components;
  bool               m_alive = true;    // if false, will delete entity
  std::string        m_tag = "default"; // type of entity
  size_t             m_id = 0;          // unique integer id
public:
  Entity() {}
  void               add<T>(args);
  T&                 get<T>();
  size_t             id() const;
  bool               isAlive() const;
  void               destroy();
  const std::string& tag() const;
```

Game Main Loop
```
void GameEngine::mainLoop()
{
  m_entityManager.update();
  sUserInput();
  sMovement();
  sCollision();
  sRender();
  m_currentFrame++;
}
```

Entity Private Constructor
* We can ensure the safe creation of Entity objects by only allowing them to be created by the EntityManager class
* We can make Entity constructor private and add EntityManager as friend class
* This way, we cannot create Entity objects outside of the EntityManager
* Unfortunately, having a private constructor means that we can no longer use
	`auto e = std::make_shared<Entity>(args);`
* For private constructor classes, can use
	`auto e = std::shared_ptr<Entity>(new Entity(args));`
* This is not the preferred way of the creating shared pointer objects, but we make this one exception for our design
```
void GameEngine::sEnemySpawner()
{
  // manually creating entities no longer possible
  // each of these will be a compiler error
  auto e = std::make_shared<Entity>(args);
  auto e = new Entity(args);
  Entity e(args);

  // must use EntityMAnager to create entities
  auto e = m_entityManager.addEntity(args);
}
```

2D Games
* Every game's graphics are drawn to a screen
* Screens are a plane, 2D
* Screens are made of small, discrete elements: pixels
* Pixels are (usually) square
* Pixels have R,G,B elements

2D Game Colors
* In modern games, we specify the color of an object (pixel, shape, etc...) in a specific Color Space of the game/display
* RGB, RGBA, CMYK, Grayscale, Monochrome
* RGBA is most common modern color space: Red, Green, Blue, Alpha
* Each pixel gets assigned a color

2D Positions
* System display: pixel grid
* Grid is in two dimensions (x, y)
* We can use a Euclidean (x, y) plane to visualize individual positions
* Each integer position is a display pixel
* Note: Displays can be resized/full screen, so pixels can 'appear larger'
* Note: Most APIs have (0, 0) in top-left
* Example : a = (4, 2)
* Can represent this 2D value as a vector
* Vec2 = (X, Y)
	* Distance from Origin (0, 0)
* Another point b: (6, 7)
* Let's try some vector math
* Subtraction: b - a = c
	* b(6, 7) - a(4, 2) = c(2, 5)
* Vector Subtraction: Destination - Origin = Distance
* Vector Addition: Origin + Distance = Destination
* SOH, CAH, TOA at 51:00

Example: Vector From Angle & Distance (Speed)
* Entity speed S units/frame
* Entity to travel at angle of Theta (T)
* "Radius of the unit circle" = S (speed)
* cos(T) = X/R, sin(T) = Y/R
	* cos(T) = 'x change if speed was 1'
	* sin(T) = 'y change if speed was 1'
* To get (x, y) velocity vector, must multiply by S
* Vec2 `velocity(S*cos(T), S*sin(T))`
* Speed is the magnitude (length) of velocity vector

Example: Angle & Distance from Vector
* Wanted: Distance and Angle
* Given: (x, y) vector
* Player (x1, y1), Entity (x2, y2)
* Distance from Player to Entity?
* Difference Vector D
	* D = Entity - Player
	* D = (x2-x1, y2-y1)
	* Distance = `sqrtf(D.x*D.x + D.y*D.y)` = Pythagoras
* Angle from Player to Entity?
	* tan(T) = `Y/X = D.y/D.x`
	* T = `atan2f(d.y, d.x)`, do not use `atan`, use `atan2`

Vector Normalization
* Vector length = distance it travels
	* Distance from Origin = (0, 0)
	* L = `sqrtf(x*x + y*y)`
* Normalizing a vector maintains its angle, but changes its length to 1 (unit vector)
	* N = `Vec2(V.x/L, V.y/L)`
* Where is this useful?

Example: Shooting
* Player `(px, py)`, Mouse `(mx, my)`
* Assignment 2
	* Bullet shoot toward mouse
	* We are given speed scalar S
	* How to get velocity vector?
* `D = (mx-px, my-py)`
* D has some length L, we want it to be S
* Normalize D, `N = (D.x/L, D.y/L)`, now length 1
* Vec2 `velocity(S*N.x, S*N.y)`
* Another way:
	* D has angle T = `atan2f(D.y, D.x)`
	* Recall: "Vector from Angle & Distance"
	* Vec2 `velocity(S*cos(T), S*sin(T))`

Circle Collisions
* For Assignment 2, all Entities have been given circular Collision components
* Entity therefore has 2 components which will determine a collision
	* Entity Position (x, y)
	* Entity Collision Radius (r)
* How can we determine a collision?
* Given two circles
	* `C1(x1,y1,r1), C2(x2,y2,r2)
* Distance between them
	* Diff Vector `D=(x2-x1, y2-y1)
	* Distance = `sqrt(D.x*D.x + D.y*D.y)`
* Collision = Distance < Radius Sum
	* Collision = Distance < r1 + r2
* Square Root is expensive
* Needed for actual distance
* Do we need actual distance?
* `sqrt(D.x*D.x + D.y*D.y) < r1 + r2
	* We can square both sizes
* `DistSq = D.x*D.x + D.y*D.y
* Collision = `DistSq < (r1+r2)*(r1+r2)`
* Don't calculate actual distance, save the ?

Random Numbers in C++
* Simplest way to generate random numbers in C++ is rand() function, which is also present in C
* For serious security, this function is not considered secure/safe, but for this course it is fine for our needs
* Random number generators take a 'seed' which determines the outcome of future RNG
* We can seed rand() with `srand(int)`
* Default seed is 0, so if not specified, your program will generate the same sequence of RNG every time
* Example: `srand(time(null))` seeds based on current second

Random Number in Range
* Given range `[min, max], [] = inclusive`
* `diff = 1 + max - min;`
* `r = rand() % diff`
	* r is now a random value from `[0, diff-1]`
* `r = r + min`
	* add back the minimum to get proper range
* `r = min + (rand() % (1 + max-min))`

Random in Range Example
* `r = min + (rand() % (1 + max-min))`
* Range = `[30, 40]`
	* `Diff = 1 + 40 - 30 = 11`
	* `r = rand() % 11 = [0, 10]`
	* `r = r + min = 30 + [0, 10]`
* Range = `[20, 20]`
	* `r = 20 + (rand() % (1 + 20 - 20))`
	* `r = 20 + (rand() % 1)`
	* `r = 20`