#pragma once

#include <string>
#include <common/obd_config.h>

class ClientConfig : public ObdConfig {
	bool m_fullScreen = false;
	int m_windowWidth = 1280;
	int m_windowHeight = 720;
	int m_fpsLimit = 60;
	int m_fov = 65;

	bool m_isFpsCapped = true;
	bool m_shouldShowInstructions = true;

	std::string m_skinName = "player";
	std::string m_texturePack = "default";

	std::string m_serverIp = "127.0.0.1";
	public:
		ClientConfig(const std::string& fileName);
		~ClientConfig();

		bool get_fullScreen() const { return m_fullScreen; }
		void set_fullScreen(const bool value) { m_fullScreen = value; }

		bool get_isFpsCapped() const { return m_isFpsCapped; }
		void set_isFpsCapped(const bool value) { m_isFpsCapped = value; }

		bool get_shouldShowInstructions() const { return m_shouldShowInstructions; }
		void set_shouldShowInstructions(const bool value) { m_shouldShowInstructions = value; }

		int get_windowWidth() const { return m_windowWidth; }
		void set_windowWidth(const int value)  { m_windowWidth = value; }

		int get_windowHeight() const { return m_windowHeight; }
		void set_windowHeight(const int value) { m_windowHeight = value; }

		int get_fpsLimit() const { return m_fpsLimit; }
		void set_fpsLimit(const int value) { m_fpsLimit = value; }

		int get_fov() const { return m_fov; }
		void set_fov(const int value) { m_fov = value; }

		std::string get_skinName() const { return m_skinName; }
		void set_skinName(const std::string& value) { m_skinName = value; }

		std::string get_texturePack() const { return m_texturePack; }
		void set_texturePack(const std::string& value) { m_texturePack = value; }

		std::string get_serverIp() const { return m_serverIp; }
		void set_serverIp(const std::string& value) { m_serverIp = value; }
};
