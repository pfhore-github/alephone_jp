#ifndef _HPL_STRING_MODIFIER_
#define _HPL_STRING_MODIFIER_

#include <string>
#include <vector>

namespace hpl{
namespace string{
    /**
	    文字列の分割
    */
    std::vector<std::string> Split( std::string src, const char* key);

};
};

#endif
