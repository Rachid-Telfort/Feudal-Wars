#ifndef FIRST_BATTLE_STAGE_H_INCLUDED
#define FIRST_BATTLE_STAGE_H_INCLUDED

#include<cstddef>
#include<memory>
#include<utility>
#include<vector>
#include<boost/optional/optional.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/Window/VideoMode.hpp>
#include<battle_stage.h>
#include<drawable_adapter_scene_node.h>
#include<graphics_resource_holder.h>
#include<scene.h>
#include<scene_node.h>

namespace feudal_wars
{
    class first_battle_stage:public battle_stage
    {
        sf::Texture background_texture;

        void set_background_layer_sprite_scene_node();

        public:
            first_battle_stage();
            first_battle_stage(const first_battle_stage&)=delete;
            first_battle_stage(first_battle_stage&&)=delete;

            first_battle_stage& operator=(const first_battle_stage&)=delete;
            first_battle_stage& operator=(first_battle_stage&&)=delete;

            void create(const scene::state&);
    };
}

#endif
