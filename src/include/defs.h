#ifndef HDC_DEFS_H
#define HDC_DEFS_H

#include <map>

#define NORMAL  "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"   
#define WHITE   "\033[37m"

#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"   
#define BRIGHT_WHITE   "\033[97m"
#define DBG do { std::cout << __FILE__ << ' ' << __LINE__ << std::endl; } while (0);

namespace haard {
    typedef enum LogKind {
        LOG_WARNING,
        LOG_INFO,
        LOG_ERROR
    } LogKind;

    typedef enum TokenKind {
        TK_DEF,
        TK_CLASS,
        TK_VAR,
        TK_CONSTANT,
        TK_STRUCT,
        TK_ENUM,
        TK_UNION,
        TK_THIS,
        TK_IF,
        TK_ELIF,
        TK_ELSE,
        TK_FOR,
        TK_WHILE,
        TK_CONTINUE,
        TK_RETURN,
        TK_GOTO,
        TK_BREAK,
        TK_YIELD,
        TK_FROM,
        TK_AS,
        TK_IMPORT,
        TK_DO,
        TK_IN,
        TK_PASS,
        TK_NEW,
        TK_DELETE,
        TK_CASE,
        TK_DEFAULT,
        TK_WITH,
        TK_SIZEOF,
        TK_AND,
        TK_OR,
        TK_NOT,
        TK_U8,
        TK_U16,
        TK_U32,
        TK_U64,
        TK_I8,
        TK_I16,
        TK_I32,
        TK_I64,
        TK_VOID,
        TK_SYMBOL,
        TK_BOOL,
        TK_UCHAR,
        TK_CHAR,
        TK_USHORT,
        TK_SHORT,
        TK_UINT,
        TK_INT,
        TK_ULONG,
        TK_LONG,
        TK_FLOAT,
        TK_DOUBLE,
        TK_STR,
        TK_ID,
        TK_TRUE,
        TK_FALSE,
        TK_NULL,
        TK_LITERAL_CHAR,
        TK_LITERAL_INTEGER,
        TK_LITERAL_FLOAT,
        TK_LITERAL_DOUBLE,
        TK_LITERAL_STRING,
        TK_LITERAL_SYMBOL,
        TK_PLUS,
        TK_MINUS,
        TK_TIMES,
        TK_DIVISION,
        TK_INTEGER_DIVISION,
        TK_MODULO,
        TK_POWER,
        TK_INC,
        TK_DEC,
        TK_BITWISE_AND,
        TK_BITWISE_NOT,
        TK_BITWISE_OR,
        TK_BITWISE_XOR,
        TK_SLL,
        TK_SRL,
        TK_SRA,
        TK_ARROW,
        TK_DOT,
        TK_DOLAR,
        TK_DOUBLE_DOLAR,
        TK_SCOPE,
        TK_INCLUSIVE_RANGE,
        TK_EXCLUSIVE_RANGE,
        TK_EQ,
        TK_NE,
        TK_GT,
        TK_LT,
        TK_GE,
        TK_LE,
        TK_LOGICAL_AND,
        TK_LOGICAL_OR,
        TK_LOGICAL_NOT,
        TK_TERNARY,
        TK_ASSIGNMENT,
        TK_BITWISE_AND_ASSIGNMENT,
        TK_BITWISE_XOR_ASSIGNMENT,
        TK_BITWISE_OR_ASSIGNMENT,
        TK_BITWISE_NOT_ASSIGNMENT,
        TK_DIVISION_ASSIGNMENT,
        TK_INTEGER_DIVISION_ASSIGNMENT,
        TK_MINUS_ASSIGNMENT,
        TK_MODULO_ASSIGNMENT,
        TK_PLUS_ASSIGNMENT,
        TK_TIMES_ASSIGNMENT,
        TK_SLL_ASSIGNMENT,
        TK_SRA_ASSIGNMENT,
        TK_SRL_ASSIGNMENT,
        TK_SPECIAL_ASSIGNMENT,
        TK_HASH_ARROW,
        TK_NEWLINE,
        TK_EOF,
        TK_AT,
        TK_BEGIN,
        TK_END,
        TK_COLON,
        TK_COMMA,
        TK_SEMICOLON,
        TK_LEFT_CURLY_BRACKET,
        TK_LEFT_PARENTHESIS,
        TK_LEFT_SQUARE_BRACKET,
        TK_RIGHT_CURLY_BRACKET,
        TK_RIGHT_PARENTHESIS,
        TK_RIGHT_SQUARE_BRACKET,
        TK_BEGIN_TEMPLATE,
        TK_END_TEMPLATE,
        TK_UNKNOWN,
    } TokenKind;

