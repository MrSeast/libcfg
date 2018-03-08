
#include "compat.h"
#include "strbuf.h"
#include "libcfg.hpp"
#include "parser.hpp"


namespace libcfg
{
	Comment::Comment(CommentStyle style)
		: _style(style)
		, _text(NULL)
		, _next(NULL)
	{
	}

	Comment::~Comment()
	{
		if (_text)
			strbuf_free(_text);

		if (_next)
			delete[] _next;
	}

	Setting::Setting()
		: _type(TYPE_NULL)
		, _name(NULL)
		, _parent(NULL)
		, _next(NULL)
		, _comments(NULL)
	{
		memset(&_value, 0, sizeof(_value));
	}

	Setting::~Setting()
	{
		if (_name)
			strbuf_free(_name);

		if (_next)
			delete[] _next;

		if (_comments)
			delete[] _comments;

		if (_type == TYPE_STRING)
		{
			if (_value.sval)
				strbuf_free(_value.sval);
		}
		else if (_type == TYPE_GROUP
			  || _type == TYPE_ARRAY
			  || _type == TYPE_LIST)
		{
			if (_value.pval)
				delete[] _value.pval;
		}
	}

	Config::Config()
		: _options(OPT_DEFAULT)
	{
		_type = TYPE_GROUP;
	}

	Config::~Config()
	{
	}

	void Config::load_string(const char_t* contents)
	{
		Parser p;
		p.parse(this, contents, strlength(contents));
	}

	bool Config::is_collect_comment()
	{
		return (_options & OPT_COLLECT_COMMENT) != 0;
	}

} // namespace libcfg
