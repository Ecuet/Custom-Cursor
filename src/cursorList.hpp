#include <map>
#include <vector>
namespace CursorList {
    inline std::vector<std::string> m_cursorList = {"Custom", "Pixel","Gothic","Calamitas","Stone","Omori1","Omori2","Pink","Boykisser","GD","MyLittlePony","Maxwell"};
    inline std::set<std::string> m_animatedCursors = {"Maxwell"};

    inline std::map<std::string, float> addScales = {
        {"Maxwell", 2.f}
    };

    inline float getScale(std::string name){
        auto it = addScales.find(name);
        if(it != addScales.end()) {
            return it->second;
        }
        return 1.f;
    }

    inline std::string getPrefix(std::string name){
        return CursorList::m_animatedCursors.count(name) ? "gif" : "png";
    }
}