#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.hpp"
#include "strbuf.h"

#define next_line(s,e)		while((s) != (e) && '\n' != *(s)++) {}
#define find_char(s,e,c)	while((s) != (e) && (c) != *(s)) {++(s);}
#define is_space(c)			isspace(c)
#define is_name(c)			(isalpha(c) || '_' == (c))
#define is_sign(c)			('+' == (c) || '-' == (c))
#define is_digit(c)			isdigit(c)
#define is_hex(c)			isxdigit(c)
#define is_octal(c)			('0' <= (c) && (c) <= '7')


namespace libcfg
{
	class Token
	{
	public:
		enum Type
		{
			TOK_DIRECTIVE,
			TOK_COMMENT,
			TOK_EQUALS, // '='
			TOK_COLON, // ':'
			TOK_COMMA, // ','
			TOK_POINT, // '.'
			TOK_SEMICOLON, // ';'
			TOK_NAME, // Include variable and keyword
			TOK_NUMBER, // Include integer and float
			TOK_STRING,
			TOK_GROUP_BEGIN,
			TOK_GROUP_END,
			TOK_ARRAY_BEGIN,
			TOK_ARRAY_END,
			TOK_LIST_BEGIN,
			TOK_LIST_END,
			TOK_STREAM_END,
			TOK_UNKNOWN
		};

		Type _type;
		const char_t* _begin;
		const char_t* _end;
		Token* _next;

		Token();
		virtual ~Token();
	}; // class Token

	Token::Token()
		: _type(TOK_UNKNOWN)
		, _begin(NULL)
		, _end(NULL)
		, _next(NULL)
	{
	}

	Token::~Token()
	{
		if (_next)
			delete[] _next;
	}


	Reader::Reader()
		: _result(RES_SUCCESS)
		, _config(NULL)
		, _begin(NULL)
		, _end(NULL)
		, _current(NULL)
	{
	}

	Reader::~Reader()
	{
	}

	bool Reader::parse(Config* config, const char_t* buffer, size_t length)
	{
		assert(config && buffer && length);
		_config = config;
		_begin = _end = _current = buffer;

		// Find real end
		for (size_t i = 0; i < length; ++i)
		{
			if (*_end != '\0')
				++_end;
		}

		Token root;
		Token* token = &root;
		while (read_token(token))
		{
			if (Token::TOK_STREAM_END == token->_type)
				break;

			token->_next = new Token;
			token = token->_next;
		}

		if (Token::TOK_STREAM_END != token->_type)
		{
			_result = RES_PARSE_FAILED;
			_current = token->_begin;
			return false;
		}

		return true;
	}

	bool Reader::is_keyword(const char_t* str)
	{
		assert(str);
		return strcompare_nocase(str, LIBCFG_TEXT("null")) == 0
			|| strcompare_nocase(str, LIBCFG_TEXT("true")) == 0
			|| strcompare_nocase(str, LIBCFG_TEXT("false")) == 0;
	}

	void Reader::skip_spaces()
	{
		while (_current != _end)
		{
			char_t c = *_current;
			if (is_space(c))
				++_current;
			else
				break;
		}
	}

	bool Reader::read_token(Token* token)
	{
		assert(token);
		skip_spaces();
		token->_begin = _current;
		if (token->_begin == _end)
		{
			token->_type = Token::TOK_STREAM_END;
			token->_end = _end;
			return true;
		}

		bool ok = true;
		char_t c = *(token->_begin);

		if ('@' == c)
		{
			token->_type = Token::TOK_DIRECTIVE;
			ok = read_directive(token);
		}
		else if ('#' == c || '/' == c)
		{
			token->_type = Token::TOK_COMMENT;
			ok = read_comment(token);
		}
		else if ('=' == c)
		{
			token->_type = Token::TOK_EQUALS;
			_current = token->_end = (token->_begin + 1);
		}
		else if (':' == c)
		{
			token->_type = Token::TOK_COLON;
			_current = token->_end = (token->_begin + 1);
		}
		else if (',' == c)
		{
			token->_type = Token::TOK_COMMA;
			_current = token->_end = (token->_begin + 1);
		}
		else if ('.' == c)
		{
			token->_type = Token::TOK_POINT;
			_current = token->_end = (token->_begin + 1);
		}
		else if (';' == c)
		{
			token->_type = Token::TOK_SEMICOLON;
			_current = token->_end = (token->_begin + 1);
		}
		else if (is_name(c))
		{
			token->_type = Token::TOK_NAME;
			ok = read_name(token);
		}
		else if (is_digit(c) || is_sign(c))
		{
			token->_type = Token::TOK_NUMBER;
			ok = read_number(token);
		}
		else if ('"' == c)
		{
			token->_type = Token::TOK_STRING;
			ok = read_string(token);
		}
		else if ('{' == c)
		{
			token->_type = Token::TOK_GROUP_BEGIN;
			_current = token->_end = (token->_begin + 1);
		}
		else if ('}' == c)
		{
			token->_type = Token::TOK_GROUP_END;
			_current = token->_end = (token->_begin + 1);
		}
		else if ('[' == c)
		{
			token->_type = Token::TOK_ARRAY_BEGIN;
			_current = token->_end = (token->_begin + 1);
		}
		else if (']' == c)
		{
			token->_type = Token::TOK_ARRAY_END;
			_current = token->_end = (token->_begin + 1);
		}
		else if ('(' == c)
		{
			token->_type = Token::TOK_LIST_BEGIN;
			_current = token->_end = (token->_begin + 1);
		}
		else if (')' == c)
		{
			token->_type = Token::TOK_LIST_END;
			_current = token->_end = (token->_begin + 1);
		}
		else
		{
			token->_type = Token::TOK_UNKNOWN;
			ok = false;
		}

		return ok;
	}

	bool Reader::read_directive(Token* token)
	{
		const char_t* p = token->_begin + 1;
		next_line(p, _end);
		_current = token->_end = p;
		return true;
	}

	bool Reader::read_comment(Token* token)
	{
		bool ok = false;
		const char_t* p = token->_begin;

		if ('#' == p[0]) // script-style
		{
			++p;
			next_line(p, _end);
			ok = true;
		}
		else if ('/' == p[0] && '/' == p[1]) // cpp-style
		{
			p += 2;
			next_line(p, _end);
			ok = true;
		}
		else if ('/' == p[0] && '*' == p[1]) // c-style
		{
			for (p += 2; p != _end; ++p)
			{
				if ('*' == p[0] && '/' == p[1])
					break;
			}
			ok = (_end - p > 1);
		}

		if (ok) {
			_current = token->_end = p;
		}

		return ok;
	}

	bool Reader::read_name(Token* token)
	{
		const char_t* p = token->_begin;
		for (++p; p != _end; ++p)
		{
			char_t c = *p;
			if (!is_name(c) || !is_digit(c))
				break;
		}
		_current = token->_end = p;
		return true;
	}

	bool Reader::read_number(Token* token)
	{
		//TODO
		return true;
	}

	bool Reader::read_string(Token* token)
	{
		//TODO
		return true;
	}

} // namespace libcfg
