#include "semantic/class_declarer.h"

using namespace haard;

ClassDeclarer::ClassDeclarer() {
    current_scope = nullptr;
    logger = nullptr;
}

void ClassDeclarer::build_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }
}

void ClassDeclarer::build_source(Source* source) {
    current_scope = source->get_scope();

    for (int i = 0; i < source->classes_count(); ++i) {
        build_class(source->get_class(i));
    }
}

void ClassDeclarer::build_class(Class* klass) {
    std::string name = klass->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_class(name, klass);
        logger->info("file.hd: declaring class " + name);
    }
}

Logger* ClassDeclarer::get_logger() const {
    return logger;
}

void ClassDeclarer::set_logger(Logger* value) {
    logger = value;
}
