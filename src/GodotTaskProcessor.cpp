//
// Created by Harris.Lu on 2024/2/3.
//

#include "GodotTaskProcessor.h"

namespace CesiumForGodot {

	void GodotTaskProcessor::startTask( std::function<void()> f )
    {
        f();
	}

} // CesiumForGodot