    const std::map<int, std::string> token_kind_to_str_map = {
        {TK_DEF, "TK_DEF"},
        {TK_CLASS, "TK_CLASS"},
        {TK_VAR, "TK_VAR"},
        {TK_CONSTANT, "TK_CONSTANT"},
        {TK_STRUCT, "TK_STRUCT"},
        {TK_ENUM, "TK_ENUM"},
        {TK_UNION, "TK_UNION"},
        {TK_THIS, "TK_THIS"},
        {TK_IF, "TK_IF"},
        {TK_ELIF, "TK_ELIF"},
        {TK_ELSE, "TK_ELSE"},
        {TK_FOR, "TK_FOR"},
        {TK_WHILE, "TK_WHILE"},
        {TK_CONTINUE, "TK_CONTINUE"},
        {TK_RETURN, "TK_RETURN"},
        {TK_GOTO, "TK_GOTO"},
        {TK_BREAK, "TK_BREAK"},
        {TK_YIELD, "TK_YIELD"},
        {TK_FROM, "TK_FROM"},
        {TK_AS, "TK_AS"},
        {TK_IMPORT, "TK_IMPORT"},
        {TK_DO, "TK_DO"},
        {TK_IN, "TK_IN"},
        {TK_PASS, "TK_PASS"},
        {TK_NEW, "TK_NEW"},
        {TK_DELETE, "TK_DELETE"},
        {TK_CASE, "TK_CASE"},
        {TK_DEFAULT, "TK_DEFAULT"},
        {TK_WITH, "TK_WITH"},
        {TK_SIZEOF, "TK_SIZEOF"},
        {TK_AND, "TK_AND"},
        {TK_OR, "TK_OR"},
        {TK_NOT, "TK_NOT"},
        {TK_U8, "TK_U8"},
        {TK_U16, "TK_U16"},
        {TK_U32, "TK_U32"},
        {TK_U64, "TK_U64"},
        {TK_I8, "TK_I8"},
        {TK_I16, "TK_I16"},
        {TK_I32, "TK_I32"},
        {TK_I64, "TK_I64"},
        {TK_VOID, "TK_VOID"},
        {TK_SYMBOL, "TK_SYMBOL"},
        {TK_BOOL, "TK_BOOL"},
        {TK_UCHAR, "TK_UCHAR"},
        {TK_CHAR, "TK_CHAR"},
        {TK_USHORT, "TK_USHORT"},
        {TK_SHORT, "TK_SHORT"},
        {TK_UINT, "TK_UINT"},
        {TK_INT, "TK_INT"},
        {TK_ULONG, "TK_ULONG"},
        {TK_LONG, "TK_LONG"},
        {TK_FLOAT, "TK_FLOAT"},
        {TK_DOUBLE, "TK_DOUBLE"},
        {TK_STR, "TK_STR"},
        {TK_ID, "TK_ID"},
        {TK_TRUE, "TK_TRUE"},
        {TK_FALSE, "TK_FALSE"},
        {TK_NULL, "TK_NULL"},
        {TK_LITERAL_CHAR, "TK_LITERAL_CHAR"},
        {TK_LITERAL_INTEGER, "TK_LITERAL_INTEGER"},
        {TK_LITERAL_FLOAT, "TK_LITERAL_FLOAT"},
        {TK_LITERAL_DOUBLE, "TK_LITERAL_DOUBLE"},
        {TK_LITERAL_STRING, "TK_LITERAL_STRING"},
        {TK_LITERAL_SYMBOL, "TK_LITERAL_SYMBOL"},
        {TK_PLUS, "TK_PLUS"},
        {TK_MINUS, "TK_MINUS"},
        {TK_TIMES, "TK_TIMES"},
        {TK_DIVISION, "TK_DIVISION"},
        {TK_INTEGER_DIVISION, "TK_INTEGER_DIVISION"},
        {TK_MODULO, "TK_MODULO"},
        {TK_POWER, "TK_POWER"},
        {TK_INC, "TK_INC"},
        {TK_DEC, "TK_DEC"},
        {TK_BITWISE_AND, "TK_BITWISE_AND"},
        {TK_BITWISE_NOT, "TK_BITWISE_NOT"},
        {TK_BITWISE_OR, "TK_BITWISE_OR"},
        {TK_BITWISE_XOR, "TK_BITWISE_XOR"},
        {TK_SLL, "TK_SLL"},
        {TK_SRL, "TK_SRL"},
        {TK_SRA, "TK_SRA"},
        {TK_ARROW, "TK_ARROW"},
        {TK_DOT, "TK_DOT"},
        {TK_DOLAR, "TK_DOLAR"},
        {TK_DOUBLE_DOLAR, "TK_DOUBLE_DOLAR"},
        {TK_SCOPE, "TK_SCOPE"},
        {TK_INCLUSIVE_RANGE, "TK_INCLUSIVE_RANGE"},
        {TK_EXCLUSIVE_RANGE, "TK_EXCLUSIVE_RANGE"},
        {TK_EQ, "TK_EQ"},
        {TK_NE, "TK_NE"},
        {TK_GT, "TK_GT"},
        {TK_LT, "TK_LT"},
        {TK_GE, "TK_GE"},
        {TK_LE, "TK_LE"},
        {TK_LOGICAL_AND, "TK_LOGICAL_AND"},
        {TK_LOGICAL_OR, "TK_LOGICAL_OR"},
        {TK_LOGICAL_NOT, "TK_LOGICAL_NOT"},
        {TK_TERNARY, "TK_TERNARY"},
        {TK_ASSIGNMENT, "TK_ASSIGNMENT"},
        {TK_BITWISE_AND_ASSIGNMENT, "TK_BITWISE_AND_ASSIGNMENT"},
        {TK_BITWISE_XOR_ASSIGNMENT, "TK_BITWISE_XOR_ASSIGNMENT"},
        {TK_BITWISE_OR_ASSIGNMENT, "TK_BITWISE_OR_ASSIGNMENT"},
        {TK_BITWISE_NOT_ASSIGNMENT, "TK_BITWISE_NOT_ASSIGNMENT"},
        {TK_DIVISION_ASSIGNMENT, "TK_DIVISION_ASSIGNMENT"},
        {TK_INTEGER_DIVISION_ASSIGNMENT, "TK_INTEGER_DIVISION_ASSIGNMENT"},
        {TK_MINUS_ASSIGNMENT, "TK_MINUS_ASSIGNMENT"},
        {TK_MODULO_ASSIGNMENT, "TK_MODULO_ASSIGNMENT"},
        {TK_PLUS_ASSIGNMENT, "TK_PLUS_ASSIGNMENT"},
        {TK_TIMES_ASSIGNMENT, "TK_TIMES_ASSIGNMENT"},
        {TK_SLL_ASSIGNMENT, "TK_SLL_ASSIGNMENT"},
        {TK_SRA_ASSIGNMENT, "TK_SRA_ASSIGNMENT"},
        {TK_SRL_ASSIGNMENT, "TK_SRL_ASSIGNMENT"},
        {TK_SPECIAL_ASSIGNMENT, "TK_SPECIAL_ASSIGNMENT"},
        {TK_HASH_ARROW, "TK_HASH_ARROW"},
        {TK_NEWLINE, "TK_NEWLINE"},
        {TK_EOF, "TK_EOF"},
        {TK_AT, "TK_AT"},
        {TK_BEGIN, "TK_BEGIN"},
        {TK_END, "TK_END"},
        {TK_COLON, "TK_COLON"},
        {TK_COMMA, "TK_COMMA"},
        {TK_SEMICOLON, "TK_SEMICOLON"},
        {TK_LEFT_CURLY_BRACKET, "TK_LEFT_CURLY_BRACKET"},
        {TK_LEFT_PARENTHESIS, "TK_LEFT_PARENTHESIS"},
        {TK_LEFT_SQUARE_BRACKET, "TK_LEFT_SQUARE_BRACKET"},
        {TK_RIGHT_CURLY_BRACKET, "TK_RIGHT_CURLY_BRACKET"},
        {TK_RIGHT_PARENTHESIS, "TK_RIGHT_PARENTHESIS"},
        {TK_RIGHT_SQUARE_BRACKET, "TK_RIGHT_SQUARE_BRACKET"},
        {TK_BEGIN_TEMPLATE, "TK_BEGIN_TEMPLATE"},
        {TK_END_TEMPLATE, "TK_END_TEMPLATE"},
        {TK_UNKNOWN, "TK_UNKNOWN"},
    };

