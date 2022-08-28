#include <iostream>
#include <cstring>
#include "scope/scope_definition_builder.h"
#include "log/info_messages.h"
#include "log/error_messages.h"

using namespace haard;

ScopeDefinitionBuilder::ScopeDefinitionBuilder() {
    logger = nullptr;
    current_scope = nullptr;
    current_class = nullptr;
    current_function = nullptr;
    var_counter = 0;
}

void ScopeDefinitionBuilder::build(Sources* sources) {
    define_sources(sources);
    build_sources(sources);
}

void ScopeDefinitionBuilder::build_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::build_source(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->classes_count(); ++i) {
        build_class(source->get_class(i));
    }

    for (int i = 0; i < source->function_count(); ++i) {
        build_function(source->get_function(i));
    }

    leave_scope();
}

void ScopeDefinitionBuilder::build_class(Class* klass) {
    enter_scope(klass->get_scope());

    define_class_template_header(klass);
    define_class_super(klass);
    define_class_variables(klass);
    define_class_methods(klass);
    build_class_methods(klass);

    leave_scope();
}

void ScopeDefinitionBuilder::build_class_methods(Class* klass) {

}

void ScopeDefinitionBuilder::build_function(Function* function) {
    enter_scope(function->get_scope());
    current_function = function;

    var_counter = 0;
    build_compound_statement(function->get_statements());

    current_scope->debug();
    std::cout << std::endl;
    leave_scope();
}

// build statements
void ScopeDefinitionBuilder::build_statement(Statement* statement) {
    int kind = statement->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) statement);
        break;

    case STMT_WHILE:
        build_while_statement((WhileStatement*) statement);
        break;

    case STMT_FOR:
    case STMT_FOREACH:
        build_for_statement((ForStatement*) statement);
        break;

    case STMT_EXPRESSION:
        build_expression_statement((ExpressionStatement*) statement);
        break;

    case STMT_IF:
    case STMT_ELIF:
    case STMT_ELSE:
        build_branch_statement((BranchStatement*) statement);
        break;

    case STMT_RETURN:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_GOTO:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_YIELD:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_CONTINUE:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_BREAK:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_VAR_DECL:
   //     build_variable_declaration((VarDeclaration*) statement);
        break;
    }
}

void ScopeDefinitionBuilder::build_compound_statement(CompoundStatement* stmts) {
    for (int i = 0; i < stmts->statements_count(); ++i) {
        build_statement(stmts->get_statement(i));
    }
}

void ScopeDefinitionBuilder::build_expression_statement(ExpressionStatement* statement) {
    build_expression(statement->get_expression());
}

void ScopeDefinitionBuilder::build_jump_statement(JumpStatement* statement) {
    build_expression(statement->get_expression());
}

void ScopeDefinitionBuilder::build_while_statement(WhileStatement* statement) {
    enter_scope(statement->get_scope());

    build_expression(statement->get_condition());
    build_compound_statement(statement->get_statements());

    leave_scope();
}

void ScopeDefinitionBuilder::build_for_statement(ForStatement* statement) {
    enter_scope(statement->get_scope());

    if (statement->get_kind() == STMT_FOR) {
        build_expression(statement->get_initialization());
        build_expression(statement->get_condition());
        build_expression(statement->get_increment());
    }

    build_compound_statement(statement->get_statements());
    leave_scope();
}

void ScopeDefinitionBuilder::build_branch_statement(BranchStatement* statement) {
    enter_scope(statement->get_scope());

    if (statement->get_condition()) {
        build_expression(statement->get_condition());
    }

    if (statement->get_true_statements()) {
        build_statement(statement->get_true_statements());
    }

    leave_scope();

    if (statement->get_false_statements()) {
        build_statement(statement->get_false_statements());
    }
}

// build expressions
void ScopeDefinitionBuilder::build_expression(Expression* expression) {
    if (expression == nullptr) {
        return;
    }

    int kind = expression->get_kind();
    BinOp* bin = (BinOp*) expression;
    UnOp* un = (UnOp*) expression;
    Literal* literal = (Literal*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_ID:
        build_identifier((Identifier*) expression);
        break;

    case EXPR_ASSIGN:
        build_assignment(bin);
        break;

    case EXPR_LITERAL_BOOL:
        build_literal(literal, TYPE_BOOL);
        break;

    case EXPR_LITERAL_INTEGER:
        build_literal(literal, TYPE_INT);
        break;

    case EXPR_LITERAL_FLOAT:
        build_literal(literal, TYPE_FLOAT);
        break;

    case EXPR_LITERAL_DOUBLE:
        build_literal(literal, TYPE_DOUBLE);
        break;

    case EXPR_LITERAL_CHAR:
        build_literal(literal, TYPE_CHAR);
        break;

    case EXPR_LITERAL_STRING:
        build_literal(literal, TYPE_STR);
        break;

    case EXPR_LITERAL_SYMBOL:
        build_literal(literal, TYPE_SYMBOL);
        break;

    }
}

