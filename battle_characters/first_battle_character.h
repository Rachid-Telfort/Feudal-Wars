#ifndef FIRST_BATTLE_CHARACTER_H_INCLUDED
#define FIRST_BATTLE_CHARACTER_H_INCLUDED

#include<cstddef>
#include<utility>
#include<vector>
#include<boost/optional/optional.hpp>
#include<SFML/Graphics/Image.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<SFML/System/Vector2.hpp>
#include<SFML/Window/VideoMode.hpp>
#include<battle_character.h>
#include<battle_stage.h>
#include<graphics_resource_holder.h>
#include<scene.h>
#include<texture_sheet.h>

namespace feudal_wars
{
    class first_battle_character:public battle_character
    {
        bool create_battle_character_scene_node(const scene::state&, const std::unique_ptr<battle_stage>&, const std::size_t);
        bool create_this(const scene::state&, const std::unique_ptr<battle_stage>&, const std::size_t);
        bool create_texture_sheets();
        bool set_character_facing(const std::size_t);
        void cycle_this_animation(sf::Sprite&);
        void ground_character(const float, const std::unique_ptr<battle_stage>&, sf::Sprite&);
        void jump_character(const float, const std::unique_ptr<battle_stage>&, sf::Sprite&);
        void move_character(const float, const float, const std::unique_ptr<battle_stage>&, sf::Sprite&);
        void set_animation_state(const animation_state, sf::Sprite&);

        public:
            first_battle_character();
            first_battle_character(const first_battle_character&)=delete;
            first_battle_character(first_battle_character&&)=delete;

            first_battle_character& operator=(const first_battle_character&)=delete;
            first_battle_character& operator=(first_battle_character&&)=delete;
    };
}

#endif
