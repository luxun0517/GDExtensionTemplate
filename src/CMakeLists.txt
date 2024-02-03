# SPDX-License-Identifier: Unlicense

target_sources( ${PROJECT_NAME}
    PRIVATE
        constants.h
        TestRendering.h
        TestRendering.cpp
        Cesium3DTilesetLoadFailureDetails.h
        CesiumWgs84Ellipsoid.h
        CesiumWgs84Ellipsoid.cpp
        GodotTransforms.h
        GodotTransforms.cpp
        GodotTilesetExternals.h
        GodotTilesetExternals.cpp
        CesiumRuntimeSettings.h
        CesiumRuntimeSettings.cpp
        CesiumCreditSystem.h
        CesiumCreditSystem.cpp
        GodotTaskProcessor.h
        GodotTaskProcessor.cpp
        CameraManager.h
        CameraManager.cpp
        TextureLoader.h
        TextureLoader.cpp
        CesiumGeoreference.h
        CesiumGeoreference.cpp
        CesiumShaderProperties.h
        CesiumShaderProperties.cpp
        GodotPrepareRendererResources.h
        GodotPrepareRendererResources.cpp
        GD3DTileset.h
        GD3DTileset.cpp
#        Cesium.h
#        Cesium.cpp
#        GodotAssetAccessor.h
#        GodotAssetAccessor.cpp
#        GodotExternals.h
#        GodotExternals.cpp
        RegisterExtension.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)
