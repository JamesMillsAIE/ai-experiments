#include "Random.h"

#include <numeric>
#include <string>

using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::bernoulli_distribution;

using std::numeric_limits;

uint Hash(const string& str)
{
	uint hash = 0;
	uint x;
	const ullong len = str.length();

	for (uint i = 0; i < len; i++)
	{
		hash = (hash << 4) + str[i];
		if ((x = hash & 0xF0000000) != 0)
			hash ^= x >> 24;

		hash &= ~x;
	}

	return hash;
}

Random::Random()
	: m_engine{ nullptr }
{
}

Random::~Random()
{
	delete m_engine;
}

void Random::Seed(string seed)
{
	if(seed.empty())
	{
#pragma warning(push)
#pragma warning(disable:4244)
		srand(time(nullptr));  // NOLINT(clang-diagnostic-shorten-64-to-32, cert-msc51-cpp)
#pragma warning(pop)

		seed = std::to_string(rand());  // NOLINT(concurrency-mt-unsafe)
	}

	m_engine = new mt19937_64(Hash(seed));
}

bool Random::Boolean() const
{
	bernoulli_distribution dist;

	return dist(*m_engine);
}

byte Random::Byte() const
{
	uniform_int_distribution<> dist(0, numeric_limits<byte>::max());

	return static_cast<byte>(dist(*m_engine));
}

ushort Random::UShort() const
{
	uniform_int_distribution<ushort> dist(0, numeric_limits<ushort>::max());

	return dist(*m_engine);
}

uint Random::UInt() const
{
	uniform_int_distribution<uint> dist(0, numeric_limits<uint>::max());

	return dist(*m_engine);
}

ulong Random::ULong() const
{
	uniform_int_distribution<ulong> dist(0, numeric_limits<ulong>::max());

	return dist(*m_engine);
}

ullong Random::ULongLong() const
{
	uniform_int_distribution<ullong> dist(0, numeric_limits<ullong>::max());

	return dist(*m_engine);
}

short Random::Short() const
{
	uniform_int_distribution<short> dist(0, numeric_limits<short>::max());

	return dist(*m_engine);
}

int Random::Int() const
{
	uniform_int_distribution dist(0, numeric_limits<int>::max());

	return dist(*m_engine);
}

long Random::Long() const
{
	uniform_int_distribution<long> dist(0, numeric_limits<long>::max());

	return dist(*m_engine);
}

llong Random::LongLong() const
{
	uniform_int_distribution<llong> dist(0, numeric_limits<llong>::max());

	return dist(*m_engine);
}

float Random::Float() const
{
	uniform_real_distribution<float> dist(0, numeric_limits<float>::max());

	return dist(*m_engine);
}

short Random::Range(const short max) const
{
	uniform_int_distribution<short> dist(0, max);

	return dist(*m_engine);
}

int Random::Range(const int max) const
{
	uniform_int_distribution dist(0, max);

	return dist(*m_engine);
}

long Random::Range(const long max) const
{
	uniform_int_distribution<long> dist(0, max);

	return dist(*m_engine);
}

llong Random::Range(const llong max) const
{
	uniform_int_distribution<llong> dist(0, max);

	return dist(*m_engine);
}

byte Random::Range(const byte max) const
{
	uniform_int_distribution<> dist(0, max);

	return static_cast<byte>(dist(*m_engine));
}

ushort Random::Range(const ushort max) const
{
	uniform_int_distribution<ushort> dist(0, max);

	return dist(*m_engine);
}

uint Random::Range(const uint max) const
{
	uniform_int_distribution<uint> dist(0, max);

	return dist(*m_engine);
}

ulong Random::Range(const ulong max) const
{
	uniform_int_distribution<ulong> dist(0, max);

	return dist(*m_engine);
}

ullong Random::Range(const ullong max) const
{
	uniform_int_distribution<ullong> dist(0, max);

	return dist(*m_engine);
}

float Random::Range(const float max) const
{
	uniform_real_distribution<float> dist(0, max);

	return dist(*m_engine);
}

short Random::Range(const short min, const short max) const
{
	uniform_int_distribution dist(min, max);

	return dist(*m_engine);
}

int Random::Range(const int min, const int max) const
{
	uniform_int_distribution dist(min, max);

	return dist(*m_engine);
}

long Random::Range(const long min, const long max) const
{
	uniform_int_distribution dist(min, max);

	return dist(*m_engine);
}

llong Random::Range(const llong min, const llong max) const
{
	uniform_int_distribution dist(min, max);

	return dist(*m_engine);
}

byte Random::Range(const byte min, const byte max) const
{
	uniform_int_distribution<> dist(min, max);

	return static_cast<byte>(dist(*m_engine));
}

ushort Random::Range(const ushort min, const ushort max) const
{
	uniform_int_distribution dist(min, max);

	return dist(*m_engine);
}

uint Random::Range(const uint min, const uint max) const
{
	uniform_int_distribution dist(min, max);

	return dist(*m_engine);
}

ulong Random::Range(const ulong min, const ulong max) const
{
	uniform_int_distribution dist(min, max);

	return dist(*m_engine);
}

float Random::Range(const float min, const float max) const
{
	uniform_real_distribution dist(min, max);

	return dist(*m_engine);
}