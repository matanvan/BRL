#pragma once

#include "Common.hpp"

class IWriter
{
public:
	virtual ~IWriter() = default;

	virtual void write(const Buffer&) = 0;
};

using IWriterUPtr = std::unique_ptr<IWriter>;
