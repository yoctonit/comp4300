[COMP4300 - Game Programming - Lecture 15 - Pathfinding and Steering](https://youtu.be/OB_l5lJae6c?si=1kRal4w4E5sb7WUp)

Movement in Games
* Movement is arguably the most important part of a large number of video games
* It allows the exploration of the game world and interaction with entities
* Not only does the player move in a game world, but the NPCs / entities as well
* Good movement makes a good game
* Main video game movement problems:
	* Path-finding (how to get from A to B)
	* Path-following (how to move along path)
	* Physics (how environment reacts)
	* Animation (how entities look)
* In this lecture we will deal with decision making for movement (path find/follow)
* Multi-Agent Pathfinding: move as group, surround enemy

Pathfinding
* Video game maps are often filled with obstacles with which NPCs will collide
* Pathfinding is the method we use to determine which path an NPC should take from its current location to a goal location
* Pathfinding attempts to minimize a 'cost' of the path. It could be total length, fuel, damage taken, or an arbitrary function

Pathfinding Considerations
* Single Agent / Multi-Agent
* Single Goal / Multi-Goal
* Static (precomputed) / Dynamic (online)
* Space representation
	* Open / Grid / Graph / Poligons
* Movement directions
	* Fixed movement (up, down, left, right)
	* Arbitrary angles / movement
	* Move between nodes on a graph

Space / Action Representation
* Space and action representations are important decisions, ideas are linked
* Which representation to choose is based on many factors from the game's design
* The choice of space representation in the pathfinding engine will determine which movement actions can be performed

Choosing State Representation
* Choosing a state representation matters
	* Memory size dependent on representation
	* Defines which actions are possible
	* Defines which algorithms are applicable
* Try to find a balance between the computation required to find a path vs the final path accuracy
* Typically in games we choose a sparse state representation for easier computation, then later refine the path to look more realistic

Navigation Mesh
* What if my game has arbitrary geometry?
* Need a graph structure within our environment to enable pathfinding
* This graph is called a NavMesh
* Most game engines come with software to build and edit a level's NavMesh

Single-Agent Pathfinding
* Many graph search algorithms exist for finding shortest paths, each with their own properties
* Common algorithms: Breadth-First Search, Dijkstra, A*
* Most video game pathfinding has traditionally used some variant of the A* search algorithm

A* Search Allgorithm
* Most well-known Best-First search algorithm
* Evaluates nodes by using g(n) and h(n)
	* g(n) = path cost so far to current node n
	* Heuristic function f(n) = g(n) + h(n)
	* h(n) = estimate of cheapest solution via n
* Select node from open list with min f(n)
* Performance depends on properties of the heuristic function
```
function AStar(problem, h)
    closed = {}
    open = PriorityQueue(Node(problem.initial_state), f=g+h)
    while (true)
        if (open.empty) return fail
        node = pop_min_f(open)
        if (node.state is goal) return solution
        if (node.state in closed) continue
        closed.add(node.state)
        for c in Expand(node, problem) // for each neighboring node
            open.add(c)
```

Admissible Heuristic (skiped explanation in lecture)
* Never overestimates distance to goal
* Can be seen as 'optimistic' guesses
* h(n) <= h*(n)
* f(n) = g(n) + h(n) never overestimates true cost of a path through n when h(n) is admissible

[Pathfinding Demo - link dead](http://www.cs.mun.ca/~dchurchill/search/)

Path-finding Workflow (illustration at 38 min. approximately)
1. Identify on the problem you're solving
2. Construct a space representation
3. Decide which actions are possible
4. Be given a start/goal locations
5. Apply an algorithm which finds a path
6. Follow the path from start to goal

Path Following  (illustration at 42 min. approximately)
* Now that we have a path, how do we make our agent traverse it in the game?
* Simplest way: waypoints
	* Each point along the path is a waypoint
	* Path to the next waypoint is always 'clear'
	* Walk from one waypoint to the next
	* Stop when NPC reaches the goal (out of WP)

Vector Fields for Pathfinding  (illustration at 45 min. approximately)
* Flow fields, influence maps, vector maps
* Very efficient structure for path-finding in case of 'many paths to single goal'
* Any case where multiple NPCs need to go to a single location, consider using these
* Must balance computation time with how many NPCs, etc
* Last 4 Slides are from [this](https://gamedevelopment.tutsplus.com/tutorials/understanding-goal-based-vector-field-pathfinding--gamedev-9007) article

Influence Maps  (illustration at 51 min. approximately)
* Influence maps typically incorporate other logic than simple distance to goal
* Used when difficult to come up with an objective function for other algorithms
* Some examples include:
	* Distance to attacking enemies
	* Distance from walls/obstacles

Steering Behaviors
* Steering can be seen as a framework for controlling autonomous agents such as robots, CGI in movies, NPCs in games
* Steering adds more realism and fluidity to agent movement, can enhance experience
* Steering can be really fun to implement, play with parameters, and see what effects can be produced
* At its core, steering smooths the movement of Entities in games
* Changing direction instantaneously or 'stop on a dime' can look bad in games
* Steering acts as a sort of 'force' that smoothly guides an Entity toward a target
* Illustration ad 57:40
* Calculating the Desired Vector (Keep 'speed'):
```
	// instantaniously change direction from current (vel) to desired
	desired = target - pos
	desired.normalize()
	desired = desired * vel.length
```
* Calculating the Actual Vector (Steered Velocity):
```
	// change direction with steering
	scale = 0 < X < 1
	steering = desired - vel
	// scale steering vector (steering force)
	actual = vel + steering * scale
```

Steering Math
```
Given: vel, target, scale
1. desired = target - pos
2. desired = desired / desired.length // desired.normalize()
3. desired = desired * vel.length
4. steering = desired - vel
5. steerint = steering * scale
6. actual = vel + steering
```

Types of Steering (1:05 approximately)
* 'Seek' Behavior (what we described above)
* 'Pursue' Behavior (if target is moving)
* 'Flee' Behavior (multiply desired velocity with -1, we're running away from target)

Arrive Behavior
* One issue with steering is close to the target the entity will oscillate
* Must define an 'arrive' behavior
* Simplest solution is to scale speed linearly as it approaches target within some distance
* No 'best' values/algorithm, just try it!

[Steering Demo](http://opensteer.sourceforge.net)
