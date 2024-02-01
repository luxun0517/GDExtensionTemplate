//
// Created by Harris.Lu on 2024/2/1.
//

#ifndef GODOT_TEXTURELOADER_H
#define GODOT_TEXTURELOADER_H

#include <godot_cpp/classes/texture2d.hpp>
#include <cstdint>
using namespace godot;

namespace CesiumGltf
{
    struct Model;
    struct Texture;
    struct ImageCesium;
}
namespace CesiumForGodot {

    class TextureLoader {
    public:
        static Ref<Texture2D> loadTexture( const CesiumGltf::ImageCesium &image );

        static Ref<Texture2D> loadTexture(
			const CesiumGltf::Model& model, 
            std::int32_t textureIndex
		);

        static Ref<Texture2D> loadTexture(
            const CesiumGltf::Model& model,
			const CesiumGltf::Texture& texture
        );
    };

} // CesiumForGodot

#endif //GODOT_TEXTURELOADER_H
