#include <stdlib.h>
#include <assert.h>

#include "parser.hpp"

#define next_line(s,e)		while((s) != (e) && '\n' != *(s)++) {}
#define find_char(s,e,c)	while((s) != (e) && (c) != *(s)) {++(s);}
#define is_name(c)			(('A' <= (c) && (c) <= 'Z') || ('a' <= (c) && (c) <= 'z') || '_' == (c))
#define is_number(c)		(('0' <= (c) && (c) <= '9') || '+' == (c) || '-' == (c))


namespace libcfg
{
	Parser::Parser()
		: _result(RES_SUCCESS)
		, _config(NULL)
		, _begin(NULL)
		, _end(NULL)
		, _current(NULL)
	{
	}

	Parser::~Parser()
	{
	}

	bool Parser::parse(Config* config, const char_t* buffer, size_t length)
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

		return true;
	}

	void Parser::skip_spaces()
	{
		while (_current != _end)
		{
			char_t c = *_current;
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
				++_current;
			else
				break;
		}
	}

	Parser::Token Parser::read_token()
	{
		skip_spaces();
		Token token = TOK_UNKNOWN;
		char_t c = *_current;

		if ('@' == c) {
			token = TOK_DIRECTIVE;
		}
		else if ('#' == c || '/' == c) {
			token = TOK_COMMENT;
		}
		else if (is_name(c)) {
			token = TOK_NAME;
		}
		else if (is_number(c)) {
			token = TOK_NUMBER;
		}
		else if ('{' == c) {
			token = TOK_GROUP_BEGIN;
		}
		else if ('}' == c) {
			token = TOK_GROUP_END;
		}
		else if ('[' == c) {
			token = TOK_ARRAY_BEGIN;
		}
		else if (']' == c) {
			token = TOK_ARRAY_END;
		}
		else if ('(' == c) {
			token = TOK_LIST_BEGIN;
		}
		else if (')' == c) {
			token = TOK_LIST_END;
		}

		return token;
	}

} // namespace libcfg
