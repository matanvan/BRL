#include <iostream>
#include <fstream>

#include "BRLLib\RegNode.hpp"
#include "BRLLib\BFS.hpp"

INT wmain(INT, PCWSTR*)
{
	try
	{
		BFS reg_list(RegNode::HKLM().subkey(L"SOFTWARE\\Dell\\DTP.Transmission"));
		for (auto& node : reg_list)
		{
			std::wcout << node.path() << std::endl;
		}
	}
	catch (const BRLException& e)
	{
		std::wcout << L"BRLException. status: " << static_cast<uint32_t>(e.status()) << L", additional info: " << e.additional_info() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::wcout << L"std::exception. message: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::wcout << L"Unknown exception" << std::endl;
	}
}
