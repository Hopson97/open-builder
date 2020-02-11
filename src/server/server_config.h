#pragma once

#include <common/obd_config.h>

class ServerConfig : public ObdConfig {
	int m_maxConnections = 4;
	int m_worldSize = 4;
	public:
		ServerConfig(const std::string& filePath);
		~ServerConfig();
		
		int get_maxConnections() const { return m_maxConnections; }
		void set_maxConnections(const int value) { m_maxConnections = value; }

		int get_worldSize() const { return m_worldSize; }
		void set_worldSize(const int value) { m_worldSize = value; }
};
