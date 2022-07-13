#include "token/token.h"
#include "scanner/scanner.h"

using namespace haard;

int main(int argc, char* argv[]) {
    Token token;
    token.set_line(2);
    Scanner s;
    s.read_to_buffer(argv[1]);
    return 0;
}