    const std::map<std::string, int> str_to_token_kind_map = {
        {"TK_DEF", TK_DEF},
        {"TK_CLASS", TK_CLASS},
        {"TK_VAR", TK_VAR},
        {"TK_CONSTANT", TK_CONSTANT},
        {"TK_STRUCT", TK_STRUCT},
        {"TK_ENUM", TK_ENUM},
        {"TK_UNION", TK_UNION},
        {"TK_THIS", TK_THIS},
        {"TK_IF", TK_IF},
        {"TK_ELIF", TK_ELIF},
        {"TK_ELSE", TK_ELSE},
        {"TK_FOR", TK_FOR},
        {"TK_WHILE", TK_WHILE},
        {"TK_CONTINUE", TK_CONTINUE},
        {"TK_RETURN", TK_RETURN},
        {"TK_GOTO", TK_GOTO},
        {"TK_BREAK", TK_BREAK},
        {"TK_YIELD", TK_YIELD},
        {"TK_FROM", TK_FROM},
        {"TK_AS", TK_AS},
        {"TK_IMPORT", TK_IMPORT},
        {"TK_DO", TK_DO},
        {"TK_IN", TK_IN},
        {"TK_PASS", TK_PASS},
        {"TK_NEW", TK_NEW},
        {"TK_DELETE", TK_DELETE},
        {"TK_CASE", TK_CASE},
        {"TK_DEFAULT", TK_DEFAULT},
        {"TK_WITH", TK_WITH},
        {"TK_SIZEOF", TK_SIZEOF},
        {"TK_AND", TK_AND},
        {"TK_OR", TK_OR},
        {"TK_NOT", TK_NOT},
        {"TK_U8", TK_U8},
        {"TK_U16", TK_U16},
        {"TK_U32", TK_U32},
        {"TK_U64", TK_U64},
        {"TK_I8", TK_I8},
        {"TK_I16", TK_I16},
        {"TK_I32", TK_I32},
        {"TK_I64", TK_I64},
        {"TK_VOID", TK_VOID},
        {"TK_SYMBOL", TK_SYMBOL},
        {"TK_BOOL", TK_BOOL},
        {"TK_UCHAR", TK_UCHAR},
        {"TK_CHAR", TK_CHAR},
        {"TK_USHORT", TK_USHORT},
        {"TK_SHORT", TK_SHORT},
        {"TK_UINT", TK_UINT},
        {"TK_INT", TK_INT},
        {"TK_ULONG", TK_ULONG},
        {"TK_LONG", TK_LONG},
        {"TK_FLOAT", TK_FLOAT},
        {"TK_DOUBLE", TK_DOUBLE},
        {"TK_STR", TK_STR},
        {"TK_ID", TK_ID},
        {"TK_TRUE", TK_TRUE},
        {"TK_FALSE", TK_FALSE},
        {"TK_NULL", TK_NULL},
        {"TK_LITERAL_CHAR", TK_LITERAL_CHAR},
        {"TK_LITERAL_INTEGER", TK_LITERAL_INTEGER},
        {"TK_LITERAL_FLOAT", TK_LITERAL_FLOAT},
        {"TK_LITERAL_DOUBLE", TK_LITERAL_DOUBLE},
        {"TK_LITERAL_STRING", TK_LITERAL_STRING},
        {"TK_LITERAL_SYMBOL", TK_LITERAL_SYMBOL},
        {"TK_PLUS", TK_PLUS},
        {"TK_MINUS", TK_MINUS},
        {"TK_TIMES", TK_TIMES},
        {"TK_DIVISION", TK_DIVISION},
        {"TK_INTEGER_DIVISION", TK_INTEGER_DIVISION},
        {"TK_MODULO", TK_MODULO},
        {"TK_POWER", TK_POWER},
        {"TK_INC", TK_INC},
        {"TK_DEC", TK_DEC},
        {"TK_BITWISE_AND", TK_BITWISE_AND},
        {"TK_BITWISE_NOT", TK_BITWISE_NOT},
        {"TK_BITWISE_OR", TK_BITWISE_OR},
        {"TK_BITWISE_XOR", TK_BITWISE_XOR},
        {"TK_SLL", TK_SLL},
        {"TK_SRL", TK_SRL},
        {"TK_SRA", TK_SRA},
        {"TK_ARROW", TK_ARROW},
        {"TK_DOT", TK_DOT},
        {"TK_DOLAR", TK_DOLAR},
        {"TK_DOUBLE_DOLAR", TK_DOUBLE_DOLAR},
        {"TK_SCOPE", TK_SCOPE},
        {"TK_INCLUSIVE_RANGE", TK_INCLUSIVE_RANGE},
        {"TK_EXCLUSIVE_RANGE", TK_EXCLUSIVE_RANGE},
        {"TK_EQ", TK_EQ},
        {"TK_NE", TK_NE},
        {"TK_GT", TK_GT},
        {"TK_LT", TK_LT},
        {"TK_GE", TK_GE},
        {"TK_LE", TK_LE},
        {"TK_LOGICAL_AND", TK_LOGICAL_AND},
        {"TK_LOGICAL_OR", TK_LOGICAL_OR},
        {"TK_LOGICAL_NOT", TK_LOGICAL_NOT},
        {"TK_TERNARY", TK_TERNARY},
        {"TK_ASSIGNMENT", TK_ASSIGNMENT},
        {"TK_BITWISE_AND_ASSIGNMENT", TK_BITWISE_AND_ASSIGNMENT},
        {"TK_BITWISE_XOR_ASSIGNMENT", TK_BITWISE_XOR_ASSIGNMENT},
        {"TK_BITWISE_OR_ASSIGNMENT", TK_BITWISE_OR_ASSIGNMENT},
        {"TK_BITWISE_NOT_ASSIGNMENT", TK_BITWISE_NOT_ASSIGNMENT},
        {"TK_DIVISION_ASSIGNMENT", TK_DIVISION_ASSIGNMENT},
        {"TK_INTEGER_DIVISION_ASSIGNMENT", TK_INTEGER_DIVISION_ASSIGNMENT},
        {"TK_MINUS_ASSIGNMENT", TK_MINUS_ASSIGNMENT},
        {"TK_MODULO_ASSIGNMENT", TK_MODULO_ASSIGNMENT},
        {"TK_PLUS_ASSIGNMENT", TK_PLUS_ASSIGNMENT},
        {"TK_TIMES_ASSIGNMENT", TK_TIMES_ASSIGNMENT},
        {"TK_SLL_ASSIGNMENT", TK_SLL_ASSIGNMENT},
        {"TK_SRA_ASSIGNMENT", TK_SRA_ASSIGNMENT},
        {"TK_SRL_ASSIGNMENT", TK_SRL_ASSIGNMENT},
        {"TK_SPECIAL_ASSIGNMENT", TK_SPECIAL_ASSIGNMENT},
        {"TK_HASH_ARROW", TK_HASH_ARROW},
        {"TK_NEWLINE", TK_NEWLINE},
        {"TK_EOF", TK_EOF},
        {"TK_AT", TK_AT},
        {"TK_BEGIN", TK_BEGIN},
        {"TK_END", TK_END},
        {"TK_COLON", TK_COLON},
        {"TK_COMMA", TK_COMMA},
        {"TK_SEMICOLON", TK_SEMICOLON},
        {"TK_LEFT_CURLY_BRACKET", TK_LEFT_CURLY_BRACKET},
        {"TK_LEFT_PARENTHESIS", TK_LEFT_PARENTHESIS},
        {"TK_LEFT_SQUARE_BRACKET", TK_LEFT_SQUARE_BRACKET},
        {"TK_RIGHT_CURLY_BRACKET", TK_RIGHT_CURLY_BRACKET},
        {"TK_RIGHT_PARENTHESIS", TK_RIGHT_PARENTHESIS},
        {"TK_RIGHT_SQUARE_BRACKET", TK_RIGHT_SQUARE_BRACKET},
        {"TK_BEGIN_TEMPLATE", TK_BEGIN_TEMPLATE},
        {"TK_END_TEMPLATE", TK_END_TEMPLATE},
        {"TK_UNKNOWN", TK_UNKNOWN},
    };

