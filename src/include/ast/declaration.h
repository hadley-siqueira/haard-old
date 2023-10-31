#ifndef HAARD_AST_DECLARATION_H
#define HAARD_AST_DECLARATION_H

#include <string>

namespace haard {
    class Module;

    class Declaration {
    public:
        Declaration();
        virtual ~Declaration();
    public:
        int get_kind() const;
        void set_kind(int newKind);

        int get_line() const;
        void set_line(int newLine);

        int get_column() const;
        void set_column(int newColumn);

        const std::string& get_name() const;
        void set_name(const std::string& newName);

        Module* get_module() const;
        void set_module(Module* newModule);

    private:
        int kind;
        int line;
        int column;
        std::string name;
        Module* module;
    };
}
#endif
