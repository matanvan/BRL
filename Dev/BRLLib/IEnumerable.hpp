#pragma once

#include "IEnumerator.hpp"

template <typename TEnumerator>
class IEnumerable
{
public:
	IEnumerable() = delete;
	~IEnumerable() = delete;
	IEnumerable(const IEnumerable&) = delete;
	IEnumerable& operator=(const IEnumerable&) = delete;
	
	virtual TEnumerator enumerate() const = 0;
};