    const std::map<std::string, int> lexeme_to_token_kind_map = {
        {"def", TK_DEF},
        {"class", TK_CLASS},
        {"var", TK_VAR},
        {"const", TK_CONSTANT},
        {"struct", TK_STRUCT},
        {"enum", TK_ENUM},
        {"union", TK_UNION},
        {"this", TK_THIS},
        {"if", TK_IF},
        {"elif", TK_ELIF},
        {"else", TK_ELSE},
        {"for", TK_FOR},
        {"while", TK_WHILE},
        {"continue", TK_CONTINUE},
        {"return", TK_RETURN},
        {"goto", TK_GOTO},
        {"break", TK_BREAK},
        {"yield", TK_YIELD},
        {"from", TK_FROM},
        {"as", TK_AS},
        {"import", TK_IMPORT},
        {"do", TK_DO},
        {"in", TK_IN},
        {"pass", TK_PASS},
        {"new", TK_NEW},
        {"delete", TK_DELETE},
        {"case", TK_CASE},
        {"default", TK_DEFAULT},
        {"with", TK_WITH},
        {"sizeof", TK_SIZEOF},
        {"and", TK_AND},
        {"or", TK_OR},
        {"not", TK_NOT},
        {"u8", TK_U8},
        {"u16", TK_U16},
        {"u32", TK_U32},
        {"u64", TK_U64},
        {"i8", TK_I8},
        {"i16", TK_I16},
        {"i32", TK_I32},
        {"i64", TK_I64},
        {"void", TK_VOID},
        {"sym", TK_SYMBOL},
        {"bool", TK_BOOL},
        {"uchar", TK_UCHAR},
        {"char", TK_CHAR},
        {"ushort", TK_USHORT},
        {"short", TK_SHORT},
        {"uint", TK_UINT},
        {"int", TK_INT},
        {"ulong", TK_ULONG},
        {"long", TK_LONG},
        {"float", TK_FLOAT},
        {"double", TK_DOUBLE},
        {"str", TK_STR},
        {"id", TK_ID},
        {"true", TK_TRUE},
        {"false", TK_FALSE},
        {"null", TK_NULL},
        {"@lchar", TK_LITERAL_CHAR},
        {"@lint", TK_LITERAL_INTEGER},
        {"@lfloat", TK_LITERAL_FLOAT},
        {"@ldouble", TK_LITERAL_DOUBLE},
        {"@lstr", TK_LITERAL_STRING},
        {"@lsym", TK_LITERAL_SYMBOL},
        {"+", TK_PLUS},
        {"-", TK_MINUS},
        {"*", TK_TIMES},
        {"/", TK_DIVISION},
        {"//", TK_INTEGER_DIVISION},
        {"%", TK_MODULO},
        {"**", TK_POWER},
        {"++", TK_INC},
        {"--", TK_DEC},
        {"&", TK_BITWISE_AND},
        {"~", TK_BITWISE_NOT},
        {"|", TK_BITWISE_OR},
        {"^", TK_BITWISE_XOR},
        {"<<", TK_SLL},
        {">>>", TK_SRL},
        {">>", TK_SRA},
        {"->", TK_ARROW},
        {".", TK_DOT},
        {"$", TK_DOLAR},
        {"$$", TK_DOUBLE_DOLAR},
        {"::", TK_SCOPE},
        {"..", TK_INCLUSIVE_RANGE},
        {"...", TK_EXCLUSIVE_RANGE},
        {"==", TK_EQ},
        {"!=", TK_NE},
        {">", TK_GT},
        {"<", TK_LT},
        {">=", TK_GE},
        {"<=", TK_LE},
        {"&&", TK_LOGICAL_AND},
        {"||", TK_LOGICAL_OR},
        {"!", TK_LOGICAL_NOT},
        {"?", TK_TERNARY},
        {"=", TK_ASSIGNMENT},
        {"&=", TK_BITWISE_AND_ASSIGNMENT},
        {"^=", TK_BITWISE_XOR_ASSIGNMENT},
        {"|=", TK_BITWISE_OR_ASSIGNMENT},
        {"~=", TK_BITWISE_NOT_ASSIGNMENT},
        {"/=", TK_DIVISION_ASSIGNMENT},
        {"//=", TK_INTEGER_DIVISION_ASSIGNMENT},
        {"-=", TK_MINUS_ASSIGNMENT},
        {"%=", TK_MODULO_ASSIGNMENT},
        {"+=", TK_PLUS_ASSIGNMENT},
        {"*=", TK_TIMES_ASSIGNMENT},
        {"<<=", TK_SLL_ASSIGNMENT},
        {">>=", TK_SRA_ASSIGNMENT},
        {">>>=", TK_SRL_ASSIGNMENT},
        {":=", TK_SPECIAL_ASSIGNMENT},
        {"=>", TK_HASH_ARROW},
        {"@@@n", TK_NEWLINE},
        {"@@@eof", TK_EOF},
        {"@", TK_AT},
        {"@begin", TK_BEGIN},
        {"@end", TK_END},
        {":", TK_COLON},
        {",", TK_COMMA},
        {";", TK_SEMICOLON},
        {"{", TK_LEFT_CURLY_BRACKET},
        {"(", TK_LEFT_PARENTHESIS},
        {"[", TK_LEFT_SQUARE_BRACKET},
        {"}", TK_RIGHT_CURLY_BRACKET},
        {")", TK_RIGHT_PARENTHESIS},
        {"]", TK_RIGHT_SQUARE_BRACKET},
        {"@@@tbegin", TK_BEGIN_TEMPLATE},
        {"@@@tend", TK_END_TEMPLATE},
        {"@@@unknown", TK_UNKNOWN},
    };

