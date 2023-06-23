#include "semantic/semantic_driver.h"
#include "semantic/semantic_first_pass.h"
#include "semantic/semantic_second_pass.h"

using namespace haard;

void SemanticDriver::build_modules(Modules* modules) {
    first_pass(modules);
    second_pass(modules);
}

void SemanticDriver::first_pass(Modules* modules) {
    SemanticFirstPass pass;

    pass.build_modules(modules);
}

void SemanticDriver::second_pass(Modules* modules) {
    SemanticSecondPass pass;

    pass.build_modules(modules);
}
