#include "lib/log/LogLevel.h"

namespace lib { namespace log {

const LogLevel LogLevel::INFO("INFO", 0);
const LogLevel LogLevel::WARNING("WARN", 1);
const LogLevel LogLevel::ERROR("ERR ", 2);
const LogLevel LogLevel::CRITICAL("CRIT", 3);

}}


