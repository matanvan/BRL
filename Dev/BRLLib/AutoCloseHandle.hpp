#pragma once

#include <Windows.h>
#include <memory>
#include <functional>

class AutoCloseHandle: public std::unique_ptr<std::remove_pointer_t<HANDLE>, BOOL(*)(HANDLE)>
{
public:
	AutoCloseHandle(HANDLE handle) :
		std::unique_ptr<std::remove_pointer_t<HANDLE>, BOOL(*)(HANDLE)>(handle, CloseHandle)
	{}

	~AutoCloseHandle() = default;
	AutoCloseHandle(const AutoCloseHandle&) = delete;
	AutoCloseHandle& operator=(const AutoCloseHandle&) = delete;
};
