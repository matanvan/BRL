#pragma once

#include "Common.hpp"

template <typename T, void(*CLOSER)(T)>
class AutoCloseHandleType final: public std::shared_ptr<std::remove_pointer_t<T>>
{
private:
	using Base = std::shared_ptr<std::remove_pointer_t<T>>;

public:
	explicit AutoCloseHandleType(const T value) :
		Base(value, CLOSER)
	{}

	AutoCloseHandleType(AutoCloseHandleType&& other) noexcept :
		Base(std::move(other))
	{}
	AutoCloseHandleType& operator=(AutoCloseHandleType&& other) noexcept { Base::operator=(std::move(other)); return *this; }

	AutoCloseHandleType clone() const { return AutoCloseHandleType(static_cast<const AutoCloseHandleType&>(*this)); }

private:
	AutoCloseHandleType(const AutoCloseHandleType&) = default;
	AutoCloseHandleType& operator=(const AutoCloseHandleType&) = delete;
};

using AutoCloseHandle = AutoCloseHandleType<HANDLE, (void(*)(HANDLE))CloseHandle>;
using AutoCloseRegKey = AutoCloseHandleType<HKEY, (void(*)(HKEY))RegCloseKey>;
