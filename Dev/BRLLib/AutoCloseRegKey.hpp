#pragma once

#include "Common.hpp"

class AutoCloseRegKey : public std::shared_ptr<std::remove_pointer_t<HKEY>>
{
public:
	AutoCloseRegKey(HKEY handle) :
		std::shared_ptr<std::remove_pointer_t<HKEY>>(handle, RegCloseKey)
	{}
	AutoCloseRegKey(AutoCloseRegKey&&) = default;
	AutoCloseRegKey& operator=(AutoCloseRegKey&&) = default;
};