#ifndef __libcfg_parser_hpp__
#define __libcfg_parser_hpp__

#include "libcfg.hpp"

namespace libcfg
{
	// Forward declarations
	class Token;

	class Reader
	{
	public:
		enum Result
		{
			RES_SUCCESS,
			RES_FILE_NOT_FOUND,
			RES_IO_ERROR,
			RES_STACK_ERROR,
			RES_PARSE_FAILED
		};

		Reader();
		virtual ~Reader();

		bool parse(Config* config, const char_t* buffer, size_t length);

		bool is_keyword(const char_t* str);

	protected:
		void skip_spaces();

		bool read_token(Token* token);

		bool read_directive(Token* token);

		bool read_comment(Token* token);

		bool read_name(Token* token);

		bool read_number(Token* token);

		bool read_string(Token* token);

	private:
		Result _result;
		Config* _config;
		const char_t* _begin;
		const char_t* _end;
		const char_t* _current;
	}; // class Reader

} // namespace libcfg

#endif // __libcfg_parser_hpp__
