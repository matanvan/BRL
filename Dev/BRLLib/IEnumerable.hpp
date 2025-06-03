#pragma once

#include "IEnumerator.hpp"

template <typename TEnumerator>
class IEnumerable
{
public:
	virtual ~IEnumerable() = default;
	
	virtual TEnumerator enumerate() const = 0;
};
