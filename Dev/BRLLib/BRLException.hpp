#pragma once

#include <Windows.h>
#include <exception>

#include "BRLStatus.hpp"

#ifndef _DEBUG
#define BRLException EX5094
#endif

class BRLException: public std::exception
{
public:
	BRLException(const BRLStatus status, const uint32_t additional_info = 0) :
		m_status(status),
		m_additional_info(additional_info)
	{}

	virtual ~BRLException() = default;
	BRLException(const BRLException&) = delete;
	BRLException& operator=(const BRLException&) = delete;

	virtual BRLStatus status() const { return m_status; }
	virtual uint32_t additional_info() const { return m_additional_info; }

private:
	BRLStatus m_status;
	uint32_t m_additional_info;
};
