#ifndef LAYER_SCENE_NODE_H_INCLUDED
#define LAYER_SCENE_NODE_H_INCLUDED

#include<SFML/Graphics/RenderStates.hpp>
#include<SFML/Graphics/RenderTarget.hpp>
#include<SFML/Window/Event.hpp>
#include<interactable.h>
#include<scene_node.h>

namespace feudal_wars
{
    class layer_scene_node:public scene_node
    {
        void draw_this(sf::RenderTarget&, sf::RenderStates) const;
        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);
        void process_frame_this(const float);

        public:
            layer_scene_node(const identifier);
            layer_scene_node(const layer_scene_node&)=delete;
            layer_scene_node(layer_scene_node&&)=delete;

            layer_scene_node& operator=(const layer_scene_node&)=delete;
            layer_scene_node& operator=(layer_scene_node&&)=delete;
    };
}

#endif
