[COMP4300 - Game Programming - Lecture 08 - Collision Detection and Resolution](https://youtu.be/nQnELmUIVFk?si=EwrHY58oEUIbqdI4)

Collision in Games
* Collision programming involves 2 stages
* Collision Detection
	* Given two moving entities with some given shape/bounds, determine if they intersect
	* Detection is a geometric problem
* Collision Resolution
	* Given that two entities have collided, determine what to do to 'resolve' the physics of the simulation

Assumptions for Slides
* We will use the course definition of 'Entity'
* Assume that Entities have the following
	* Position (x, y)
	* Bounding Shape (circle, rectangle, lines)
	* Velocity (dx, dy)
* Our goal is to learn the mathematics that allows entities to interact in our game

Collision Detection Problems
* Given two entities which have a speed, position, and bounding shape, determine if, when, where they will collide (intersect)
* Given two entities which have a current position (only), do they intersect? (bool)
* Given two entities which have a current position (only), what is the overlap? (x,y)

Entity Bounding Shapes
* In the real world, objects can have arbitrary shapes/interaction surfaces
* These are difficult to store, compute, and simulate accurately/continuously
* All shapes in computer simulations must be made of primitive types
	* 2D: Line, Triangle, Circle, Rectangle
	* 3D: Sphere, Plane, Cylinder, Prism
* Bounding shape concept: simulate the interaction bounds of a complex shape with a simpler one

Entity Bounding Shape Example (9:20)

2D Bounding Shapes
* The simplest 2D bounding shape to store and compute is a circle with given radius
	* We have already covered circle intersection
* Other 2D bounding shapes can be constructed with a number of line segments
* The simplest of these is a rectangle
	* Bounding Rectangle / Bounding Box

Bounding Box Size
* Most classic 2D games use rectangular sprite graphics
* These rectangular images make a natural fit for rectangular bounding box
* Usually the bounding box will be the smallest rectangle that fits the sprite, so we can use the texture size as the bounding box size
* However sometimes, this is not the case
* Sometimes the bounding box does not fit the exact bounds of the sprite texture
* So make a custom sized bounding box

Axis Aligned Bounding Boxes
* Rectangles in a 2D plane can take any orientation as long as all 4 corners form 90 degree angles (def of rectangle)
* By aligning our rectangles with the x/y axis, we can trade arbitrary alignment for very fast intersection/collision checks
* Axis Aligned Bounding Box = AABB

Point Inside AABB
* The simplest collision check we can do is to see if a single point is inside an AABB
* Point P inside rectangle (x1,y1,x2,y2) if px > x1, px < x2, py > y1, py < y2
* Point P inside rectangle (x,y,w,h) if px > x, px < x+w, py > y, py < y+h

AABB intersection
* Next step is to determine if two AABB intersect anywhere

AABB Horizontal Overlap
* Horizontal overlap occurs if the top of each box is higher than the bottom of the other
* Horizontal Overlap of two rectangles (x1,y1,w1,h1) and (x2,y2,w2,h2) occurs if y1 < y2 + h2 and y2 < y1 + h1

AABB Vertical Overlap
* Vertical overlap occurs if the left of each box is less than the right of the other
* Vertical Overlap of two rectangles (x1,y1,w1,h1) and (x2,y2,w2,h2) occurs if x1 < x2 + w2 and x2 < x1 + w1

AABB intersection
* Two AABB intersect if they overlap both horizontally and vertically
* AABB intersect if x1 < x2 + w2 and x2 < x1 + w1 and y1 < y2 + h2 and y2 < y1 + h1
* This tells us IF two AABB intersect, but does not tell us by how much

Centered AABB Intersection
* Our game engine uses the CENTER of the entity as the position for the AABB
* How do we calculate the overlapping area for center positioned AABB?
```
class CTranform : public Component
{
public:
  Vec2 pos     = {0.0, 0.0};
  Vec2 prevPos = {0.0, 0.0};
  Vec2 scale   = {0.0, 0.0};
  Vec2 speed   = {0.0, 0.0};
  float angle = 0;
}

class CBoundingBox : public Component
{
  Vec2 size;
  Vec2 halfSize;
  CBoundingBox(const Vec2 &s)
    : size(s), halfSize(s.x / 2, s.y / 2) {}
}
```
* dx = abs(x1 - x2)
* Overlap in x-direction (w1 / 2) + (w2 / 2) - dx positive if x overlap
* dy = abs(y1 - y2)
* Overlap in y-direction (h1 / 2) + (h2 / 2) - dy positive if y overlap
```
delta = [abs(x1 - x2), abs(y1 - y2)]
ox = (w1 / 2) + (w2 / 2) - delta.x
oy = (h1 / 2) + (h2 / 2) - delta.y
overlap = [ox, oy]
```

Resolving Collisions
* Now that we have detected an AABB collision, what do we do?
* Resolution of collisions depends entirely on what the physics of your world should do
* For example: If the player collides with a tile, it should be 'pushed out' to no overlap

Detecting Collision Direction
* When detecting collisions in our game, they will first happen on a specific frame
* The frame before the collision is detected, there will be no collision
* Could try to use overlap to detect which direction the movement came from? NO we couldn't.
* Use previous overlap to detect direction
* If previous frame `overlap.y > 0`, then movement came from the side
* Resolve collision by pushing in x direction
* If previous frame `overlap.x > 0`, then movement came from the top or bottom
* Resolve collision by pushing in y direction
* A collision came from the top if it had a previous `overlap.x > 0` and its y is higher
* A collision came from the bottom if it had a previous `overlap.x > 0` and its y is lower

Tricky Resolution Case
* What if the previous frame had no overlap in either x of y direction?
* Must decide how to resolve collision based on previous position and/or overlap size
* What if an entity overlaps with multiple entities? Example: player moving near stack.
* Order of collision checks may decide which resolution occurs, can be tricky
* The 'previous overlap' method discussed here usually resolves this case, using previous position only does not

Important Note
* All collision resolution discussed here is for the simple case of avoiding overlap
* Useful for: walking on tiles, not being able to pass through tiles, bullet hits entity
* Many other kinds of collision resolution:
	* Kinematic object elastic/inelastic collision
	* Physics simulators with mas/forces

Side Note: Super Mario Bros
* Super Mario Brow collisions operate slightly differently than we presented
* Consider the case of Mario jumping on top of an enemy, how is that detected?
* Same AABB overlap is used, however Mario kills the enemy if he is moving downward (with x-direction ignored)

Recommended Videos
*  "How is this speed run possible?" by Bismuth: [https://www.youtube.com/watch?v=_FQJEzJ_cQw](https://www.youtube.com/watch?v=_FQJEzJ_cQw)
* "Walls, Floors, & Ceilings " by Pannenkoek: [https://www.youtube.com/watch?v=UnU7DJXiMAQ](https://www.youtube.com/watch?v=UnU7DJXiMAQ&pp=0gcJCTAAlc8ueATH)
* "SM64 - Watch for Rolling Rocks - 0.5x A Presses " by Pannenkoek: [https://www.youtube.com/watch?v=kpk2tdsPh0A](https://www.youtube.com/watch?v=kpk2tdsPh0A)
