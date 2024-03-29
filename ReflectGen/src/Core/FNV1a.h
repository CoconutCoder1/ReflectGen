#pragma once

/* Compile-time FNV1a 32-bit hash */
consteval uint32_t fnv1a32_CT(const char* str)
{
	constexpr uint32_t prime = 16777619;

	uint32_t hash = 0x811C9DC5;

	while (*str != '\0')
	{
		hash = (hash ^ *str) * prime;
		str++;
	}

	return hash;
}

uint32_t fnv1a32(const char* str);