    typedef enum TypeKind {
        TYPE_U8,
        TYPE_U16,
        TYPE_U32,
        TYPE_U64,
        TYPE_I8,
        TYPE_I16,
        TYPE_I32,
        TYPE_I64,
        TYPE_VOID,
        TYPE_SYMBOL,
        TYPE_BOOL,
        TYPE_UCHAR,
        TYPE_CHAR,
        TYPE_USHORT,
        TYPE_SHORT,
        TYPE_UINT,
        TYPE_INT,
        TYPE_ULONG,
        TYPE_LONG,
        TYPE_FLOAT,
        TYPE_DOUBLE,
        TYPE_STR,
        TYPE_NAMED,
        TYPE_POINTER,
        TYPE_REFERENCE,
        TYPE_ARRAY,
        TYPE_LIST,
        TYPE_HASH,
        TYPE_FUNCTION,
        TYPE_TUPLE,
        TYPE_PARENTHESIS,
        TYPE_UNION,
        TYPE_TEMPLATE
    } TypeKind;

    typedef enum ExpressionKind {
        EXPR_UNKNOWN,
        EXPR_THIS,
        EXPR_ID,
        EXPR_SCOPE,
        EXPR_GLOBAL_SCOPE,
        EXPR_ASSIGN,
        EXPR_SPECIAL_ASSIGN,
        EXPR_SRL_ASSIGN,
        EXPR_SRA_ASSIGN,
        EXPR_SLL_ASSIGN,
        EXPR_TIMES_ASSIGN,
        EXPR_PLUS_ASSIGN,
        EXPR_MODULO_ASSIGN,
        EXPR_MINUS_ASSIGN,
        EXPR_INTEGER_DIVISIO_ASSIGN,
        EXPR_DIVISION_ASSIGN,
        EXPR_BITWISE_NOT_ASSIGN,
        EXPR_BITWISE_OR_ASSIGN,
        EXPR_BITWISE_XOR_ASSIGN,
        EXPR_BITWISE_AND_ASSIGN,
        EXPR_INCLUSIVE_RANGE,
        EXPR_EXCLUSIVE_RANGE,
        EXPR_LOGICAL_OR,
        EXPR_LOGICAL_OR_OPER,
        EXPR_LOGICAL_AND,
        EXPR_LOGICAL_AND_OPER,
        EXPR_EQ,
        EXPR_NE,
        EXPR_LT,
        EXPR_GT,
        EXPR_LE,
        EXPR_GE,
        EXPR_IN,
        EXPR_NOT_IN,
        EXPR_PLUS,
        EXPR_MINUS,
        EXPR_TIMES,
        EXPR_DIVISION,
        EXPR_INTEGER_DIVISION,
        EXPR_MODULO,
        EXPR_POWER,
        EXPR_BITWISE_OR,
        EXPR_BITWISE_XOR,
        EXPR_BITWISE_AND,
        EXPR_SLL,
        EXPR_SRL,
        EXPR_SRA,
        EXPR_LOGICAL_NOT_OPER,
        EXPR_LOGICAL_NOT,
        EXPR_ADDRESS_OF,
        EXPR_DEREFERENCE,
        EXPR_BITWISE_NOT,
        EXPR_UNARY_MINUS,
        EXPR_UNARY_PLUS,
        EXPR_DOUBLE_DOLAR,
        EXPR_PRE_INC,
        EXPR_PRE_DEC,
        EXPR_POS_INC,
        EXPR_POS_DEC,
        EXPR_SIZEOF,
        EXPR_PARENTHESIS,
        EXPR_CALL,
        EXPR_INDEX,
        EXPR_ARROW,
        EXPR_DOT,
        EXPR_LITERAL_BOOL,
        EXPR_LITERAL_INTEGER,
        EXPR_LITERAL_FLOAT,
        EXPR_LITERAL_DOUBLE,
        EXPR_LITERAL_CHAR,
        EXPR_LITERAL_STRING,
        EXPR_LITERAL_SYMBOL,
        EXPR_LITERAL_NULL,
        EXPR_TUPLE,
        EXPR_ARGS,
        EXPR_LIST,
        EXPR_FOR_INIT,
        EXPR_FOR_INC,
        EXPR_ARRAY,
        EXPR_HASH,
        EXPR_HASH_RAW,
        EXPR_HASH_PAIR,
        EXPR_FUNCTION,
        EXPR_NEW,
        EXPR_DELETE,
        EXPR_DELETE_ARRAY,
        EXPR_TEMPLATE,
        EXPR_CAST
    } ExpressionKind;

