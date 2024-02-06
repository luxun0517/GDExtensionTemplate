//
// Created by Harris.Lu on 2024/2/6.
//

#ifndef GDCESIUM_LOADGLTFRESULT_H
#define GDCESIUM_LOADGLTFRESULT_H

#include <CesiumGltf/Material.h>
#include <godot_cpp/classes/rendering_server.hpp>
#include <glm/mat4x4.hpp>
#include <optional>
#include "CesiumModelMetadata.h"

using namespace godot;
namespace CesiumForGodot {

	struct LoadPrimitiveResult
    {
        std::unique_ptr<RID> RenderData = nullptr;

        const CesiumGltf::Material *pMaterial = nullptr;

        glm::dmat4x4 transform{ 1.0 };
        
    };

    struct LoadMeshResult
    {
        std::vector<LoadPrimitiveResult> primitiveResults{};
	};

    struct LoadNodeResult
    {
        std::optional<LoadMeshResult> meshResult = std::nullopt;        
    };

    struct LoadModelResult
    {
        std::vector<LoadNodeResult> nodeResults{};
        CesiumModelMetadata Metadata{};
    };


} // CesiumForGodot

#endif //GDCESIUM_LOADGLTFRESULT_H
