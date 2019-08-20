#pragma once
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
inline long GetFileSize(std::string filename)
{	
	fs::path p{ filename };
	p = fs::canonical(filename);
	return fs::file_size(p);
}