#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include "driver/driver.h"
#include "printer/printer.h"
//#include "scope/old_scope_builder.h"
#include "ir/ir_builder.h"
#include "vm/irvm.h"
#include "printer/ir_printer.h"
#include "printer/ir_cpp_printer.h"
#include "semantic/semantic_driver.h"
#include "log/actions.h"

using namespace haard;

Driver::Driver() {
    path_delimiter = '/';
    env_var = "HAARD_PATH";
    main_path = "main.hd";
    modules = new Modules();

    pretty_flag = false;
    info_flag = false;
    help_flag = false;
    show_logs_flag = false;
    show_ir_flag = false;
    cpp_flag = false;
}

Driver::~Driver() {
    delete modules;
}

void Driver::run() {
    set_root_path_from_main_file();
    configure_search_path();

    run_info_flags();

    log_info("starting parsing...");
    parse_modules();

    log_info("ending parsing...");
    check_for_errors();

    run_flags();

    log_info("starting semantic analysis...");
    semantic_analysis();
    check_for_errors();
    log_info("ending semantic analysis...");

    log_info("starting ir generation...");

    ir_generation();
    generate_cpp();
    show_ir();

    check_for_errors();
    log_info("ir generation done...");

    if (show_logs_flag) {
        log_print();
    }
}

void Driver::run_info_flags() {
    if (info_flag) {
        print_information();
    }

    if (help_flag) {
        show_help();
    }
}

void Driver::run_flags() {
    if (pretty_flag) {
        print_modules();
    }
}

void Driver::set_flags(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strstr(argv[i], ".hd") != nullptr) {
            main_path = std::string(argv[i]);
        } else if (strcmp(argv[i], "-o") == 0) {
            ++i;
            output_name = std::string(argv[i]);
        } else if (strcmp(argv[i], "-i") == 0) {
            ++i;
            search_path.push_back(argv[i]);
        } else if (strcmp(argv[i], "--pretty") == 0) {
            pretty_flag = true;
        } else if (strcmp(argv[i], "--info") == 0) {
            info_flag = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            help_flag = true;
        } else if (strcmp(argv[i], "--show-logs") == 0) {
            log_all();
            show_logs_flag = true;
        } else if (strcmp(argv[i], "--show-ir") == 0) {
            show_ir_flag = true;
        } else if (strcmp(argv[i], "--cpp") == 0) {
            cpp_flag = true;
        }
    }
}

void Driver::print_information() {
    std::cout << "Compiler information:\n";
    std::cout << " - Main file: " << main_path << std::endl;
    std::cout << " - Root path: " << root_path << std::endl;

    std::cout << " - Search path:\n";

    for (int i = 0; i < search_path.size(); ++i) {
        std::cout << "\t\"" << search_path[i] << '"' << std::endl;
    }

    exit(0);
}

void Driver::parse_modules() {
    std::string path;

    if (root_path == ".") {
        path = "./" + main_path;
    } else {
        path = main_path;
    }

    parse_imports(parse_file(path));
}

void Driver::semantic_analysis() {
    SemanticDriver driver;

    driver.build_modules(modules);
    /*ModulesScopeBuilder builder;

    builder.build_modules(modules);*/
}

void Driver::ir_generation() {
    IRBuilder builder;

    builder.build(modules);

    ir_modules = builder.get_modules();
/*
    IrVM vm;
    vm.execute_modules(modules);
    //vm.dump_memory(1024 * 2);

    delete modules;*/
}

void Driver::parse_imports(Module* file) {
    if (file == nullptr)
        return;

    int count = file->import_count();

    if (count > 0) {
        for (int i = 0; i < count; ++i) {
            parse_import(file->get_import(i));
        }
    }
}

void Driver::parse_import(Import* import) {
    parse_simple_import(import);
}

void Driver::parse_simple_import(Import* import) {
    Module* file = nullptr;
    std::string path = build_import_path(import);

    file = modules->get_module(path);

    if (file != nullptr) {
        import->set_module(file);
    } else {
        file = parse_file(path);
        import->set_module(file);
        parse_imports(file);
    }
}

