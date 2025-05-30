#include "File.hpp"

File::File(
	const std::wstring& path,
	const uint32_t desired_access,
	const uint32_t share_mode,
	const uint32_t creation_disposition,
	const uint32_t flags_and_attributes
) :
	m_handle(_s_create_file(
		path,
		desired_access,
		share_mode,
		creation_disposition,
		flags_and_attributes
	))
{}

void File::write(const Buffer& data)
{
	DWORD bytes_written = 0;

	if (!WriteFile(
		m_handle.get(),
		data.data(),
		static_cast<uint32_t>(data.size()),
		&bytes_written,
		nullptr
	))
	{
		throw std::exception("TODO");
	}

	if (bytes_written != static_cast<uint32_t>(data.size()))
	{
		throw std::exception("TODO");
	}
}

HANDLE File::_s_create_file(
	const std::wstring& path, 
	const uint32_t desired_access, 
	const uint32_t share_mode, 
	const uint32_t creation_disposition, 
	const uint32_t flags_and_attributes
)
{
	HANDLE handle = CreateFileW(
		path.c_str(),
		desired_access,
		share_mode,
		nullptr,
		creation_disposition,
		flags_and_attributes,
		nullptr
	);

	if (INVALID_HANDLE_VALUE == handle)
	{
		throw std::exception("TODO");
	}

	return handle;
}
