
// forward declaration
typedef struct stream stream;

typedef enum token_type
{
    Token_Unknown,

    Token_OpenParen,
    Token_CloseParen,
    Token_Colon,
    Token_Semicolon,
    Token_Asterisk,
    Token_OpenBracket,
    Token_CloseBracket,
    Token_OpenBrace,
    Token_CloseBrace,
    Token_Equals,
    Token_Comma,
    Token_Or,
    Token_Pound,

    Token_String,
    Token_Identifier,
    Token_Number,

    Token_Spacing,
    Token_EndOfLine,
    Token_Comment,

    Token_EndOfStream,
} token_type;

typedef struct token
{
    string FileName;
    s32 ColumnNumber;
    s32 LineNumber;

    token_type Type;
    string Text;
    f32 F32;
    s32 S32;
} token;

typedef struct tokenizer
{

} tokenizer;