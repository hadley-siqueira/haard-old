#ifndef HAARD_AST_TYPE_DECLARATION_H
#define HAARD_AST_TYPE_DECLARATION_H

// TypeDeclaration is a base class for struct, data, class and unions

#include "ast/identifier.h"
#include "ast/type_list.h"
#include "ast/function.h"
#include "ast/field.h"
#include "ast/annotation.h"
#include "ast/named_type.h"

namespace haard {
    class Scope;
    class Source;

    class TypeDeclaration {
    public:
        TypeDeclaration();
        virtual ~TypeDeclaration();

    public:
        int get_kind() const;
        int get_line() const;
        int get_column() const;
        int get_size_in_bytes();
        int get_alignment();
        int get_begin() const;
        int get_end() const;
        Source* get_source() const;
        std::vector<Annotation*> get_annotations() const;
        std::string get_name() const;
        std::string get_qualified_name();
        TypeList* get_template_header() const;
        Type* get_super_type() const;
        Scope* get_scope() const;

        void set_kind(int value);
        void set_line(int value);
        void set_column(int value);
        void set_template(bool value);
        void set_annotations(const std::vector<Annotation*>& value);
        void set_name(const std::string& value);
        void set_begin(int value);
        void set_end(int value);
        void set_source(Source* value);
        void set_from_token(Token& token);
        void set_template_header(TypeList* value);
        void set_super_type(Type* value);
        void set_scope(Scope *value);

        bool is_template();

        int methods_count();

        void add_field(Field* field);
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
        TypeList* template_header;
        NamedType* self_type;
        Type* super_type;
        Source* source;
        Scope* scope;
        std::vector<Field*> fields;
        std::vector<Function*> methods;
        std::vector<Function*> constructors;
        Function* destructor;
        std::vector<Annotation*> annotations;
    };
}

#endif
