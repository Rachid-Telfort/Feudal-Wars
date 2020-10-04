#include<scene.h>

namespace feudal_wars
{
    void scene::process_scene_graph(const std::function<void(layer_scene_node&)>& incoming_scene_graph_function)
    {
        const std::array<layer_scene_node, 4u>::iterator scene_graph_finish(scene_graph.end());
        const std::array<layer_scene_node, 4u>::iterator scene_graph_start(scene_graph.begin());
        std::for_each(scene_graph_start, scene_graph_finish, incoming_scene_graph_function);
    }

    std::array<layer_scene_node, 4u>::size_type scene::get_layer_index(const layer incoming_layer) const
    {
        const unsigned char layer_character(static_cast<unsigned char>(incoming_layer));
        const std::array<layer_scene_node, 4u>::size_type layer_index(static_cast<std::array<layer_scene_node, 4u>::size_type>(layer_character));

        return layer_index;
    }

    boost::optional<layer_scene_node&> scene::get_layer_scene_node(const layer incoming_layer)
    {
        const std::array<layer_scene_node, 4u>::size_type layer_index(get_layer_index(incoming_layer));
        const std::array<layer_scene_node, 4u>::size_type scene_graph_size(scene_graph.size());
        const bool layer_index_valid(layer_index<scene_graph_size);
        if(!layer_index_valid)
        {
            return boost::none;
        }

        layer_scene_node& incoming_layer_scene_node(scene_graph[layer_index]);

        boost::optional<layer_scene_node&> optional_layer_scene_node(incoming_layer_scene_node);

        return optional_layer_scene_node;
    }

    scene::scene():
    scene_graph
    {{
        {scene_node::identifier::background_layer},
        {scene_node::identifier::non_interactable_layer},
        {scene_node::identifier::interactable_layer},
        {scene_node::identifier::user_interface_layer}
    }},
    this_current_state(state::start),
    this_previous_state(state::start){}

    bool scene::insert_child(std::unique_ptr<scene_node>& incoming_child, const layer incoming_layer)
    {
        const boost::optional<layer_scene_node&> optional_layer_scene_node(get_layer_scene_node(incoming_layer));
        if(!optional_layer_scene_node)
        {
            return false;
        }

        layer_scene_node& insert_layer_scene_node(*optional_layer_scene_node);
        const bool child_inserted(insert_layer_scene_node.insert_child(incoming_child));

        return child_inserted;
    }

    bool scene::remove_child(const scene_node::identifier incoming_identifier, const layer incoming_layer)
    {
        const boost::optional<layer_scene_node&> optional_layer_scene_node(get_layer_scene_node(incoming_layer));
        if(!optional_layer_scene_node)
        {
            return false;
        }

        layer_scene_node& remove_layer_scene_node(*optional_layer_scene_node);
        const bool child_removed(remove_layer_scene_node.remove_child(incoming_identifier));

        return child_removed;
    }

    void scene::draw(sf::RenderTarget& render_target, sf::RenderStates render_states) const
    {
        const std::array<layer_scene_node, 4u>::const_iterator scene_graph_finish(scene_graph.cend());
        const std::array<layer_scene_node, 4u>::const_iterator scene_graph_start(scene_graph.cbegin());
        std::for_each
        (
            scene_graph_start,
            scene_graph_finish,
            [&render_target, render_states](const layer_scene_node& incoming_layer_scene_node)
            {
                render_target.draw(incoming_layer_scene_node, render_states);
            }
        );
    }

    void scene::process_action(const float incoming_frame_delta_time, const action incoming_action)
    {
        process_action_this(incoming_frame_delta_time, incoming_action);
        process_scene_graph
        (
            [incoming_frame_delta_time, incoming_action](layer_scene_node& incoming_layer_scene_node)
            {
                incoming_layer_scene_node.process_action(incoming_frame_delta_time, incoming_action);
            }
        );
    }

    void scene::process_event(const sf::Event& incoming_event)
    {
        process_event_this(incoming_event);
        process_scene_graph
        (
            [&incoming_event](layer_scene_node& incoming_layer_scene_node)
            {
                incoming_layer_scene_node.process_event(incoming_event);
            }
        );
    }

    void scene::process_frame(const float incoming_frame_delta_time)
    {
        process_scene_graph
        (
            [incoming_frame_delta_time](layer_scene_node& incoming_layer_scene_node)
            {
                incoming_layer_scene_node.process_frame(incoming_frame_delta_time);
            }
        );
    }

    void scene::propagate_next_action()
    {
        process_scene_graph
        (
            [](layer_scene_node& incoming_layer_scene_node)
            {
                incoming_layer_scene_node.propagate_next_action();
            }
        );
    }
}
