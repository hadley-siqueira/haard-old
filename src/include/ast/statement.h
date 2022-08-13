#ifndef HAARD_AST_STATEMENT_H
#define HAARD_AST_STATEMENT_H

namespace haard {
    class Statement {
        public:
            Statement();
            virtual ~Statement();

        public:
            int get_kind();

            void set_kind(int kind);
            virtual Statement* clone();

        protected:
            int kind;
    };
}

#endif
