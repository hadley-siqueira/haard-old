#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include "driver/driver.h"
#include "printer/printer.h"

using namespace haard;

Driver::Driver() {
    path_delimiter = '/';
    env_var = "HAARD_PATH";
    main_path = "main.hd";
    sources = new Sources;
}

Driver::~Driver() {
    delete sources;
}

void Driver::run() {
    set_root_path_from_main_file();
    configure_search_path();
    parse_sources();
    print_sources();
/*    run_flags();
    parse_program();
    build_scopes();

    if (cpp_flag) {
        build_cpp();
    }
    //build_ir();*/
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
        }
    }
}

void Driver::parse_sources() {
    parse_imports(parse_file(main_path));
}

void Driver::parse_imports(Source* file) {
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
    Source* file = nullptr;
    std::string path = build_import_path(import);

    file = sources->get_source(path);

    if (file != nullptr) {
        import->set_source(file);
    } else {
        file = parse_file(path);
        import->set_source(file);
        parse_imports(file);
    }
}

Source* Driver::parse_file(std::string path) {
    if (!file_exists(path)) {
        std::cout << "Error: file '" << path << "' doesn't exist\n";
        exit(0);
    }

    std::cout << "Parsing: " << path << std::endl;

    if (!sources->has_source(path)) {
        Parser parser;
        sources->add_source(path, parser.read(path.c_str()));
    }

    return sources->get_source(path);
}

void Driver::print_sources() {
    Printer printer;

    printer.print_sources(sources);
    std::cout << printer.to_str();
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
