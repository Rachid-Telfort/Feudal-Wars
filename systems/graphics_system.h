#ifndef GRAPHICS_SYSTEM_H_INCLUDED
#define GRAPHICS_SYSTEM_H_INCLUDED

#include<cstddef>
#include<functional>
#include<memory>
#include<random>
#include<unordered_map>
#include<utility>
#include<vector>
#include<boost/optional/optional.hpp>
#include<SFML/Config.hpp>
#include<SFML/Graphics/Image.hpp>
#include<SFML/Graphics/RenderWindow.hpp>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/System/Vector2.hpp>
#include<SFML/Window/Event.hpp>
#include<SFML/Window/Keyboard.hpp>
#include<SFML/Window/VideoMode.hpp>
#include<SFML/Window/WindowStyle.hpp>
#include<character_select_scene.h>
#include<game_scene.h>
#include<graphics_resource_holder.h>
#include<multiplayer_join_scene.h>
#include<interactable.h>
#include<scene.h>
#include<settings_scene.h>
#include<settings_system.h>
#include<stage_select_scene.h>
#include<start_scene.h>
#include<texture_sheet.h>

namespace feudal_wars
{
    class graphics_system:public interactable
    {
        const settings_system& options_system;
        sf::RenderWindow& render_window;
        bool full_screen_mode;
        const std::function<void(scene&)> process_scene_action_function;
        const std::vector<sf::VideoMode>& full_screen_modes;
        std::default_random_engine window_icon_index_engine;
        std::random_device window_icon_index_device;
        std::uniform_int_distribution<std::vector<sf::Image>::size_type> window_icon_index_distribution;
        std::unique_ptr<scene> current_scene;
        std::vector<sf::Image> window_icons;
        const sf::VideoMode desktop_video_mode;

        void create_desktop_render_window();
        void create_full_screen_render_window();
        void create_render_window();
        void create_window_icons();
        void process_key_event(const sf::Event::KeyEvent&);
        void process_scene(const std::function<void(scene&)>&);
        void transition_scene(const action);
        std::vector<sf::Image>::size_type generate_window_icon_index();

        public:
            graphics_system(sf::RenderWindow&, const settings_system&);
            graphics_system(const graphics_system&)=delete;
            graphics_system(graphics_system&&)=delete;

            graphics_system& operator=(const graphics_system&)=delete;
            graphics_system& operator=(graphics_system&&)=delete;

            void process_action(const float, const action);
            void process_event(const sf::Event&);
            void process_frame(const float);
            void render();
            void start();
    };
}

#endif
