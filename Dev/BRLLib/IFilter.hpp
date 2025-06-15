#pragma once

#include "Common.hpp"

template <typename TData>
class IFilter
{
public:
	virtual ~IFilter() = default;

	virtual bool pass(const TData&) = 0;
};

template <typename TData>
using IFilterUPtr = std::unique_ptr<IFilter<TData>>;
