#ifndef SCENE_NODE_H_INCLUDED
#define SCENE_NODE_H_INCLUDED

#include<algorithm>
#include<memory>
#include<utility>
#include<vector>
#include<boost/optional/optional.hpp>
#include<SFML/Graphics/Drawable.hpp>
#include<SFML/Graphics/RenderStates.hpp>
#include<SFML/Graphics/RenderTarget.hpp>
#include<SFML/Graphics/Transform.hpp>
#include<SFML/Graphics/Transformable.hpp>
#include<interactable.h>

namespace feudal_wars
{
    struct scene_node:sf::Drawable, sf::Transformable, interactable
    {
        enum class identifier:unsigned char
        {
            background_layer,
            battle_stage,
            drawable_adapter,
            interactable_layer,
            non_interactable_layer,
            user_interface_layer
        };

        private:
            identifier this_identifier;

            virtual void draw_this(sf::RenderTarget&, sf::RenderStates) const=0;
            virtual void process_action_this(const float, const action)=0;
            virtual void process_event_this(const sf::Event&)=0;
            virtual void process_frame_this(const float)=0;

            void draw(sf::RenderTarget&, sf::RenderStates) const;

        protected:
            boost::optional<scene_node&> parent;
            std::vector<std::unique_ptr<scene_node>> children;

        public:
            virtual ~scene_node()=default;

            scene_node(const identifier);
            scene_node(const scene_node&)=delete;
            scene_node(scene_node&&)=delete;

            scene_node& operator=(const scene_node&)=delete;
            scene_node& operator=(scene_node&&)=delete;

            bool insert_child(std::unique_ptr<scene_node>&);
            bool remove_child(const identifier);
            void process_action(const float, const action);
            void process_event(const sf::Event&);
            void process_frame(const float);
            void propagate_next_action();
    };
}

#endif
