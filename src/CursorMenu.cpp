#include "CursorMenu.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/cocos/robtop/sprite_nodes/CCSpriteExtra.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "Geode/loader/Mod.hpp"
#include "Geode/ui/Button.hpp"
#include "Geode/ui/Layout.hpp"
#include "Geode/ui/Popup.hpp"
#include "Geode/ui/ScrollLayer.hpp"
#include "Geode/ui/SimpleAxisLayout.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include "cursorList.hpp"
#include "Cursor.hpp"

CursorMenu* CursorMenu::create(){
    auto ret = new CursorMenu;
    if(ret->init()){
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool CursorMenu::init(){
    if(!geode::Popup::init({350.f,250.f},"GJ_square05.png")) return false;

    setTitle("Cursors");
    setID("CustomCursors"_spr);

    auto scrollLayer = geode::ScrollLayer::create(m_mainLayer->getContentSize()/1.5);
    scrollLayer->setPosition(m_mainLayer->getContentSize() / 2);
    scrollLayer->setAnchorPoint({.5f,.5f});
    scrollLayer->ignoreAnchorPointForPosition(false);

    m_mainLayer->addChild(scrollLayer);

    auto layout = geode::SimpleAxisLayout::create(geode::Axis::Column);
    layout->setMainAxisDirection(geode::AxisDirection::TopToBottom);
    layout->setMainAxisAlignment(geode::MainAxisAlignment::End);
    layout->setMainAxisScaling(geode::AxisScaling::Fit);

    scrollLayer->m_contentLayer->setLayout(layout);

    for (std::string cursor : CursorList::m_cursorList){
        auto color = cocos2d::CCLayerColor::create({ 46, 46, 46,255 });
        color->setContentSize({scrollLayer->getContentWidth(), 40.f});

        auto text = cocos2d::CCLabelBMFont::create(cursor.c_str(),"bigFont.fnt");
        text->setPosition(color->getContentSize() / 2);
        text->setScale(0.5f);

        std::string path;
        if(cursor == "Custom"){
            path = "question.png"_spr;
        }
        else{
            std::string prefix = CursorList::getPrefix(cursor);
            path = fmt::format("default{}.{}"_spr,cursor, prefix);
        }

        auto image = cocos2d::CCSprite::create(path.c_str());
        //image->setContentSize({30.f,30.f});
        image->setPosition({15.f,20.f});

        geode::cocos::limitNodeSize(image, {30.f, 30.f}, 99.f, 0.0001f);

        auto menu = cocos2d::CCMenu::create();
        menu->ignoreAnchorPointForPosition(false);

        auto sp = ButtonSprite::create("Use");
        sp->setScale(0.8f); 

        auto btn = CCMenuItemSpriteExtra::create(sp, 
        nullptr, 
        this, 
        menu_selector(CursorMenu::chooseCursor));
        btn->setID(cursor);

        btn->setPosition({195.f, 20.f});
      

        menu->addChild(btn);

        color->addChild(text);
        color->addChild(image);
        color->addChild(menu);

        scrollLayer->m_contentLayer->addChild(color);
    }

    scrollLayer->m_contentLayer->updateLayout();
    scrollLayer->scrollToTop();

    auto t = CCLabelBMFont::create("Everything else can be changed in mod settings <3","bigFont.fnt");
    t->setScale(0.3f);
    t->setPosition({m_mainLayer->getScaledContentWidth() / 2, 15.f });
    m_mainLayer->addChild(t);

    return true;
}

void CursorMenu::chooseCursor(cocos2d::CCObject* obj){
    auto btn = static_cast<CCMenuItemSpriteExtra*>(obj);
    if(btn){
        auto selected = btn->getID();
        geode::Mod::get()->setSavedValue<std::string>("Presets",selected);

        Cursor::get()->recreate();

        if(selected == "Custom"){
            FLAlertLayer::create("W-Wait",
                "To use Custom Texture you need to set it in mod's settings",
                ":pray:")->show();
        }
    }
   
}