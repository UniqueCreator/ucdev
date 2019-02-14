#pragma once

#include "amd_model.h"

#include <uc_public/assets/assets.h>

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        using ColorSRGB = Float3;

        struct Albedo
        {
            ColorSRGB m_value;
        };

        struct Specular
        {
            ColorSRGB m_fresnel;
            float     m_glossiness;
        };

        class Room
        {

            public:

            using Textures  = std::array< std::unique_ptr<UniqueCreator::Assets::Texture2D>, 7 >;
            using Speculars = std::array< Specular, 10 >;
            using Albedos   = std::array< Albedo, 10 >;

            Room(std::unique_ptr<Model>&& m, Textures&& textures);

            Model* GetModel() const;

            private:

            std::unique_ptr<Model>                                             m_model;
            std::array< std::unique_ptr<UniqueCreator::Assets::Texture2D>, 7 > m_textures;
            std::array< uint32_t, 14>                                          m_material_range;    //holds indices 
            std::vector<uint32_t>                                              m_indices;

            Albedos                                                            m_albedos;
            Speculars                                                          m_speculars;

            friend std::unique_ptr<Room> CreateRoom();
        };

        std::unique_ptr<Room> CreateRoom();
    }
}

