#include "Geode/utils/cocos.hpp"
#include <Geode/Geode.hpp>
#include <map>

using namespace geode::prelude;

enum class CursorTypes {
    Default,
    Hovered,
    Hold
};

class Cursor {
private:
    std::map<CursorTypes, geode::Ref<cocos2d::CCSprite>> m_cursors = {};
    std::map<CursorTypes, bool> m_activeCursors = {};

    CursorTypes m_currentState = CursorTypes::Default;

    bool m_isHovered = false;
    int m_hovered = 0;

    bool m_isHolding = false;

    bool m_active = false;


    

public:
    Cursor();
    Cursor(const Cursor&) = delete;

    static Cursor* get();

    void recreate();
    void update();

    void addHovered();
    void setHolding(bool value);

    std::string enumToSTR(CursorTypes type);
    std::string enumToSTRTexture(CursorTypes type);
    

};