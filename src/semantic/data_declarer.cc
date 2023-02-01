#include "semantic/data_declarer.h"

using namespace haard;

DataDeclarer::DataDeclarer() {
    current_scope = nullptr;
    logger = nullptr;
}

void DataDeclarer::build_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }
}

void DataDeclarer::build_source(Source* source) {
    current_scope = source->get_scope();

    for (int i = 0; i < source->classes_count(); ++i) {
        build_data(source->get_data(i));
    }
}

void DataDeclarer::build_data(Data* data) {
    std::string name = data->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_data(name, data);
        logger->info("file.hd: declaring class " + name);
    }
}

Logger* DataDeclarer::get_logger() const {
    return logger;
}

void DataDeclarer::set_logger(Logger* value) {
    logger = value;
}
