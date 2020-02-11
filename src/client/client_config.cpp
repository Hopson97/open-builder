#include <common/obd_config.h>

#include "client_config.h"

ClientConfig::ClientConfig(const std::string& fileName)
	: ObdConfig(fileName)
{
	if (m_keyvalues.count("isFpsCapped"))
		m_isFpsCapped = m_keyvalues["isFpsCapped"] == "true" ? true : false;
	if (m_keyvalues.count("shouldShowInstructions"))
		m_shouldShowInstructions = m_keyvalues["shouldShowInstructions"] == "true" ? true : false;
	if (m_keyvalues.count("fullScreen"))
		m_fullScreen = m_keyvalues["fullScreen"] == "true" ? true : false;

	if (m_keyvalues.count("windowWidth"))
		m_windowWidth = std::stoi(m_keyvalues["windowWidth"]);
	if (m_keyvalues.count("windowHeight"))
		m_windowHeight = std::stoi(m_keyvalues["windowHeight"]);
	if (m_keyvalues.count("fpsLimit"))
		m_fpsLimit = std::stoi(m_keyvalues["fpsLimit"]);
	if (m_keyvalues.count("fov"))
		m_fov = std::stoi(m_keyvalues["fov"]);

	if (m_keyvalues.count("skinName"))
		m_skinName = m_keyvalues["skinName"];
	if (m_keyvalues.count("texturePack"))
		m_texturePack = m_keyvalues["texturePack"];
	if (m_keyvalues.count("serverIp"))
		m_serverIp = m_keyvalues["serverIp"];
}

ClientConfig::~ClientConfig()
{
	m_keyvalues["isFpsCapped"] = m_isFpsCapped ? "true" : "false";
	m_keyvalues["shouldShowInstructions"] = m_shouldShowInstructions ? "true" : "false";
	m_keyvalues["fullScreen"] = m_fullScreen ? "true" : "false";

	m_keyvalues["windowWidth"] = std::to_string(m_windowWidth);
	m_keyvalues["windowHeight"] = std::to_string(m_windowHeight);
	m_keyvalues["fpsLimit"] = std::to_string(m_fpsLimit);
	m_keyvalues["fov"] = std::to_string(m_fov);

	m_keyvalues["skinName"] = m_skinName;
	m_keyvalues["texturePack"] = m_texturePack;
	m_keyvalues["serverIp"] = m_serverIp;
}