void ScopeDefinitionBuilder::build_identifier(Identifier* id) {
    Symbol* sym = current_scope->has(id->get_lexeme());

    if (!sym) {
        logger->error("id not in scope");
    }

    id->set_symbol(sym);
}


void ScopeDefinitionBuilder::build_assignment(BinOp* bin) {
    build_expression(bin->get_right());

    if (is_new_var_assign(bin)) {
        create_new_var(bin);
    }
 
    build_expression(bin->get_left());
}

void ScopeDefinitionBuilder::build_literal(Literal* literal, int kind) {
    literal->set_type(new Type(kind));
}

bool ScopeDefinitionBuilder::is_new_var_assign(BinOp* bin) {
    if (bin->get_left()->get_kind() != EXPR_ID) {
        return false;
    }

    Identifier* id = (Identifier*) bin->get_left();
    Symbol* sym = current_scope->has(id->get_lexeme());

    return sym == nullptr;
}

void ScopeDefinitionBuilder::create_new_var(BinOp* bin) {
    Identifier* id = (Identifier*) bin->get_left();

    Variable* var = new Variable(id);
    var->set_type(bin->get_right()->get_type());
    var->set_uid(var_counter++);
    var->set_kind(VAR_LOCAL);
    bin->set_initial_assign(true);
    current_scope->define(var);
    current_function->add_variable(var);
}

// define methods
void ScopeDefinitionBuilder::define_sources(Sources* sources) {
    connect_sibling_scopes(sources);

    define_sources_classes(sources);
    define_sources_functions(sources);
}

void ScopeDefinitionBuilder::define_sources_classes(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_classes(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_sources_functions(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_functions(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_source_classes(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->classes_count(); ++i) {
        define_class(source->get_class(i));
    }

    leave_scope();
}

void ScopeDefinitionBuilder::define_source_functions(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->function_count(); ++i) {
        define_function(source->get_function(i));
    }

    leave_scope();
}

void ScopeDefinitionBuilder::define_class(Class* klass) {
    Class* other;
    Symbol* sym;

    logger->info(info_message_defining_class(klass));
    sym = current_scope->local_has(klass->get_name());

    if (!sym) {
        current_scope->define(klass);
    } else {
        logger->error(error_message_cant_define_class(klass, sym));
    }
}

void ScopeDefinitionBuilder::define_class_variables(Class* klass) {
    for (int i = 0; i < klass->variables_count(); ++i) {
        define_class_variable(klass->get_variable(i));
        klass->get_variable(i)->set_uid(i); 
    }
}

void ScopeDefinitionBuilder::define_class_variable(Variable* var) {
    Symbol* sym;
    Symbol* lsym;

    sym = current_scope->has_field(var->get_name());
    lsym = current_scope->local_has(var->get_name());

    if (!sym) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        link_type(var->get_type());
    } else if (sym->get_kind() != SYM_CLASS_VARIABLE) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        link_type(var->get_type());
    } else {
        if (lsym == nullptr) {
            logger->error("<red>error: </red>class variable already defined in super class");
        } else {
            logger->error("<red>error: </red>class variable already defined");
        }
    }
}

void ScopeDefinitionBuilder::define_class_methods(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        define_class_method(klass->get_method(i));
    }
}

void ScopeDefinitionBuilder::define_class_method(Function* method) {
    Symbol* sym = current_scope->local_has(method->get_name());

    define_method_signature(method);

    if (!sym) {
        logger->info(info_message_defining_method(method));
        current_scope->define(method);
    } else if (sym->get_kind() == SYM_METHOD) {
        logger->info(info_message_defining_method(method));
        define_overloaded_function(sym, method);
    } else {
        logger->error("can't define method");
    }
}

void ScopeDefinitionBuilder::define_method_signature(Function* method) {
    define_function_signature(method);
}

void ScopeDefinitionBuilder::define_class_template_header(Class* klass) {
    TemplateHeader* header = klass->get_template_header();

    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            TemplateType* t = (TemplateType*) header->get_type(i);

            if (current_scope->local_has(t->get_name())) {
                logger->error("error: already used type on template header");
            } else {
                current_scope->define(t);
            }
        }
    }
}

void ScopeDefinitionBuilder::define_class_super(Class* klass) {
    // FIXME handle super with templates
    if (klass->has_super_class()) {
        link_type(klass->get_super_class());
        NamedType* named = (NamedType*) klass->get_super_class();
        Symbol* sym = named->get_symbol();
        Class* super = (Class*) sym->get_descriptor();
        current_scope->set_super(super->get_scope());
    }
}

void ScopeDefinitionBuilder::connect_sibling_scopes(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        Source* src = sources->get_source(i);
        Scope* scope = src->get_scope();

        for (int j = 0; j < src->import_count(); ++j) {
            scope->add_sibling(src->get_import(j)->get_source()->get_scope());
        }
    }
}

