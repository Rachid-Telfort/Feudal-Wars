#ifndef DRAWABLE_ADAPTER_SCENE_NODE_H_INCLUDED
#define DRAWABLE_ADAPTER_SCENE_NODE_H_INCLUDED

#include<algorithm>
#include<functional>
#include<vector>
#include<SFML/Graphics/RenderStates.hpp>
#include<SFML/Graphics/RenderTarget.hpp>
#include<SFML/Window/Event.hpp>
#include<interactable.h>
#include<scene_node.h>

namespace feudal_wars
{
    template<class drawable_type> class drawable_adapter_scene_node:public scene_node
    {
        std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>> process_draw_functions;
        std::vector<std::function<void(const float, const action, drawable_type&)>> process_action_functions;
        std::vector<std::function<void(const sf::Event&, drawable_type&)>> process_event_functions;
        std::vector<std::function<void(const float, drawable_type&)>> process_frame_functions;
        drawable_type drawable;

        void draw_this(sf::RenderTarget&, sf::RenderStates) const;
        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);
        void process_frame_this(const float);

        public:
            drawable_adapter_scene_node();
            drawable_adapter_scene_node(const std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>&, const std::vector<std::function<void(const float, const action, drawable_type&)>>&, const std::vector<std::function<void(const sf::Event&, drawable_type&)>>&, const std::vector<std::function<void(const float, drawable_type&)>>&);
            drawable_adapter_scene_node(std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>&&, std::vector<std::function<void(const float, const action, drawable_type&)>>&&, std::vector<std::function<void(const sf::Event&, drawable_type&)>>&&, std::vector<std::function<void(const float, drawable_type&)>>&&);
            drawable_adapter_scene_node(const drawable_adapter_scene_node&)=delete;
            drawable_adapter_scene_node(drawable_adapter_scene_node&&)=delete;

            const drawable_type* operator->() const;
            drawable_type* operator->();
            drawable_adapter_scene_node& operator=(const drawable_adapter_scene_node&)=delete;
            drawable_adapter_scene_node& operator=(drawable_adapter_scene_node&&)=delete;

            void insert_functions(const std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>&, const std::function<void(const float, const action, drawable_type&)>&, const std::function<void(const sf::Event&, drawable_type&)>&, const std::function<void(const float, drawable_type&)>&);
            void insert_functions(std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>&&, std::function<void(const float, const action, drawable_type&)>&&, std::function<void(const sf::Event&, drawable_type&)>&&, std::function<void(const float, drawable_type&)>&&);
            void set_functions(const std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>&, const std::vector<std::function<void(const float, const action, drawable_type&)>>&, const std::vector<std::function<void(const sf::Event&, drawable_type&)>>&, const std::vector<std::function<void(const float, drawable_type&)>>&);
            void set_functions(std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>&&, std::vector<std::function<void(const float, const action, drawable_type&)>>&&, std::vector<std::function<void(const sf::Event&, drawable_type&)>>&&, std::vector<std::function<void(const float, drawable_type&)>>&&);

