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
#include "fstream"
#include "iomanip"
#include "iostream"
#include "random"
#include "regex"
#include "sstream"
#include "stdio.h"
// Project headers
#include "../../headers/CBuild.hpp"
#include "../../headers/CBuild_defs.hpp"
#include "../../headers/filesystem++.hpp"
#include "../../headers/print.hpp"
#include "../../headers/register.hpp"
#include "../../headers/system.hpp"
/* CBuild.hpp */
CBuild::RType CBuild::parse(lib::map<std::string, std::string> *args, int argc,
                            char **argv) {
  // Init registry
  CBuild::Registry::init();
  // Argument counters
  int acnt = 0;
  int pacnt = 0;
  // Default return
  CBuild::RType ret = CBuild::ERROR;
  // Parse all command line arguments
  for (int i = 1; i < argc; i++) {
    std::string tmp = argv[i];
    // Force, not use cache compilation
    if (tmp == std::string("-f")) {
      try {
        args->push_back_check("force", "force");
      } catch (std::exception &e) {
      }
      // Verbose flag
    } else if (tmp == std::string("-v")) {
      try {
        args->push_back_check("out", "verbose");
      } catch (std::exception &e) {
      }
      // Disabling output
    } else if (tmp == std::string("--no-cli-out")) {
      try {
        args->push_back_check("out", "none");
      } catch (std::exception &e) {
      }
      // Makefile gen
    } else if (tmp == std::string("-gm")) {
      try {
        args->push_back_check("gen", "make");
      } catch (std::exception &e) {
      }
      // Compil database (compile_command.json) gen
    } else if (tmp == std::string("-gc")) {
      try {
        args->push_back_check("gen", "ccj");
      } catch (std::exception &e) {
      }
      // Load deps
    } else if (tmp == std::string("-ld")) {
      ret = CBuild::RType::LOAD_DEPS;
      i++;
      // Load toolchain name and perform all checks
      if (i < argc) {
        tmp = argv[i];
        if (tmp.at(0) == '-') {
          CBuild::print("-ld <toolchain name>");
          exit(0xFF);
        } else {
          try {
            args->push_back_check("toolchain_id", tmp);
          } catch (std::exception &e) {
          }
        }
      } else {
        CBuild::print("-ld <toolchain name>");
        exit(0xFF);
      }
      // Compile
    } else if (tmp == std::string("-b")) {
      // We build now, not error
      ret = CBuild::BUILD;
      i++;
      // Load toolchain name and perform all checks
      if (i < argc) {
        tmp = argv[i];
        if (tmp.at(0) == '-') {
          CBuild::print("-b <toolchain name>");
          exit(0xFF);
        } else {
          try {
            args->push_back_check("toolchain_id", tmp);
          } catch (std::exception &e) {
          }
        }
      } else {
        CBuild::print("-b <toolchain name>");
        exit(0xFF);
      }
      // Run
    } else if (tmp == std::string("-r")) {
      // Run now, not error
      ret = CBuild::RUN;
      i++;
      // Load toolchain name and perform all checks
      if (i < argc) {
        tmp = argv[i];
        if (tmp.at(0) == '-') {
          CBuild::print("-r <toolchain name>");
          exit(0xFF);
        } else {
          try {
            args->push_back_check("toolchain_id", tmp);
          } catch (std::exception &e) {
          }
        }
      } else {
        try {
          args->push_back_check("toolchain_id", "CBuild_all");
        } catch (std::exception &e) {
        }
      }
      // Build and run
    } else if (tmp == std::string("-br")) {
      // Build and run now, not error
      ret = CBuild::BUILD_RUN;
      i++;
      // Load toolchain name and perform all checks
      if (i < argc) {
        tmp = argv[i];
        if (tmp.at(0) == '-') {
          CBuild::print("-br <toolchain name>");
          exit(0xFF);
        } else {
          try {
            args->push_back_check("toolchain_id", tmp);
          } catch (std::exception &e) {
          }
        }
      } else {
        CBuild::print("-br <toolchain name>");
        exit(0xFF);
      }
      // Debug
    } else if (tmp == std::string("-d")) {
      // Debug now, not error
      ret = CBuild::DEBUG;
      i++;
      // Load toolchain name and perform all checks
      if (i < argc) {
        tmp = argv[i];
        if (tmp.at(0) == '-') {
          CBuild::print("-d <toolchain name>");
          exit(0xFF);
        } else {
          try {
            args->push_back_check("toolchain_id", tmp);
          } catch (std::exception &e) {
          }
        }
      } else {
        CBuild::print("-d <toolchain name>");
        exit(0xFF);
      }
      // Cleran builded app
    } else if (tmp == std::string("-c")) {
      // Clear now, not error
      ret = CBuild::CLEAR;
      i++;
      // Load toolchain name and perform all checks
      if (i < argc) {
        tmp = argv[i];
        if (tmp.at(0) == '-') {
          CBuild::print("-c <toolchain name>");
          exit(0xFF);
        } else {
          try {
            args->push_back_check("toolchain_id", tmp);
          } catch (std::exception &e) {
          }
        }
      } else {
        CBuild::print("-c <toolchain name>");
        exit(0xFF);
      }
      // Run task
    } else if (tmp == std::string("-t")) {
      // Run task (legacy TASK) now
      ret = CBuild::TASK;
      i++;
      // Load toolchain name and perform all checks
      if (i < argc) {
        tmp = argv[i];
        if (tmp.at(0) == '-') {
          CBuild::print("-t <task name>");
          exit(0xFF);
        } else {
          args->push_back_check("task_id", tmp);
        }
      } else {
        CBuild::print("-t <task name>");
        exit(0xFF);
      }
      // Add argument
    } else if (tmp == std::string("-a")) {
      i++;
      // Load argument and perform some checks
      if (i < argc) {
        tmp = argv[i];
        args->push_back("a" + std::to_string(acnt), tmp);
        acnt++;
      } else {
        exit(0xFF);
      }
      // Program arguments
    } else if (tmp == std::string("-pa")) {
      i++;
      // Load program argument and perform some checks
      if (i < argc) {
        tmp = argv[i];
        args->push_back("pa" + std::to_string(pacnt), tmp);
        pacnt++;
      } else {
        exit(0xFF);
      }
      // Rebuild of CBuild
    } else if (tmp == std::string("--rebuild")) {
      ret = CBuild::REBUILD;
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
      for (unsigned int i = 0; i < keys.size(); i++) {
        if (tmp == keys.at(i).key) {
          ret = CBuild::TASK;
          args->push_back_check("task_id", keys.at(i).data);
        }
      }
    }
  }
  return ret;
}
void CBuild::loop(CBuild::RType mode,
                  lib::map<std::string, std::string> *args) {
  // Check if cache directory exists
  if (!CBuild::fs::exists(CBUILD_CACHE_DIR)) {
    // Create dir
    CBuild::fs::create({CBUILD_CACHE_DIR}, CBuild::fs::DIR);
  }
  // Check if copy cache dir exists
  if (!CBuild::fs::exists(CBUILD_CACHE_DIR + "/" + CBUILD_COPY_CACHE_DIR)) {
    // Create dir
    CBuild::fs::create({CBUILD_CACHE_DIR + "/" + CBUILD_COPY_CACHE_DIR},
                       CBuild::fs::DIR);
  }
  // Check if lib cache sir exists
  if (!CBuild::fs::exists(CBUILD_COPY_CACHE_DIR + "/" +
                          CBUILD_PROJECT_DEPS_DIR)) {
    // Create dir
    CBuild::fs::create({CBUILD_CACHE_DIR + "/" + CBUILD_PROJECT_DEPS_DIR},
                       CBuild::fs::DIR);
  }
  // Check if lib headers cache sir exists
  if (!CBuild::fs::exists(CBUILD_COPY_CACHE_DIR + "/" +
                          CBUILD_PROJECT_DEPS_HEADERS)) {
    // Create dir
    CBuild::fs::create({CBUILD_CACHE_DIR + "/" + CBUILD_PROJECT_DEPS_HEADERS},
                       CBuild::fs::DIR);
  }
  // Args
  std::vector<std::string> pargs, ppargs;
  bool force;
  // Load args
  for (unsigned int i = 0; i < args->size(); i++) {
    auto elem = args->at(i);
    try {
      // Custom arg for CBuild, user-proccesed
      if (elem.key.at(0) == 'a') {
        pargs.push_back(elem.data);
        // Custom arg for runned app, for -r and -br
      } else if (elem.key.at(0) == 'p' && elem.key.at(1) == 'a') {
        ppargs.push_back(elem.data);
        // Force recompilation
      } else if (elem.key == std::string("force")) {
        force = true;
        // Different output modes
      } else if (elem.key == std::string("out")) {
        // More verbose output, some debugging things included;)
        if (elem.data == std::string("verbose")) {
          CBuild::print_verbose();
          // No output at all, only produced by compiler
        } else if (elem.data == std::string("none")) {
          CBuild::print_none();
        }
      }
    } catch (std::exception &e) {
    }
  }
  // Select mode
  switch (mode) {
  // Build
  case CBuild::BUILD: {
    if (*(args->get("toolchain_id")) == std::string("all")) {
      CBuild::Registry::ToolchainAll(force, *(args->get("curr_path")), &pargs);
    } else {
      // Load toolchain
      std::string id = *(args->get("toolchain_id"));
      CBuild::Toolchain *target = CBuild::Registry::GetToolchain(id);
      // Error
      if (target == NULL) {
        printf("Toolchain %s not found. Exiting...\n", id.c_str());
        exit(0xFF);
      }
      // Load libs
      target->load_project_deps(*(args->get("curr_path")));
      // Call tolchain in build mode
      target->call(&pargs, force);
    }
  } break;
  // Build and run
  case CBuild::BUILD_RUN: {
    // Load toolchain
    std::string id = *(args->get("toolchain_id"));
    CBuild::Toolchain *target = CBuild::Registry::GetToolchain(id);
    // Error
    if (target == NULL) {
      printf("Toolchain %s not found. Exiting...\n", id.c_str());
      exit(0xFF);
    }
    // Load libs
    target->load_project_deps(*(args->get("curr_path")));
    // Run toolchain in build mode
    target->call(&pargs, force);
    // Run toolchain in run mode
    target->run(&ppargs);
  } break;
  // Run
  case CBuild::RUN: {
    // Load toolchain
    std::string id = *(args->get("toolchain_id"));
    CBuild::Toolchain *target = CBuild::Registry::GetToolchain(id);
    // Error
    if (target == NULL) {
      printf("Toolchain %s not found. Exiting...\n", id.c_str());
      exit(0xFF);
    }
    // Run toolchain in run mode
    target->run(&ppargs);
  } break;
  // Build and run indebug mode
  case CBuild::DEBUG: {
    // Load toolchain
    std::string id = *(args->get("toolchain_id"));
    CBuild::Toolchain *target = CBuild::Registry::GetToolchain(id);
    // Error
    if (target == NULL) {
      printf("Toolchain %s not found. Exiting...\n", id.c_str());
      exit(0xFF);
    }
    // Load libs
    target->load_project_deps(*(args->get("curr_path")));
    // Run toolchain in debug mode
    target->debug(&pargs, &ppargs);
  } break;
  // Clear (files)
  case CBuild::CLEAR: {
    // Load toolchain
    std::string id = *(args->get("toolchain_id"));
    CBuild::Toolchain *target = CBuild::Registry::GetToolchain(id);
    // Error
    if (target == NULL) {
      printf("Toolchain %s not found. Exiting...\n", id.c_str());
      exit(0xFF);
    }
    // Run toolchain is self-clear mode
    target->clear();
  } break;
  // Run task
  case CBuild::TASK:
    // Call task
    CBuild::Registry::CallTask(*(args->get("task_id")), pargs);
    break;
  // Load deps
  case CBuild::LOAD_DEPS: {
    // Load toolchain
    std::string id = *(args->get("toolchain_id"));
    CBuild::Toolchain *target = CBuild::Registry::GetToolchain(id);
    // Error
    if (target == NULL) {
      printf("Toolchain %s not found. Exiting...\n", id.c_str());
      exit(0xFF);
    }
    // Load libs
    target->load_project_deps(*(args->get("curr_path")));
  } break;
  // Error
  case CBuild::ERROR:
    [[fallthrough]];
  // Also error ;)
  default:
    exit(0xFF);
  }
  // Check if we need to generate something
  auto gen = args->get_ptr("gen");
  // Wee need to generate Makefile
  if (gen != NULL && gen->data == std::string("make")) {
    CBuild::print("Generating Makefile ...", CBuild::color::MAGENTA);
    // Get command log
    auto log = CBuild::get_log();
    // Create Makefile if needed
    if (!CBuild::fs::exists(CBUILD_MAKEFILE)) {
      CBuild::fs::create({CBUILD_MAKEFILE}, CBuild::fs::FILE);
    }
    // Open file
    std::ofstream makefile;
    makefile.open(CBUILD_MAKEFILE, std::ios::app);
    std::string name = "";
    // Create name of make task, first part - CBuild run type or
    // cbuild_ placeholder
    switch (mode) {
    case CBuild::BUILD:
      name += "build_";
      break;
    case CBuild::BUILD_RUN:
      name += "build_run_";
      break;
    case CBuild::RUN:
      name += "run_";
      break;
    case CBuild::CLEAR:
      name += "clear_";
      break;
    case CBuild::DEBUG:
      name += "debug_";
      break;
    case CBuild::TASK:
      name += "task_";
      break;
    default:
      name += "cbuild_";
      break;
    }
    // Second part of make task - target or task id or cbuild
    // placeholder
    if (mode == CBuild::BUILD || mode == CBuild::BUILD_RUN ||
        mode == CBuild::RUN || mode == CBuild::DEBUG || mode == CBuild::CLEAR) {
      name += *(args->get("toolchain_id"));
    } else if (mode == CBuild::TASK) {
      name += *(args->get("task_id"));
    } else {
      name += CBuild::get_random_string(6);
    }
    // Write all executed command to Makefile
    makefile << "\n";
    makefile << name << ":\n";
    for (auto elem : *log) {
      makefile << "\t" << elem << "\n";
    }
    // End Makefile
    makefile << "\n";
    makefile.close();
    // Wee need to generate compile_command.json
  } else if (gen != NULL && gen->data == std::string("ccj")) {
    // We can generate it only for compilation
    if (mode == CBuild::RUN || mode == CBuild::TASK || mode == CBuild::CLEAR ||
        mode == CBuild::RType::REBUILD || mode == CBuild::RType::ERROR) {
      goto gen_end;
    }
    CBuild::print("Generating compile_commands.json ...",
                  CBuild::color::MAGENTA);
    // Get command log
    auto log = CBuild::get_log();
    // Recreate file
    if (CBuild::fs::exists(CBUILD_COMPILE_COMMAND_JSON)) {
      CBuild::fs::remove(CBUILD_COMPILE_COMMAND_JSON, true);
    }
    CBuild::fs::create({CBUILD_COMPILE_COMMAND_JSON}, CBuild::fs::type::FILE);
    // Open file
    std::ofstream ccj;
    ccj.open(CBUILD_COMPILE_COMMAND_JSON, std::ios::out);
    ccj << "[\n";
    // For every executed command
    bool first = true;
    for (unsigned int i = 0; i < log->size(); i++) {
      // Get line data
      std::string cmd = log->at(i);
      // Does it is compilation command
      std::regex pattern("(\\S+)\\s+-c\\s+(\\S+)\\s+(.*)\\s+-o\\s+(\\S+)",
                         std::regex_constants::icase);
      if (std::regex_match(cmd, pattern)) {
        // Preload some data to .json
        if (first == false)
          ccj << ",\n";
        first = false;
        ccj << "{\n";
        // Get file name
        std::istringstream iss(cmd);
        std::string word;
        while (iss >> word) {
          std::regex file(".*\\.(cpp|cc|c|cxx)");
          if (std::regex_match(word, file)) {
            break;
          }
        }
        // Add \ for needed data
        std::string cmd_prep = "";
        std::ostringstream ss;
        for (char c : cmd) {
          switch (c) {
          case '\"':
            ss << "\\\"";
            break;
          case '\\':
            ss << "\\\\";
            break;
          case '/':
            ss << "\\/";
            break;
          case '\b':
            ss << "\\b";
            break;
          case '\f':
            ss << "\\f";
            break;
          case '\n':
            ss << "\\n";
            break;
          case '\r':
            ss << "\\r";
            break;
          case '\t':
            ss << "\\t";
            break;
          case '$':
            ss << "$";
            break;
          default:
            ss << c;
            break;
          }
        }
        // Other data, properly formatted
        cmd_prep = ss.str();
        ccj << "\t\"directory\": \"";
        ccj << args->get_ptr("curr_path")->data;
        ccj << "\",\n";
        ccj << "\t\"command\": \"";
        ccj << cmd_prep;
        ccj << "\",\n";
        ccj << "\t\"file\": \"";
        ccj << word;
        ccj << "\"\n";
        ccj << "}";
      }
    }
    ccj << "\n]\n";
    ccj.close();
  }
gen_end:
  return;
}
void CBuild::rebuild(std::string scripts) {
  // Base
  std::string args = scripts + " ";
  // Pus rebuild args
  args += CBuild::Registry::GetRebuildArgs();
  // Execute command
  CBuild::system(("CBuild_rebuild " + args));
}
namespace CBuild {
int version[] = {CBUILD_VERSION_MAJOR, CBUILD_VERSION_MINOR};
} // namespace CBuild
std::string CBuild::get_version_string() { return CBUILD_VERSION_STR; }
int CBuild::get_version_major() { return CBUILD_VERSION_MAJOR; }
int CBuild::get_version_minor() { return CBUILD_VERSION_MINOR; }
int *CBuild::get_version_array() { return CBuild::version; }
std::string CBuild::get_random_string(unsigned int length) {
  // Random string and uniform distribution, for generating char
  const std::string characters =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";
  std::uniform_int_distribution<> dis(0, characters.size() - 1);
  // Random generator
  std::mt19937_64 gen;
#ifdef __linux__
  // Open random device
  std::ifstream urandom("/dev/urandom", std::ios::binary);
  if (!urandom) {
    goto pseudo;
  }

  // Get random data from /dev/urandom
  std::random_device::result_type seed;
  urandom.read(reinterpret_cast<char *>(&seed), sizeof(seed));

  // Seed random generator
  {
    CBuild::print_full("Using /dev/urandom for seed", CBuild::MAGENTA);
    std::seed_seq seedSeq{seed};
    gen.seed(seedSeq);
    goto gen;
  }
pseudo:
#endif
  // Seed random generator - fallback
  {
    CBuild::print_full("Using std::random_device for seed", CBuild::MAGENTA);
    std::random_device rd;
    gen.seed(rd());
  }
gen:
  // Generate random string with specified length
  std::string randomString;
  for (unsigned int i = 0; i < length; ++i) {
    randomString += characters[dis(gen)];
  }
  return randomString;
}
