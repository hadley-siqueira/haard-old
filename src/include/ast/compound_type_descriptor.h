#ifndef HAARD_AST_COMPOUND_TYPE_DESCRIPTOR_H
#define HAARD_AST_COMPOUND_TYPE_DESCRIPTOR_H

// TypeDeclaration is a base class for struct, data, class and unions

#include <vector>
#include "ast/identifier.h"
#include "ast/template_header.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/annotation.h"
#include "ast/named_type.h"

namespace haard {
    class Scope;
    class Module;
    class Function;

    class CompoundTypeDescriptor {
    public:
        CompoundTypeDescriptor();
        virtual ~CompoundTypeDescriptor();

    public:
        int get_kind() const;
        void set_kind(int value);

        int get_line() const;
        void set_line(int value);

        int get_column() const;
        void set_column(int value);

        int get_size_in_bytes();

        int get_alignment();
        void set_alignment(int value);

        int get_begin() const;
        void set_begin(int value);

        int get_end() const;
        void set_end(int value);

        Module* get_module() const;
        void set_module(Module* value);

        std::vector<Annotation*> get_annotations() const;
        void set_annotations(const std::vector<Annotation*>& value);

        std::string get_name() const;
        std::string get_qualified_name();

        CompoundTypeDescriptor* get_super_descriptor();
        Type* get_super_type() const;
        void set_super_type(Type* value);

        NamedType* get_self_type();
        void set_self_type(NamedType* type);

        Scope* get_scope() const;
        std::string get_full_filepath();
        std::string get_relative_filepath();

        Variable* get_field(int idx);
        Function* get_method(int idx);

        Function* get_destructor();
        void set_destructor(Function* value);

        void set_template(bool value);

        void set_name(const std::string& value);

        void set_from_token(Token& token);

        TemplateHeader* get_template_header() const;
        void set_template_header(TemplateHeader* value, bool is_template=false);

        void set_scope(Scope *value);

        bool is_template();

        int methods_count();
        int constructors_count();
        int fields_count();

        void add_field(Variable* field);
        void add_method(Function* method);

    protected:
        int kind;
        int line;
        int column;
        int begin;
        int end;
        int size_in_bytes;
        int alignment;

        bool template_flag;

        std::string name;
        TemplateHeader* template_header;
        NamedType* self_type;
        Type* super_type;
        Module* module;
        Scope* scope;
        std::vector<Variable*> fields;
        std::vector<Function*> methods;
        std::vector<Function*> constructors;
        Function* destructor;
        std::vector<Annotation*> annotations;
    };
}

#endif
