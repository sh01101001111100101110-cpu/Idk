#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

class $modify(BigPauseHitboxPlayLayer, PlayLayer) {

    // Runs every time a level is (re)loaded
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        this->resizePauseHitbox();

        return true;
    }

    void resizePauseHitbox() {
        // Geode assigns readable string IDs to most vanilla nodes.
        // "pause-button" is the id Geode gives the top-right pause button.
        // If this ever stops working (game update changes layout),
        // install the "DevTools" mod in-game, press F11 and inspect the
        // node tree while paused to find the current id, then update it here.
        auto pauseBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(
            this->getChildByIDRecursive("pause-button")
        );

        if (!pauseBtn) {
            log::warn("Big Pause Hitbox: couldn't find the pause button node");
            return;
        }

        auto scale = Mod::get()->getSettingValue<double>("hitbox-scale");

        // The visible sprite is a child of this menu item, centered on it.
        // Growing the menu item's own content size only grows its *touch*
        // area (used for hit-testing), it does NOT resize the sprite you see.
        auto originalSize = pauseBtn->getContentSize();
        pauseBtn->setContentSize({
            originalSize.width * static_cast<float>(scale),
            originalSize.height * static_cast<float>(scale)
        });

        log::debug(
            "Big Pause Hitbox: resized from {}x{} to {}x{}",
            originalSize.width, originalSize.height,
            pauseBtn->getContentSize().width, pauseBtn->getContentSize().height
        );
    }
};
