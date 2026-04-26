#include "Geode/cocos/base_nodes/CCNode.h"
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/utils/cocos.hpp"
#include <Geode/Geode.hpp>
#include <map>
#include <vector>

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

    std::set<CCNode*> m_activeNodes;

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

    void addButton(CCNode* node);
    void removeButton(CCNode* node);

    bool isHovered(CCNode* node);

    void updateCursor();
    void updateHover();

    std::string enumToSTR(CursorTypes type);
    std::string enumToSTRTexture(CursorTypes type);
    

};