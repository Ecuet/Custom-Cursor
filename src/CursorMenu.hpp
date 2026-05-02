#include "Geode/Geode.hpp"
#include <Geode/binding/FLAlertLayer.hpp>

class CursorMenu : public geode::Popup {
private:
    
public:
    static CursorMenu* create();
    bool init();

    void chooseCursor(CCObject* obj);
};