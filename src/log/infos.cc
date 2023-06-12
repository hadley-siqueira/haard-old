#include <sstream>
#include <fstream>
#include "log/infos.h"
#include "log/utils.h"

namespace haard {
    std::string info_define_class(Class* decl) {
        std::stringstream ss;
        int line = decl->get_line();
        int column = decl->get_column();
        int count = decl->get_name().size();
        std::string path = decl->get_module()->get_path();

        ss << "declaring class <white>" << decl->get_name() << "</white>\n";
        ss << extract_line(path, line, column, count);

        return ss.str();
    }
}
