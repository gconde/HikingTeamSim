#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#define BUFSIZE 4096
#include <cstdint>
//This nonsense is needed b/c Winsock 1 would be included causing macro redefs.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN
#include "Windows.h" //pid
typedef DWORD pid_type;
#ifdef _UNICODE
#define STR_TYPE	std::wstring
#else
#define STR_TYPE	std::string
#endif
#else
#include <unistd.h> //pid
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif //__STDC_LIMIT_MACROS
#include <stdint.h>
#include <linux/limits.h> //for PATH_MAX
typedef pid_t pid_type;
#define STR_TYPE	std::string
#endif

#ifndef MARKER
#define stringize(x) #x
#define stringer(x) stringize(x)
#define MARKER "File: " stringer(__FILE__) ", Line: " stringer(__LINE__)
#endif

class Util {
private:
	Util();
	Util(Util& util);

public:

	enum eEmpties {
		empties_ok,
		no_empties,
	};

	static void split(std::vector<std::string>& result, const std::string& s, char delimiter,
	eEmpties empty = empties_ok );

	static bool is_number(const std::string& s);

	static uint16_t ToUInt16(std::string integer);

	static uint64_t ToUInt64(std::string integer);

	static int ToInt(std::string integer);

	static float ToFloat(std::string num);

	static std::string ToString(int integer);

	static std::string ToString(uint64_t integer);

	static std::string ToString(uint16_t integer);

	static bool ToBool(std::string boolean);


	// trim from start
	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}

	//extract the filename from a path, both forward and back slash are delimiters
	static void GetFileName(std::string& file);

	//remove everything after the last dot including the dot
	static void RemoveFileNameExt(std::string& file);

	static std::string GetFileNameExt(const std::string& file);

	//just get everything before the last delimiter
	static std::string GetDirectory(const std::string& path);

	static STR_TYPE GetCanonicalPath(const std::string& path);

	static inline bool HasSep(const std::string& text) {
		return text.find("/") != std::string::npos || text.find("\\") != std::string::npos;
	}

	static inline std::string AddSep(std::string folder) {
		if( (folder[folder.length() - 1] != '/') && (folder[folder.length() - 1] != '\\') )
			folder.append("/");
		return folder;
	}

	//environment variable retrieval
	static std::string get_env_var(const std::string& key);

	static bool FileExists(std::string filename);

	static bool BinaryFileRead(const std::string& filename, char*& data, size_t& size);

	static void ThrowIf(bool cond, const char* msg);

#ifdef _WIN32
	static inline pid_type GetProcessId() { return GetCurrentProcessId(); }
#else
	static inline pid_type GetProcessId() { return getpid(); }
#endif

};

#endif //__UTIL_H__
