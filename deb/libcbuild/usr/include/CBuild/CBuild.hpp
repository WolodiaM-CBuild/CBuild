/**
 * @file CBuild.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Main headers of CBuild core
 * @version 1.0
 * @date 2023-01-18
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
#include "string"
#include "vector"
#include "map.hpp"
#ifndef __CBUILD_CORE_HPP__
#define __CBUILD_CORE_HPP__
namespace CBuild
{
	/**
	 * @brief Run mode
	 */
	typedef enum
	{
		/**
		 * @brief Run task
		 */
		TASK,
		/**
		 * @brief Build target
		 */
		BUILD,
		/**
		 * @brief Run target
		 */
		RUN,
		/**
		 * @brief Build and run target
		 */
		BUILD_RUN,
		/**
		 * @brief Build and run target in debug mode
		 */
		DEBUG,
		/**
		 * @brief Clear build output and cache
		 */
		CLEAR,
		/**
		 * @brief Some error occurred
		 */
		ERROR
	} RType;
	/**
	 * @brief Parse arguments
	 * @param args => lib::map<std::string, std::string>* -> Returned arguments for other functions
	 * @param argc => int -> argc from main
	 * @param argv => char** -> argv from main
	 * @return RType -> type of CBuild task
	 */
	RType parse(lib::map<std::string, std::string>* args, int argc, char** argv);
	/**
	 * @brief loop of CBuild
	 * @param mode => CBuild::RType -> from CBuild::parse function 
	 * @param args => lib::map<std::string, std::string>* -> args from CBuild::parse
	 */
	void loop(RType mode, lib::map<std::string, std::string>* args);
} // namespace CBuild
#endif // __CBUILD_CORE_HPP__