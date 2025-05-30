#pragma once

#include <Windows.h>
#include <string>
#include <variant>

#include "IEnumerable.hpp"

class RegNode: public IEnumerable<void>
{
private:
	class Key
	{
	public:
		Key() = delete;
		~Key() = delete;
		Key(const Key&) = delete;
		Key& operator=(const Key&) = delete;

	private:

	};

	class Value
	{
	public:
		Value() = delete;
		~Value() = delete;
		Value(const Value&) = delete;
		Value& operator=(const Value&) = delete;

	private:
	};

	RegNode();

public:
	~RegNode() = default;
	RegNode(const RegNode&) = delete;
	RegNode& operator=(const RegNode&) = delete;

	virtual RegNode key(const HKEY parent, const std::wstring& subpath);

private:
	const std::variant<Key, Value> m_node;
};