    typedef enum StatementKind {
        STMT_UNKNOWN,
        STMT_EXPRESSION,
        STMT_WHILE,
        STMT_FOR,
        STMT_FOREACH,
        STMT_COMPOUND,
        STMT_IF,
        STMT_ELIF,
        STMT_ELSE,
        STMT_RETURN,
        STMT_GOTO,
        STMT_YIELD,
        STMT_CONTINUE,
        STMT_BREAK,
        STMT_VAR_DECL
    } StatementKind;

    typedef enum VariableKind {
        VAR_LOCAL,
        VAR_PARAM,
        VAR_CLASS
    } VariableKind;

    typedef enum SymbolKind {
        SYM_CLASS,
        SYM_FUNCTION,
        SYM_METHOD,
        SYM_VARIABLE,
        SYM_CLASS_VARIABLE,
        SYM_PARAMETER,
        SYM_TEMPLATE
    } SymbolKind;

    typedef enum IRValueKind {
        IR_VALUE_VAR,
        IR_VALUE_TEMP,
        IR_VALUE_LABEL,
        IR_VALUE_LITERAL_BOOL,
        IR_VALUE_LITERAL_INTEGER,
        IR_VALUE_LITERAL_FLOAT,
        IR_VALUE_LITERAL_DOUBLE,
        IR_VALUE_LITERAL_CHAR,
        IR_VALUE_LITERAL_STRING,
        IR_VALUE_LITERAL_SYMBOL,
    } IRValueKind;

    typedef enum IRKind {
        IR_FUNCTION,
        IR_LABEL,
        IR_EQ,
        IR_LT,
        IR_ADD,
        IR_SUB,
        IR_MUL,
        IR_DIV,
        IR_ADDI,
        IR_COPY,
        IR_LI,
        IR_LOAD,
        IR_STORE,
        IR_ALLOCA,
        IR_CALL,
        IR_RETURN,
        IR_BZ,
        IR_GOTO,
    } IRKind;
}

#endif
