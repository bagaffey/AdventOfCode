
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
    string FileName;
    s32 ColumnNumber;
    s32 LineNumber;
    stream* ErrorStream;

    string Input;
    char At[2];

    xbool32 Error;
} tokenizer;

internal xbool32 Parsing(tokenizer* Tokenizer);
internal void ErrorOnToken(tokenizer* Tokenizer, token OnToken, char* Format, ...);
internal void Error(tokenizer* Tokenizer, char* Format, ...);

internal xbool32 IsValidToken(token Token);
internal xbool32 TokenEquals(token Token, char* Match);
internal token GetTokenRaw(tokenizer* Tokenizer);
internal token PeekTokenRaw(tokenizer* Tokenizer);
internal token GetToken(tokenizer* Tokenizer);
internal token PeekToken(tokenizer* Tokenizer);
internal token RequireToken(tokenizer* Tokenizer, token_type DesiredType);
internal token RequiredIdentifier(tokenizer* Tokenizer, token_type DesiredType);
internal token RequireIntegerRange(tokenizer* Tokenizer, s32 MinValue, s32 MaxValue);
internal token OptionalToken(tokenizer* Tokenizer, token_type DesiredType);
internal tokenizer Tokenize(string Input, string FileName);