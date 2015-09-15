#pragma once
#include <stdint.h>
#include <stddef.h>
#include <memory>
#include <vector>
#include <map>
#include <chrono>
#include <algorithm>
#include <string>
#include <assert.h>

#pragma warning (disable:4127)	// conditional expression is constant
#pragma warning (disable:4100)	// unreferenced formal parameter
#pragma warning (disable:4512)	// assignment operator could not be generated

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;
typedef float float32;
typedef double float64;

namespace Core
{
	// A platform-independent mutex. Allows calling the constructor during static initialization, unlike std::mutex.
	struct Mutex
	{
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();

	private:
		void* handle;
	};

	// RAII-style lock for Mutex.
	struct Lock
	{
		Lock(Mutex& inMutex) : mutex(&inMutex) { mutex->Lock(); }
		~Lock() { Release(); }

		void Release()
		{
			if(mutex)
			{
				mutex->Unlock();
			}
			mutex = NULL;
		}

	private:
		Mutex* mutex;
	};

	struct Timer
	{
		Timer(): startTime(std::chrono::high_resolution_clock::now()), isStopped(false) {}
		void stop() { endTime = std::chrono::high_resolution_clock::now(); }
		uint64 getMicroseconds()
		{
			if(!isStopped) { stop(); }
			return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		}
		float64 getMilliseconds() { return getMicroseconds() / 1000.0; }
		float64 getSeconds() { return getMicroseconds() / 1000000.0; }
	private:
		std::chrono::system_clock::time_point startTime;
		std::chrono::system_clock::time_point endTime;
		bool isStopped;
	};

	// The base of the 4GB virtual address space allocated for the VM.
	extern uint8* vmVirtualAddressBase;

	// Commits or decommits memory in the VM virtual address space.
	extern uint32 vmSbrk(int32 numBytes);

	// Initializes intrinsic values used by WASM from Emscripten.
	void initEmscriptenIntrinsics();
}