#include <fstream>

#include "run/Config.h"
#include "lib/macro.h"

namespace haw
{

using lib::Time;

Config::Config(const std::string& p)
: path_(p), min_(0), max_(0)
{
	std::ifstream in(path_.c_str());

	if(in.is_open())
	{
		uint32_t t;

		in >> min_;
		in >> max_;
		in >> t; slow_ = Time(t);
		in >> t; fast_ = Time(t);
		in >> t; toHM_ = Time(t);
		in >> t; puk_ = Time(t);

		in.close();
	}
}

void Config::save(void) const
{
	std::ofstream out(path_.c_str());

	if(out.is_open())
	{
		out << min_ << "\n";
		out << max_ << "\n";
		out << slow_.raw() << "\n";
		out << fast_.raw() << "\n";
		out << toHM_.raw() << "\n";
		out << puk_.raw() << "\n";

		out.close();
	}
	else
	{
		MXT_TODO_ERROR; //TODO
	}
}

}
