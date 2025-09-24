
internal string
GetTokenTypeName(token_type Type)
{
	switch (Type)
	{
	case Token_OpenParen: { return(StringZFunc("open parentheses")); }
	case Token_CloseParen: { return(StringZFunc("close parentheses")); }
	case Token_Colon: { return(StringZFunc("colon")); }
	case Token_Semicolon: { return(StringZFunc("semicolon")); }
	case Token_Asterisk: { return(StringZFunc("asterisk")); }
	case Token_OpenBracket: { return(StringZFunc("open bracket")); }
	case Token_CloseBracket: { return(StringZFunc("close bracket")); }
	case Token_OpenBrace: { return(StringZFunc("open brace")); }
	case Token_CloseBrace: { return(StringZFunc("close brace")); }
	case Token_Equals: { return(StringZFunc("equals")); }
	case Token_Comma: { return(StringZFunc("comma")); }
	case Token_Or: { return(StringZFunc("or")); }
	case Token_Pound: { return(StringZFunc("pound")); }
	case Token_String: { return(StringZFunc("string")); }
	case Token_Identifier: { return(StringZFunc("identifier")); }
	case Token_Number: { return(StringZFunc("number")); }
	case Token_Spacing: { return(StringZFunc("whitespace")); }
	case Token_EndOfLine: { return(StringZFunc("end of line")); }
	case Token_Comment: { return(StringZFunc("comment")); }
	case Token_EndOfStream: { return(StringZFunc("end of stream")); }
	case Token_Unknown:
	}

	return (StringZFunc("unknown"));
}

internal xbool32
Parsing(tokenizer* Tokenizer)
{
	xbool32 Result = (!Tokenizer->Error);
	return (Result); 
}

internal void
ErrorArgList(tokenizer* Tokenizer, token OnToken, char* Format, va_list ArgList)
{
	Outf(Tokenizer->ErrorStream, "\\#f00%S(%u,%u)\\#fff: \"%S\" - ", OnToken.FileName, OnToken.LineNumber, OnToken.ColumnNumber, OnToken.Text);
	OutfArgList(DEBUG_MEMORY_NAME("ErrorArgList") Tokenizer->ErrorStream, Format, ArgList);
	Outf(Tokenizer->ErrorStream, "\n");

	Tokenizer->Error = true;
}

internal void
ErrorOnToken(tokenizer* Tokenizer, token OnToken, char* Format, ...)
{
	va_list ArgList;
	va_start(ArgList, Format);

	ErrorArgList(Tokenizer, OnToken, Format, ArgList);

	va_end(ArgList);
}