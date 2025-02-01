[COMP4300 - Game Programming - Lecture 19 - Caching + ECS](https://youtu.be/IF1YHCvMP54?si=Y8n9Ti_OuPHNdGPx)

Game Performance
* There are many factors that determine the overall performance of our game
* With ECS, we can profile our systems independently and determine which are bottlenecking the program's speed
* Typical bottlenecks are Physics, AI, Rendering, Ray Casting, etc.

C++ Profiling Videos
* Visual Benchmarking in C++
	* Manual Tools for Profiling / Visualization
	* https://www.youtube.com/watch?v=xIAH4dbMVnU
* Debugger/Diagnostics in VS 2019
	* Tips/Tricks for VS2019
	* https://www.youtube.com/watch?v=i6gdmT-BdOU
* Tuning C++: Benchmarks, CPUs, Compilers
	* https://www.youtube.com/watch?v=nXaxk27zwlk
* More in the next lecture

Non-Obvious Performance Hits
* It's easy to think that the only things that affect performance are algorithms / math
* This is somewhat true - changing an n^2 algorithm to log(n) can save lots of time
* However, in practice, often times the implementation of these algorithms can impact performance even more

Important Optimizations
* Using more efficient algorithms
	* This is where computer science helps
	* But also where computer science stops
* Minimize size of data structures
* Code re-use and modularity
* Minimize dynamic heap allocations
	* Memory Pooling
* Write cache-friendly code
	* Data-oriented design/programming

Minimizing Data Structure Size
* C++ gives us fine control over the size of data structures we can use for games
* Deciding on a size type for your data depends entirely on the context for your game, function, structure, etc.
* Minimizing data structure size can help your game in several ways
1. Reducing size = use less memory; Obvious benefit: less RAM usage
2. Reduced size = less time copying data; If data needs to be copied in your program this can dramatically time (example: vector)
3. Size < 8 bytes = pass by value; Eliminates dereference on function call / cache miss on function calls

Data Size Example (at 10:52)
* Game world size 64x64 
* Don't need full `int` to store player location
* `uchar` = 256 values
* But maybe later, map will be bigger? Careful
* Optimize once finalized

Code Reuse and Modularity
* We have talked about this before
* Put any re-used code into its own function
* Optimize the function in one place
* Entity Manager class - We use this all the time without having to know how it works
* Any optimizations underneath are invisible and don't require to level changes

Minimize Dynamic Allocations
* Dynamic heap allocations are expensive
* Call to malloc(), OS dependent
* Tries to find a contiguous block of memory of a given size
* Given the complexities of memory layout, is much much slower than stack allocation

Memory Pooling
* Heap memory is slower to allocate
* Stack memory is too limited (few MB)
* What we could do: Allocate a large chunk of heap memory at the start of the program and use only that space (memory pooling)
* Eliminates allocations during runtime
* However, adds the complexity of managing the memory pool and where to put new objects

Cache Coherency
* Writing cache-friendly code is one of the most important speed optimizations in practice
* When data is fetched from RAM, surrounding values also fetched and stored in cache (~64b typical)
* Cache memory is faster, physically closer, but is much smaller than normal RAM
* Organizing frequently used data contiguously allows cache to fetch more of it each lookup
* When RAM is not in cache = 'cache miss'

Memory Lookup Procedure
* When you access data in code, the following sequence occurs:
	1. Look up data in L1 cache
	2. Look up data in L2 cache
	3. Look up data in L3 cache
	4. Look up data in DRAM
* Illustration at 24:05

DRAM vs SRAM
* DRAM: Dynamic Random Access Memory
	* Used in main system memory (RAM)
	* Stores data in capacitors, which require periodic refreshing (more power)
	* Lower cost per byte (cheaper to make)
	* Slower than SRAM
* SRAM: Static Random Access Memory
	* Used in CPU cache memory
	* Stores data in flip flops, which do not need refreshing like DRAM
	* Higher cost per byte
	* Faster than DRAM
	* Used in conjunction with DRAM for balance

Cache Misses
* Cache misses are expensive
	* Mote: Example Calculations (not exact)
* Cache miss ~= 200 cycles
* Multiplication = 3 cycles
* You can do 65 multiplications in the same time it takes you to look up an variable value in RAM if it's not cached

Example 1: Memory accesses and performance
How much faster do you expect Loop 2 to run, compared to Loop 1
```
int[] arr = new int[64 * 1024 * 1024];

// Loop 1
for (int i = 0; i < arr.Length; i++) arr[i] *= 3;

// Loop 2
for (int i = 0; i < arr.Length; i += 16) arr[i] *= 3;
```
Loop 2 does 6% of the CPU 'work', but takes same time
* Memory access dominates running time
* Example: Cache will grab 64b = 16 `ints`
* Loop 1 has 1 normal lookup and 15 cached lookups (almost free)
* Loop 2 has 1 normal lookup, but 1/16th of them. Similar memory access time
* Multiplications are pipelined, negligible

More Cache Examples
* [Gallery of Processor Cache Effects](http://igoro.com/archive/gallery-of-processor-cache-effects/)
* [Data-Oriented Design and C++ Writing cache-friendly C++ code](https://www.youtube.com/watch?v=rX0ItVEVjHc)

Cache Friendly Data Structures
* In order to avoid cache misses, we want to store data contiguously in memory
* Arrays and vectors do this, but certain lists, sets, maps, dictionaries do not
* Linked lists often use pointers to heap allocated memory, which is very slow
* Use vector / array where possible

Vector Note
* A vector is a dynamically sized array
* When a vector is filled, it will create a new, larger temp array and copy everything over
* To avoid vector resize, we can reserve an amount of space equal to some maximum size
	* `vec.reserve(MaxElements)`
	* Uses a single malloc, all RAM used immediately
	* Inserts into vector won't resize until > `MaxElements`

Vector Storage Options
* `vector<Data>`
	* Storing raw data in vector makes it contiguous
	* Raw data is larger, more expensive to copy
* `vector<Data *> vec;`
	* Storing pointers usually means non-conriguous data
	* Pointers are easier to copy, but not cache friendly (unless what it points to is intentionally contiguous (pooled))
	* If `std::shared_ptr`, everything will cache miss
* Right now, `EntityManager` class has a vector of pointers
* Every time we get entity -> cache miss

Vector: Store Data vs Pointer
* Contiguous data allows cache to fetch multiple
* When pointers are cached, not the data they point to, each dereference will likely be a cache miss, unless the data it points to is contiguous

ECS and Caching
* Entities are containers for components
* Entities currently contain all component data in a `std::tuple`, which is contiguous
* If all of the Entity components fit within the cache line, the entire entity will be cached for use
* However, is this what we want?
* If Entities stored in Vector, they are contiguous
* Cache could load multiple entities
* Currently, we store `shared_ptr<Entity>`, but let's pretend that we are doing it this way
* Entities stored contiguously, components of same type are not. Why does this matter?
* Storing Entities contiguously makes sense, since the cache can now fetch multiple entities
* However, caching is all about ACCESS
* How we want to access the date inside Entities determines how we should store the data
* Let's look at some examples of the systems we use in our game engine

ECS Systems Example
```
// movement
for (auto e: entities)
	e.get<Transform>.pos += e.get<Transform>.velocity;
// rendering
for (auto e: entities)
	draw(e.get<Animation>.sprite);
// collisions
for (auto e: entities)
	// e.get<CBoundingBox>;
	// e.get<Transform>;
```

ECS Systems and Caching
* ECS systems usually only access one or two different types of components for each Entity that they operate on
* Given this access pattern, we would want the cache to store similar components rather than entire entities
* Components are small, so the cache line can fit many more than full Entities
* Ideally, this is what we want to have (illustration at 51:41). ECS systems operate on similar components. Components contiguous = more cache hits. How can we achieve this?

ECS Memory Pool
* A memory pool is a pre-allocated chunk of memory that will be used for the storage of data of a given type (example: Entities)
* By pre-allocating space, we don't need to use dynamic memory allocation
* May need to know max size before hand
* Illustration at 53:40
* This memory pool can eliminate all runtime dynamic memory allocations by being big enough to fit all the entities
* We trade upfront memory for runtime speed
* When new entity created, we need to find an 'empty spot' for it in the table (memory pool)
* This is much faster than malloc in practice
	* Example: scan left to right for available space
	* When entity dies, mark its space available
	* Also save time by not 'deleting' dead entities
* Instead of storing vectors of Entities, let's have a vector of each Component type.
* Instead of columns (Entities) being contiguous, change so that rows (components) are contiguous
* Illustration at 56:08
* Each row is a `Vector<Component>`
* Each Entity ID is the index into the vector
* Now each Component stored contiguously

Entity as an Index
* The Entity class will no longer store its components as member data
* Entity will hold ONLY an integer index
* Entity functions will now access our memory pool and look up the data at the associated index
* How the hell do we implement this?

```
typedef std::tuple<
	std::vector<Component1>,
	std::vector<Component2>,
	std::vector<Component3>,
	std::vector<Component4>
> EntityComponentVectorTuple;

// to get an Entity's Component type T
EntityComponentVectorTuple data;
std::get<std::vector<T>>(data)[entityID];

// our system now changes like this:
class EntityMemoryPool
{
	size_t m_numEntities;
	EntityComponentVectorTuple m_data;
	std::vector<std::string> m_tags;
	std::vector<bool> m_active;
	EntityMemoryPool(size_t maxEntities);
public:
	static EntityMemoryPool & Instance() {
		static EntityMemoryPool pool(MAX_ENTITIES)	;
		return pool;
	}
	template <typename T>
	T& getComponent(size_t entityID) {
		return std::get<std::vector<T>>(m_data)[entityID];
	}
	const std::string & getTag(size_t entityID) const {
		return m_tags[entityID];
	}
	Entity addEntity(const std::string &tag)
	{
		size_t index = getNextEntityIndex();
		// set all vectors[index] = 0;
		// set tag[index], active[index] = defaults
		return Entity(index);
	}
};

class Entity
{
	size_t m_id;
public:
	template <typename T>
	T & getComponent() {
		return EntityMemoryPool::Instance().getComponent<T>(m_id);
	}
	template <typename T>
	bool hasComponent() {
		return EntityMemoryPool::Instance().hasComponent<T>(m_id);
	}
};

class EntityManager
{
	std::vector<Entity> m_entities;
public:
	Entity EntityManager::addEntity(const std::string &tag)
	{
		Entity e = EntityMemoryPool::Instance().addEntity(tag);
		m_entitiesToAdd.push_back(e);
		return e;
	}
};
```

ECS Memory Pool
* When we create the memory pool, we have many copies of a blank default constructed Component of each given type
* Recall: Each Component has a Boolean associated with whether a given Entity actually contains that component
* Entity also has an 'active' boolean

ECS Memory Pool Indexing
* When we want to put a new Entity into the memory pool, we need to find a location for it within the pool
* Simplest way: scan from beginning to end of pool to find the first location of a non-active entity and use that slot
* Illustration at 1:07:10