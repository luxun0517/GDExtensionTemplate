//
// Created by Harris.Lu on 2024/2/1.
//

#include "TextureLoader.h"
#include "constants.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <CesiumGltf/Model.h>
#include <CesiumGltf/Sampler.h>
#include <CesiumUtility/Tracing.h>

using namespace CesiumGltf;

namespace CesiumForGodot {

	//Ref<Texture2D> TextureLoader::loadTexture( const CesiumGltf::ImageCesium &image )
 //   {
 //       std::int32_t mipCount =
 //           image.mipPositions.empty() ? 1 : std::int32_t( image.mipPositions.size() );

 //       godot::Image::Format textureFormat;
 //       switch ( image.compressedPixelFormat )
 //       {
 //           case GpuCompressedPixelFormat::ETC1_RGB:
 //               textureFormat = godot::Image::Format::FORMAT_ETC;
 //               break;
 //           case GpuCompressedPixelFormat::ETC2_RGBA:
 //               textureFormat = godot::Image::Format::FORMAT_ETC2_RGBA8;
 //           case GpuCompressedPixelFormat::BC1_RGB:
 //               textureFormat = godot::Image::Format::FORMAT_DXT1;
 //               break;
 //           case GpuCompressedPixelFormat::BC3_RGBA:
 //               textureFormat = godot::Image::Format::FORMAT_DXT5;
	//			break;
 //           case GpuCompressedPixelFormat::BC4_R:
 //               WARN_PRINT( "BC4_R is not supported" );
 //               //textureFormat = godot::Image::Format::BC;
	//			break;
 //           case GpuCompressedPixelFormat::BC5_RG:
 //               WARN_PRINT( "BC5_RG is not supported" );
 //               break;
 //           case GpuCompressedPixelFormat::BC7_RGBA:
 //               WARN_PRINT( "BC7_RGBA is not supported" );
 //               break;
 //           case GpuCompressedPixelFormat::ASTC_4x4_RGBA:
 //               textureFormat = godot::Image::Format::FORMAT_ASTC_4x4;
 //               break;
 //           case GpuCompressedPixelFormat::PVRTC1_4_RGB:
 //               WARN_PRINT( "PVRTC1_4_RGB is not supported" );
 //               break;
 //           case GpuCompressedPixelFormat::PVRTC1_4_RGBA:
 //               WARN_PRINT( "PVRTC1_4_RGBA is not supported" );
	//			break;
 //           case GpuCompressedPixelFormat::ETC2_EAC_R11:
 //               textureFormat = godot::Image::Format::FORMAT_ETC2_R11;
	//			break;
 //           case GpuCompressedPixelFormat::ETC2_EAC_RG11:
 //               textureFormat = godot::Image::Format::FORMAT_ETC2_RG11;
 //               break;
 //           case GpuCompressedPixelFormat::PVRTC2_4_RGB:
 //           case GpuCompressedPixelFormat::PVRTC2_4_RGBA:
 //           default:
 //               textureFormat = godot::Image::Format::FORMAT_RGBA8;
 //               break;
 //       }

 //       PackedByteArray imgdata;
 //       imgdata.resize( image.pixelData.size() );

 //       uint8_t *wr = imgdata.ptrw();
 //       memcpy( wr, image.pixelData.data(), image.pixelData.size() );

 //       Ref<godot::Image> result = godot::Image::create_from_data(
 //           image.width, image.height, mipCount > 1, textureFormat, imgdata );

 //       if ( mipCount > 1 )
 //       {
 //           result->generate_mipmaps();
 //       }

 //       return ImageTexture::create_from_image( result );
 //   }

   /* Ref<Texture2D> TextureLoader::loadTexture(
        const CesiumGltf::Model& model,
        std::int32_t textureIndex
    )
    {
        const CesiumGltf::Texture *pTexture = Model::getSafe( &model.textures, textureIndex );
        if ( pTexture )
        {
            return TextureLoader::loadTexture( model, *pTexture );
		}
		else
		{
			return Ref<Texture2D>();
        }
    }

    Ref<Texture2D> TextureLoader::loadTexture(
        const CesiumGltf::Model& model,
        const CesiumGltf::Texture& texture
    )
    {
        const CesiumGltf::Image *pImage = Model::getSafe( &model.images, texture.source );
        if (!pImage)
        {
			return Ref<Texture2D>();
		}

        const ImageCesium &imageCesium = pImage->cesium;
        Ref<Texture2D> godotTexture = loadTexture( imageCesium );

        const Sampler *pSampler = Model::getSafe( &model.samplers, texture.sampler );

        WARN_PRINT( "TextureLoader::loadTexture: ");
        if ( pSampler )
        {
            switch ( pSampler->wrapS )
            {
                case CesiumGltf::Sampler::WrapS::MIRRORED_REPEAT:
                    break;
                default:
                    break;
            }
        }

        return godotTexture;
	}*/


} // CesiumForGodot
//