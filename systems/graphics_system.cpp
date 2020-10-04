#include<graphics_system.h>

namespace feudal_wars
{
    void graphics_system::create_desktop_render_window()
    {
        const std::vector<sf::Image>::size_type window_icon_index(generate_window_icon_index());
        const sf::Image& window_icon(window_icons[window_icon_index]);
        const sf::Vector2u window_icon_size(window_icon.getSize());
        const unsigned int window_icon_height(window_icon_size.y);
        const unsigned int window_icon_width(window_icon_size.x);
        const sf::Uint8* const window_icon_start(window_icon.getPixelsPtr());

        render_window.create(desktop_video_mode, "Feudal Wars");
        render_window.setIcon(window_icon_width, window_icon_height, window_icon_start);
    }

    void graphics_system::create_full_screen_render_window()
    {
        const bool full_screen_modes_available(!full_screen_modes.empty());
        if(!full_screen_modes_available)
        {
            full_screen_mode=false;

            return;
        }

        const sf::VideoMode& full_screen_video_mode(full_screen_modes.front());
        render_window.create(full_screen_video_mode, "Feudal Wars", sf::Style::Fullscreen);
    }

    void graphics_system::create_render_window()
    {
        if(full_screen_mode)
        {
            create_full_screen_render_window();
        }

        else
        {
            create_desktop_render_window();
        }
    }

    void graphics_system::create_window_icons()
    {
        const boost::optional<const std::vector<unsigned char>&> optional_resouce_data(graphics_resource_holder::resources[graphics_resource_holder::identifier::feudal_wars_window_icons]);
        if(!optional_resouce_data)
        {
            return;
        }

        const std::pair<std::size_t, std::size_t> window_icon_frame_dimensions(48u, 65u);
        const std::vector<unsigned char>& resource_data(*optional_resouce_data);
        const texture_sheet window_icons_sheet(window_icon_frame_dimensions, resource_data);
        const texture_sheet::texture_sheet_iterator window_icons_sheet_finish(window_icons_sheet.end());
        const texture_sheet::texture_sheet_iterator window_icons_sheet_start(window_icons_sheet.begin());
        std::for_each
        (
            window_icons_sheet_start,
            window_icons_sheet_finish,
            [this](const sf::Texture& incoming_texture)
            {
                const sf::Image window_icons_image(incoming_texture.copyToImage());
                window_icons.push_back(window_icons_image);
            }
        );
    }

    void graphics_system::process_key_event(const sf::Event::KeyEvent& incoming_key_event)
    {
        const sf::Keyboard::Key key(incoming_key_event.code);
        switch(key)
        {
            case sf::Keyboard::Escape:
            {
                render_window.close();

                break;
            }

            case sf::Keyboard::F11:
            {
                full_screen_mode=!full_screen_mode;
                create_render_window();

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void graphics_system::process_scene(const std::function<void(scene&)>& incoming_process_scene_function)
    {
        if(!current_scene)
        {
            return;
        }

        if(!incoming_process_scene_function)
        {
            return;
        }

        scene& processible_scene(*current_scene);
        incoming_process_scene_function(processible_scene);
    }

    void graphics_system::transition_scene(const action incoming_action)
    {
        switch(incoming_action)
        {
            case action::transition_character_select_scene:
            {
                actions.push(action::play_character_select_music);
                current_scene=std::make_unique<character_select_scene>(render_window, options_system);

                break;
            }

            case action::transition_game_scene:
            {
                current_scene=std::make_unique<game_scene>(render_window, options_system);

                break;
            }

            case action::transition_multiplayer_join_scene:
            {
                current_scene=std::make_unique<multiplayer_join_scene>();

                break;
            }

            case action::transition_settings_scene:
            {
                current_scene=std::make_unique<settings_scene>();

                break;
            }

            case action::transition_stage_select_scene:
            {
                current_scene=std::make_unique<stage_select_scene>();

                break;
            }

            case action::transition_start_scene:
            {
                actions.push(action::character_unselected);
                actions.push(action::play_start_music);
                current_scene=std::make_unique<start_scene>(render_window);
            }

            default:
            {
                break;
            }
        }
    }

    std::vector<sf::Image>::size_type graphics_system::generate_window_icon_index()
    {
        window_icon_index_engine.seed(window_icon_index_device());
        const std::vector<sf::Image>::size_type window_icon_index(window_icon_index_distribution(window_icon_index_engine));

        return window_icon_index;
    }

    graphics_system::graphics_system(sf::RenderWindow& incoming_render_window, const settings_system& incoming_settings_system):
    options_system(incoming_settings_system),
    render_window(incoming_render_window),
    full_screen_mode(false),
    process_scene_action_function
    (
        [this](scene& processible_scene)
        {
            processible_scene.propagate_next_action();
            const boost::optional<action> optional_next_action(processible_scene.get_next_action());
            if(optional_next_action)
            {
                const action next_action(*optional_next_action);
                actions.push(next_action);
            }
        }
    ),
    full_screen_modes(sf::VideoMode::getFullscreenModes()),
    window_icon_index_distribution(0u, 11u),
    current_scene(std::make_unique<start_scene>(render_window)),
    desktop_video_mode(sf::VideoMode::getDesktopMode()){}

    void graphics_system::process_action(const float incoming_frame_delta_time, const action incoming_action)
    {
        switch(incoming_action)
        {
            case action::quit_application:
            {
                render_window.close();

                return;
            }

            default:
            {
                break;
            }
        }

        process_scene
        (
            [this, incoming_frame_delta_time, incoming_action](scene& processible_scene)
            {
                processible_scene.process_action(incoming_frame_delta_time, incoming_action);
                process_scene_action_function(processible_scene);
            }
        );

        transition_scene(incoming_action);
    }

    void graphics_system::process_event(const sf::Event& incoming_event)
    {
        process_scene
        (
            [this, &incoming_event](scene& processible_scene)
            {
                processible_scene.process_event(incoming_event);
                process_scene_action_function(processible_scene);
            }
        );

        const sf::Event::EventType event_type(incoming_event.type);
        switch(event_type)
        {
            case sf::Event::Closed:
            {
                render_window.close();

                break;
            }

            case sf::Event::KeyPressed:
            {
                const sf::Event::KeyEvent key_event(incoming_event.key);
                process_key_event(key_event);

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void graphics_system::process_frame(const float incoming_frame_delta_time)
    {
        process_scene
        (
            [this, incoming_frame_delta_time](scene& processible_scene)
            {
                processible_scene.process_frame(incoming_frame_delta_time);
                process_scene_action_function(processible_scene);
            }
        );
    }

    void graphics_system::render()
    {
        render_window.clear();
        process_scene
        (
            [this](scene& processible_scene)
            {
                render_window.draw(processible_scene);
            }
        );

        render_window.display();
    }

    void graphics_system::start()
    {
        render_window.setFramerateLimit(0u);
        render_window.setVerticalSyncEnabled(true);
        create_window_icons();
        create_render_window();
    }
}
