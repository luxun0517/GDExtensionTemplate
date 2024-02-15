# SPDX-License-Identifier: Unlicense

target_sources( ${PROJECT_NAME}
    PRIVATE
        constants.h
        Cesium.h
        Cesium.cpp
        GodotAssetAccessor.h
        GodotAssetAccessor.cpp
        GodotTaskProcessor.h
        GodotTaskProcessor.cpp
        CesiumCreditSystem.h
        CesiumCreditSystem.cpp
        GodotExternals.h
        GodotExternals.cpp
        GodotPrepareRendererResources.h
        GodotPrepareRendererResources.cpp
#        TestRendering.h
#        TestRendering.cpp
#        CesiumModelMetadata.h
#        CesiumModelMetadata.cpp
        CesiumWgs84Ellipsoid.h
        CesiumWgs84Ellipsoid.cpp
        GodotTransforms.h
        GodotTransforms.cpp
        CameraManager.h
        CameraManager.cpp
#        LoadGltfResult.h
        GodotTilesetExternals.h
        GodotTilesetExternals.cpp
        CesiumRuntimeSettings.h
        CesiumRuntimeSettings.cpp
##        TextureLoader.h
##        TextureLoader.cpp
        CesiumGeoreference.h
        CesiumGeoreference.cpp
        CesiumShaderProperties.h
        CesiumShaderProperties.cpp
#        GodotPrepareRendererResources.h
#        GodotPrepareRendererResources.cpp
        CesiumIonServer.h
        CesiumIonServer.cpp
        GD3DTileset.h
        GD3DTileset.cpp
#        Cesium3DTilesetLoadFailureDetails.h


        TestTilesetJson.h
        TestTilesetJson.cpp

        RegisterExtension.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)
