#ifndef START_SCENE_H_INCLUDED
#define START_SCENE_H_INCLUDED

#include<algorithm>
#include<array>
#include<chrono>
#include<functional>
#include<memory>
#include<random>
#include<unordered_map>
#include<utility>
#include<boost/logic/tribool.hpp>
#include<boost/optional/optional.hpp>
#include<SFML/Config.hpp>
#include<SFML/Graphics/RenderWindow.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/Graphics/View.hpp>
#include<SFML/System/Vector2.hpp>
#include<SFML/Window/Mouse.hpp>
#include<SFML/Window/VideoMode.hpp>
#include<drawable_adapter_scene_node.h>
#include<graphics_resource_holder.h>
#include<scene.h>

namespace feudal_wars
{
    class start_scene:public scene
    {
        bool first_layer_animating;
        float first_layer_animation_start_time;
        float fifth_layer_animation_start_time;
        float third_layer_animation_start_time;
        float title_text_zoom_count;
        std::chrono::duration<float> first_layer_duration;
        std::chrono::duration<float> fifth_layer_duration;
        std::chrono::duration<float> third_layer_duration;
        std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u> background_layer_scene_nodes;
        std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u> interactable_text_scene_nodes;
        std::array<sf::Texture, 6u> background_layer_textures;
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>> fifth_layer_time_points;
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>> first_layer_time_points;
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>> third_layer_time_points;
        std::default_random_engine random_engine;
        std::size_t texture_switch_count;
        std::uniform_real_distribution<float> random_distribution;
        std::unique_ptr<drawable_adapter_scene_node<sf::Text>> title_text_scene_node;
        boost::optional<std::size_t> button_pressed_index;
        boost::tribool fifth_layer_fading_state;
        boost::tribool third_layer_fading_state;
        const sf::RenderWindow& render_window;
        sf::Font start_scene_font;
        sf::View title_text_view;

        bool process_interactable_text_hovered(const sf::Event::MouseMoveEvent&, const sf::Text&) const;
        bool process_interactable_text_pressed(const sf::Event::MouseButtonEvent&, const sf::Text&) const;
        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);
        void process_draw_title_text(sf::RenderTarget&, sf::RenderStates, const sf::Text&, const state);
        void set_background_layers();
        void set_background_layer_textures();
        void set_interactable_texts();
        void set_fifth_background_layer();
        void set_first_background_layer();
        void set_first_interactable_text();
        void set_fourth_background_layer();
        void set_fourth_interactable_text();
        void set_second_background_layer();
        void set_second_interactable_text();
        void set_start_scene_font();
        void set_third_background_layer();
        void set_third_interactable_text();
        void set_title_text();
        boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> get_background_layer_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type);
        boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> get_interactable_text_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type);

        public:
            start_scene(const sf::RenderWindow&);
            start_scene(const start_scene&)=delete;
            start_scene(start_scene&&)=delete;

            start_scene& operator=(const start_scene&)=delete;
            start_scene& operator=(start_scene&&)=delete;
    };
}

#endif
