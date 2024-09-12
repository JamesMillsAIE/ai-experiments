#pragma once

#include <random>

#include <aie/bootstrap/Bootstrap2D.h>

using std::string;
using std::mt19937_64;

namespace aie
{
	class DLL Random
	{
		friend class Application;

	public:
		Random() = delete;
		Random(const Random&) = delete;
		Random(Random&&) = delete;

	public:
		bool Boolean() const;
		unsigned char Byte() const;
		unsigned short UShort() const;
		unsigned int UInt() const;
		unsigned long ULong() const;
		unsigned long long ULongLong() const;

		short Short() const;
		int Int() const;
		long Long() const;
		long long LongLong() const;
		float Float() const;

		short Range(short max) const;
		int Range(int max) const;
		long Range(long max) const;
		long long Range(long long max) const;

		unsigned char Range(unsigned char max) const;
		unsigned short Range(unsigned short max) const;
		unsigned int Range(unsigned int max) const;
		unsigned long Range(unsigned long max) const;
		unsigned long long Range(unsigned long long max) const;
		float Range(float max) const;

		short Range(short min, short max) const;
		int Range(int min, int max) const;
		long Range(long min, long max) const;
		long long Range(long long min, long long max) const;

		unsigned char Range(unsigned char min, unsigned char max) const;
		unsigned short Range(unsigned short min, unsigned short max) const;
		unsigned int Range(unsigned int min, unsigned int max) const;
		unsigned long Range(unsigned long min, unsigned long max) const;
		float Range(float min, float max) const;

	public:
		Random& operator=(const Random&) = delete;
		Random& operator=(Random&&) = delete;

	private:
		mt19937_64* m_engine;

	private:
		explicit Random(string seed);
		~Random();

	};
}

