#include<start_scene.h>

namespace feudal_wars
{
    bool start_scene::process_interactable_text_hovered(const sf::Event::MouseMoveEvent& incoming_mouse_move_event, const sf::Text& incoming_text) const
    {
        const sf::Vector2i mouse_move_coordinate(incoming_mouse_move_event.x, incoming_mouse_move_event.y);
        const sf::Vector2f mapped_mouse_button_coordinate(render_window.mapPixelToCoords(mouse_move_coordinate));
        const sf::FloatRect text_global_bounds(incoming_text.getGlobalBounds());
        const bool text_hovered(text_global_bounds.contains(mapped_mouse_button_coordinate));

        return text_hovered;
    }

    bool start_scene::process_interactable_text_pressed(const sf::Event::MouseButtonEvent& incoming_mouse_button_event, const sf::Text& incoming_text) const
    {
        const sf::Mouse::Button mouse_button(incoming_mouse_button_event.button);
        switch(mouse_button)
        {
            case sf::Mouse::Left:
            {
                const sf::Vector2i mouse_button_coordinate(incoming_mouse_button_event.x, incoming_mouse_button_event.y);
                const sf::Vector2f mapped_mouse_button_coordinate(render_window.mapPixelToCoords(mouse_button_coordinate));
                const sf::FloatRect text_global_bounds(incoming_text.getGlobalBounds());
                const bool text_pressed(text_global_bounds.contains(mapped_mouse_button_coordinate));

                return text_pressed;
            }

            default:
            {
                break;
            }
        }

        return false;
    }

    void start_scene::process_action_this(const float incoming_frame_delta_time, const action incoming_action){}

