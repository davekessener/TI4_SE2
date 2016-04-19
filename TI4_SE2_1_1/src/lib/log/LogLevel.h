#ifndef HAW_LIB_LOG_LOGLEVEL_H
#define HAW_LIB_LOG_LOGLEVEL_H

namespace lib
{
	namespace log
	{
		class LogLevel
		{
			public:
				static const LogLevel INFO;
				static const LogLevel WARNING;
				static const LogLevel ERROR;
				static const LogLevel CRITICAL;

			public:
				int level( ) const { return level_; }
				const char *label( ) const { return label_; }

				bool operator==(const LogLevel& ll) const { return level_ == ll.level_; }
				bool operator!=(const LogLevel& ll) const { return !(*this == ll); }
				bool operator<(const LogLevel& ll)  const { return level_ < ll.level_; }
				bool operator>(const LogLevel& ll)  const { return !(*this < ll) && !(*this == ll); }
				bool operator<=(const LogLevel& ll) const { return (*this < ll) || (*this == ll); }
				bool operator>=(const LogLevel& ll) const { return !(*this < ll); }

			private:
				LogLevel(const char *s, int l) : label_(s), level_(l) { }

			private:
				const char *label_;
				int level_;
		};
	}
}

#endif

