#pragma once

#include <optional>

template <typename T>
class IEnumerator
{
public:
	IEnumerator() = delete;
	~IEnumerator() = delete;
	IEnumerator(const IEnumerator&) = delete;
	IEnumerator& operator=(const IEnumerator&) = delete;

	virtual std::optional<T> next() = 0;
};
