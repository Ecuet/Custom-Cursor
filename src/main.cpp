#include "Geode/DefaultInclude.hpp"
#include "Geode/cocos/CCScheduler.h"
#include "Geode/cocos/base_nodes/CCNode.h"
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <filesystem>
#include "Cursor.hpp"
#include "Geode/loader/SettingV3.hpp"
#include "Geode/utils/Keyboard.hpp"
#include "Geode/utils/cocos.hpp"
#include "Geode/modify/CCMenuItemSpriteExtra.hpp"
#include "Geode/modify/CCLayer.hpp"
#include "Geode/modify/CCMenuItem.hpp"
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>
#include "Geode/ui/Button.hpp"

using namespace geode::prelude;

class SixSeven : public CCObject {
public:
	void update(float dt){
		Cursor::get()->update();
	}
};

class $baseModify(MyCC, CCMenuItem){
	void modify(){
		addOnEnterCallback([this] {
			Cursor::get()->addButton(this);
		});
		addOnExitCallback([this] {
			Cursor::get()->removeButton(this);
		});
	}
};

class $baseModify(MyBT, Button){
	void modify(){
		addOnEnterCallback([this] {
			Cursor::get()->addButton(this);
		});
		addOnExitCallback([this] {
			Cursor::get()->removeButton(this);
		});
	}
};


// class $modify(my,CCMenuItemSpriteExtra) {
// 	bool init(CCNode* sprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler callback) {
// 		if (!CCMenuItemSpriteExtra::init(sprite, selectedSprite, target, callback)) return false;
		
// 		this->schedule(schedule_selector(my::customUpdate));

// 		return true;
// 	}
// 	bool isHovered(){
// 		if(!isEnabled()) return false;
// 		auto mousePos = getMousePos();
// 		auto localPos = this->getParent()->convertToNodeSpace(mousePos);
		
// 		auto rect = boundingBox();
// 		return rect.containsPoint(localPos);
// 	}
// 	void customUpdate(float dt){
// 		if(isHovered()){
// 			Cursor::get()->addHovered();
// 		}
// 	}
	
// };

// class $modify(myLayer, CCLayer){
// 	struct Fields {
// 		CCPoint oldPos;
		
// 	};
	
// 	bool init(){
// 		if(!CCLayer::init()) return false;

// 		this->schedule(schedule_selector(my::customUpdate));

// 		return true;
// 	}
// 	void checkIf(CCNode* c){
// 		for(auto child : CCArrayExt<CCNode*>(c->getChildren())){
// 			if(child->isVisible()) {
				
// 			}
// 		}
// 	}
// 	void customUpdate(float dt){
// 		auto mousePos = getMousePos();
// 		if(mousePos == m_fields->oldPos) return;
// 		m_fields->oldPos = mousePos;

		
// 	} 
// };


$on_mod(Loaded) {
	auto Cursor = Cursor::get();
    listenForSettingChanges<std::filesystem::path>("DefaultCursor", [](std::filesystem::path p){
		Cursor::get()->recreate();
	});
	listenForSettingChanges<std::filesystem::path>("HoveredCursor", [](std::filesystem::path p){
		Cursor::get()->recreate();
	});
	listenForSettingChanges<std::filesystem::path>("HoldCursor", [](std::filesystem::path p){
		Cursor::get()->recreate();
	});

	listenForSettingChanges<std::string>("Presets", [](std::string p){
		Cursor::get()->recreate();
	});
	listenForSettingChanges<bool>("DisableHoveredCursor", [](bool s){
		Cursor::get()->recreate();
	});
	listenForSettingChanges<double>("CursorScale", [](double s){
		Cursor::get()->recreate();
	});

	MouseInputEvent().listen([](MouseInputData& data) {
		if(data.button == geode::MouseInputData::Button::Left){
			Cursor::get()->setHolding(data.action == MouseInputData::Action::Press);
		}
	}).leak();

	Loader::get()->queueInMainThread([] {
		CCScheduler::get()->scheduleUpdateForTarget(new SixSeven{}, 2067, false);
	});
}