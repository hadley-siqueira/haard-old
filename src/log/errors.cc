#include <sstream>
#include <fstream>
#include "log/errors.h"
#include "log/utils.h"

namespace haard {
    std::string error_unexpected_token(std::string path, Token& token) {
        std::stringstream ss;
        int line = token.get_line();
        int column = token.get_column();
        int count = token.get_lexeme().size();

        ss << "<red>error</red>: unexpected token\n";
        ss << extract_line(path, line, column, count);

        return ss.str();
    }

    std::string error_expected_elif_expression(std::string path, Elif* stmt) {
        std::stringstream ss;
        int line = stmt->get_line();
        int column = stmt->get_column();
        int count = 4;

        ss << "<red>error</red>: elif should have a condition expression\n";
        ss << extract_line(path, line, column, count);

        return ss.str();
    }

    std::string error_expected_while_expression(std::string path, WhileStatement* stmt) {
        std::stringstream ss;
        int line = stmt->get_line();
        int column = stmt->get_column();
        int count = 5;

        ss << "<red>error</red>: while should have a condition expression\n";
        ss << extract_line(path, line, column, count);

        return ss.str();
    }

    std::string error_redefinition(Declaration* decl, Declaration* org) {
        std::stringstream ss;
        int line = decl->get_line();
        int column = decl->get_column();
        int count = decl->get_name().size();
        std::string path = decl->get_module()->get_path();

        ss << "<white>" << decl->get_name() << "</white> already declared\n";
        ss << extract_line(path, line, column, count);

        ss << "\nother occurence here:\n";

        line = org->get_line();
        column = org->get_column();
        count = org->get_name().size();
        path = org->get_module()->get_path();
        ss << extract_line(path, line, column, count);

        return ss.str();
    }
}
