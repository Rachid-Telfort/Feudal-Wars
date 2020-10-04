#include<layer_scene_node.h>

namespace feudal_wars
{
    void layer_scene_node::draw_this(sf::RenderTarget& render_target, sf::RenderStates render_states) const{}
    void layer_scene_node::process_action_this(const float incoming_frame_delta_time, const action incoming_action){}
    void layer_scene_node::process_event_this(const sf::Event& incoming_event){}
    void layer_scene_node::process_frame_this(const float incoming_frame_delta_time){}

    layer_scene_node::layer_scene_node(const identifier incoming_identifier):scene_node(incoming_identifier){}
}
