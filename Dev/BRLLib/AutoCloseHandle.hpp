#pragma once

#include <Windows.h>
#include <memory>
#include <functional>

class AutoCloseHandle: public std::shared_ptr<std::remove_pointer_t<HANDLE>>
{
public:
	AutoCloseHandle(HANDLE handle) :
		std::shared_ptr<std::remove_pointer_t<HANDLE>>(handle, CloseHandle)
	{}

	~AutoCloseHandle() = default;
};
