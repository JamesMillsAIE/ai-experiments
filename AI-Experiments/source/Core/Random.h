#pragma once

#include <random>

#include "Global.h"

using std::string;
using std::mt19937_64;

class Random
{
public:
	Random();
	~Random();

	Random(const Random&) = delete;
	Random(Random&&) = delete;

public:
	void Seed(string seed);

	[[nodiscard]] bool Boolean() const;
	[[nodiscard]] byte Byte() const;
	[[nodiscard]] ushort UShort() const;
	[[nodiscard]] uint UInt() const;
	[[nodiscard]] ulong ULong() const;
	[[nodiscard]] ullong ULongLong() const;

	[[nodiscard]] short Short() const;
	[[nodiscard]] int Int() const;
	[[nodiscard]] long Long() const;
	[[nodiscard]] llong LongLong() const;
	[[nodiscard]] float Float() const;

	[[nodiscard]] short Range(short max) const;
	[[nodiscard]] int Range(int max) const;
	[[nodiscard]] long Range(long max) const;
	[[nodiscard]] llong Range(llong max) const;

	[[nodiscard]] byte Range(byte max) const;
	[[nodiscard]] ushort Range(ushort max) const;
	[[nodiscard]] uint Range(uint max) const;
	[[nodiscard]] ulong Range(ulong max) const;
	[[nodiscard]] ullong Range(ullong max) const;
	[[nodiscard]] float Range(float max) const;

	[[nodiscard]] short Range(short min, short max) const;
	[[nodiscard]] int Range(int min, int max) const;
	[[nodiscard]] long Range(long min, long max) const;
	[[nodiscard]] llong Range(llong min, llong max) const;

	[[nodiscard]] byte Range(byte min, byte max) const;
	[[nodiscard]] ushort Range(ushort min, ushort max) const;
	[[nodiscard]] uint Range(uint min, uint max) const;
	[[nodiscard]] ulong Range(ulong min, ulong max) const;
	[[nodiscard]] float Range(float min, float max) const;

public:
	Random& operator=(const Random&) = delete;
	Random& operator=(Random&&) = delete;

private:
	mt19937_64* m_engine;

};