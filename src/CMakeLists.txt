# SPDX-License-Identifier: Unlicense

target_sources( ${PROJECT_NAME}
    PRIVATE
        constants.h
        TestRendering.h
        TestRendering.cpp
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
