#pragma once

#include "Common.hpp"

template <typename T>
class IEnumerator
{
public:
	virtual ~IEnumerator() = default;

	virtual std::optional<T> next() = 0;
};