    void start_scene::process_event_this(const sf::Event& incoming_event)
    {
        const sf::Event::EventType event_type(incoming_event.type);
        switch(event_type)
        {
            case sf::Event::GainedFocus:
            {
                actions.push(interactable::action::resume_audio);
                this_current_state=this_previous_state;

                break;
            }

            case sf::Event::LostFocus:
            {
                this_previous_state=this_current_state;
                this_current_state=state::pause;

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void start_scene::process_draw_title_text(sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text, const state incoming_state)
    {
        switch(incoming_state)
        {
            case state::transition:
            {
                const sf::View& render_target_view(incoming_render_target.getDefaultView());
                incoming_render_target.setView(title_text_view);
                incoming_render_target.draw(incoming_text, incoming_render_states);
                incoming_render_target.setView(render_target_view);

                break;
            }

            default:
            {
                incoming_render_target.draw(incoming_text, incoming_render_states);

                break;
            }
        }
    }

    void start_scene::set_background_layers()
    {
        set_first_background_layer();
        set_second_background_layer();
        set_third_background_layer();
        set_fourth_background_layer();
        set_fifth_background_layer();
    }

    void start_scene::set_background_layer_textures()
    {
        const std::unordered_map<std::size_t, graphics_resource_holder::identifier> background_layer_identifier_library
        {
            {0u, graphics_resource_holder::identifier::start_scene_first_layer},
            {1u, graphics_resource_holder::identifier::start_scene_second_layer},
            {2u, graphics_resource_holder::identifier::start_scene_third_layer},
            {3u, graphics_resource_holder::identifier::start_scene_fourth_layer},
            {4u, graphics_resource_holder::identifier::start_scene_fifth_layer}
        };

        const std::unordered_map<std::size_t, graphics_resource_holder::identifier>::const_iterator background_layer_identifier_library_finish(background_layer_identifier_library.cend());
        const std::unordered_map<std::size_t, graphics_resource_holder::identifier>::const_iterator background_layer_identifier_library_start(background_layer_identifier_library.cbegin());
        std::for_each
        (
            background_layer_identifier_library_start,
            background_layer_identifier_library_finish,
            [this](const std::unordered_map<std::size_t, graphics_resource_holder::identifier>::value_type& incoming_background_layer_index_identifier)
            {
                const graphics_resource_holder::identifier background_layer_identifier(incoming_background_layer_index_identifier.second);
                const boost::optional<const std::vector<unsigned char>&> optional_background_layer_data(graphics_resource_holder::resources[background_layer_identifier]);
                if(!optional_background_layer_data)
                {
                    return;
                }

                const std::array<sf::Texture, 5u>::size_type background_layer_index(static_cast<std::array<sf::Texture, 5u>::size_type>(incoming_background_layer_index_identifier.first));
                const std::vector<unsigned char>& background_layer_data(*optional_background_layer_data);
                const unsigned char* const background_layer_data_start(background_layer_data.data());
                const std::size_t background_layer_data_size(static_cast<std::size_t>(background_layer_data.size()));

                sf::Texture& background_layer_texture(background_layer_textures[background_layer_index]);
                background_layer_texture.loadFromMemory(background_layer_data_start, background_layer_data_size);
            }
        );
    }

    void start_scene::set_interactable_texts()
    {
        set_first_interactable_text();
        set_second_interactable_text();
        set_third_interactable_text();
        set_fourth_interactable_text();
    }

    void start_scene::set_fifth_background_layer()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> optional_background_layer_scene_node(get_background_layer_scene_node(4u));
        if(!optional_background_layer_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>& background_layer_scene_node(*optional_background_layer_scene_node);
        background_layer_scene_node->insert_functions
        (
            [this](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            [&](const float incoming_frame_delta_time, sf::Sprite& incoming_sprite)
            {
                const bool pause_state(this_current_state==state::pause);
                if(pause_state)
                {
                    return;
                }

                std::chrono::time_point<std::chrono::system_clock>& fifth_layer_finish_point(fifth_layer_time_points.second);
                std::chrono::time_point<std::chrono::system_clock>& fifth_layer_start_point(fifth_layer_time_points.first);
                fifth_layer_finish_point=std::chrono::system_clock::now();
                fifth_layer_duration=fifth_layer_finish_point-fifth_layer_start_point;
                const float fifth_layer_duration_count(fifth_layer_duration.count());
                const bool fifth_layer_fading_in(fifth_layer_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*51.0f)&&fifth_layer_fading_state);
                const bool fifth_layer_fading_out(fifth_layer_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*51.0f)&&!fifth_layer_fading_state);
                const bool fifth_layer_fading_switch(fifth_layer_duration_count>=fifth_layer_animation_start_time);
                if(fifth_layer_fading_in)
                {
                    sf::Color sprite_color(incoming_sprite.getColor());
                    sf::Uint8& sprite_color_alpha(sprite_color.a);
                    sprite_color_alpha+=5u;
                    incoming_sprite.setColor(sprite_color);
                    const bool fifth_layer_faded_in(sprite_color_alpha==255u);
                    if(fifth_layer_faded_in)
                    {
                        fifth_layer_fading_state=boost::indeterminate;
                    }

                    fifth_layer_start_point=std::chrono::system_clock::now();
                }

                else if(fifth_layer_fading_out)
                {
                    sf::Color sprite_color(incoming_sprite.getColor());
                    sf::Uint8& sprite_color_alpha(sprite_color.a);
                    sprite_color_alpha-=5u;
                    incoming_sprite.setColor(sprite_color);
                    const bool fifth_layer_faded_out(sprite_color_alpha==0u);
                    if(fifth_layer_faded_out)
                    {
                        fifth_layer_fading_state=boost::indeterminate;
                    }

                    fifth_layer_start_point=std::chrono::system_clock::now();
                }

                else if(fifth_layer_fading_switch)
                {
                    const sf::Color& sprite_color(incoming_sprite.getColor());
                    const sf::Uint8 sprite_color_alpha(sprite_color.a);
                    const bool faded_in(sprite_color_alpha==255u);
                    if(faded_in)
                    {
                        fifth_layer_fading_state=false;
                    }

                    else
                    {
                        fifth_layer_fading_state=true;
                    }

                    fifth_layer_animation_start_time=random_distribution(random_engine);
                    fifth_layer_start_point=std::chrono::system_clock::now();
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(background_layer_scene_node));
        insert_child(layer_child_scene_node, layer::background);
    }

    void start_scene::set_first_background_layer()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> optional_background_layer_scene_node(get_background_layer_scene_node(0u));
        if(!optional_background_layer_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>& background_layer_scene_node(*optional_background_layer_scene_node);
        background_layer_scene_node->insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            [this](const float incoming_frame_delta_time, sf::Sprite& incoming_sprite)
            {
                const bool pause_state(this_current_state==state::pause);
                if(pause_state)
                {
                    return;
                }

                std::chrono::time_point<std::chrono::system_clock>& first_layer_finish_point(first_layer_time_points.second);
                std::chrono::time_point<std::chrono::system_clock>& first_layer_start_point(first_layer_time_points.first);
                first_layer_finish_point=std::chrono::system_clock::now();
                first_layer_duration=first_layer_finish_point-first_layer_start_point;
                const float first_layer_duration_count(first_layer_duration.count());
                const bool do_first_layer_animation(first_layer_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*30.0f)&&first_layer_animating);
                const bool first_layer_animation_start(first_layer_duration_count>=first_layer_animation_start_time);
                if(do_first_layer_animation)
                {
                    const bool set_default_background_layer_texture(texture_switch_count%2u==0u);
                    const std::array<sf::Texture, 6u>::size_type background_layer_textures_size(background_layer_textures.size());
                    if(set_default_background_layer_texture)
                    {
                        const std::array<sf::Texture, 6u>::size_type default_background_layer_texture_index(5u);
                        const bool default_background_layer_texture_index_valid(default_background_layer_texture_index<background_layer_textures_size);
                        if(default_background_layer_texture_index_valid)
                        {
                            const sf::Texture& default_background_layer_texture(background_layer_textures[default_background_layer_texture_index]);
                            incoming_sprite.setTexture(default_background_layer_texture);
                        }
                    }

                    else
                    {
                        const std::array<sf::Texture, 6u>::size_type first_background_layer_texture_index(0u);
                        const bool first_background_layer_texture_index_valid(first_background_layer_texture_index<background_layer_textures_size);
                        if(first_background_layer_texture_index_valid)
                        {
                            const sf::Texture& first_background_layer_texture(background_layer_textures[first_background_layer_texture_index]);
                            incoming_sprite.setTexture(first_background_layer_texture);
                        }
                    }

                    ++texture_switch_count;
                    first_layer_start_point=std::chrono::system_clock::now();

                    const bool first_layer_animation_finished(texture_switch_count==30u);
                    if(first_layer_animation_finished)
                    {
                        first_layer_animating=false;
                        texture_switch_count=0u;
                        const std::array<sf::Texture, 6u>::size_type first_background_layer_texture_index(0u);
                        const bool first_background_layer_texture_index_valid(first_background_layer_texture_index<background_layer_textures_size);
                        if(first_background_layer_texture_index_valid)
                        {
                            const sf::Texture& first_background_layer_texture(background_layer_textures[first_background_layer_texture_index]);
                            incoming_sprite.setTexture(first_background_layer_texture);
                        }
                    }
                }

                else if(first_layer_animation_start)
                {
                    first_layer_animating=true;
                    first_layer_animation_start_time=random_distribution(random_engine);
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(background_layer_scene_node));
        insert_child(layer_child_scene_node, layer::background);
    }

    void start_scene::set_first_interactable_text()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> optional_interactable_text_scene_node(get_interactable_text_scene_node(0u));
        if(!optional_interactable_text_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Text>>& interactable_text_scene_node(*optional_interactable_text_scene_node);
        interactable_text_scene_node->insert_functions
        (
            [this](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text)
            {
                incoming_render_target.draw(incoming_text, incoming_render_states);
            },
            nullptr,
            [this](const sf::Event& incoming_event, sf::Text& incoming_text)
            {
                const bool non_process_event_state(this_current_state==state::pause||this_current_state==state::transition);
                if(non_process_event_state)
                {
                    return;
                }

                const sf::Event::EventType event_type(incoming_event.type);
                switch(event_type)
                {
                    case sf::Event::MouseButtonPressed:
                    {
                        const sf::Event::MouseButtonEvent mouse_button_event(incoming_event.mouseButton);
                        const bool text_pressed(process_interactable_text_pressed(mouse_button_event, incoming_text));
                        if(text_pressed)
                        {
                            this_current_state=scene::state::transition;
                            button_pressed_index=0u;
                            actions.push(action::play_button_press_sound);
                        }

                        break;
                    }

                    case sf::Event::MouseMoved:
                    {
                        const sf::Event::MouseMoveEvent mouse_move_event(incoming_event.mouseMove);
                        const bool text_hovered(process_interactable_text_hovered(mouse_move_event, incoming_text));
                        if(text_hovered)
                        {
                            incoming_text.setOutlineColor(sf::Color::Green);
                        }

                        else
                        {
                            incoming_text.setOutlineColor(sf::Color(162u, 165u, 101u));
                        }

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            },
            [this](const float incoming_frame_delta_time, sf::Text& incoming_text)
            {
                switch(this_current_state)
                {
                    case state::transition:
                    {
                        const float text_move_horizontal_direction(-incoming_frame_delta_time*1000.0f);
                        incoming_text.move(text_move_horizontal_direction, 0.0f);

                        const float text_horizontal_position(incoming_text.getPosition().x);
                        const float destination_horizontal_position(-incoming_text.getGlobalBounds().width/2.0f);
                        const bool text_destination_position_reached(text_horizontal_position<=destination_horizontal_position);
                        const bool transition_finished(text_destination_position_reached&&button_pressed_index&&button_pressed_index==0u);
                        if(transition_finished)
                        {
                            actions.push(action::transition_character_select_scene);
                        }
                    }

                    default:
                    {
                        break;
                    }
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(interactable_text_scene_node));
        insert_child(layer_child_scene_node, layer::user_interface);
    }

    void start_scene::set_fourth_background_layer()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> optional_background_layer_scene_node(get_background_layer_scene_node(3u));
        if(!optional_background_layer_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>& background_layer_scene_node(*optional_background_layer_scene_node);
        background_layer_scene_node->insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            nullptr
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(background_layer_scene_node));
        insert_child(layer_child_scene_node, layer::background);
    }

    void start_scene::set_fourth_interactable_text()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> optional_interactable_text_scene_node(get_interactable_text_scene_node(3u));
        if(!optional_interactable_text_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Text>>& interactable_text_scene_node(*optional_interactable_text_scene_node);
        interactable_text_scene_node->insert_functions
        (
            [this](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text)
            {
                incoming_render_target.draw(incoming_text, incoming_render_states);
            },
            nullptr,
            [this](const sf::Event& incoming_event, sf::Text& incoming_text)
            {
                const bool non_process_event_state(this_current_state==state::pause||this_current_state==state::transition);
                if(non_process_event_state)
                {
                    return;
                }

                const sf::Event::EventType event_type(incoming_event.type);
                switch(event_type)
                {
                    case sf::Event::MouseButtonPressed:
                    {
                        const sf::Event::MouseButtonEvent mouse_button_event(incoming_event.mouseButton);
                        const bool text_pressed(process_interactable_text_pressed(mouse_button_event, incoming_text));
                        if(text_pressed)
                        {
                            this_current_state=scene::state::transition;
                            button_pressed_index=3u;
                            actions.push(action::play_button_press_sound);
                        }

                        break;
                    }

                    case sf::Event::MouseMoved:
                    {
                        const sf::Event::MouseMoveEvent mouse_move_event(incoming_event.mouseMove);
                        const bool text_hovered(process_interactable_text_hovered(mouse_move_event, incoming_text));
                        if(text_hovered)
                        {
                            incoming_text.setOutlineColor(sf::Color::Green);
                        }

                        else
                        {
                            incoming_text.setOutlineColor(sf::Color(162u, 165u, 101u));
                        }

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            },
            [this](const float incoming_frame_delta_time, sf::Text& incoming_text)
            {
                switch(this_current_state)
                {
                    case state::transition:
                    {
                        const float text_move_horizontal_direction(incoming_frame_delta_time*1000.0f);
                        incoming_text.move(text_move_horizontal_direction, 0.0f);

                        const float text_horizontal_position(incoming_text.getPosition().x);
                        const float destination_horizontal_position(static_cast<float>(sf::VideoMode::getDesktopMode().width)+incoming_text.getGlobalBounds().width/2.0f);
                        const bool text_destination_position_reached(text_horizontal_position>=destination_horizontal_position);
                        const bool transition_finished(text_destination_position_reached&&button_pressed_index&&button_pressed_index==3u);
                        if(transition_finished)
                        {
                            actions.push(action::quit_application);
                        }
                    }

                    default:
                    {
                        break;
                    }
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(interactable_text_scene_node));
        insert_child(layer_child_scene_node, layer::user_interface);
    }

    void start_scene::set_second_background_layer()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> optional_background_layer_scene_node(get_background_layer_scene_node(1u));
        if(!optional_background_layer_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>& background_layer_scene_node(*optional_background_layer_scene_node);
        background_layer_scene_node->insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            nullptr
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(background_layer_scene_node));
        insert_child(layer_child_scene_node, layer::background);
    }

    void start_scene::set_second_interactable_text()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> optional_interactable_text_scene_node(get_interactable_text_scene_node(1u));
        if(!optional_interactable_text_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Text>>& interactable_text_scene_node(*optional_interactable_text_scene_node);
        interactable_text_scene_node->insert_functions
        (
            [this](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text)
            {
                incoming_render_target.draw(incoming_text, incoming_render_states);
            },
            nullptr,
            [this](const sf::Event& incoming_event, sf::Text& incoming_text)
            {
                const bool non_process_event_state(this_current_state==state::pause||this_current_state==state::transition);
                if(non_process_event_state)
                {
                    return;
                }

                const sf::Event::EventType event_type(incoming_event.type);
                switch(event_type)
                {
                    case sf::Event::MouseButtonPressed:
                    {
                        const sf::Event::MouseButtonEvent mouse_button_event(incoming_event.mouseButton);
                        const bool text_pressed(process_interactable_text_pressed(mouse_button_event, incoming_text));
                        if(text_pressed)
                        {
                            this_current_state=scene::state::transition;
                            button_pressed_index=1u;
                            actions.push(action::play_button_press_sound);
                        }

                        break;
                    }

                    case sf::Event::MouseMoved:
                    {
                        const sf::Event::MouseMoveEvent mouse_move_event(incoming_event.mouseMove);
                        const bool text_hovered(process_interactable_text_hovered(mouse_move_event, incoming_text));
                        if(text_hovered)
                        {
                            incoming_text.setOutlineColor(sf::Color::Green);
                        }

                        else
                        {
                            incoming_text.setOutlineColor(sf::Color(162u, 165u, 101u));
                        }

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            },
            [this](const float incoming_frame_delta_time, sf::Text& incoming_text)
            {
                switch(this_current_state)
                {
                    case state::transition:
                    {
                        const float text_move_horizontal_direction(-incoming_frame_delta_time*1000.0f);
                        incoming_text.move(text_move_horizontal_direction, 0.0f);

                        const float text_horizontal_position(incoming_text.getPosition().x);
                        const float destination_horizontal_position(-incoming_text.getGlobalBounds().width/2.0f);
                        const bool text_destination_position_reached(text_horizontal_position<=destination_horizontal_position);
                        const bool transition_finished(text_destination_position_reached&&button_pressed_index&&button_pressed_index==1u);
                        if(transition_finished)
                        {
                            actions.push(action::transition_multiplayer_join_scene);
                        }
                    }

                    default:
                    {
                        break;
                    }
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(interactable_text_scene_node));
        insert_child(layer_child_scene_node, layer::user_interface);
    }

    void start_scene::set_start_scene_font()
    {
        const boost::optional<const std::vector<unsigned char>&> optional_feudal_wars_font_data(graphics_resource_holder::resources[graphics_resource_holder::identifier::feudal_wars_font]);
        if(!optional_feudal_wars_font_data)
        {
            return;
        }

        const std::vector<unsigned char>& feudal_wars_font_data(*optional_feudal_wars_font_data);
        const unsigned char* const feudal_wars_font_data_start(feudal_wars_font_data.data());
        const std::size_t feudal_wars_font_data_size(static_cast<std::size_t>(feudal_wars_font_data.size()));
        start_scene_font.loadFromMemory(feudal_wars_font_data_start, feudal_wars_font_data_size);
    }

    void start_scene::set_third_background_layer()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> optional_background_layer_scene_node(get_background_layer_scene_node(2u));
        if(!optional_background_layer_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>& background_layer_scene_node(*optional_background_layer_scene_node);
        background_layer_scene_node->insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            [this](const float incoming_frame_delta_time, sf::Sprite& incoming_sprite)
            {
                const bool pause_state(this_current_state==state::pause);
                if(pause_state)
                {
                    return;
                }

                std::chrono::time_point<std::chrono::system_clock>& third_layer_finish_point(third_layer_time_points.second);
                std::chrono::time_point<std::chrono::system_clock>& third_layer_start_point(third_layer_time_points.first);
                third_layer_finish_point=std::chrono::system_clock::now();
                third_layer_duration=third_layer_finish_point-third_layer_start_point;
                const float third_layer_duration_count(third_layer_duration.count());
                const bool third_layer_fading_in(third_layer_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*51.0f)&&third_layer_fading_state);
                const bool third_layer_fading_out(third_layer_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*51.0f)&&!third_layer_fading_state);
                const bool third_layer_fading_switch(third_layer_duration_count>=third_layer_animation_start_time);
                if(third_layer_fading_in)
                {
                    sf::Color sprite_color(incoming_sprite.getColor());
                    sf::Uint8& sprite_color_alpha(sprite_color.a);
                    sprite_color_alpha+=5u;
                    incoming_sprite.setColor(sprite_color);
                    const bool third_layer_faded_in(sprite_color_alpha==255u);
                    if(third_layer_faded_in)
                    {
                        third_layer_fading_state=boost::indeterminate;
                    }

                    third_layer_start_point=std::chrono::system_clock::now();
                }

                else if(third_layer_fading_out)
                {
                    sf::Color sprite_color(incoming_sprite.getColor());
                    sf::Uint8& sprite_color_alpha(sprite_color.a);
                    sprite_color_alpha-=5u;
                    incoming_sprite.setColor(sprite_color);
                    const bool third_layer_faded_out(sprite_color_alpha==0u);
                    if(third_layer_faded_out)
                    {
                        third_layer_fading_state=boost::indeterminate;
                    }

                    third_layer_start_point=std::chrono::system_clock::now();
                }

                else if(third_layer_fading_switch)
                {
                    const sf::Color& sprite_color(incoming_sprite.getColor());
                    const sf::Uint8 sprite_color_alpha(sprite_color.a);
                    const bool faded_in(sprite_color_alpha==255u);
                    if(faded_in)
                    {
                        third_layer_fading_state=false;
                    }

                    else
                    {
                        third_layer_fading_state=true;
                    }

                    third_layer_animation_start_time=random_distribution(random_engine);
                    third_layer_start_point=std::chrono::system_clock::now();
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(background_layer_scene_node));
        insert_child(layer_child_scene_node, layer::background);
    }

    void start_scene::set_third_interactable_text()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> optional_interactable_text_scene_node(get_interactable_text_scene_node(2u));
        if(!optional_interactable_text_scene_node)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Text>>& interactable_text_scene_node(*optional_interactable_text_scene_node);
        interactable_text_scene_node->insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text)
            {
                incoming_render_target.draw(incoming_text, incoming_render_states);
            },
            nullptr,
            [this](const sf::Event& incoming_event, sf::Text& incoming_text)
            {
                const bool non_process_event_state(this_current_state==state::pause||this_current_state==state::transition);
                if(non_process_event_state)
                {
                    return;
                }

                const sf::Event::EventType event_type(incoming_event.type);
                switch(event_type)
                {
                    case sf::Event::MouseButtonPressed:
                    {
                        const sf::Event::MouseButtonEvent mouse_button_event(incoming_event.mouseButton);
                        const bool text_pressed(process_interactable_text_pressed(mouse_button_event, incoming_text));
                        if(text_pressed)
                        {
                            this_current_state=scene::state::transition;
                            button_pressed_index=2u;
                            actions.push(action::play_button_press_sound);
                        }

                        break;
                    }

                    case sf::Event::MouseMoved:
                    {
                        const sf::Event::MouseMoveEvent mouse_move_event(incoming_event.mouseMove);
                        const bool text_hovered(process_interactable_text_hovered(mouse_move_event, incoming_text));
                        if(text_hovered)
                        {
                            incoming_text.setOutlineColor(sf::Color::Green);
                        }

                        else
                        {
                            incoming_text.setOutlineColor(sf::Color(162u, 165u, 101u));
                        }

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            },
            [this](const float incoming_frame_delta_time, sf::Text& incoming_text)
            {
                switch(this_current_state)
                {
                    case state::transition:
                    {
                        const float text_move_horizontal_direction(incoming_frame_delta_time*1000.0f);
                        incoming_text.move(text_move_horizontal_direction, 0.0f);

                        const float text_horizontal_position(incoming_text.getPosition().x);
                        const float destination_horizontal_position(static_cast<float>(sf::VideoMode::getDesktopMode().width)+incoming_text.getGlobalBounds().width/2.0f);
                        const bool text_destination_position_reached(text_horizontal_position>=destination_horizontal_position);
                        const bool transition_finished(text_destination_position_reached&&button_pressed_index&&button_pressed_index==2u);
                        if(transition_finished)
                        {
                            actions.push(action::transition_settings_scene);
                        }
                    }

                    default:
                    {
                        break;
                    }
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(interactable_text_scene_node));
        insert_child(layer_child_scene_node, layer::user_interface);
    }

    void start_scene::set_title_text()
    {
        const float title_text_horizontal_position(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u));
        const float title_text_vertical_position(static_cast<float>(sf::VideoMode::getDesktopMode().height/8u));
        const unsigned int title_text_character_size(sf::VideoMode::getDesktopMode().width/10u);
        const sf::Vector2f title_text_view_size(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
        const sf::Vector2f title_text_view_center(title_text_view_size/2.0f);
        title_text_view.setSize(title_text_view_size);
        title_text_view.setCenter(title_text_view_center);

        drawable_adapter_scene_node<sf::Text>& title_text(*title_text_scene_node);
        title_text->setCharacterSize(title_text_character_size);
        title_text->setFillColor(sf::Color(159u, 24u, 24u));
        title_text->setFont(start_scene_font);
        title_text->setOutlineColor(sf::Color(162u, 165u, 101u));
        title_text->setOutlineThickness(3.0f);
        title_text->setString("FEUDAL WARS");
        title_text->setStyle(sf::Text::Bold|sf::Text::Italic);

        const sf::FloatRect title_text_rectangle(title_text->getLocalBounds());
        const float title_text_origin_horizontal_position(title_text_rectangle.left+title_text_rectangle.width/2.0f);
        const float title_text_origin_vertical_position(title_text_rectangle.top+title_text_rectangle.height/2.0f);
        title_text->setOrigin(title_text_origin_horizontal_position, title_text_origin_vertical_position);
        title_text->setPosition(title_text_horizontal_position, title_text_vertical_position);
        title_text.insert_functions
        (
            [this](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text)
            {
                switch(this_current_state)
                {
                    case state::pause:
                    {
                        process_draw_title_text(incoming_render_target, incoming_render_states, incoming_text, this_previous_state);

                        break;
                    }

                    default:
                    {
                        process_draw_title_text(incoming_render_target, incoming_render_states, incoming_text, this_current_state);

                        break;
                    }
                }
            },
            nullptr,
            nullptr,
            [this](const float incoming_frame_delta_time, sf::Text& incoming_text)
            {
                switch(this_current_state)
                {
                    case state::transition:
                    {
                        incoming_text.rotate(incoming_frame_delta_time*360.0f);

                        title_text_zoom_count+=incoming_frame_delta_time;
                        const bool title_text_zoom_reached(title_text_zoom_count>=0.01171875f);
                        if(title_text_zoom_reached)
                        {
                            title_text_view.zoom(1.0625f);
                            title_text_zoom_count=0.0f;
                        }

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(title_text_scene_node));
        insert_child(layer_child_scene_node, layer::non_interactable);
    }

    boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> start_scene::get_background_layer_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type incoming_background_layer_index)
    {
        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type background_layer_scene_nodes_size(background_layer_scene_nodes.size());
        const std::array<sf::Texture, 6u>::size_type background_layer_textures_size(background_layer_textures.size());
        const bool background_layer_index_valid(incoming_background_layer_index<background_layer_scene_nodes_size&&incoming_background_layer_index<background_layer_textures_size);
        if(!background_layer_index_valid)
        {
            return boost::none;
        }

        const sf::Texture& background_layer_texture(background_layer_textures[incoming_background_layer_index]);
        const sf::Vector2u background_layer_texture_size(background_layer_texture.getSize());
        const bool background_layer_texture_size_valid(background_layer_texture_size.x>0u&&background_layer_texture_size.y>0u);
        if(!background_layer_texture_size_valid)
        {
            return boost::none;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>& background_layer_scene_node_pointer(background_layer_scene_nodes[incoming_background_layer_index]);
        if(!background_layer_scene_node_pointer)
        {
            return boost::none;
        }

        drawable_adapter_scene_node<sf::Sprite>& background_layer_scene_node(*background_layer_scene_node_pointer);

        const float background_layer_texture_height(static_cast<float>(background_layer_texture_size.y));
        const float background_layer_texture_width(static_cast<float>(background_layer_texture_size.x));
        const float desktop_height(static_cast<float>(sf::VideoMode::getDesktopMode().height));
        const float desktop_width(static_cast<float>(sf::VideoMode::getDesktopMode().width));
        const float background_layer_height_scale(desktop_height/background_layer_texture_height);
        const float background_layer_width_scale(desktop_width/background_layer_texture_width);

        background_layer_scene_node->setTexture(background_layer_texture);
        background_layer_scene_node->scale(background_layer_width_scale, background_layer_height_scale);

        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>&> optional_background_layer_scene_node(background_layer_scene_node_pointer);

        return optional_background_layer_scene_node;
    }

    boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> start_scene::get_interactable_text_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type incoming_interactable_text_index)
    {
        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type interactable_text_scene_nodes_size(interactable_text_scene_nodes.size());
        const bool interactable_text_index_valid(incoming_interactable_text_index<interactable_text_scene_nodes_size);
        if(!interactable_text_index_valid)
        {
            return boost::none;
        }

        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type, sf::Vector2f> interactable_text_scene_node_position_library
        {
            {0u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height*2u/6u))},
            {1u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height/2u))},
            {2u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height*4u/6u))},
            {3u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height*5u/6u))}
        };

        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type, sf::String> interactable_text_scene_node_string_library
        {
            {0u, "SINGLE PLAYER"},
            {1u, "MULTIPLAYER"},
            {2u, "SETTINGS"},
            {3u, "QUIT"}
        };

        std::unique_ptr<drawable_adapter_scene_node<sf::Text>>& interactable_text_scene_node_pointer(interactable_text_scene_nodes[incoming_interactable_text_index]);
        if(!interactable_text_scene_node_pointer)
        {
            return boost::none;
        }

        drawable_adapter_scene_node<sf::Text>& interactable_text_scene_node(*interactable_text_scene_node_pointer);

        const unsigned int interactable_text_character_size(sf::VideoMode::getDesktopMode().width/20u);
        interactable_text_scene_node->setCharacterSize(interactable_text_character_size);
        interactable_text_scene_node->setFillColor(sf::Color(159u, 24u, 24u));
        interactable_text_scene_node->setFont(start_scene_font);
        interactable_text_scene_node->setOutlineColor(sf::Color(162u, 165u, 101u));
        interactable_text_scene_node->setOutlineThickness(3.0f);

        const sf::String& interactable_text_string(interactable_text_scene_node_string_library.at(incoming_interactable_text_index));
        interactable_text_scene_node->setString(interactable_text_string);

        const sf::FloatRect interactable_text_rectangle(interactable_text_scene_node->getLocalBounds());
        const float interactable_text_origin_horizontal_position(interactable_text_rectangle.left+interactable_text_rectangle.width/2.0f);
        const float interactable_text_origin_vertical_position(interactable_text_rectangle.top+interactable_text_rectangle.height/2.0f);
        const sf::Vector2f& interactable_text_position(interactable_text_scene_node_position_library.at(incoming_interactable_text_index));
        interactable_text_scene_node->setOrigin(interactable_text_origin_horizontal_position, interactable_text_origin_vertical_position);
        interactable_text_scene_node->setPosition(interactable_text_position);

        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> optional_interactable_text_scene_node(interactable_text_scene_node_pointer);

        return optional_interactable_text_scene_node;
    }

    start_scene::start_scene(const sf::RenderWindow& incoming_render_window):
    first_layer_animating(false),
    first_layer_animation_start_time(3.0f),
    fifth_layer_animation_start_time(3.0f),
    third_layer_animation_start_time(3.0f),
    title_text_zoom_count(0.0f),
    background_layer_scene_nodes
    {
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>()
    },
    interactable_text_scene_nodes
    {
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>()
    },
    fifth_layer_time_points(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
    first_layer_time_points(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
    third_layer_time_points(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
    random_engine(std::random_device()()),
    texture_switch_count(0u),
    random_distribution(0.0f, 10.0f),
    title_text_scene_node(std::make_unique<drawable_adapter_scene_node<sf::Text>>()),
    render_window(incoming_render_window)
    {
        set_background_layer_textures();
        set_background_layers();
        set_start_scene_font();
        set_title_text();
        set_interactable_texts();
        this_current_state=state::active;
    }
}
