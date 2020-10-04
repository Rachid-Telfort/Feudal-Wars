#ifndef BATTLE_CHARACTER_H_INCLUDED
#define BATTLE_CHARACTER_H_INCLUDED

#include<cstddef>
#include<map>
#include<memory>
#include<utility>
#include<vector>
#include<SFML/Graphics/Sprite.hpp>
#include<battle_stage.h>
#include<drawable_adapter_scene_node.h>
#include<input_source.h>
#include<scene.h>
#include<texture_sheet.h>

namespace feudal_wars
{
    struct battle_character
    {
        enum animation_state:unsigned char
        {
            attack,
            death,
            defense,
            idle,
            move
        };

        enum class power_level:unsigned char
        {
            high=80u,
            medium=50u,
            medium_high=65u,
            medium_low=35u,
            low=20u
        };

        private:
            virtual bool create_this(const scene::state&, const std::unique_ptr<battle_stage>&, const std::size_t)=0;

        protected:
            bool battle_character_face_right;
            bool grounded;
            bool jumping;
            const std::map<animation_state, std::pair<std::size_t, std::size_t>> animation_state_boundaries;
            const std::pair<std::size_t, std::size_t> frame_dimensions;
            const std::pair<power_level, power_level> power_levels;
            std::size_t animation_index;
            std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>> battle_character_scene_node;
            std::unique_ptr<input_source> battle_character_input_source;
            std::vector<texture_sheet> texture_sheets;
            animation_state this_animation_state;

        public:
            battle_character(const std::map<animation_state, std::pair<std::size_t, std::size_t>>&, const std::pair<std::size_t, std::size_t>&, const std::pair<power_level, power_level>&);
            battle_character(const battle_character&)=delete;
            battle_character(battle_character&&)=delete;

            battle_character& operator=(const battle_character&)=delete;
            battle_character& operator=(battle_character&&)=delete;

            bool create(const scene::state&, std::unique_ptr<input_source>&, const std::unique_ptr<battle_stage>&, const std::size_t);

            std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>> get_battle_character_scene_node();
    };
}

#endif
