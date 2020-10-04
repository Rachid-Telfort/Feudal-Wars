#include<battle_stage.h>

namespace feudal_wars
{
    battle_stage::battle_stage(const std::vector<sf::Vector2f>& incoming_starting_positions, const sf::Vector2f& incoming_horizontal_boundaries, const sf::Vector2f& incoming_vertical_boundaries, const interactable::action incoming_music_action):music_action(incoming_music_action), starting_positions(incoming_starting_positions), horizontal_boundaries(incoming_horizontal_boundaries), vertical_boundaries(incoming_vertical_boundaries){}

    const std::vector<sf::Vector2f>& battle_stage::get_starting_positions() const
    {
        return starting_positions;
    }

    const sf::Vector2f& battle_stage::get_horizontal_boundaries() const
    {
        return horizontal_boundaries;
    }

    const sf::Vector2f& battle_stage::get_vertical_boundaries() const
    {
        return vertical_boundaries;
    }

    std::multimap<scene::layer, std::unique_ptr<scene_node>> battle_stage::get_layered_stage_scene_nodes()
    {
        return std::move(layered_stage_scene_nodes);
    }

    interactable::action battle_stage::get_music_action() const
    {
        return music_action;
    }
}
