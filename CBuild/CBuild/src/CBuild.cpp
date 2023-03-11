/**
 * @file CBuild.cpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Main source file of CBuild core
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
#include "stdio.h"
// Project headers
#include "../../headers/CBuild.hpp"
#include "../../headers/register.hpp"
#include "../../headers/filesystem++.hpp"
#include "../../headers/CBuild_defs.hpp"
/* CBuild.hpp */
CBuild::RType CBuild::parse(lib::map<std::string, std::string>* args, int argc, char** argv)
{
	// Init registry
	CBuild::Registry::init();
	// Argument counters
	int acnt = 0;
	int pacnt = 0;
	// Default return
	CBuild::RType ret = CBuild::ERROR;
	// Parse all command line arguments
	for (int i = 1; i < argc; i++)
	{
		std::string tmp = argv[i];
		// Force, not use cache compilation
		if (tmp == std::string("-f")) {
			try{args->push_back_check("force", "force");}catch(std::exception& e){}
		// Compile
		} else if (tmp == std::string("-b")) {
			// We build now, not error
			ret = CBuild::BUILD;
			i++;
			// Load toolchain name and perform all checks
			if (i < argc)
			{
				tmp = argv[i];
				if (tmp.at(0) == '-')
				{
					puts("-b <toolchain name>");
					exit(0xFF);
				}
				else
				{
					try {args->push_back_check("toolchain_id", tmp);}catch(std::exception& e){}
				}
			}
			else
			{
				puts("-b <toolchain name>");
				exit(0xFF);
			}
		// Run
		} else if (tmp == std::string("-r")) {
			// Run now, not error
			ret = CBuild::RUN;
			i++;
			// Load toolchain name and perform all checks
			if (i < argc)
			{
				tmp = argv[i];
				if (tmp.at(0) == '-')
				{
					puts("-r <toolchain name>");
					exit(0xFF);
				}
				else
				{
					try{args->push_back_check("toolchain_id", tmp);}catch(std::exception& e){}
				}
			}
			else
			{
				try {args->push_back_check("toolchain_id", "CBuild_all");}catch(std::exception& e){}
			}
		// Build and run
		} else if (tmp == std::string("-br")) {
			// Build and run now, not error
			ret = CBuild::BUILD_RUN;
			i++;
			// Load toolchain name and perform all checks
			if (i < argc)
			{
				tmp = argv[i];
				if (tmp.at(0) == '-')
				{
					puts("-br <toolchain name>");
					exit(0xFF);
				}
				else
				{
					try{args->push_back_check("toolchain_id", tmp);}catch(std::exception& e){}
				}
			}
			else
			{
				puts("-br <toolchain name>");
				exit(0xFF);
			}
		// Debug
		} else if (tmp == std::string("-d")) {
			// Debug now, not error
			ret = CBuild::DEBUG;
			i++;
			// Load toolchain name and perform all checks
			if (i < argc)
			{
				tmp = argv[i];
				if (tmp.at(0) == '-')
				{
					puts("-d <toolchain name>");
					exit(0xFF);
				}
				else
				{
					try{args->push_back_check("toolchain_id", tmp);}catch(std::exception& e){}
				}
			}
			else
			{
				puts("-d <toolchain name>");
				exit(0xFF);
			}
		// Cleran builded app
		} else if (tmp == std::string("-c")) {
			// Clear now, not error
			ret = CBuild::CLEAR;
			i++;
			// Load toolchain name and perform all checks
			if (i < argc)
			{
				tmp = argv[i];
				if (tmp.at(0) == '-')
				{
					puts("-c <toolchain name>");
					exit(0xFF);
				}
				else
				{
					try{args->push_back_check("toolchain_id", tmp);}catch(std::exception& e){}
				}
			}
			else
			{
				puts("-c <toolchain name>");
				exit(0xFF);
			}
		// Run task
		} else if (tmp == std::string("-t")) {
			// Run task (legacy TASK) now
			ret = CBuild::TASK;
			i++;
			// Load toolchain name and perform all checks
			if (i < argc)
			{
				tmp = argv[i];
				if (tmp.at(0) == '-')
				{
					puts("-t <task name>");
					exit(0xFF);
				}
				else
				{
					args->push_back_check("task_id", tmp);
				}
			}
			else
			{
				puts("-t <task name>");
				exit(0xFF);
			}
		// Add argument
		} else if (tmp == std::string("-a")) {
			i++;
			// Load argument and perform some checks
			if (i < argc)
			{
				tmp = argv[i];
				args->push_back("a" + std::to_string(acnt), tmp);
				acnt++;
			}
			else
			{
				exit(0xFF);
			}
		// Program arguments
		} else if (tmp == std::string("-pa")) {
			i++;
			// Load program argument and perform some checks
			if (i < argc)
			{
				tmp = argv[i];
				args->push_back("pa" + std::to_string(pacnt), tmp);
				pacnt++;
			}
			else
			{
				exit(0xFF);
			}
		// Init workspace
		} else if (tmp == std::string("--init")) {
			// Fresh start
			args->clear();
			// Load init task
			args->push_back("task_id", "CBuild_init");
			return CBuild::TASK;
		// Get help message
		} else if ((tmp == std::string("--help")) || (tmp == std::string("-h"))) {
			// Fresh start
			args->clear();
			// Load help task
			args->push_back("task_id", "CBuild_help");
			return CBuild::TASK;
		// Parse user-defined keywords
		} else {
			auto keys = Registry::GetKeywordsList();
			for (unsigned int i = 0; i < keys.size(); i++)
			{
				if (tmp == keys.at(i).key)
				{
					ret = CBuild::TASK;
					args->push_back_check("task_id", keys.at(i).data);
				}
			}
		}
	}
	return ret;
}
void CBuild::loop(CBuild::RType mode, lib::map<std::string, std::string>* args)
{
	// Check if cache directory exists
	if (!CBuild::fs::exists(CBUILD_CACHE_DIR))
	{
		// Create dir
		CBuild::fs::create({CBUILD_CACHE_DIR}, CBuild::fs::DIR);
	}
	// Check if copy cache dir exists
	if (!CBuild::fs::exists(CBUILD_CACHE_DIR + "/" + CBUILD_COPY_CACHE_DIR))
	{
		// Create dir
		CBuild::fs::create({CBUILD_CACHE_DIR + "/" + CBUILD_COPY_CACHE_DIR}, CBuild::fs::DIR);
	}
	// Args
	std::vector<std::string> pargs, ppargs;
	bool force;
	// Load args
	for (unsigned int i = 0; i < args->size(); i++)
	{
		auto elem = args->at(i);
		try
		{
			if (elem.key.at(0) == 'a')
			{
				pargs.push_back(elem.data);
			}
			else if (elem.key.at(0) == 'p' && elem.key.at(1) == 'a')
			{
				ppargs.push_back(elem.data);
			}
			else if (elem.key == "force")
			{
				force = true;
			}
		}
		catch (std::exception& e){}
	}
	// Select mode
	switch(mode)
	{
	// Build
	case CBuild::BUILD:
		{
			// Load toolchain
			std::string id = *(args->get("toolchain_id"));
			CBuild::Toolchain* target = CBuild::Registry::GetToolchain(id);
			// Error
			if (target == NULL)
			{
				printf("Toolchain %s not found. Exiting...\n", id.c_str());
				exit(0xFF);
			}
			// Call tolchain in build mode
			target->call(&pargs, force);
		}
		break;
	// Build and run
	case CBuild::BUILD_RUN:
		{
			// Load toolchain
			std::string id = *(args->get("toolchain_id"));
			CBuild::Toolchain* target = CBuild::Registry::GetToolchain(id);
			// Error
			if (target == NULL)
			{
				printf("Toolchain %s not found. Exiting...\n", id.c_str());
				exit(0xFF);
			}
			// Run toolchain in build mode
			target->call(&pargs, force);
			// Run toolchain in run mode
			target->run(&ppargs);
		}
		break;
	// Run 
	case CBuild::RUN:
		{
			// Load toolchain
			std::string id = *(args->get("toolchain_id"));
			CBuild::Toolchain* target = CBuild::Registry::GetToolchain(id);
			// Error
			if (target == NULL)
			{
				printf("Toolchain %s not found. Exiting...\n", id.c_str());
				exit(0xFF);
			}
			// Run toolchain in run mode
			target->run(&ppargs);
		}
		break;
	// Build and run indebug mode
	case CBuild::DEBUG:
		{
			// Load toolchain
			std::string id = *(args->get("toolchain_id"));
			CBuild::Toolchain* target = CBuild::Registry::GetToolchain(id);
			// Error
			if (target == NULL)
			{
				printf("Toolchain %s not found. Exiting...\n", id.c_str());
				exit(0xFF);
			}
			// Run toolchain in debug mode
			target->debug(&pargs, &ppargs);
		}
		break;
	// Clear (files)
	case CBuild::CLEAR:
		{
			// Load toolchain
			std::string id = *(args->get("toolchain_id"));
			CBuild::Toolchain* target = CBuild::Registry::GetToolchain(id);
			// Error
			if (target == NULL)
			{
				printf("Toolchain %s not found. Exiting...\n", id.c_str());
				exit(0xFF);
			}
			// Run toolchain is self-clear mode
			target->clear();
		}
		break;
	// Run task
	case CBuild::TASK:
		// Call task
		CBuild::Registry::CallTask(*(args->get("task_id")), pargs);
		break;
	// Error
	case CBuild::ERROR:
		[[fallthrough]];
	// Also error ;)
	default:
		exit(0xFF);
	}
}