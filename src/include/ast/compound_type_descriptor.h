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
#include "ast/declaration.h"

namespace haard {
    class Scope;
    class Module;
    class Function;

    class CompoundTypeDescriptor : public Declaration {
    public:
        CompoundTypeDescriptor();
        virtual ~CompoundTypeDescriptor();

    public:
        virtual int get_size_in_bytes();

        int get_alignment();
        void set_alignment(int value);

        std::vector<Annotation*> get_annotations() const;
        void set_annotations(const std::vector<Annotation*>& value);

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

        void set_from_token(Token& token);

        void set_scope(Scope *value);

        bool is_template();

        int methods_count();
        int constructors_count();
        int fields_count();

        void add_field(Variable* field);
        void add_method(Function* method);

    protected:
        int size_in_bytes;
        int alignment;

        bool template_flag;
        NamedType* self_type;
        Type* super_type;
        Scope* scope;
        std::vector<Variable*> fields;
        std::vector<Function*> methods;
        std::vector<Function*> constructors;
        Function* destructor;
        std::vector<Annotation*> annotations;
    };
}

#endif
