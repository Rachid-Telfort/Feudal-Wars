#include<battle_character.h>

namespace feudal_wars
{
    battle_character::battle_character(const std::map<animation_state, std::pair<std::size_t, std::size_t>>& incoming_animation_state_boundaries, const std::pair<std::size_t, std::size_t>& incoming_frame_dimensions, const std::pair<power_level, power_level>& incoming_power_levels):battle_character_face_right(true), grounded(true), jumping(false), animation_state_boundaries(incoming_animation_state_boundaries), frame_dimensions(incoming_frame_dimensions), power_levels(incoming_power_levels), animation_index(animation_state_boundaries.at(animation_state::idle).first), this_animation_state(animation_state::idle){}

    bool battle_character::create(const scene::state& incoming_scene_state, std::unique_ptr<input_source>& incoming_input_source, const std::unique_ptr<battle_stage>& incoming_battle_stage, const std::size_t incoming_battle_character_index)
    {
        const bool incoming_battle_stage_valid(static_cast<bool>(incoming_battle_stage));
        if(!incoming_battle_stage_valid)
        {
            return false;
        }

        battle_character_input_source=std::move(incoming_input_source);
        const bool incoming_input_source_valid(static_cast<bool>(battle_character_input_source));
        if(!incoming_input_source_valid)
        {
            return false;
        }

        const bool this_battle_character_created(create_this(incoming_scene_state, incoming_battle_stage, incoming_battle_character_index));

        return this_battle_character_created;
    }

    std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>> battle_character::get_battle_character_scene_node()
    {
        return std::move(battle_character_scene_node);
    }
}
