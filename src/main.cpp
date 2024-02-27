#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <regex>

using namespace geode::prelude;

const std::regex lobotomyName("(?:[\\S ]+)?(?:lom?(?:g|b)o)(?:[\\S ]+)?", std::regex_constants::icase);

class $modify(MyPlayLayer, PlayLayer) {
	static void onModify(auto & self)
    {
        self.setHookPriority("PlayLayer::addObject", 900);
    }
	void addObject(GameObject* p0) {
        if (Mod::get()->getSettingValue<bool>("enabled") && std::regex_match(std::string(this->m_level->m_levelName), lobotomyName)) {
			bool dontSkipThisObject = true;
			auto id = p0->m_objectID;
			if (Mod::get()->getSettingValue<bool>("disableNormalFace") && id == 3854) dontSkipThisObject = false; // normal difficulty
			if (Mod::get()->getSettingValue<bool>("disableAllFaces") && (3852 <= id && id <= 3863)) dontSkipThisObject = false; // all difficulties
			if (Mod::get()->getSettingValue<bool>("disableSFX") && (id == 3602 || id == 3603)) dontSkipThisObject = false; // sfx and edit sfx trigger
			if (Mod::get()->getSettingValue<bool>("disableSong") && (id == 3605 || id == 1934)) dontSkipThisObject = false; // song and edit song triggers
			if ((!Mod::get()->getSettingValue<bool>("enableOnRated") && this->m_level->m_stars != 0)) dontSkipThisObject = true;
			if (dontSkipThisObject) PlayLayer::addObject(p0);
		} else PlayLayer::addObject(p0);
	}
};