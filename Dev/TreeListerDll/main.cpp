#include <iostream>
#include <fstream>
#include <Windows.h>

#include <BFS.hpp>

INT wmain(INT argc, PCWSTR* argv)
{
	BFS registry_lister(RegKey("SYSTEM\\CurrentControlSet\\Services\\BFE"));
	std::wfstream out_file(L"C:\\temp\\out_file.txt");
	for (auto value : registry_lister) {

	}
}
