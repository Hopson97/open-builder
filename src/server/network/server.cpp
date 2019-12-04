#include "server.h"

#include <iostream>

int ClientManager::emptySlot()
{
    for (int i = 0; i < static_cast<int>(isConnected.size()); i++) {
        if (!isConnected[i]) {
            return i;
		}	
    }
    return -1;
}
