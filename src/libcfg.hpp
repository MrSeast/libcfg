#ifndef __libcfg_hpp__
#define __libcfg_hpp__

#define LIBCFGXX_VER_MAJOR	0
#define LIBCFGXX_VER_MINOR	1
#define LIBCFGXX_VER_PATCH	0

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#if defined(LIBCFG_STATIC)
#define LIBCFGXX_API
#elif defined(LIBCFG_EXPORTS)
#define LIBCFGXX_API __declspec(dllexport)
#else // !LIBCFG_EXPORTS
#define LIBCFGXX_API __declspec(dllimport)
#endif // LIBCFG_STATIC
#else // !WIN32
#define LIBCFGXX_API
#endif // WIN32

// Forward declarations
struct strbuf_t;

namespace libcfg
{
	// Character type used for all internal storage and operations
#if LIBCFG_WCHAR_MODE
#	define LIBCFG_TEXT(T)	L ## T
	typedef wchar_t			char_t;
#else
#	define LIBCFG_TEXT(T)	T
	typedef char			char_t;
#endif

	class LIBCFGXX_API Comment
	{
	public:
		enum CommentStyle
		{
			STYLE_C,
			STYLE_CPP,
			STYLE_SCRIPT
		};

	private:
		// Non-copyable semantics
		Comment();
		Comment(const Comment&);
		Comment& operator=(const Comment&);

	protected:
		CommentStyle _style;
		strbuf_t* _text;
		Comment* _next;

	public:
		// Default constructor/destructor
		explicit Comment(CommentStyle style);
		virtual ~Comment();
	}; // class Comment

	class LIBCFGXX_API Setting
	{
	public:
		enum Type
		{
			TYPE_NULL,
			// scalar types
			TYPE_BOOL,
			TYPE_INT,
			TYPE_INT64,
			TYPE_FLOAT,
			TYPE_STRING,
			// aggregate types
			TYPE_GROUP,
			TYPE_ARRAY,
			TYPE_LIST,
			// special types
			TYPE_REFERENCE
		};

	private:
		// Non-copyable semantics
		Setting(const Setting&);
		Setting& operator=(const Setting&);

	protected:
		Type _type;
		strbuf_t* _name;
		union {
			bool bval;
			int ival;
			long long i64val;
			double fval;
			strbuf_t* sval;
			Setting* pval;
		} _value;
		Setting* _parent;
		Setting* _next;
		Comment* _comments;

	public:
		// Default constructor/destructor
		Setting();
		virtual ~Setting();
	}; // class Setting

	class LIBCFGXX_API Config : public Setting
	{
	public:
		enum Option
		{
			OPT_NONE				= 0,
			OPT_STRICT_MODE			= 1 << 0,
			OPT_CASE_SENSITIVE		= 1 << 1,
			OPT_AUTO_CONVERT		= 1 << 2,
			OPT_COLLECT_COMMENT		= 1 << 3,
			OPT_DEFAULT				= OPT_AUTO_CONVERT|OPT_COLLECT_COMMENT
		};

	private:
		// Non-copyable semantics
		Config(const Config&);
		Config& operator=(const Config&);

	protected:
		Option _options;

	public:
		// Default constructor/destructor
		Config();
		virtual ~Config();

		// Load document from zero-terminated string
		void load_string(const char_t* contents);

		bool is_strict_mode();
		bool is_case_sensitive();
		bool is_collect_comment();
	}; // class Config

} // namespace libcfg

#endif // __libcfg_hpp__
