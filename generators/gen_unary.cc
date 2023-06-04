#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

/* This generator is used for generate unary operators (most of them)
 * It generates the .h and .cc files
 *
 * The reason I decided to go down this path of using generators
 * is to keep a sane mind: organization.
 *
 * First I used just a BinOp class with a constructor receiving the kind
 * but as I programmed, it turned out that was hard to keep track of what
 * operators I have implemented. It was prone error due to copy n paste
 *
 * By having all those classes, it is a lot more verbose, but help me keep track
 * of what was already implemented and what is missing. For instance, g++ can help me
 * by reporting wrong node types on function calls.
 *
 * The theory of compilers are not so hard after you understand them, but implementing 
 * is a pain in the ass because there are many cases to handle. Lot of operators and
 * corner cases. Hard to keep track of all of this.
 *
 * Probably not the best strategy in the world, but you know? It works for me
 * and keep me going. Next reimplementation of the compiler I may come up with
 * a better solution :)
 */

typedef struct ClassInfo {
    std::string name;
    std::string lexeme;
} ClassInfo;

std::string to_upper(std::string s) {
    std::stringstream ss;

    for (int i = 0; i < s.size(); ++i) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            ss << (char) ((s[i] - 'a') + 'A');
        } else {
            ss << (char) s[i];
        }
    }

    return ss.str();
}

std::string to_lower(std::string s) {
    std::stringstream ss;

    for (int i = 0; i < s.size(); ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            ss << (char) ((s[i] - 'A') + 'a');
        } else {
            ss << (char) s[i];
        }
    }

    return ss.str();
}

std::string to_snake_case(std::string s) {
    std::stringstream ss;

    for (int i = 0; i < s.size(); ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            if (i > 0 && s[i - 1] >= 'a' && s[i - 1] <= 'z') {
                ss << '_';
            }

            ss << ((char) ((s[i] - 'A') + 'a'));
        } else {
            ss << (char) s[i];
        }
    }

    return ss.str();
}

void generate_headers(std::vector<ClassInfo>& class_names) {
    for (int i = 0; i < class_names.size(); ++i) {
        std::stringstream ss;
        std::stringstream path;
        std::ofstream f;
        ClassInfo c = class_names[i];

        path << "../src/include/ast/" << to_snake_case(c.name) << ".h";
        f.open(path.str());

        ss << "#ifndef HAARD_AST_" << to_upper(to_snake_case(c.name)) << "_H\n";
        ss << "#define HAARD_AST_" << to_upper(to_snake_case(c.name)) << "_H\n\n";

        ss << "#include \"token/token.h\"\n";
        ss << "#include \"expression.h\"\n\n";

        ss << "namespace haard {\n";
        ss << "    class " << c.name << " : public Expression {\n";
        ss << "    public:\n";
        ss << "        " << c.name << "(Expression* expression=nullptr);\n";
        ss << "        " << c.name << "(Token& token, Expression* expression=nullptr);\n";
        ss << "        ~" << c.name << "();\n\n";

        ss << "    public:\n";
        ss << "        std::string to_str();\n\n";

        ss << "        int get_line() const;\n";
        ss << "        void set_line(int value);\n\n";

        ss << "        int get_column() const;\n";
        ss << "        void set_column(int value);\n\n";
        ss << "
        ss << "        Expression* get_expression() const;\n";
        ss << "        void set_expression(Expression* expression);\n\n";

        ss << "    private:\n";
        ss << "        int line;\n";
        ss << "        int column;\n";
        ss << "        Expression* expression;\n";
        ss << "    };\n";
        ss << "}\n\n";

        ss << "#endif\n";

        f << ss.str();
        f.close();
    }
}

