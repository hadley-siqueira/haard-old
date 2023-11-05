#include <sstream>
#include <fstream>
#include "log/infos.h"
#include "log/utils.h"

namespace haard {
    std::string info_define_user_type(CompoundTypeDescriptor* decl) {
        std::stringstream ss;
        int line = decl->get_line();
        int column = decl->get_column();
        int count = decl->get_name().size();
        std::string path = decl->get_module()->get_path();
        std::string kind;

        switch (decl->get_kind()) {
        case DECL_CLASS:
            kind = "class";
            break;

        case DECL_ENUM:
            kind = "enum";
            break;

        case DECL_STRUCT:
            kind = "struct";
            break;

        case DECL_UNION:
            kind = "union";
            break;
        }

        ss << "declaring " << kind << " <white>" << decl->get_qualified_name() << "</white>\n";
        ss << extract_line(path, line, column, count);

        return ss.str();
    }

    std::string info_define_function(Function* function) {
        std::stringstream ss;
        int line = function->get_line();
        int column = function->get_column();
        int count = function->get_name().size();
        std::string path = function->get_module()->get_path();

        ss << "declaring function <white>" << function->get_qualified_name() << "</white>\n";
        ss << extract_line(path, line, column, count);

        return ss.str();
    }

    std::string info_define_method(Function* method) {
        std::stringstream ss;
        int line = method->get_line();
        int column = method->get_column();
        int count = method->get_name().size();
        std::string path = method->get_module()->get_path();

        ss << "declaring method <white>" << method->get_qualified_name() << "</white>\n";
        ss << extract_line(path, line, column, count);

        return ss.str();
    }
}
