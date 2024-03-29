#ifndef HAARD_DRIVER_H
#define HAARD_DRIVER_H

#include <string>
#include <vector>

#include "ast/modules.h"
#include "ast/module.h"
#include "parser/parser.h"
#include "log/logger.h"
#include "ir/ir_modules.h"

namespace haard {
    class Driver {
        public:
            Driver();
            ~Driver();

        public:
            void run();
            void set_flags(int argc, char* argv[]);

        private:
            void run_info_flags();
            void run_flags();
            void parse_modules();
            void semantic_analysis();
            void ir_generation();

            void parse_imports(Module* file);
            void parse_import(Import* import);
            void parse_simple_import(Import* import);
            void print_information();
            Module* parse_file(std::string path);
            void print_modules();

            void generate_cpp();
            void show_ir();

            std::string build_import_path(Import* import);
            bool file_exists(std::string path);
            std::string get_env_path(std::string key);
            void configure_search_path();
            void set_root_path_from_main_file();
            std::vector<std::string> get_files_from_dir(std::string path);
            void check_for_errors();
            std::string build_relative_path(std::string path);

            void show_help();

        private:
            char path_delimiter;
            std::string main_path;
            std::string root_path;
            std::string env_var;
            std::string output_name;
            std::vector<std::string> search_path;
            Modules* modules;
            IRModules* ir_modules;

            bool pretty_flag;
            bool info_flag;
            bool help_flag;
            bool show_logs_flag;
            bool show_ir_flag;
            bool cpp_flag;
    };
}

#endif
