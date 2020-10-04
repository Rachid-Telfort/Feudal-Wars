#ifndef BATTLE_STAGE_H_INCLUDED
#define BATTLE_STAGE_H_INCLUDED

#include<map>
#include<memory>
#include<vector>
#include<SFML/System/Vector2.hpp>
#include<interactable.h>
#include<scene.h>
#include<scene_node.h>

namespace feudal_wars
{
    class battle_stage
    {
        const interactable::action music_action;

        protected:
            const std::vector<sf::Vector2f> starting_positions;
            const sf::Vector2f horizontal_boundaries;
            const sf::Vector2f vertical_boundaries;
            std::multimap<scene::layer, std::unique_ptr<scene_node>> layered_stage_scene_nodes;

        public:
            battle_stage(const std::vector<sf::Vector2f>&, const sf::Vector2f&, const sf::Vector2f&, const interactable::action);
            battle_stage(const battle_stage&)=delete;
            battle_stage(battle_stage&&)=delete;

            battle_stage& operator=(const battle_stage&)=delete;
            battle_stage& operator=(battle_stage&&)=delete;

            virtual ~battle_stage()=default;
            virtual void create(const scene::state&)=0;

            const std::vector<sf::Vector2f>& get_starting_positions() const;
            const sf::Vector2f& get_horizontal_boundaries() const;
            const sf::Vector2f& get_vertical_boundaries() const;

            std::multimap<scene::layer, std::unique_ptr<scene_node>> get_layered_stage_scene_nodes();
            interactable::action get_music_action() const;
    };
}

#endif
