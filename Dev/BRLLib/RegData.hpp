#pragma once

#include <variant>

class RegData
{
private:
	class Key
	{

	};

	class Value
	{

	};

private:
	const std::variant<Key, Value> m_data;
};
