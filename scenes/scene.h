#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include<algorithm>
#include<array>
#include<memory>
#include<boost/optional/optional.hpp>
#include<SFML/Window/Event.hpp>
#include<layer_scene_node.h>
#include<scene_node.h>

namespace feudal_wars
{
    struct scene:sf::Drawable, interactable
    {
        enum class layer:unsigned char
        {
            background=0u,
            interactable=2u,
            non_interactable=1u,
            user_interface=3u
        };

        enum class state:unsigned char
        {
            active,
            pause,
            start,
            transition
        };

        private:
            std::array<layer_scene_node, 4u> scene_graph;

            virtual void process_action_this(const float, const action)=0;
            virtual void process_event_this(const sf::Event&)=0;

            void process_scene_graph(const std::function<void(layer_scene_node&)>&);
            std::array<layer_scene_node, 4u>::size_type get_layer_index(const layer) const;
            boost::optional<layer_scene_node&> get_layer_scene_node(const layer);

        protected:
            state this_current_state;
            state this_previous_state;

        public:
            virtual ~scene()=default;

            scene();
            scene(const scene&)=delete;
            scene(scene&&)=delete;

            scene& operator=(const scene&)=delete;
            scene& operator=(scene&&)=delete;

            bool insert_child(std::unique_ptr<scene_node>&, const layer);
            bool remove_child(const scene_node::identifier, const layer);
            void draw(sf::RenderTarget&, sf::RenderStates) const;
            void process_action(const float, const action);
            void process_event(const sf::Event&);
            void process_frame(const float);
            void propagate_next_action();
    };
}

#endif
