
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

internal void
Error(tokenizer* Tokenizer, char* Format, ...)
{
	va_list ArgList;
	va_start(ArgList, Format);

	token OnToken = PeekTokenRaw(Tokenizer);
	ErrorOnToken(Tokenizer, OnToken, Format, ArgList);

	va_end(ArgList);
}

internal void
Refill(tokenizer* Tokenizer)
{
	if (Tokenizer->Input.Count == 0)
	{
		Tokenizer->At[0] = 0;
		Tokenizer->At[1] = 0;
	}
	else if (Tokenizer->Input.Count == 1)
	{
		Tokenizer->At[0] = Tokenizer->Input.Data[0];
		Tokenizer->At[1] = 0;
	}
	else
	{
		Tokenizer->At[0] = Tokenizer->Input.Data[0];
		Tokenizer->At[1] = Tokenizer->Input.Data[1];
	}
}

internal void
AdvanceChars(tokenizer* Tokenizer, u32 Count)
{
	Tokenizer->ColumnNumber += Count;
	Advance(&Tokenizer->Input, Count);
	Refill(Tokenizer);
}

internal xbool32
TokenEquals(token Token, char* Match)
{
	xbool32 Result = AreStringCStringEqual(Token.Text, Match);
	return (Result);
}

internal xbool32
IsValidToken(token Token)
{
	xbool32 Result = (Token.Type != Token_Unknown);
	return (Result);
}

internal token
GetTokenRaw(tokenizer* Tokenizer)
{
	token Token = {};
	Token.FileName = Tokenizer->FileName;
	Token.ColumnNumber = Tokenizer->ColumnNumber;
	Token.LineNumber = Tokenizer->LineNumber;
	Token.Text = Tokenizer->Input;

	char C = Tokenizer->At[0];
	AdvanceChars(Tokenizer, 1);
	switch (C)
	{
	case '\0': { Token.Type = Token_EndOfStream; }
			 break;
	case '(': { Token.Type = Token_OpenParen; }
			break;
	case ')': { Token.Type = Token_CloseParen; }
			break;
	case ':': { Token.Type = Token_Colon; }
			break;
	case ';': { Token.Type = Token_Semicolon; }
			break;
	case '*': { Token.Type = Token_Asterisk; }
			break;
	case '[': { Token.Type = Token_OpenBracket; }
			break;
	case ']': { Token.Type = Token_CloseBracket; }
			break;
	case '{': { Token.Type = Token_OpenBrace; }
			break;
	case '}': { Token.Type = Token_CloseBrace; }
			break;
	case '=': { Token.Type = Token_Equals; }
			break;
	case ',': { Token.Type = Token_Comma; }
			break;
	case '|': { Token.Type = Token_Or; }
			break;
	case '#': { Token.Type = Token_Pound; }
			break;

	case '"':
	{
		Token.Type = Token_String;

		while (Tokenizer->At[0] &&
			Tokenizer->At[0] != '"')
		{
			if ((Tokenizer->At[0] == '\\') &&
				Tokenizer->At[1])
				AdvanceChars(Tokenizer, 1);
			AdvanceChars(Tokenizer, 1);
		}

		if (Tokenizer->At[0] == '"')
			AdvanceChars(Tokenizer, 1);
	} break;

	default:
	{
		if (IsSpacing(C))
		{
			Token.Type = Token_Spacing;
			while (IsSpacing(Tokenizer->At[0]))
				AdvanceChars(Tokenizer, 1);
		}
		else if (IsEndOfLine(C))
		{
			Token.Type = Token_EndOfLine;
			if (((C == '\r') &&
				(Tokenizer->At[0] == '\n')) ||
				((C == '\n') &&
					(Tokenizer->At[0] == '\r')))
			{
				AdvanceChars(Tokenizer, 1);
			}

			Tokenizer->ColumnNumber = 1;
			++Tokenizer->LineNumber;
		}
		else if ((C == '/') &&
			(Tokenizer->At[0] == '/'))
		{
			Token.Type = Token_Comment;

			AdvanceChars(Tokenizer, 2);
			while (Tokenizer->At[0] && !IsEndOfLine(Tokenizer->At[0]))
				AdvanceChars(Tokenizer, 1);
		}
		else if ((C == '/') &&
			(Tokenizer->At[0] == '*'))
		{
			Token.Type = Token_Comment;

			AdvanceChars(Tokenizer, 2);
			while (Tokenizer->At[0] &&
				!((Tokenizer->At[0] == '*') &&
					(Tokenizer->At[1] == '/')))
			{
				if (((Tokenizer->At[0] == '\r') &&
					(Tokenizer->At[1] == '\n')) ||
					((Tokenizer->At[0] == '\n') &&
						(Tokenizer->At[1] == '\r')))
				{
					AdvanceChars(Tokenizer, 1);
				}

				if (IsEndOfLine(Tokenizer->At[0]))
					++Tokenizer->LineNumber;

				AdvanceChars(Tokenizer, 1);
			}

			if (Tokenizer->At[0] == '*')
				AdvanceChars(Tokenizer, 2);
		}
		else if (IsAlpha(C))
		{
			Token.Type = Token_Identifier;

			while (IsAlpha(Tokenizer->At[0]) ||
				IsNumber(Tokenizer->At[0]) ||
				(Tokenizer->At[0] == '_'))
			{
				AdvanceChars(Tokenizer, 1);
			}
		}
		else if (IsNumber(C))
		{
			f32 Number = (f32)(C - '0');

			while (IsNumber(Tokenizer->At[0]))
			{
				f32 Digit = (f32)(Tokenizer->At[0] - '0');
				Number = 10.0f * Number + Digit;
				AdvanceChars(Tokenizer, 1);
			}

			if (Tokenizer->At[0] == '.')
			{
				AdvanceChars(Tokenizer, 1);
				f32 Coefficient = 0.1f;
				while (IsNumber(Tokenizer->At[0]))
				{
					f32 Digit = (f32)(Tokenizer->At[0] - '0');
					Number += Coefficient * Digit;
					Coefficient *= 0.1f;
					AdvanceChars(Tokenizer, 1);
				}
			}

			Token.Type = Token_Number;
			Token.F32 = Number;
			Token.S32 = RoundReal32ToInt32(Number);
		}
		else
			Token.Type = Token_Unknown;
	} break;

	}

	Token.Text.Count = (Tokenizer->Input.Data - Token.Text.Data);

	return (Token);
}

