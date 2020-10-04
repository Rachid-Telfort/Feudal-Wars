#ifndef CHARACTER_SELECT_SCENE_H_INCLUDED
#define CHARACTER_SELECT_SCENE_H_INCLUDED

#include<algorithm>
#include<array>
#include<chrono>
#include<cstddef>
#include<memory>
#include<unordered_map>
#include<tuple>
#include<utility>
#include<boost/optional/optional.hpp>
#include<SFML/Graphics/Font.hpp>
#include<SFML/Graphics/RenderStates.hpp>
#include<SFML/Graphics/RenderTarget.hpp>
#include<SFML/Graphics/RenderWindow.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/System/String.hpp>
#include<SFML/Window/VideoMode.hpp>
#include<drawable_adapter_scene_node.h>
#include<graphics_resource_holder.h>
#include<scene.h>
#include<settings_system.h>
#include<texture_sheet.h>

namespace feudal_wars
{
    class character_select_scene:public scene
    {
        std::array<bool, 3u> character_select_scene_transitions;
        std::array<bool, 5u> character_select_sprite_blackeners;
        std::array<bool, 5u> character_select_sprite_hovers;
        std::array<std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>, 5u> character_select_sprite_time_point_pairs;
        std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u> character_select_sprite_scene_nodes;
        std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u> character_select_description_scene_nodes;
        std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 3u> interactable_text_scene_nodes;
        std::array<sf::Texture, 5u> character_select_textures;
        std::chrono::duration<float> character_background_sprite_duration;
        std::chrono::duration<float> character_select_sprite_duration;
        std::chrono::duration<float> character_select_text_duration;
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>> character_background_sprite_time_points;
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>> character_select_text_time_points;
        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>> character_select_background_scene_node;
        std::unique_ptr<drawable_adapter_scene_node<sf::Text>> character_select_text_scene_node;
        std::size_t character_select_text_outline_switch_count;
        boost::optional<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type> character_select_pressed_index;
        boost::optional<std::size_t> button_pressed_index;
        const sf::RenderWindow& render_window;
        sf::Font character_select_scene_font;
        sf::Texture background_texture;
        const settings_system& options_system;

        bool process_interactable_text_hovered(const sf::Event::MouseMoveEvent&, const sf::Text&) const;
        bool process_interactable_text_pressed(const sf::Event::MouseButtonEvent&, const sf::Text&) const;
        bool process_sprite_hovered(const sf::Event::MouseMoveEvent&, const sf::Sprite&) const;
        bool process_sprite_pressed(const sf::Event::MouseButtonEvent&, const sf::Sprite&) const;
        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);
        void set_character_description_text(const sf::String&, const sf::Vector2f&, sf::Text&);
        void set_character_select_background_sprite_scene_node();
        void set_character_select_description_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type, const sf::Vector2f&, const std::array<sf::String, 5u>&);
        void set_character_select_description_scene_nodes();
        void set_character_select_scene_font();
        void set_character_select_sprite_scene_nodes();
        void set_character_select_text();
        void set_character_select_texture(const std::array<sf::Texture, 5u>::size_type, const std::pair<std::size_t, std::size_t>&, const graphics_resource_holder::identifier);
        void set_character_select_textures();
        void set_first_interactable_text_scene_node();
        void set_interactable_text_scene_nodes();
        void set_second_interactable_text_scene_node();
        void set_third_interactable_text_scene_node();
        boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> get_interactable_text_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 3u>::size_type);
        sf::String get_character_select_transition_string() const;
        action get_character_select_transition_action() const;

        public:
            character_select_scene(const sf::RenderWindow&, const settings_system&);
            character_select_scene(const character_select_scene&)=delete;
            character_select_scene(character_select_scene&&)=delete;

            character_select_scene& operator=(const character_select_scene&)=delete;
            character_select_scene& operator=(character_select_scene&&)=delete;
    };
}

#endif
