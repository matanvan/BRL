#pragma once

#include "Common.hpp"

class AutoCloseHandle : public std::shared_ptr<std::remove_pointer_t<HANDLE>>
{
public:
	AutoCloseHandle(HANDLE handle) :
		std::shared_ptr<std::remove_pointer_t<HANDLE>>(handle, CloseHandle)
	{}
	AutoCloseHandle(AutoCloseHandle&&) = default;
	AutoCloseHandle& operator=(AutoCloseHandle&&) = default;
};
