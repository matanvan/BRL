#include <iostream>
#include <fstream>

#include "BRLLib\RegNode.hpp"
//#include "BRLLib\BFS.hpp"

INT wmain(INT, PCWSTR*)
{
	const auto key = RegNode::HKLM.subkey(L"SYSTEM\\CurrentControlSet\\Services\\BFE");
	std::wcout << std::get<RegKeyData>(key.info()).m_path << std::endl;
}
