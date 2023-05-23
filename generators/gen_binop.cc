#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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

int main() {
    std::vector<std::string> class_names = {
        "Plus", "Minus", 
        "Times", "Division", "Modulo",
        "ShiftLeftLogical", "ShiftRightLogical", "ShiftRightArithmetic"
    };

    for (int i = 0; i < class_names.size(); ++i) {
        std::stringstream ss;

        ss << to_snake_case(class_names[i]) << "\n";
        ss << "#ifndef HAARD_AST_" << to_upper(to_snake_case(class_names[i])) << "_H\n";
        ss << "#define HAARD_AST_" << to_upper(to_snake_case(class_names[i])) << "_H\n\n";

        ss << "#include \"token.h\"\n";
        ss << "#include \"expression.h\"\n\n";

        ss << "namespace haard {\n";
        ss << "    class " << class_names[i] << " : public Expression {\n";
        ss << "    public:\n";
        ss << "        " << class_names[i] << "(Token& token, Expression* left=nullptr, Expression* right=nullptr);\n";
        ss << "        ~" << class_names[i] << "();\n\n";
        ss << "    public:\n";
        ss << "        std::string to_str();\n";
        ss << "        Expression* get_left();\n";
        ss << "        void set_left(Expression* value);\n\n";
        ss << "        Expression* get_right();\n";
        ss << "        void set_right(Expression* value);\n\n";
        ss << "    private:\n";
        ss << "        int line;\n";
        ss << "        int column;\n";
        ss << "        Expression* left;\n";
        ss << "        Expression* right;\n";
        ss << "    };\n";
        ss << "}\n\n";
        ss << "#endif\n";
        std::cout << ss.str() << std::endl;
    }

    return 0;
};
