[COMP4300 - Game Programming - Lecture 13 - Ray Casting, Lighting, Vision](https://youtu.be/rHI8WP3zdF0?list=PL_xRyXins84_Jf-aCh7chj47HR4oZLPwK)

What is Ray Casting?
* Ray Casting is the name for a technique which has many applications in games
* Ray Tracing = Iterative Ray Casting
* Intuitively in 3D: "Cast rays outward from my eye to the world, what do they hit?"
* Very heavy on vector math, but has many useful applications
Diagram at 1:53

Ray Casting
* Draw a line between two points (when we say line in this context we mean line segment)
* Detect if and where a collision occurs
* Ray Tracing = Where did it  'bounce' to?
* If no collision occurs on a line between two points, then they can "see" each other
* 2D Ray Casting priparily solves problems that deal with light/visibility
2D Ray Casting - Lighting (diagram at 5:40)
Nice article about [2D Ray Casting - Visibiliry](https://www.redblobgames.com/articles/visibility)
Example game that uses this is Third Eye Crime - Moonshot Games

Ray Casting Basics
* Ray Casting asks "is anything blocking the path between these two 2D points?"

Line Segment Intersection
* The "path" between two points is "blocked" if the line segment between them intersects with another existing line segment
* Depending on the application, we may also want to know the "first" such intersection, and exactly where it occurred
* So far in the course, we have been using the vec2 class to represent points in 2D space as a 2D Vector with (x, y)
* A line segment can be defined by the start and end point of a line using vec2 (line segment AB)

Line Segments
* Points a, b
	* Stored as vec2
* Line segment ab
* Let's vector subtract
* r = b - a
	* So: b = a + r
* Let's scale r by a scalar t
* b = a + t * r (t = 1)
* Add another line
* Points c, d
* Line segment cd
* s = d - c
* d = c + u * s
* Scale s by scalar u
* When r and s have correct lengths, the segments intersect
* To find line segments intersection we need to find the correct scale values for r and s that make them meet
* b = a + t * r
* d = c + u * s
* Lines intersect when b and d are equal
* a + t * r = c + u * s
* Lines intersect if both t and u between [0, 1]
* How to solve for two unknowns t and u?
* 2D Cross Product to the rescue!

3D Cross Product
* Cross Product a X b
* Produces a 3D vector which is orthogonal to both vector a and b
* We get a "Normal Vector" to a, b
* Doesn't make intuitive sense when applied to 2D vectors

2D cross product
* a X b = a.x * b.y - a.y * b.x
* Produces a scalar
* Important properties
	* a X a = 0
	* (a + r) X s = a X s + r X s

Intersection: Solve for t
* a + t * r = c + u * s
* Apply cross product s both sides
* (a + t * r) X s = (c + u * s) X s
* a X s + t * (r X s) = c X s + u * (s X s)
* s X s = 0
* a X s + t * (r X s) = c X s
* t * (r X s) = c X s - a X s
* t = (c X s - a X s) / r X s
* t = ((c  - a) X s) / r X s

Intersection: Solve for u
* a + t * r = c + u * s
* Apply cross product r both sides
* u = ((a  - c) X r) / s X r

Intersection solve
* t = ((c  - a) X s) / r X s
* u = ((a  - c) X r) / s X r
* Another property: (r X s) = -(s X r)
* Rearrange above
	* u = ((a  - c) X r) / s X r
	* u = ((c  - a) X r) / r X s

Optimized Version
* t = ((c  - a) X s) / r X s
* u = ((c  - a) X r) / r X s
* (c - a) and (r X s) used twice
* Compute them only once!

Final Version
* t = ((c  - a) X s) / r X s
* u = ((c  - a) X r) / r X s
* Intersection if:
	* t belongs to [0, 1] and 
	* u belongs to [0, 1]
* Intersection point:
	* p = a + t * r or
	* p = c + u * s

Line Segment Intersection
```
1. struct Intersect { bool result; vec2 pos; };
2. Intersect LineIntersect(vec2: a, b, c, d)
3.   vec2 r = (b - a);
4.   vec2 s = (d - c);
5.   float rxs = r X s;
6.   vec2 cma = c - a;
7.   float t = (cma X s) / rxs;
8.   float u = (cma X r) / rxs;
9.   if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
10.    return {true, vec2(a.x + t*r.x, a.y +t*r.y)};
11.   else
12.     return {false, vec2(0, 0)};
```

2D Game Visibility
* Now that we know how to do line segment intersection, how do we use it for visibility?
* If line segment ab does not collide with any vision-blocking entities, b is visible to a
* What about non-line game objects?
* Easy: they are made up of line segments

2D Game Lighting
* Entity can see up to the closest intersection
* Anything beyond that is outside vision

2D Game Lighting Effects
* Light travels outward in all directions
* How do we calculate everywhere it lands?
* There are several possible solutions
* Let's look at some interactive demos
	* https://ncase,me/sight-and-light/
	* https://www.redblobgames.com/articles/visibility/
