[COMP4300 - Game Programming - Lecture 20 - C++ Visual Code Profiling](https://youtu.be/6Pp_v1N_zV0?si=Y5HAbH_xYTCg3UFD)

Singletons
* A Singleton is a programming pattern
* Typically implemented with OOP
* A Singleton class ensures that only one instance of the class can be created
* Typical use case is for 'global' functionality
	* System Logger, Blackboard, etc.
* Implementation depends on language

C++ Singleton
* C++ makes implementing singleton easy
* We can make use of a static variable and a static function to ensure that only one instance of a class can be created
* We will also make the constructor of the class private so that no other code can 'accidentally' instantiate it
```
// Example C++ Singleton Class
class Singleton
{
	Singleton() {} // private constructor
public:
	static Singleton & Instance()
	{
		static Singleton instance; // only run once
		return instance;
	}
	void doThing() {}
};

// Outside usage: Singleton::Instance().doThing();
```

```
// File: Logger.hpp
class Logger
{
	std::ofstream m_fout = std::ofstream("log.txt");
	Logger() {}
public:
	static Logger & Instance()
	{
		static Logger instance;
		return instance;
	}
	void log(const std::string & str) { m_fout << str << "\n; }
};

// Outside usage: Logger::Instance().log("Hello there");
```

Removing Instance() call
* If you get annoyed by the Instance() call, you can do some trickery to eliminate it
* Note: Is tedious if many public functions

```
// File: Logger.hpp
class Logger
{
	std::ofstream m_fout = std::ofstream("log.txt");
	Logger() {}
	static Logger & Instance()
	{
		static Logger instance;
		return instance;
	}
	void logImpl(const std::string & str) { m_fout << str << "\n; }
public:
	static void Log(const std::string & str) { Instance().logImpl(str); }
};

// Outside usage: Logger::Log("Hello there");
```

Blackboards
* Game dev articles often refer to a data structure known as a 'BlackBoard'
* Global data storage / management
* Can be implemented as a Singleton
* Keeping everything in one class keeps it from polluting the global space
* Code is cleaner and safer than globals

Profiling Code
* Profiling code involves timing specific functionality in order to see how long different functions/methods take to run
* Visual Studio has profiling tools built-in, which can help with profiling your code
* There are several types of code profilers:

Event-Based Profilers
* Language or API has built-in events which trigger profiler data collection
* Provide hooks to events such as function calls, memory allocation, class construction, entering/leaving threads, etc.
* Rely on specific functionality implemented in the language or API being used
* Lowest resolution of 3 main profiler types
* illustration at 13:46

Statistical Profilers
* Collect profiling data via sampling
* The call-stack of a program is probed at regular intervals using OS interrupts
* Statistics about samples are displayed to the user for interpretation
* Example: 30% of samples collected showed the program was in the render() function
* Inexact results, but very little overhead
* illustration at 16:18

Instrumentation Profilers
* Instructions are added to the program to collect and output the desired type of profiling data
	* Can be done manually, by language, by compiler, etc.
* Profiling data will be exact, however the introduction of instrumentation code can change the runtime of the program
* Starting/stopping timers, collating data, file output can all take tangible amount of time
* Allows fine control over what we want to profile
* Let's implement a manual instrumentation timer

```
int DoSomeCalculation()
{
	PROFILE_FUNCTION(); // Manual Instrumentation code insertion
	// some code here which takes a while
}

void MyFunction()
{
	PROFILE_FUNCTION();
	for (int i = 0; i < 10; i++) {
		PROFILE_SCOPE("LoopIteratioin"); // Profile 1 loop iteration
		ReadSomeData();
		DoSomeCalculation();
	}
}
```

DEMO
Visual Studio Profiler
Sampling / Instrumentation
* At 20:55

C++ Code Timer
* Basic logic for timing code:
	* Record the starting time somewhere
	* Record the finish time somewhere
	* Elapsed = finish time - starting time
* `std::chrono` gives us timing tools
	* `std::chrono::high_resolution_clock::now();`
	* Can use this to calculate time elapsed

C++ Timer Class
* Having to manually record start and end times gets cumbersome, let's make a class
* C++ Timer class functionality
	* Store the start time via start()
	* Query how much time passed via elapsed()
	* Ability to start/stop the timer at will
	* Abstracts away the `std::chrono` syntax

```
// File: Timer.hpp
#include <chrono>
class Timer
{
	long long m_start = 0, m_end = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_stp; // start time point
public:
	Timer() { start(); }
	void start() { m_stp = std::chrono::high_resolution_clock::now(); }
	long long elapsed() // returns time elapsed in microseconds
	{
		using namespace std::chrono;
		auto etp = high_resolution_clock::now(); // end time point
		m_start = time_point_cast<microseconds>(m_stp).time_since_epoch().count();
		m_end = time_point_cast<microseconds>(m_etp).time_since_epoch().count();
		return m_end - m_start;
	}
};

// Example Timer usage
#include "Timer.hpp"

int main()
{
	Timer t;
	// maybe some other things here
	t.start();
	functionThatTakesSomeTime();
	auto elapsed = t.elapsed();
	std::cout << "Elapsed time is " << elapsed << "us\n";
}
```

Visual C++ Profiling
* The following slides and code were inspired by a YouTube video by TheCherno:
	* [Visual Benchmarking in C++](https://www.youtube.com/watch?v=xIAH4dbMVnU)
* This code is an updated, simplified version with several bug fixes / improvements
	* [TheCherno GitHub Discussion](https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e)

Visual Code Profiling
* Humans are visual creatures, staring at text output is difficult to understand
* What if we want to visually profile code?
* What if we could get something like this (chart at 41:22)

chrome://tracing
* It turns out that chrome has really nice visual tracing functionality built-in
* Initially meant for website profiling, it can input files in specific `json` format
	* Chrome Tracing JSON Format Specification
* If we can output our timing code with this specific format, chrome can visualize it!

```
// chrome://tracing json format
{
	"name": "myName",        // event name
	"cat": "category,list",  // event category
	"ph": "B",               // event type (usually 'X')
	"ts": 12345,             // tracing clock timestamp (us)
	"pid": 123,              // process id
	"tid": 456,              // thread id
	"args": {
		"someArg": 1,
		"anotherArg": {
			"value": "my value"
		}
	}
}

// example tracing file
{
	"otherData": {},
	"traceEvents":
	[
		{ "cat": "function", "dur": 2000565, "name": "Function1, "ph": "X", "pid": 0, "tid": 3414070038, "ts": 2515589687304},
		{ "cat": "function", "dur": 1000526, "name": "Function2, "ph": "X", "pid": 0, "tid": 3414070038, "ts": 2515591688288},
	]
}
```
* DEMO at 44:20

Visual Profiling in C++
* How do we transition from the Timer class to outputting this JSON file to visualize?
* Timer class cannot output to file itself because all data needs to go in single file
* If only we had a way to globally write to a single file somehow...
* Singleton to the rescue!

```
// File: Profiler.hpp - Initial Implementation
class Profiler
{
	std::ofstream m_fout = std::ofstream("result.json");
	Profiler() { /* write JSON header */ }
public:
	static Profiler & Instance()
	{
		static Profiler instance;
		return instance;
	}
	void writeProfile(const ProfileResult & r) { /* write result JSON */ }
	~Profiler() { /* write JSON footer */ }
}; // singleton will destruct when program exits normally
// Main Idea. When Timer stops, call Profiler::Instance().writePRofile()

// File: ProfileResult struct
struct ProfileResult
{
	const std::string name = "Default";
	long long start = 0;
	long long end   = 0;
	size_t threadID = 0;
};

// Previous Timer elapsed() function pseudocode
long long Timer::elapsed()
{
	m_start = // calculate start time
	m_end =   // calculate end time
	return m_end - m_start;
}
// New ProfileTimer stop() function
void ProfileTimer::stop()
{
	m_result.m_start = // calculate start time
	m_result.m_end =   // calculate end time
	Profiler::Instance().writeResult(m_result);
}

// Example usage of new ProfileTimer
#include "ProfileTimer.hpp"

int main()
{
	ProfileTimer t("main");
	// maybe some other things here
	t.start();
	functionThatTakesSomeTime();
	t.stop(); // stops timer and PRofiler logs it
}
```

Convenience is Key
* If we want to profile a lot of areas of code, manually starting and stopping the timer each time will be annoying, mess up code
* Let's develop a way to time a block of code by simply typing one command
* Any block of code that has this command will automatically be profiled to JSON

```
// Make ProfileTimer stop/log on destructor
int myFunc()
{
	ProfileTimer t("myFunc"); // timer starts on construction
	// some code runs here which takes a while
}   // timer will stop and log on destructor call

void myFunction()
{
	for (int i = 0; i < 10; i++) {
		ProfileTimer t("LoopIteration"); // timer starts
		// do stuff in the loop
	}   // ~Timer(), Profiler logs it
}

// File: Timer.hpp
#include <chrono>
class ProfileTimer
{
	ProfileResult m_result;
	bool m_stopped = false;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_stp;
public:
	ProfileTimer(const std::string& name)
	{
		m_result.name = name;
		m_stp = std::chrono::high_resolution_clock::now();
	}
	~ProfileTimer() { stop(); }
	
	void stop()
	{
		using namespace std::chrono;
		if (m_stopped) { return; }
		m_stopped = true;
		
		auto etp = high_resolution_clock::now(); // end time point
		m_result.m_start = time_point_cast<microseconds>(m_stp).time_since_epoch().count();
		m_result.m_end = time_point_cast<microseconds>(m_etp).time_since_epoch().count();
		m_result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		
		Profiler::Instance().writeProfile(m_result);
	}
};
```

Convenience++
* This is still a little bit annoying - we don't want to have to manually name timers for every function we call it on
* Function names might change, then we have to go edit the timers too
* What if we want to turn profiling on/off?
* Let's create a macro we can call that does this all for us automatically

```
// C++ Macro for profiling calls
#define PROFILING 1
#ifdef PROFILING
	#define PROFILE_SCOPE(name) \
		ProfileTimer timer##__LINE__(name)
	#define PROFILE_FUNCTION() \
		PROFILE_SCOPE(__FUNCTION__)
#else
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
#endif
```

Macro `#ifdef`
* We define a macro: PROFILING
* If PROFILING is defined, then the profiling macro is defined normally, causing the program to log to the JSON file
* If PROFILING is not defined, then the macro is left blank by the preprocessor, turning off profiling with 0 overhead

```
// Using the ProfileTimer macros
int myFunc()
{
	PROFILE_FUNCTION();
	// some code runs here which takes a while
}

void myFunction()
{
	for (int i = 0; i < 10; i++) {
		PROFILE_SCOPE("LoopIteration");
		// do stuff in the loop
	} 
}

// Example, illustration at 1:05:05
void myFunction()
{
	PROFILE_FUNCTION();
	std::this_thread::sleep_for(1s);
	for (int i = 0; i < 10; i++) {
		PROFILE_SCOPE("LoopIteration");
		std::this_thread::sleep_for(100ms);
	} 
}
```

DEMO (1:05:51)
/code/ folder on course website
```
// File: Profiler.hpp
#pragma once

#include <chrono>
#include <fstream>
#include <mutex>
#include <map>
#include <string>
#include <algorithm>

#define PROFILING 1
#ifdef PROFILING
	#define PROFILE_SCOPE(name) \
		ProfileTimer timer##__LINE__(name)
	#define PROFILE_FUNCTION() \
		PROFILE_SCOPE(__FUNCTION__)
#else
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
#endif

struct ProfileResult
{
	const std::string name = "Default";
	long long start = 0;
	long long end   = 0;
	size_t threadID = 0;
}

class Profiler
{
	std::string m_outputFile = "results.json";
	size_t m_profileCount = 0;
	std::ofstream m_outputStream;
	std::mutex m_lock;
	Profiler()
	{
		m_outputStream = std::ofstream(m_outputFile);
		writeHeader();
	}

	void writeHeader() {
		m_outputStream << "{\"otherData\": {}, \"traceEvents\":[";
	}
	void writeFooter() { m_outputStream << "]}"; }

public:
	static Profiler & Instance()
	{
		static Profiler instance;
		return instance;
	}
	~Profiler()
	{
		writeFooter();
	}
	
	void writeProfile(const ProfileResult & r)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		if (m_profileCount++ > 0) { m_outputStream << ","; }
	
		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\"');

		m_outputStream << "\n{";
		m_outputStream << "\"cat\":\"function\",";
		m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
		m_outputStream << "\"name\":\"" << name << "\",";
		m_outputStream << "\"ph\":\"X\",";
		m_outputStream << "\"pid\":0,";
		m_outputStream << "\"tid\":" << result.threadID << ",";
		m_outputStream << "\"ts\"" << result.start;
		m_outputStream << "}",
	}
};

class ProfileTimer
{
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> ClockType;
	
	ClockType m_startTimepoint;
	ProfileResult m_result;
	bool m_stopped = false;

public:
	ProfileTimer(const std::string& name)
		: m_result({name, 0, 0, 0})
	{
		start();
	}

	~ProfileTimer()
	{
		stop();
	}

	// this function is used to set the start time
	// it adds 1 time unit if it's the same start time as the previous start time
	// this solves a display issue in chrome://tracing for identical start times
	// this is a 2/10 on the janky fix scale but it gas worked for me in practice
	void start()
	{
		// a static variable to store the last start time recorded
		static long long lastStartTime = 0;

		// grab the actual start time using std::chrono
		m_startTimepoint = std::chrono::high_resolution_clock::now();
		m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
		
		// if it's the same as the previous start time, add 1 to it
		m_result.start += (m_result.start == lastStartTime ? 1 : 0);

		// record the fixed time as the previous start time
		lastStartTime = m_result.start;
		
		m_stopped = false;
	}
	
	void stop()
	{
		if (m_stopped) { return; }
		
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		m_result.m_end = std::chrono::time_point_cast<microseconds>(endTimepoint).time_since_epoch().count();
		m_result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		
		Profiler::Instance().writeProfile(m_result);

		m_stopped = true;
	}
};
```
