#include<scene_node.h>

namespace feudal_wars
{
    scene_node::scene_node(const identifier incoming_identifier):this_identifier(incoming_identifier){}

    void scene_node::draw(sf::RenderTarget& render_target, sf::RenderStates render_states) const
    {
        const sf::Transform& this_transform(getTransform());
        render_states.transform*=this_transform;
        draw_this(render_target, render_states);

        const std::vector<std::unique_ptr<scene_node>>::const_iterator children_finish(children.cend());
        const std::vector<std::unique_ptr<scene_node>>::const_iterator children_start(children.cbegin());
        std::for_each
        (
            children_start,
            children_finish,
            [&render_target, render_states](const std::unique_ptr<scene_node>& incoming_child)
            {
                if(!incoming_child)
                {
                    return;
                }

                const scene_node& child_node(*incoming_child);
                render_target.draw(child_node, render_states);
            }
        );
    }

    bool scene_node::insert_child(std::unique_ptr<scene_node>& incoming_child)
    {
        if(!incoming_child)
        {
            return false;
        }

        incoming_child->parent=*this;
        children.push_back(std::move(incoming_child));

        return true;
    }

    bool scene_node::remove_child(const identifier incoming_identifier)
    {
        bool child_removed(false);
        const std::vector<std::unique_ptr<scene_node>>::const_iterator children_finish(children.cend());
        const std::vector<std::unique_ptr<scene_node>>::const_iterator children_start(children.cbegin());
        const std::vector<std::unique_ptr<scene_node>>::const_iterator children_iterator=std::find_if
        (
            children_start,
            children_finish,
            [incoming_identifier](const std::unique_ptr<scene_node>& incoming_child)
            {
                bool child_found(false);
                if(incoming_child)
                {
                    child_found=incoming_child->this_identifier==incoming_identifier;
                }

                return child_found;
            }
        );

        child_removed=children_iterator!=children_finish;
        if(child_removed)
        {
            children.erase(children_iterator);
        }

        return child_removed;
    }

    void scene_node::process_action(const float incoming_frame_delta_time, const action incoming_action)
    {
        process_action_this(incoming_frame_delta_time, incoming_action);

        const std::vector<std::unique_ptr<scene_node>>::iterator children_finish(children.end());
        const std::vector<std::unique_ptr<scene_node>>::iterator children_start(children.begin());
        std::for_each
        (
            children_start,
            children_finish,
            [incoming_frame_delta_time, incoming_action](std::unique_ptr<scene_node>& incoming_child)
            {
                if(!incoming_child)
                {
                    return;
                }

                incoming_child->process_action(incoming_frame_delta_time, incoming_action);
            }
        );
    }

    void scene_node::process_event(const sf::Event& incoming_event)
    {
        process_event_this(incoming_event);

        const std::vector<std::unique_ptr<scene_node>>::iterator children_finish(children.end());
        const std::vector<std::unique_ptr<scene_node>>::iterator children_start(children.begin());
        std::for_each
        (
            children_start,
            children_finish,
            [&incoming_event](std::unique_ptr<scene_node>& incoming_child)
            {
                if(!incoming_child)
                {
                    return;
                }

                incoming_child->process_event(incoming_event);
            }
        );
    }

    void scene_node::process_frame(const float incoming_frame_delta_time)
    {
        process_frame_this(incoming_frame_delta_time);

        const std::vector<std::unique_ptr<scene_node>>::iterator children_finish(children.end());
        const std::vector<std::unique_ptr<scene_node>>::iterator children_start(children.begin());
        std::for_each
        (
            children_start,
            children_finish,
            [incoming_frame_delta_time](std::unique_ptr<scene_node>& incoming_child)
            {
                if(!incoming_child)
                {
                    return;
                }

                incoming_child->process_frame(incoming_frame_delta_time);
            }
        );
    }

    void scene_node::propagate_next_action()
    {
        if(parent)
        {
            boost::optional<action> optional_child_next_action;
            action child_next_action;
            optional_child_next_action=get_next_action();
            if(optional_child_next_action)
            {
                child_next_action=*optional_child_next_action;
                parent->actions.push(child_next_action);
            }
        }

        const std::vector<std::unique_ptr<scene_node>>::iterator children_finish(children.end());
        const std::vector<std::unique_ptr<scene_node>>::iterator children_start(children.begin());
        std::for_each
        (
            children_start,
            children_finish,
            [](std::unique_ptr<scene_node>& incoming_child)
            {
                if(!incoming_child)
                {
                    return;
                }

                incoming_child->propagate_next_action();
            }
        );
    }
}
