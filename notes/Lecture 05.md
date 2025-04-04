
[COMP4300 - Game Programming - Lecture 05 - Introduction to ECS](https://youtu.be/ChY0GETWn1o?si=6V6lKOMpLqHNd40t)

ECS Game Programming
* Why use ECS for games?
* ECS = Entities, Components, Systems
* Entity: Any object in the game
	* Player, Platform, Tile, Bullet, Enemy
* ECS is actually a software design paradigm
* Let's first look at Object-Oriented design

Important Notes
* Large AAA games can and have been made with Object Oriented Design for years
* I have decided to teach ECS because in my opinion it is better for this course, and will allow us to do many thins quite easily
* Some agree with using ECS and some prefer OOD, neither is 100% probably best

Object-Oriented
* OOP/ OO Design helps us manage data / code
* Entity: any object in game
	* Each of them have (x, y) position
	* Entity used as base-class
	* Tile, Player, inherit from it
* Example diagram at 4:00

Entity-Component
* Entity: object in game
* Component: properties that can attach to Entities
* Component examples:
	* Position, Speed
	* Bounding Box
	* Health, Weapon, Damage
* Example diagram at 6:00

ECS Game Programming
* ECS uses Composition-based design
* Entity: Anything object in the game
	* Player, Platform, Tile, Bullet, Enemy
* Component: Properties attached to entities
	* Position, Texture, Animation, Health, Gravity
	* Components are PURE DATA
* Systems: Code/Logic that drives behavior
	* Movement, Rendering, Sound, Physics

ECS Example Entity/Component
* Player
	* Position, Speed, BBox, Sprite, Health, Gravity, Input
* Enemy
	* Position, Speed, BBox, Sprite, Health, AI
* Bullet
	* Position, Speed, Angle, BBox, Sprite, Damage, Lifespan
* Tile
	* Position, BBox, Sprite

Important Notes
* This is not the only way to implement ECS
* The architecture in this course is a balance between efficiency, ease of use, and ease of learning for an undergraduate class
* We will make improvements as we go and at the end of the course discuss how we would optimize things if making a real game

ECS Example System
```
// movement system
for (e : entities) { e.pos += e.speed; }
// collision system
for (b : bullets)
  for (e : enemies)
    if (Physics:IsCollision(b, e))
      e.health -= b.damage;
      b.destroy();
// rendering system
for (e : entities) { window.draw(e.sprite, e.pos); }
``` 

Engine Architecture
```
GameEngine
  > Scene
    > Systems
    > EntityManager
      > Entity
        > Component
```

What is a Component?
* An ECS Component is just DATA!
	* (Maybe some logic in the constructor)
	* No helper functionality within components
* A Component class has some intuitive meaning to an Entity which contains it
	* Example: Position, Gravity, Health, etc...
* How to implement them in C++?

What is an Entity?
* Entity = any object in the game
	* Usually, any object with a position
* No unique functionality, typically just stores a number of Components
* Stores at most 1 of each component type
* How Components are stored and used within an Entity can be a complex topic

Components Storage Options
1. Since there will be at most one of every component, store a variable for each component type.
2. Store a single collection of Components. We can then use `addComonent/getComponent` functions for more generality than individual variables

Option 1: Component Variables
```
class Entity
{
public:
  CTransform cTransform;
  CName      cName;
  CShape     cShape;
  CBBox      cBBox;
  Entity() {}
}

// usage
Entity e;
e.ctransform = CTransform(Vec2(100, 200));
e.cName      = CName("Red Box");
e.cShape     = CShape(args);
if (e.cTransform.exists) { ... }
// pro:  memory safe, contiguous memory, no heap usage
// cons: - directly accesing variables is undesirable
//         - requires knowledge of internal structure of Entity class
//         - unsafe, allows users to derectly modify vars
//       - Entity 'contains' component check requires another variable
//         - also requires knowing that variable exists
```

Option 2: Component Raw Pointers
```
class Entity
{
public:
  CTransform * cTransform = nullptr;
  CName      * cName      = nullptr;
  CShape     * cShape     = nullptr;
  CBBox      * cBBox      = nullptr;
  Entity() {}
  ~Entity() {} // must free memory
}

// usage
Entity e;
e.cTransform = new CTransform(Vec2(100, 200));
e.cName      = new CName("Red Box");
e.cShape     = new CShape(args);
if (e.cTransform) { ... }
// pro:  Entity 'contains' component check slightly easier
// cons: - memory leak, must check and delete
//         - makes this method very compersome and unsafe
//       - heap memory usage makes it slow as well
//       - direct variable access undesirable
```

Option 3: Component Smart Pointers
```
class Entity
{
public:
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CName>      cName;
  std::shared_ptr<CShape>     cShape;
  std::shared_ptr<CBBox>      cBBox;
  Entity() {}
}

// usage
Entity e;
e.cTransform = std::make_shared<CTransform>(Vec2(100, 200));
e.cName      = std::make_shared<CName>("Red Box");
e.cShape     = std::make_shared<CShape>(args);
if (e.cTransform) { ... }
// pro:  - memory safe, shared ptr implements RAII
//       - Entity 'contains' component check is still easy
// cons: - slower, smart pointer overhead
//       - still slow due to heap memory usage
//       - longer syntax, pain to type every time
//       - still allows direct memory access
```

Option 4: Component Container
```
class Entity
{
  std::vector<Component> m_components;
public:
  Entity() {}
  void add<T>(args);
  T& get<T>();
}

// usage
Entity e;
e.add<CTransform>(Vec2(100, 200));
e.add<CName>("Red Box");
e.add<cShape>(args);
if (e.has<CTransform>()) { ... }
// pro:  - abstracts away storage of components details
//         - allows storage changes without usage code changes
//       - components stored contiguously in memory
//       - least amount of code to type
// cons: - ends up being slow in practice due to implementation details
//         - difficult to implement, must map Component class to index
```

Option 5: Component Tuple
```
class Entity
{
  std::tuple<C1, C2, C3, C4> m_components;
public:
  Entity() {}
  void add<T>(args);
  T& get<T>();
  bool has<T>();
  void remove<T>();
}
// much faster and easier in practice than vector-based

// usage
Entity e;
e.add<CTransform>(Vec2(100, 200));
e.add<CName>("Red Box");
e.add<cShape>(args);
if (e.has<CTransform>()) { ... }
// pro:  - abstracts away storage of components details
//         - allows storage changes without usage code changes
//       - tuple uses stack, but not guaranteed contiguous
//       - least amount of code to type
```

Additional Entity Variables
```
class Entity
{
  std::tuple<C1, C2, C3, C4> m_components;
  bool                       m_alive = true;    // if false, will delete entity
  std::string                m_tag = "default"; // type of entity
  size_t                     m_id = 0;          // unique integer id
public:
  Entity() {}
  void               add<T>(args);
  T&                 get<T>();
  size_t             id() const;
  bool               isAlive() const;
  void               destroy();
  const std::string& tag() const;
}
```

Convenient Syntax
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

// so in the above code we can say
ComponentTuple m_components;
```

Using `std::tuple`
```
// declare tuple of various types
std::tuple<int, double, int, float, size_t> myTuple;
// if type present only once, can std::get by type
std::get<double>(myTupple) = 3.14159;
// if not unique, you can specify the 'index'
std::get<2>(myTuple) = 42;
// same syntax used to get values for use
std::cout << std::get<double>(myTuple) << "\n";
// can also initialize with different types
std::tuple<int, std::string, float> myTuple(42, "Hello", 3.14f);
```

Entity Functions: get
```
template <typename T>
T & get()
{
  return std::get<T>(m_components);
}
// const version to keep const correctness
template <typename T>
const T & get()
{
  return std::get<T>(m_components);
}
```

Entity Functions: has/remove
```
template <typename T>
bool has() const
{
  return get<T>().exists;
}

template <typename T>
void remove()
{
  get<T>() = T();
}
```

Entity Functions: add
```
template <typename T, typename... TArgs>
T& add(Targs&&... mArgs)
{
  auto & component = get<T>();
  component = T(std::forward<TArgs>(mArgs)...);
  component.exists = true;
  return component;
}
// component default constructor sets 'exists' to false
// only set it to true when we explicitly add a component
```

Component Implementation
* We will have a different Component class for each Component we want to implement
* Naming: class `CComponentName`
* Components will inherit from a base Component class which stores any data we want to be common to all components

Component Base Class
```
class Component
{
public:
  bool exists = false;
};
// exists = whether entity actually contains the derived component
// default to false so default constructed components
// are not considered part of the Entity
```

ECS Component: CTransform
```
class CTransform : public Component
{
public:
  Vec2 pos = {0,0};
  Vec2 velocity = {0,0};
  CTransform() {}
  CTransform(const Vec2 & p, const Vec2 & v)
    : pos(p), velocity(v) {}
};
```

ECS Component: CShape
```
class CShape : public Component
{
public:
  sf::CircleShape shape;
  CShape() {}
};
```

ECS Component: CLifeSpan
```
class CLifeSpan : public Component
{
public:
  int totalLifespan = 0;
  int remainingLifespan = 0;
  CLifeSpan() {}
  CLifeSpan(int totalLifespan)
    : totalLifespan(totalLifespan)
    , remainingLifespan(totalLifespan) {}
};
```

Using Entities
```
void doStuff(std::vector<Entity> & entities)
{
  for (auto& e : entities)
  {
    e.get<CTransform>().pos += e.cTransform.velocity;
    e.get<CShape>().shape.setPosition(e.cTransform.pos);
    window.draw(e.get<CShape>().shape);
  }
}
```

ECS: Systems
```
void sMovement(std::vector<Entity> & entities)
{
  for (auto& e : entities)
  {
    e.get<CTransform>().pos += e.cTransform.velocity;
  }
}
```
* Some systems only operate on entities which contain certain Components
	* Movement: CTransform
	* Render: CTransform + CShape
	* Physics: CBoundingBox
* We can filter entities before passing them into system, or just check entities for required components before using them
```
void sLifespan()
{
  for (auto& e : m_entityManager.getEntities())
  {
    if (!e->has<CLifespan()) { continue; }
    e.get<CLifespan>().remainingLifespan--;
    if (e.get<CLifespan>().remainingLifespan <= 0)
    {
      e.destroy();
    }
  }
}
```
```
void sRender()
{
  for (auto& e : m_entityManager.getEntities())
  {
    if (e.has<CShape() && e.has<CTransform>)
    {
      e.get<CShape>().shape.setPosition(e.get<CTransform>().pos);
      window.draw(e.get<CShape>().shape);
    }
  }
}
```
* Diagram at 59:20

Vectors in Games
* For 2D games, many quantities/properties of game objects have both an X and a Y coordinate, these form a 2D Vector
	* Mathematical Vector, not collection vector
* Example: Position(x, y) and Velocity(x, y)
* Annoying to store/refer to x and y separately
* Let's make a 2D Vector class: Vec2

Notes on Vec2
* We will go more in-depth on vector math for games later in the course
* We first introduce the programming architecture, then later use it to implement game math/physics concepts
* Note: `sf::Vector2f` already exists, we will basically be reinventing it in this course

Vec2 Class
* What do we want to store in Vec2?
	* Member variables x, y
* What do we want to do with those values?
	* Functions/Operations on Vec2
* Possible Operations on Vec2
	* `+, -, *, /`
	* length, normalize, rotate, etc...
```
class Vec2
{
public:
  float x = 0, y = 0;
  Vec2();
  Vec2(float xin, float yin);
  bool operator == (const Vec2 & rhs) const;
  Vec2 operator + (const Vec2 & rhs) const;
  Vec2 operator * (const Vec2 & rhs) const;
  void operator += (const Vec2 & rhs);
};
```

Vec2 Sample Usage
```
Vec2 pos      (100,  200);
Vec2 velocity ( -5,   10);
Vec2 accel    (  0, -9.8);
while (true)
{
  pos += velocity;
  velocity += accel;
}
float dist = velocity.length();
```
* Live coding at 1:04:30
* I have code of Vec2 class starting in assignment 2, so I won't take notes of code here.
