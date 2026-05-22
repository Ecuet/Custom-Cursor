#include "Cursor.hpp"
#include "Geode/cocos/CCDirector.h"
#include "Geode/cocos/base_nodes/CCNode.h"
#include "Geode/cocos/cocoa/CCGeometry.h"
//#include "Geode/cocos/platform/win32/CCEGLView.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "Geode/cocos/support/CCPointExtension.h"
#include "Geode/cocos/textures/CCTextureCache.h"
#include "Geode/loader/Log.hpp"
#include "Geode/ui/LazySprite.hpp"
#include "Geode/ui/OverlayManager.hpp"
#include "Geode/utils/cocos.hpp"
#include <Geode/Result.hpp>
#include <Geode/binding/EndLevelLayer.hpp>
#include <Geode/binding/PlatformToolbox.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/RetryLevelLayer.hpp>
#include <filesystem>
#include <map>
#include <string>
#include "Geode/utils/string.hpp"
#include "Utils.hpp"
#include "cursorList.hpp"

Cursor::Cursor() {
    recreate();

    
}

void Cursor::recreate(){
    for(auto i : m_cursors) {
        if(i.second) i.second->removeFromParent();
    }
    auto Preset = Mod::get()->getSavedValue<std::string>("Presets","Custom");
    bool IsCustom = Preset == "Custom";
    
    auto defaultCursor = Mod::get()->getSettingValue<std::filesystem::path>("DefaultCursor");
    auto hoveredCursor = Mod::get()->getSettingValue<std::filesystem::path>("HoveredCursor");
    auto HoldCursor = Mod::get()->getSettingValue<std::filesystem::path>("HoldCursor");

    std::map<CursorTypes, std::filesystem::path> texturesPath = {
        {CursorTypes::Default, defaultCursor},
        {CursorTypes::Hovered, hoveredCursor},
        {CursorTypes::Hold, HoldCursor}
    };
    
    if(IsCustom && (defaultCursor.empty() || !std::filesystem::exists(defaultCursor))){
        m_active = false;
        PlatformToolbox::showCursor();
       // CCEGLView::sharedOpenGLView()->showCursor(true);
        return;
    }
    m_active = true;
   //CCEGLView::sharedOpenGLView()->showCursor(false);
    PlatformToolbox::hideCursor();
    auto allTypes = {
    CursorTypes::Default,
    CursorTypes::Hold,
    CursorTypes::Hovered
    };

    for(auto type : allTypes){
        auto scaleSet = Mod::get()->getSettingValue<double>("CursorScale");
        auto addScale = CursorList::getScale(Preset);

        float scale = 15.f * (float) scaleSet * addScale;

        CCSprite* newSprite;
        if(IsCustom) {
            auto path = texturesPath[type];
            if(!path.empty()){
                auto Texture = CCTextureCache::sharedTextureCache()->addImage(path.string().c_str(), true);

                newSprite = CCSprite::createWithTexture(Texture);
            }
            else { m_activeCursors[type] = false; continue; }
            
        }
        else {
            std::string prefix =  CursorList::getPrefix(Preset); 
            std::string path = fmt::format("{}{}.{}", enumToSTRTexture(type), Preset, prefix).c_str();

            newSprite = CCSprite::create(fmt::format("{}"_spr,path).c_str());
        }
       
        //newSprite->setZOrder(5000);
        newSprite->setID(enumToSTR(type));

        geode::cocos::limitNodeSize(newSprite, {scale, scale}, 99.f, 0.0001f);

        m_cursors[type] = newSprite;
        m_activeCursors[type] = true;

        OverlayManager::get()->addChild(newSprite);
        
        
    }
   
}

std::string Cursor::enumToSTR(CursorTypes type){
    switch (type) {
        case CursorTypes::Default: return "Default"_spr;
        case CursorTypes::Hovered: return "Hovered"_spr;
        case CursorTypes::Hold: return "Hold"_spr;
    }
    return "";
}

std::string Cursor::enumToSTRTexture(CursorTypes type){
     switch (type) {
        case CursorTypes::Default: return "default";
        case CursorTypes::Hovered: return "hover";
        case CursorTypes::Hold: return "hold";
    }
    return "";
}

void Cursor::addHovered(){
    ++m_hovered;
}

void Cursor::setHolding(bool value){
    m_isHolding = value;
}

void Cursor::addButton(CCNode* node){
    if(!node) return;

    m_activeNodes.insert(node);
}
void Cursor::removeButton(CCNode* node){
    if(!node) return;

    m_activeNodes.erase(node);
}

Cursor* Cursor::get(){
    static Cursor inst;

    return &inst;
}

bool Cursor::isHovered(CCNode* node){
    auto parent = node->getParent();
    if(!parent) return false;

    auto mousePos = getMousePos();
    auto localPos = parent->convertToNodeSpace(mousePos);
    
    auto rect = node->boundingBox();
    return rect.containsPoint(localPos);
	
}


void Cursor::updateHover(){
    auto mousePos = getMousePos();
    for(auto node : m_activeNodes){
        bool hovered = isHovered(node);
        if(hovered && tooltips::utils::isHoverable(node, mousePos)){
            addHovered();
            break;
        }
    }
}

void Cursor::updateCursor(){
 if(!m_active || !m_cursors[CursorTypes::Default]) {
        return;
    }
    bool isHoveredCursorDisabled = Mod::get()->getSettingValue<bool>("DisableHoveredCursor");

    PlatformToolbox::hideCursor();
    //CCEGLView::sharedOpenGLView()->showCursor(false);

    if(m_hovered > 0) m_isHovered = true;
    else m_isHovered = false;

    m_hovered = 0;
    if (m_cursors[CursorTypes::Hold] && m_activeCursors[CursorTypes::Hold] && m_isHolding) {
        m_currentState = CursorTypes::Hold;
    }
    else if(m_cursors[CursorTypes::Hovered] && m_activeCursors[CursorTypes::Hovered] && m_isHovered && !isHoveredCursorDisabled){
        m_currentState = CursorTypes::Hovered;
    }
    else {
        m_currentState = CursorTypes::Default;
    }
    
    bool showCursor = true;

    auto pl = PlayLayer::get();
    if (pl){
        auto levelretry = pl->getChildByType<RetryLevelLayer>(0);
        auto completed =  pl->getChildByType<EndLevelLayer>(0);
        if(!(pl->m_isPaused || levelretry || completed)) showCursor = false;
        
    }

    auto pos = getMousePos() - ccp(0, 2) ;

    for(auto [type, cursor] : m_cursors){
        if(cursor) {
            cursor->setPosition(pos); 

            if(!showCursor) {cursor->setVisible(false); continue; } 
            
            cursor->setVisible(  m_currentState == type);
        }
    }
}

void Cursor::update(){
    updateHover();
    updateCursor();
   
}