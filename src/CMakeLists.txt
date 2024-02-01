# SPDX-License-Identifier: Unlicense

target_sources( ${PROJECT_NAME}
    PRIVATE
        constants.h
        TestRendering.h
        TestRendering.cpp
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
#        GD3DTileset.h
#        GD3DTileset.cpp
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
