#pragma once

#include <string>

#include "Common.hpp"

#include "AutoCloseHandle.hpp"
#include "IWriter.hpp"

class File : public IWriter
{
public:
	explicit File(
		const std::wstring& path,
		const uint32_t desired_access = GENERIC_READ | GENERIC_WRITE,
		const uint32_t share_mode = 0,
		const uint32_t creation_disposition = OPEN_ALWAYS,
		const uint32_t flags_and_attributes = FILE_ATTRIBUTE_NORMAL
	);
	virtual ~File() = default;
	File(File&&) = default;
	File& operator=(File&&) = default;

	virtual void write(const Buffer& data) override;

private:
	static HANDLE _s_create_file(
		const std::wstring& path,
		const uint32_t desired_access,
		const uint32_t share_mode,
		const uint32_t creation_disposition,
		const uint32_t flags_and_attributes
	);

private:
	AutoCloseHandle m_handle;
};