void generate_cc_files(std::vector<ClassInfo>& class_names) {
    for (int i = 0; i < class_names.size(); ++i) {
        std::stringstream ss;
        std::stringstream path;
        std::ofstream f;

        ClassInfo c = class_names[i];

        path << "../src/ast/" << to_snake_case(c.name) << ".cc";
        f.open(path.str());

        ss << "#include <sstream>\n";
        ss << "#include \"ast/" << to_snake_case(c.name) << ".h\"\n\n";

        ss << "using namespace haard;\n\n";

        ss << "" << c.name << "::" << c.name << "(Expression* left, Expression* right) {\n";
        ss << "    this->kind = EXPR_" << to_upper(to_snake_case(c.name)) << ";\n";
        ss << "    this->left = left;\n";
        ss << "    this->right = right;\n";
        ss << "}\n\n";

        ss << "" << c.name << "::" << c.name << "(Token& token, Expression* left, Expression* right) {\n";
        ss << "    this->kind = EXPR_" << to_upper(to_snake_case(c.name)) << ";\n";
        ss << "    this->left = left;\n";
        ss << "    this->right = right;\n";
        ss << "    this->line = token.get_line();\n";
        ss << "    this->column = token.get_column();\n";
        ss << "}\n\n";

        ss << "" << c.name << "::~" << c.name << "() {\n";
        ss << "    delete left;\n";
        ss << "    delete right;\n";
        ss << "}\n\n";

        ss << "Expression* " << c.name << "::get_left() const {\n";
        ss << "    return left;\n";
        ss << "}\n\n";

        ss << "void " << c.name << "::set_left(Expression* value) {\n";
        ss << "    left = value;\n";
        ss << "}\n\n";

        ss << "Expression* " << c.name << "::get_right() const {\n";
        ss << "    return right;\n";
        ss << "}\n\n";

        ss << "void " << c.name << "::set_right(Expression* value) {\n";
        ss << "    right = value;\n";
        ss << "}\n\n";

        ss << "std::string " << c.name << "::to_str() {\n";
        ss << "    std::stringstream ss;\n\n";

        ss << "    ss << left->to_str();\n";
        ss << "    ss << \" " << c.lexeme << " \";\n";
        ss << "    ss << right->to_str();\n\n";

        ss << "    return ss.str();\n";
        ss << "}\n";

        f << ss.str();
        f.close();
    }
}

int main() {
    std::vector<ClassInfo> class_names = {
        {"SpecialAssignment", ":="},
        {"BitwiseAndAssignment", "&="},
        {"BitwiseXorAssignment", "^="},
        {"BitwiseOrAssignment", "|="},
        {"BitwiseNotAssignment", "~="},
        {"TimesAssignment", "*="},
        {"DivisionAssignment", "/="},
        {"IntegerDivisionAssignment", "//="},
        {"MinusAssignment", "-="},
        {"PlusAssignment", "+="},
        {"ShiftLeftLogicalAssignment", "<<="},
        {"ShiftRightLogicalAssignment", ">>>="},
        {"ShiftRightArithmeticAssignment", ">>="},

        {"LogicalOr", "or"},
        {"LogicalOrOper", "or"},
        {"LogicalAnd", "and"},
        {"LogicalAndOper", "and"},


        {"Equal", "=="},
        {"NotEqual", "!="},

        {"LessThan", "<"},
        {"GreaterThan", ">"},
        {"LessThanOrEqual", "<="},
        {"GreaterThanOrEqual", ">="},
        {"In", "in"},
        {"NotIn", "not in"},

        {"InclusiveRange", ".."},
        {"ExclusiveRange", "..."},

        {"Plus", "+"},
        {"Minus", "-"},

        {"Times", "*"},
        {"Division", "*"},
        {"Modulo", "*"},
        {"IntegerDivision", "*"},

        {"Power", "**"},

        {"BitwiseOr", "|"},
        {"BitwiseXor", "^"},
        {"BitwiseAnd", "&"},

        {"ShiftLeftLogical", "<<"},
        {"ShiftRightLogical", ">>>"},
        {"ShiftRightArithmetic", ">>"}
    };

    generate_headers(class_names);
    generate_cc_files(class_names);

    return 0;
};
