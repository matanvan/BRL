#pragma once

#include "Common.hpp"

template <typename TData>
class ISerializer
{
public:
	virtual ~ISerializer() = default;

	virtual Buffer serialize(const TData&) = 0;
};

template <typename TData>
using ISerializerUPtr = std::unique_ptr<ISerializer<TData>>;
