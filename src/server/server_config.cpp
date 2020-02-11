#include <common/obd_config.h>

#include "server_config.h"

ServerConfig::ServerConfig(const std::string& fileName)
	: ObdConfig(fileName)
{
	if (m_keyvalues.count("maxConnections"))
		m_maxConnections = std::stoi(m_keyvalues["maxConnections"]);
	if (m_keyvalues.count("worldSize"))
		m_maxConnections = std::stoi(m_keyvalues["worldSize"]);
}

ServerConfig::~ServerConfig()
{
	m_keyvalues["maxConnections"] = std::to_string(m_maxConnections);
	m_keyvalues["worldSize"] = std::to_string(m_worldSize);
}