internal token
PeekTokenRaw(tokenizer* Tokenizer)
{
	tokenizer Temp = *Tokenizer;
	token Result = GetTokenRaw(Tokenizer);
	*Tokenizer = Temp;
	return (Result);
}

internal token
GetToken(tokenizer* Tokenizer)
{
	token Token;
	for (;;)
	{
		Token = GetTokenRaw(Tokenizer);
		if ((Token.Type == Token_Spacing) ||
			(Token.Type == Token_EndOfLine) ||
			(Token.Type == Token_Comment))
		{
			// Do nothing
		}
		else
		{
			if (Token.Type == Token_String)
			{
				if (Token.Text.Count &&
					(Token.Text.Data[0] == '"'))
				{
					++Token.Text.Data;
					--Token.Text.Count;
				}

				if (Token.Text.Count &&
					(Token.Text.Data[Token.Text.Count - 1] == '"'))
					--Token.Text.Count;
			}
			break;
		}
	}

	return (Token);
}

internal token
PeekToken(tokenizer* Tokenizer)
{
	tokenizer Temp = *Tokenizer;
	token Result = GetToken(&Temp);

	return (Result);
}

internal token
RequireToken(tokenizer* Tokenizer, token_type DesiredType)
{
	token Token = GetToken(Tokenizer);
	if (Token.Type != DesiredType)
	{
		ErrorOnToken(Tokenizer, Token, "Unexpected token type (expected %S)",
			GetTokenTypeName(DesiredType));
	}

	return (Token);
}

internal token
RequireIdentifier(tokenizer* Tokenizer, char* Match)
{
	token ID = RequireToken(Tokenizer, Token_Identifier);
	if (!TokenEquals(ID, Match))
		ErrorOnToken(Tokenizer, ID, "Expected \"%s\"", Match);

	return (ID);
}

internal token
RequireIntegerRange(tokenizer* Tokenizer, s32 MinValue, s32 MaxValue)
{
	token Token = RequireToken(Tokenizer, Token_Number);
	if (Token.Type == Token_Number)
	{
		if ((Token.S32 >= MinValue) &&
			(Token.S32 <= MaxValue))
		{
			// Invalid
		}
		else
			ErrorOnToken(Tokenizer, Token, "Expected a number between %d and %d",
				MinValue,
				MaxValue);
	}

	return (Token);
}

internal xbool32
OptionalToken(tokenizer *Tokenizer, token_type DesiredType)
{
	token Token = PeekToken(Tokenizer);
	xbool32 Result = (Token.Type == DesiredType);

	if (Result)
		GetToken(Tokenizer);

	return (Result);
}