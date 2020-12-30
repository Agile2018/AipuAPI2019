#ifndef Format_h
#define Format_h
#include <string>
#include <sstream>
#include <cstdarg>
#include <memory>
#include <vector>

using namespace std;

class Format
{
public:
	Format();
	~Format();
	string FormatString(const char* format, ...);
	std::vector<std::string> Split(const std::string& s, char delimiter);
private:

};


#endif // !Format_h

