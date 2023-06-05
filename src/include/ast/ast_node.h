#ifndef HAARD_AST_NODE_H
#define HAARD_AST_NODE_H

namespace haard {
    class AstNode {
    public:
        AstNode();
        virtual ~AstNode();

    public:
        int get_kind() const;
        void set_kind(int value);

        int get_line() const;
        void set_line(int value);

        int get_column() const;
        void set_column(int value);

    private:
        int kind;
        int line;
        int column;
    };
}

#endif
