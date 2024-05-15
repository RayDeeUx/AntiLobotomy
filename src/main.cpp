#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <regex>

using namespace geode::prelude;

const std::regex lobotomyName("(?:[\\S ]+)?(?:lom?(?:g|b)o)(?:[\\S ]+)?", std::regex_constants::icase);

class $modify(MyPlayLayer, PlayLayer) {
	static void onModify(auto & self)
	{
		(void) self.setHookPriority("PlayLayer::addObject", 900);
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

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
	void showWarningIfFirstInstall() {
		createQuickPopup(
			"Hold it right there!",
			"This seems to be your first time using AntiLobotomy. Be aware that (by default), AntiLobotomy does not apply to rated levels to protect you from leaderboard bans. If you want to play this level with AntiLobotomy enabled, open the mod settings now. Otherwise, close this popup and press \"Play\" again.",
			"Open Mod Settings",
			"Close",
			[](FLAlertLayer* alert, bool close) {
				if (!close) {
					openSettingsPopup(Mod::get());
				}
			}
		);
	}
	void onPlay(CCObject* sender) {
		if (Mod::get()->getSettingValue<bool>("enabled") && this->m_level->m_stars != 0 && !Mod::get()->setSavedValue("firstInstall", true) && !Mod::get()->getSettingValue<bool>("enableOnRated")) {
			MyLevelInfoLayer::showWarningIfFirstInstall();
		} else {
			LevelInfoLayer::onPlay(sender);
		}
	}
};