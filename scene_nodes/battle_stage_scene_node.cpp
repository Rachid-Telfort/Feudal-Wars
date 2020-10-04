#include<battle_stage_scene_node.h>

namespace feudal_wars
{
    battle_stage_scene_node::battle_stage_scene_node(const resource_holder::identifier incoming_identifier):scene_node(scene_node::identifier::battle_stage)
    {
        const boost::optional<const std::vector<unsigned char>&> optional_x =resource_holder::get_resource_data(incoming_identifier);

        background_texture.loadFromMemory(optional_x->data(), optional_x->size());
        background_sprite.setTexture(background_texture);
    }

    void battle_stage_scene_node::draw_this(sf::RenderTarget& render_target, sf::RenderStates render_states) const
    {
        render_target.draw(background_sprite, render_states);
    }

    void battle_stage_scene_node::process_action_this(const float incoming_frame_delta_time, const action incoming_action)
    {

    }

    void battle_stage_scene_node::process_event_this(const sf::Event& event)
    {

    }

    void battle_stage_scene_node::process_frame_this(const float incoming_frame_delta_time)
    {

    }

    sf::FloatRect battle_stage_scene_node::get_local_bounds() const
    {
        return background_sprite.getLocalBounds();
    }
}
