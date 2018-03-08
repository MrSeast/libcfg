#ifndef __libcfg_parser_hpp__
#define __libcfg_parser_hpp__

#include "libcfg.hpp"

namespace libcfg
{
	class Parser
	{
	public:
		enum Token
		{
			TOK_UNKNOWN,
			TOK_DIRECTIVE,
			TOK_COMMENT,
			TOK_NAME,
			TOK_NUMBER, // Include integer and float
			TOK_GROUP_BEGIN,
			TOK_GROUP_END,
			TOK_ARRAY_BEGIN,
			TOK_ARRAY_END,
			TOK_LIST_BEGIN,
			TOK_LIST_END
		};

		enum Result
		{
			RES_SUCCESS,
			RES_FILE_NOT_FOUND,
			RES_IO_ERROR,
			RES_STACK_ERROR,
			RES_PARSE_FAILED
		};

		Parser();
		virtual ~Parser();

		bool parse(Config* config, const char_t* buffer, size_t length);

		bool is_keyword(const char_t* str);

	protected:
		void skip_spaces();

		Token read_token();

	private:
		Result _result;
		Config* _config;
		const char_t* _begin;
		const char_t* _end;
		const char_t* _current;
	}; // class Parser

} // namespace libcfg

#endif // __libcfg_parser_hpp__
