//
// Created by Harris.Lu on 2024/1/31.
//

#include "CameraManager.h"
#include "CesiumGeoreference.h"
#include <godot_cpp/classes/camera3d.hpp>
#include <CesiumGeospatial/Ellipsoid.h>
#include <CesiumGeospatial/GlobeTransforms.h>
#include <CesiumUtility/Math.h>

#include <glm/trigonometric.hpp>

using namespace Cesium3DTilesSelection;
using namespace CesiumGeospatial;
using namespace CesiumUtility;

namespace CesiumForGodot {
	
	/*ViewState godotCameraToViewState( 
		const LocalHorizontalCoordinateSystem *pCoordinateSystem,
        const glm::dmat4 &unityWorldToTileset,
		Camera3D& camera
	)
    {
    
	}*/

} // CesiumForGodot