Module* Driver::parse_file(std::string path) {
    std::stringstream ss;
    std::string relative_path;

    if (!file_exists(path)) {
        ss << "Error: file '" << path << "' doesn't exist";
        log_error_and_exit(ss.str());
    }

    ss << "Parsing: " << path;
    log_info(ss.str());

    if (!modules->has_module(path)) {
        Parser parser;
        relative_path = build_relative_path(path);
        modules->add_module(path, parser.read(path, relative_path));
    }

    return modules->get_module(path);
}

void Driver::print_modules() {
    Printer printer;

    printer.print_modules(modules);
    std::cout << printer.to_str();
    exit(0);
}

void Driver::generate_cpp() {
    if (!cpp_flag) return;

    IRCppPrinter cp;
    std::ofstream file("out.cc");
    cp.print_modules(ir_modules);

    file << cp.get_output() << std::endl;
    file.close();
}

void Driver::show_ir() {
    if (!show_ir_flag) return;

    IRPrinter p;

    p.print_modules(ir_modules);
    std::cout << p.get_output() << std::endl;

}

std::string Driver::build_import_path(Import* import) {
    std::string str;

    for (int i = 0; i < import->path_count(); ++i) {
        str += path_delimiter;
        str += import->get_path(i);
    }

    str += ".hd";

    for (int i = 0; i < search_path.size(); ++i) {
        std::string full_path = search_path[i] + str;

        if (file_exists(full_path)) {
            return full_path;
        }
    }

    return str;
}

bool Driver::file_exists(std::string path) {
    std::ifstream f(path.c_str());
    return f.good();
}

std::string Driver::get_env_path(std::string key) {
    char* val;
    val = std::getenv(key.c_str());
    std::string retval = "";

    if (val != nullptr) {
        retval = val;
    }

    return retval;
}

void Driver::configure_search_path() {
    std::string tmp;

    search_path.push_back(root_path);
    std::string env_path = get_env_path(env_var);

    for (int i = 0; i < env_path.size(); ++i) {
        if (env_path[i] != ':') {
            tmp += env_path[i];
        } else {
            search_path.push_back(tmp);
            tmp = "";
        }
    }

    if (tmp.size() > 0) {
        search_path.push_back(tmp);
    }
}

void Driver::set_root_path_from_main_file() {
    int c;

    for (c = main_path.size() - 1; c >= 0; --c) {
        if (main_path[c] == path_delimiter) break;
    }

    // name.hd
    if (c == -1) {
        root_path = ".";
    } else {
        for (int i = 0; i < c; ++i) {
            root_path += main_path[i];
        }
    }
}

std::vector<std::string> Driver::get_files_from_dir(std::string path) {
    std::vector<std::string> files;

    DIR* dirp;
    struct dirent* dp;

    dirp = opendir(path.c_str());

    if (dirp == nullptr) {
        // err = 1;
        return files;
    }

    while ((dp = readdir(dirp)) != nullptr) {
        char* name = dp->d_name;

        if (strlen(name) > 3) {
            while (*name != '\0') {
                ++name;
            }

            name = name - 3;

            if (strcmp(name, ".hd") == 0) {
                files.push_back(std::string(dp->d_name));
            }
        }
    }

    return files;
}

void Driver::show_help() {
    const char msg[] = 
        "Usage: hdc [options] file...\n";
        "Options:\n"
        "    -h         Show this help message\n"
        "    -o         Choose output name\n"
        "    -i         Include search path\n"
        "    -cpp       Generate high level C++\n"
        "    -pretty    Pretty Print file\n"
        "    -info      Show compiler info\n";

    std::cout << msg;
    exit(0);
}

void Driver::check_for_errors() {
    if (log_has_error()) {
        log_print();
        exit(0);
    }
}

std::string Driver::build_relative_path(std::string path) {
    int i;
    int j;
    bool found;
    std::string s;

    for (i = 0; i < search_path.size(); ++i) {
        found = true;

        for (j = 0; j < search_path[i].size(); ++j) {
            if (search_path[i][j] != path[j]) {
                found = false;
            }
        }

        if (found) {
            // -3 to remove '.hd'
            for (j = j + 1; j < path.size() - 3; ++j) {
                if (path[j] == path_delimiter) {
                    s += ".";
                } else {
                    s += path[j];
                }
            }

            break;
        }
    }

    return s;
}
