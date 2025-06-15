#pragma once

#include "Common.hpp"

template <typename TData>
class IAction
{
public:
	virtual ~IAction() = default;

	virtual void execute(const TData&) = 0;
};

template <typename TData>
using IActionUPtr = std::unique_ptr<IAction<TData>>;