void ScopeDefinitionBuilder::define_function(Function* function) {
    Symbol* sym = current_scope->local_has(function->get_name());

    define_function_signature(function);

    if (!sym) {
        logger->info(info_message_defining_function(function));
        current_scope->define(function);
    } else if (sym->get_kind() == SYM_FUNCTION) {
        logger->info(info_message_defining_function(function));
        define_overloaded_function(sym, function);
    } else {
        logger->error("can't define function");
    }
}

void ScopeDefinitionBuilder::define_function_signature(Function* function) {
    enter_scope(function->get_scope());

    define_function_template_header(function);
    define_function_parameters(function);
    link_type(function->get_return_type());
    define_function_self_type(function);

    leave_scope();
}

void ScopeDefinitionBuilder::define_function_template_header(Function* function) {
    Symbol* sym;
    TemplateHeader* header = function->get_template_header();

    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            TemplateType* type = (TemplateType*) header->get_type(i);
            sym = current_scope->local_has(type->get_name());

            if (!sym) {
                current_scope->define(type);
                link_type(type->get_bind_type());
            } else {
                logger->error("<red>error: </red>template name already defined");
            }
        }
    }
}

void ScopeDefinitionBuilder::define_function_parameters(Function* function) {
    Variable* param;
    Symbol* sym;

    for (int i = 0; i < function->parameters_count(); ++i) {
        param = function->get_parameter(i);
        param->set_uid(i);
        link_type(param->get_type());

        sym = current_scope->has(param->get_name());

        if (!sym) {
            current_scope->define(SYM_PARAMETER, param);
        } else if (sym->get_kind() != SYM_PARAMETER) {
            current_scope->define(SYM_PARAMETER, param);
        } else {
            logger->error("<red>error: </red>parameter already defined");
        }
    }
}

void ScopeDefinitionBuilder::define_function_self_type(Function* function) {
    FunctionType* ftype = new FunctionType();

    if (function->get_template_header()) {
        TemplateHeader* header = function->get_template_header();

        for (int i = 0; i < header->types_count(); ++i) {
            ftype->add_template(header->get_type(i));
        }
    }

    if (function->parameters_count() > 0) {
        for (int i = 0; i < function->parameters_count(); ++i) {
            ftype->add_param_type(function->get_parameter(i)->get_type());
        }
    } else {
        ftype->add_param_type(new Type(TYPE_VOID));
    }

    ftype->set_return_type(function->get_return_type());
    function->set_self_type(ftype);
    link_type(ftype);
}

void ScopeDefinitionBuilder::define_overloaded_function(Symbol* symbol, Function* function) {
    for (int i = 0; i < symbol->overloaded_count(); ++i) {
        Function* other = (Function*) symbol->get_descriptor(i);

        if (function->same_signature(other)) {
            logger->error("<red>error: </red>function with same signature");
        }
    }

    symbol->add_descriptor(function);
    function->set_overloaded_index(symbol->overloaded_count() - 1);
}

void ScopeDefinitionBuilder::link_type(Type* type) {
    IndirectionType* it = (IndirectionType*) type;

    if (type == nullptr) {
        return;
    }

    switch (type->get_kind()) {
    case TYPE_NAMED:
        link_named_type((NamedType*) type);
        break;

    case TYPE_POINTER:
    case TYPE_REFERENCE:
        link_type(it->get_subtype());
        break;

    case TYPE_FUNCTION:
        link_function_type((FunctionType*) type);
        break;
    }
}

void ScopeDefinitionBuilder::link_named_type(NamedType* type) {
    TemplateHeader* header = type->get_template_header();
    Symbol* sym = current_scope->has(type->get_name());

    if (!sym) {
        logger->error("<red>error: </red> named type not in scope");
    }

    int kind = sym->get_kind();

    if (kind == SYM_CLASS) {
        type->set_symbol(sym);
    } else if (kind == SYM_TEMPLATE) {
        type->set_symbol(sym);
    } else {
        logger->error("error: named type not in scope but is another entity");
    }

    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            link_type(header->get_type(i));
        }
    } 
}

void ScopeDefinitionBuilder::link_function_type(FunctionType* type) {
    link_template_header(type->get_template_header());

    for (int i = 0; i < type->params_count(); ++i) {
        link_type(type->get_param_type(i));
    }

    link_type(type->get_return_type());
}

void ScopeDefinitionBuilder::link_template_header(TemplateHeader* header) {
    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            link_type(header->get_type(i));
        }
    }
}

void ScopeDefinitionBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

// PRIVATE
void ScopeDefinitionBuilder::enter_scope(Scope* scope) {
    scopes.push(current_scope);
    scope->set_parent(current_scope);
    current_scope = scope;
}

void ScopeDefinitionBuilder::leave_scope() {
    current_scope = scopes.top();
    scopes.pop();
}