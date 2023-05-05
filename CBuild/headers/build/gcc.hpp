/**
 * @file gcc.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief gcc toolchain realization
 * @version 1.0
 * @date 2023-02-03
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
// Project files
#include "Build.hpp"
#include "../print.hpp"
#include "../hash.hpp"
#include "../system.hpp"
// Code
#ifndef _CBUILD_GCC_TOOLCHAIN
#define _CBUILD_GCC_TOOLCHAIN
namespace CBuild
{
    class GCC : public CBuild::Toolchain
    {
    public:
        /**
         * @brief Construct a new GCC object
         *
         * @param id Id
         */
        GCC(std::string id)
        {
            this->id = id;
            this->linker = "gcc";
            this->compiler = "gcc";
            this->packer = "ar cr";
        }
        /**
         * @brief Construct a new GCC object
         *
         * @param id Id
         * @param name Name
         */
        GCC(std::string id, std::string name)
        {
            this->id = id;
            this->name = name;
            this->linker = "gcc";
            this->compiler = "gcc";
            this->packer = "ar cr";
        }

    protected:
        // For docs see g++.hpp
        void build()
        {
            std::string args;
            for (auto elem : this->compiler_args)
            {
                args += elem;
                args += " ";
            }
            auto files = this->gen_file_list(this->force);
            std::vector<std::string> hash_files;
            if (files.size() > 0)
            {
                for (unsigned int i = 0; i < files.size(); i++)
                {
                    std::string cmd = this->compiler + " -c ";
                    cmd += files.at(i).key;
                    cmd += " ";
                    cmd += args;
                    cmd += " -o ";
                    cmd += files.at(i).data;
                    // CBuild::print(cmd.c_str(), CBuild::BLUE);
                    CBuild::system(cmd);
                    hash_files.push_back(this->gen_hash_file(files.at(i).key));
                }
            }
            CBuild::save_hashes(hash_files, files.keys());
        }
        void link()
        {
            std::string args;
            for (auto elem : this->link_args)
            {
                args += elem;
                args += " ";
            }
            auto files = this->gen_file_list(true);
            std::string flist;
            for (unsigned int i = 0; i < files.size(); i++)
            {
                flist += files.at(i).data;
                flist += " ";
            }
            if (files.size() > 0)
            {
                std::string cmd = this->linker + " ";
                cmd += flist;
                cmd += " ";
                cmd += args;
                cmd += " ";
                cmd += " -o ";
                cmd += this->gen_out_name();
                // CBuild::print(cmd.c_str(), CBuild::BLUE);
                CBuild::system(cmd);
            }
        }
        void link_pack()
        {
            std::string args;
            for (auto elem : this->link_args)
            {
                args += elem;
                args += " ";
            }
            auto files = this->gen_file_list(true);
            std::string flist;
            for (unsigned int i = 0; i < files.size(); i++)
            {
                flist += files.at(i).data;
                flist += " ";
            }
            if (files.size() > 0)
            {
                std::string cmd = this->packer + " ";
                cmd += this->gen_out_name();
                cmd += " ";
                cmd += flist;
                cmd += " ";
                // CBuild::print(cmd.c_str(), CBuild::BLUE);
                CBuild::system(cmd);
            }
        }
    };
} // namespace CBuild
#endif // _CBUILD_GCC_TOOLCHAIN
