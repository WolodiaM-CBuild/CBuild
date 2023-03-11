/**
 * @file filesystem++.cpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief filesystem++ api implementation
 * @version 1.0
 * @date 2023-01-19
 *
 *
 * @license GPL v3.0 or later
 *
 * Copyright (C) 2023  WolodiaM
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// C++ libraries
#include "vector"
#include "string"
#include "regex"
#include "filesystem"
#include "iostream"
#include "fstream"
#include "stdlib.h"
// Project files
#include "../../headers/filesystem++.hpp"
#include "../../headers/files.hpp"
#include "../../headers/CBuild_defs.hpp"
/* CBuild::fs namespace */
namespace CBuild
{
	namespace fs
	{
		std::regex c_cpp_file(".*\\.(cpp|cxx|cc|c)");
	} // namespace fs
} // namespace CBuild
/* filesystem++.hpp */
std::vector<std::string> CBuild::fs::dir(std::string path, std::string search)
{
	// List of all found files
	std::vector<std::string> files;
	// List of files that need to be returned
	std::vector<std::string> ret;
	// Regex for search, use ECMAScript syntax
	std::regex reg(search, std::regex::ECMAScript);
	// Found all files in directory and store in array
	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		files.push_back(entry.path().generic_string());
	}
	// Check all elements with regex and  add it to return array
	for (auto elem : files)
	{
		if (std::regex_match(elem, reg))
		{
			ret.push_back(elem);
		}
	}
	// Return list of found files that match given regex
	return ret;
}
std::vector<std::string> CBuild::fs::dir(std::string path)
{
	// List of found files
	std::vector<std::string> files;
	// List of returned files
	std::vector<std::string> ret;
	// Get all files and store it's paths in array
	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		files.push_back(entry.path().generic_string());
	}
	// Check every file with internal regex and add to return array
	for (auto elem : files)
	{
		if (std::regex_match(elem, c_cpp_file))
		{
			ret.push_back(elem);
		}
	}
	// Return list of found files
	return ret;
}
std::vector<std::string> CBuild::fs::dir_rec(std::string path, std::string search)
{
	// List of all found files
	std::vector<std::string> files;
	// List of files for return
	std::vector<std::string> ret;
	// Create regex
	std::regex reg(search);
	// Check all directories recursively and add to array
	for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
	{
		files.push_back(entry.path().generic_string());
	}
	// Check all files with given regex and add to return array
	for (auto elem : files)
	{
		if (std::regex_match(elem, reg))
		{
			ret.push_back(elem);
		}
	}
	// Return list of found files
	return ret;
}
std::vector<std::string> CBuild::fs::dir_rec(std::string path)
{

	// List of all found files
	std::vector<std::string> files;
	// List of files for return
	std::vector<std::string> ret;
	// Check all directories recursively and add to array
	for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
	{
		files.push_back(entry.path().generic_string());
	}
	// Check all files with internal regex and add to return array
	for (auto elem : files)
	{
		if (std::regex_match(elem, c_cpp_file))
		{
			ret.push_back(elem);
		}
	}
	// Return list of found filews
	return ret;
}
bool CBuild::fs::rename(std::string start, std::string end)
{
	// Two paths
	std::filesystem::path from, to;
	// Create paths from strings
	from.assign(start);
	to.assign(end);
	// Try to rename file
	try
	{
		std::filesystem::rename(from, to);
	}
	catch (std::exception &e)
	{
		// Something goes wrong
		return false;
	}
	// All good
	return true;
}
bool CBuild::fs::remove(std::string path, bool force)
{
	// Create path from string
	std::filesystem::path p;
	p.assign(path);
	// If we need force remove
	if (force)
	{
		// Try force remove
		try
		{
			if (std::filesystem::remove_all(p) == 0)
			{
				// Something goes wrong
				return false;
			}
		}
		catch (std::exception &e)
		{
			// Something goes wrong
			return false;
		}
	}
	// We can use "lite" remove
	else
	{
		// Try force remove
		try
		{
			if (std::filesystem::remove(p) == false)
				return false;
		}
		catch (std::exception &e)
		{
			// Something goes wrong
			return false;
		}
	}
	// All good
	return true;
}
bool CBuild::fs::copy(std::string start, std::string end)
{
	// Create paths from strings
	std::filesystem::path from, to;
	from.assign(start);
	to.assign(end);
	// Try to copy elements
	try
	{
		std::filesystem::copy(from, to);
	}
	catch (std::exception &e)
	{
		// Something goes wrong
		return false;
	}
	// All good
	return true;
}
bool CBuild::fs::move(std::string start, std::string end)
{
	// Create paths from strings
	std::filesystem::path from, to;
	from.assign(start);
	to.assign(end);
	// Try to copy files
	try
	{
		std::filesystem::copy(from, to);
		if (CBuild::fs::remove(start, true) == false)
			return false; // Something wrong with removal
	}
	catch (std::exception &e)
	{
		// Something goes wrong
		return false;
	}
	// All good
	return true;
}
bool CBuild::fs::create(std::vector<std::string> paths, CBuild::fs::type what)
{
	// Check type of new element
	switch (what)
	{
	// We creating simple file
	case CBuild::fs::type::FILE:
	{
		// Argument number check
		std::string path = "";
		try
		{
			path = paths.at(0);
		}
		catch (std::exception &e)
		{
			// Invalid argument count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		// Try to create file using std::ofstream
		try
		{
			std::ofstream file(path);
			if (file.bad())
				return false;
			file.close();
		}
		catch (std::exception &e)
		{
			// Something goes wrong
			return false;
		}
	}
	break;
	// We creating directory
	case CBuild::fs::DIR:
	case CBuild::fs::type::DIRECTORY:
	{
		// Check argument count
		std::filesystem::path p;
		try
		{
			p.assign(paths.at(0));
		}
		catch (std::exception &e)
		{
			// Invalid arguments count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		// Try to create new directory
		try
		{
			std::filesystem::create_directory(p);
		}
		catch (std::exception &e)
		{
			// Something goes wrong
			return false;
		}
	}
	break;
	// We creating symlink to file
	case CBuild::fs::type::SYMLINK_FILE:
	{
		// Check argument count
		std::filesystem::path file, link;
		try
		{
			file.assign(paths.at(0));
		}
		catch (std::exception &e)
		{
			// Invalid argument count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		try
		{
			link.assign(paths.at(1));
		}
		catch (std::exception &e)
		{
			// Invalid argument count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		// Try to create new symlink, can be unsupported
		try
		{
			std::filesystem::create_symlink(file, link);
		}
		catch (std::exception &e)
		{
			// Something goes wrong
			return false;
		}
	}
	break;
	// We creating symlink to directory
	case CBuild::fs::type::SYMLINK_DIR:
	case CBuild::fs::type::SYMLINK_DIRECTORY:
	{
		// Check argument count
		std::filesystem::path directory, link;
		try
		{
			directory.assign(paths.at(0));
		}
		catch (std::exception &e)
		{
			// Invalid argument count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		try
		{
			link.assign(paths.at(1));
		}
		catch (std::exception &e)
		{
			// Invalid argument count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		// try to create symlink, can be unsupported
		try
		{
			std::filesystem::create_directory_symlink(directory, link);
		}
		catch (std::exception &e)
		{
			// Something goes wrong
			return false;
		}
	}
	break;
	// We creating hardlink, can be unsupported
	case CBuild::fs::type::HARDLINK:
	{
		// Checking argument count
		std::filesystem::path element, link;
		try
		{
			element.assign(paths.at(0));
		}
		catch (std::exception &e)
		{
			// Invalid argument count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		try
		{
			link.assign(paths.at(1));
		}
		catch (std::exception &e)
		{
			// Invalid argument count
			throw std::runtime_error("Invalid count of elements");
			return false;
		}
		// Try to create hardlink, can be unsupported
		try
		{
			std::filesystem::create_hard_link(element, link);
		}
		catch (std::exception &e)
		{
			// Something goes wrong
			return false;
		}
	}
	break;
	default:
		// Invalid type of new element
		throw std::runtime_error("Invalid type of new element");
		return false;
		break;
	}
	// All good
	return true;
}
/* files.hpp */
int CBuild::fs::replace(std::string file, std::string token, std::string data)
{
	// Path to tmp file
	std::string cache;
	cache = CBUILD_CACHE_DIR + "/" + CBUILD_COPY_CACHE_DIR + "/" + file + ".tmp";
	// std::cout << cache << "\n";
	// Open two files
	std::ofstream tmp_file(cache);
	std::ifstream main_file;
	main_file.open(file);
	// Number of replaced tokens
	static int check_num;
	check_num = 0;
	// Read and copy file to temp file line by line
	std::string line;
	while (std::getline(main_file, line))
	{
		// If token found - replace it and increment counter
		while (line.find(token) != std::string::npos)
		{
			line.replace(line.find(token), token.size(), data);
			check_num++;
		}
		tmp_file << line << "\n";
	}
	// Close files
	main_file.close();
	tmp_file.close();
	// Replace file by it's temp version
	CBuild::fs::remove(file, true);
	CBuild::fs::move(cache, file);
	// Return number of replaced element or -1 if nothing has been replaced
	if (check_num == 0)
		return -1;
	return check_num;
}
int CBuild::fs::set_var(std::string file, std::string var_type, std::string var_name, std::string data)
{
	// Path to tmp file
	std::string cache;
	cache = CBUILD_CACHE_DIR + "/" + CBUILD_COPY_CACHE_DIR + "/" + file + ".tmp";
	// std::cout << cache << "\n";
	// Open two files
	std::ofstream tmp_file(cache);
	std::ifstream main_file;
	main_file.open(file);
	// Number of replaced tokens
	static int check_num;
	check_num = 0;
	// Read and copy file to temp file line by line
	std::string line;
	while (std::getline(main_file, line))
	{
		// We can refer to #define by define
		if (var_type == "define")
			var_type = "#define";
		// If variable found
		if ((line.find(var_type) != std::string::npos) && (line.find(var_name) != std::string::npos))
		{
			// All content before variable
			std::string prefix = line.substr(0, line.find(var_type));
			// All content after variable
			std::string postfix = line.substr(line.find(var_name) + var_name.size(), line.size() - 1);
			// Variable
			std::string var = var_type + " " + var_name + " = " + data;
			// Recreate line
			line = prefix + var + postfix;
			check_num++;
		}
		tmp_file << line << "\n";
	}
	// Close files
	main_file.close();
	tmp_file.close();
	// Replace file by it's temp version
	CBuild::fs::remove(file, true);
	CBuild::fs::move(cache, file);
	// Return number of replaced element or -1 if nothing has been replaced
	if (check_num == 0)
		return -1;
	return check_num;
}
bool CBuild::fs::exists(std::string path)
{
	std::filesystem::path p;
	p.assign(path);
	return std::filesystem::exists(p);
}