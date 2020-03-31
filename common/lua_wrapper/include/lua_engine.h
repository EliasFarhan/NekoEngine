#define  LUAPP_HEADER_ONLY
#include <luapp/lua.hpp>
namespace neko
{

class LuaEngine
{
public:
    void InterpretLine(const std::string& line)
    {
        state_.runString(line);
    }

private:
	lua::State state_;
};

}
