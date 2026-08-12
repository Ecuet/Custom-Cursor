#include "Geode/DefaultInclude.hpp"
#include "Geode/cocos/CCDirector.h"
#include "Geode/cocos/CCScheduler.h"
#include "Geode/cocos/base_nodes/CCNode.h"
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <cmath>
#include <filesystem>
#include "Cursor.hpp"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include "Geode/loader/Loader.hpp"
#include "Geode/loader/Mod.hpp"
#include "Geode/loader/SettingV3.hpp"
#include "Geode/ui/GeodeUI.hpp"
#include "Geode/ui/Popup.hpp"
#include "Geode/utils/Keyboard.hpp"
#include "Geode/utils/cocos.hpp"
#include "Geode/modify/CCMenuItemSpriteExtra.hpp"
#include "Geode/modify/CCLayer.hpp"
#include "Geode/modify/CCMenuItem.hpp"
#include "Geode/modify/MenuLayer.hpp"
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>
#include "Geode/ui/Button.hpp"
#include "CursorMenu.hpp"

using namespace geode::prelude;

class SixSeven : public CCObject {
public:
	void update(float dt){
		Cursor::get()->update();
	}
};

class $modify(MyMenu, MenuLayer){
	void onBTN(CCObject* target){
		CursorMenu::create()->show();
	}
	bool init() {
		if (!MenuLayer::init()) return false;
		
		auto winSize = CCDirector::sharedDirector()->getWinSize();

		auto sprite = CCSprite::create("defaultGothic.png"_spr);
		sprite->setScale(0.6f);

		auto cursorBtn = CCMenuItemSpriteExtra::create(
			sprite, nullptr, this, 
			menu_selector(MyMenu::onBTN)
		);

		cursorBtn->setID("cursurselect"_spr);

		auto rightMenu = getChildByID("right-side-menu");
		rightMenu->addChild(cursorBtn);
			
		rightMenu->updateLayout();

		return true;
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

	listenForSettingChanges<bool>("DisableHoveredCursor", [](bool s){
		Cursor::get()->recreate();
	});
	listenForSettingChanges<double>("CursorScale", [](double s){
		Cursor::get()->recreate();
	});
		listenForSettingChanges<bool>("ModMenusFix", [](bool s){
		Cursor::get()->forceDisableTab();
	});

	MouseInputEvent().listen([](MouseInputData& data) {
		if(data.button == geode::MouseInputData::Button::Left){
			Cursor::get()->setHolding(data.action == MouseInputData::Action::Press);
		}
	}).leak();

	KeyboardInputEvent().listen([](KeyboardInputData& data) {
		bool isFixEnabled = Mod::get()->getSettingValue<bool>("ModMenusFix");
		if(isFixEnabled && data.action == KeyboardInputData::Action::Press && data.key == cocos2d::enumKeyCodes::KEY_Tab) {
			Cursor::get()->togglebyTab();
		}
	}).leak();

	Loader::get()->queueInMainThread([] {
		CCScheduler::get()->scheduleUpdateForTarget(new SixSeven{}, 2067, false);
	});

	
}
$on_game(Loaded) {
	auto wasWarningShown = Mod::get()->getSavedValue<bool>("warningshown", false);

	if(!wasWarningShown) {
		auto tabModList = {"eclipse.eclipse-menu","absolllute.megahack","thesillydoggo.qolmod","firee.prism", "tobyadd.gdh"};
		for(auto id: tabModList) {
			if(geode::Loader::get()->isModLoaded(id)) {
				geode::createQuickPopup("Warning!", 
					"Sooo, I noticed you have some mod menus that open with TAB... And they overlap the custom cursor, so I added an experimental feature that fixes it. You can enable it in the settings.", "Ok", "Open Settings", 
					[](FLAlertLayer* layer, bool btn2) {
						Mod::get()->setSavedValue<bool>("warningshown", true);
						if(btn2) {
							openSettingsPopup(Mod::get(), false);
						}
					}
				);
			}
		}
	}
}