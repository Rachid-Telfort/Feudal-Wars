#ifndef DECISION_TREE_H_INCLUDED
#define DECISION_TREE_H_INCLUDED

#include<functional>
#include<boost/optional/optional.hpp>
#include<SFML/System/Vector2.hpp>
#include<battle_character.h>
#include<interactable.h>

namespace feudal_wars
{
    struct decision_tree
    {
        struct decision_criteria
        {
            std::optional<sf::Vector2f> last_position;
            std::optional<battle_character::animation_state> last_animation_state;
            std::optional<interactable::action> last_action;

            decision_criteria(const decision_criteria&)=delete;
            decision_criteria(decision_criteria&&)=delete;

            decision_criteria& operator=(const decision_criteria&)=delete;
            decision_criteria& operator=(decision_criteria&&)=delete;
        };

        class decision_tree_node
        {
            std::function<std::optional<std::vector<decision_tree_node>::size_type>(const decision_criteria&)> child_position_function;
            std::function<std::optional<interactable::action>(const decision_criteria&)> action_function;
            std::vector<decision_tree_node> children;

            public:
                decision_tree_node();
                decision_tree_node(const std::function<std::optional<std::vector<decision_tree_node>::size_type>(const decision_criteria&)>&, const std::function<std::optional<interactable::action>(const decision_criteria&)>&, const std::vector<decision_tree_node>&);
                decision_tree_node(std::function<std::optional<std::vector<decision_tree_node>::size_type>(const decision_criteria&)>&&, std::function<std::optional<interactable::action>(const decision_criteria&)>&&, std::vector<decision_tree_node>&&);
                decision_tree_node(const decision_tree_node&)=delete;
                decision_tree_node(decision_tree_node&&)=delete;

                decision_tree_node& operator=(const decision_tree_node&)=delete;
                decision_tree_node& operator=(decision_tree_node&&)=delete;

                void set_arguments(const std::function<std::optional<std::vector<decision_tree_node>::size_type>(const decision_criteria&)>&, const std::function<std::optional<interactable::action>(const decision_criteria&)>&, const std::vector<decision_tree_node>&);
                void set_arguments(std::function<std::optional<std::vector<decision_tree_node>::size_type>(const decision_criteria&)>&&, std::function<std::optional<interactable::action>(const decision_criteria&)>&&, std::vector<decision_tree_node>&&);
                std::optional<interactable::action> next_decision(const decision_criteria&);
        };

        private:
            decision_tree_node root;
    };
}

#endif
