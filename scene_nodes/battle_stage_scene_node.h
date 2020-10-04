#ifndef BATTLE_STAGE_SCENE_NODE_H_INCLUDED
#define BATTLE_STAGE_SCENE_NODE_H_INCLUDED

#include<vector>
#include<SFML/Graphics/Sprite.hpp>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/System/Vector2.hpp>
#include<resource_holder.h>
#include<scene_node.h>
#include<SFML/Graphics/Rect.hpp>

namespace feudal_wars
{
    class battle_stage_scene_node:public scene_node
    {
        sf::Sprite background_sprite;
        sf::Texture background_texture;

        public:
            battle_stage_scene_node(const resource_holder::identifier);
            void draw_this(sf::RenderTarget&, sf::RenderStates) const;
            void process_action_this(const float, const action);
            void process_event_this(const sf::Event&);
            void process_frame_this(const float);

            sf::FloatRect get_local_bounds() const;
    };
}

#endif