            const drawable_type& get_drawable() const;
    };

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::draw_this(sf::RenderTarget& render_target, sf::RenderStates render_states) const
    {
        const typename std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>::const_iterator process_draw_functions_finish(process_draw_functions.cend());
        const typename std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>::const_iterator process_draw_functions_start(process_draw_functions.cbegin());
        std::for_each
        (
            process_draw_functions_start,
            process_draw_functions_finish,
            [this, &render_target, render_states](const std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>& process_draw_function)
            {
                if(!process_draw_function)
                {
                    return;
                }

                process_draw_function(render_target, render_states, drawable);
            }
        );
    }

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::process_action_this(const float incoming_frame_delta_time, const action incoming_action)
    {
        const typename std::vector<std::function<void(const float, const action, drawable_type&)>>::const_iterator process_action_functions_finish(process_action_functions.cend());
        const typename std::vector<std::function<void(const float, const action, drawable_type&)>>::const_iterator process_action_functions_start(process_action_functions.cbegin());
        std::for_each
        (
            process_action_functions_start,
            process_action_functions_finish,
            [this, incoming_frame_delta_time, incoming_action](const std::function<void(const float, const action, drawable_type&)>& process_action_function)
            {
                if(!process_action_function)
                {
                    return;
                }

                process_action_function(incoming_frame_delta_time, incoming_action, drawable);
            }
        );
    }

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::process_event_this(const sf::Event& incoming_event)
    {
        const typename std::vector<std::function<void(const sf::Event&, drawable_type&)>>::const_iterator process_event_functions_finish(process_event_functions.cend());
        const typename std::vector<std::function<void(const sf::Event&, drawable_type&)>>::const_iterator process_event_functions_start(process_event_functions.cbegin());
        std::for_each
        (
            process_event_functions_start,
            process_event_functions_finish,
            [this, &incoming_event](const std::function<void(const sf::Event&, drawable_type&)>& process_event_function)
            {
                if(!process_event_function)
                {
                    return;
                }

                process_event_function(incoming_event, drawable);
            }
        );
    }

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::process_frame_this(const float incoming_frame_delta_time)
    {
        const typename std::vector<std::function<void(const float, drawable_type&)>>::const_iterator process_frame_functions_finish(process_frame_functions.cend());
        const typename std::vector<std::function<void(const float, drawable_type&)>>::const_iterator process_frame_functions_start(process_frame_functions.cbegin());
        std::for_each
        (
            process_frame_functions_start,
            process_frame_functions_finish,
            [this, incoming_frame_delta_time](const std::function<void(const float, drawable_type&)>& process_frame_function)
            {
                if(!process_frame_function)
                {
                    return;
                }

                process_frame_function(incoming_frame_delta_time, drawable);
            }
        );
    }

    template<class drawable_type> drawable_adapter_scene_node<drawable_type>::drawable_adapter_scene_node():scene_node(identifier::drawable_adapter){}

    template<class drawable_type> drawable_adapter_scene_node<drawable_type>::drawable_adapter_scene_node(const std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>& process_draw_functions, const std::vector<std::function<void(const float, const action, drawable_type&)>>& process_action_functions, const std::vector<std::function<void(const sf::Event&, drawable_type&)>>& process_event_functions, const std::vector<std::function<void(const float, drawable_type&)>>& process_frame_functions)
    {
        set_functions(process_draw_functions, process_action_functions, process_event_functions, process_frame_functions);
    }

    template<class drawable_type> drawable_adapter_scene_node<drawable_type>::drawable_adapter_scene_node(std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>&& process_draw_functions, std::vector<std::function<void(const float, const action, drawable_type&)>>&& process_action_functions, std::vector<std::function<void(const sf::Event&, drawable_type&)>>&& process_event_functions, std::vector<std::function<void(const float, drawable_type&)>>&& process_frame_functions)
    {
        set_functions(std::move(process_draw_functions), std::move(process_action_functions), std::move(process_event_functions), std::move(process_frame_functions));
    }

    template<class drawable_type> const drawable_type* drawable_adapter_scene_node<drawable_type>::operator->() const
    {
        const drawable_type* drawable_type_pointer(&drawable);

        return drawable_type_pointer;
    }

    template<class drawable_type> drawable_type* drawable_adapter_scene_node<drawable_type>::operator->()
    {
        drawable_type* drawable_type_pointer(&drawable);

        return drawable_type_pointer;
    }

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::insert_functions(const std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>& incoming_process_draw_function, const std::function<void(const float, const action, drawable_type&)>& incoming_process_action_function, const std::function<void(const sf::Event&, drawable_type&)>& incoming_process_event_function, const std::function<void(const float, drawable_type&)>& incoming_process_frame_function)
    {
        process_draw_functions.push_back(incoming_process_draw_function);
        process_action_functions.push_back(incoming_process_action_function);
        process_event_functions.push_back(incoming_process_event_function);
        process_frame_functions.push_back(incoming_process_frame_function);
    }

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::insert_functions(std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>&& incoming_process_draw_function, std::function<void(const float, const action, drawable_type&)>&& incoming_process_action_function, std::function<void(const sf::Event&, drawable_type&)>&& incoming_process_event_function, std::function<void(const float, drawable_type&)>&& incoming_process_frame_function)
    {
        process_draw_functions.push_back(std::move(incoming_process_draw_function));
        process_action_functions.push_back(std::move(incoming_process_action_function));
        process_event_functions.push_back(std::move(incoming_process_event_function));
        process_frame_functions.push_back(std::move(incoming_process_frame_function));
    }

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::set_functions(const std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>& incoming_process_draw_functions, const std::vector<std::function<void(const float, const action, drawable_type&)>>& incoming_process_action_functions, const std::vector<std::function<void(const sf::Event&, drawable_type&)>>& incoming_process_event_functions, const std::vector<std::function<void(const float, drawable_type&)>>& incoming_process_frame_functions)
    {
        process_draw_functions=incoming_process_draw_functions;
        process_action_functions=incoming_process_action_functions;
        process_event_functions=incoming_process_event_functions;
        process_frame_functions=incoming_process_frame_functions;
    }

    template<class drawable_type> void drawable_adapter_scene_node<drawable_type>::set_functions(std::vector<std::function<void(sf::RenderTarget&, sf::RenderStates, const drawable_type&)>>&& incoming_process_draw_functions, std::vector<std::function<void(const float, const action, drawable_type&)>>&& incoming_process_action_functions, std::vector<std::function<void(const sf::Event&, drawable_type&)>>&& incoming_process_event_functions, std::vector<std::function<void(const float, drawable_type&)>>&& incoming_process_frame_functions)
    {
        process_draw_functions=std::move(incoming_process_draw_functions);
        process_action_functions=std::move(incoming_process_action_functions);
        process_event_functions=std::move(incoming_process_event_functions);
        process_frame_functions=std::move(incoming_process_frame_functions);
    }

    template<class drawable_type> const drawable_type& drawable_adapter_scene_node<drawable_type>::get_drawable() const
    {
        return drawable;
    }
}

#endif
