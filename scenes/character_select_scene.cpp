#include<character_select_scene.h>

namespace feudal_wars
{
    bool character_select_scene::process_interactable_text_hovered(const sf::Event::MouseMoveEvent& incoming_mouse_move_event, const sf::Text& incoming_text) const
    {
        const sf::Vector2i mouse_move_coordinate(incoming_mouse_move_event.x, incoming_mouse_move_event.y);
        const sf::Vector2f mapped_mouse_button_coordinate(render_window.mapPixelToCoords(mouse_move_coordinate));
        const sf::FloatRect text_global_bounds(incoming_text.getGlobalBounds());
        const bool text_hovered(text_global_bounds.contains(mapped_mouse_button_coordinate));

        return text_hovered;
    }

    bool character_select_scene::process_interactable_text_pressed(const sf::Event::MouseButtonEvent& incoming_mouse_button_event, const sf::Text& incoming_text) const
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

    bool character_select_scene::process_sprite_hovered(const sf::Event::MouseMoveEvent& incoming_mouse_move_event, const sf::Sprite& incoming_sprite) const
    {
        const sf::Vector2i mouse_move_coordinate(incoming_mouse_move_event.x, incoming_mouse_move_event.y);
        const sf::Vector2f mapped_mouse_button_coordinate(render_window.mapPixelToCoords(mouse_move_coordinate));
        const sf::FloatRect sprite_global_bounds(incoming_sprite.getGlobalBounds());
        const bool sprite_hovered(sprite_global_bounds.contains(mapped_mouse_button_coordinate));

        return sprite_hovered;
    }

    bool character_select_scene::process_sprite_pressed(const sf::Event::MouseButtonEvent& incoming_mouse_button_event, const sf::Sprite& incoming_sprite) const
    {
        const sf::Mouse::Button mouse_button(incoming_mouse_button_event.button);
        switch(mouse_button)
        {
            case sf::Mouse::Left:
            {
                const sf::Vector2i mouse_button_coordinate(incoming_mouse_button_event.x, incoming_mouse_button_event.y);
                const sf::Vector2f mapped_mouse_button_coordinate(render_window.mapPixelToCoords(mouse_button_coordinate));
                const sf::FloatRect sprite_global_bounds(incoming_sprite.getGlobalBounds());
                const bool sprite_pressed(sprite_global_bounds.contains(mapped_mouse_button_coordinate));

                return sprite_pressed;
            }

            default:
            {
                break;
            }
        }

        return false;
    }

    void character_select_scene::process_action_this(const float incoming_frame_delta_time, const action incoming_action)
    {
        switch(incoming_action)
        {
            case action::character_select_scene_transitions_ended:
            {
                const bool interactable_text_pressed(static_cast<bool>(button_pressed_index));
                if(interactable_text_pressed)
                {
                    const action character_select_scene_transition_action(get_character_select_transition_action());
                    actions.push(character_select_scene_transition_action);
                }

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void character_select_scene::process_event_this(const sf::Event& incoming_event)
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

    void character_select_scene::set_character_description_text(const sf::String& incoming_string, const sf::Vector2f& incoming_text_position, sf::Text& incoming_text)
    {
        incoming_text.setString(incoming_string);
        const sf::FloatRect incoming_text_rectangle(incoming_text.getLocalBounds());
        const float incoming_text_origin_horizontal_position(incoming_text_rectangle.left+incoming_text_rectangle.width/2.0f);
        const float incoming_text_origin_vertical_position(incoming_text_rectangle.top+incoming_text_rectangle.height/2.0f);
        incoming_text.setOrigin(incoming_text_origin_horizontal_position, incoming_text_origin_vertical_position);
        incoming_text.setPosition(incoming_text_position);
    }

    void character_select_scene::set_character_select_background_sprite_scene_node()
    {
        const boost::optional<const std::vector<unsigned char>&> optional_background_sprite_data(graphics_resource_holder::resources[graphics_resource_holder::identifier::character_select_scene_default]);
        if(!optional_background_sprite_data)
        {
            return;
        }

        const std::vector<unsigned char>& background_sprite_data(*optional_background_sprite_data);
        const unsigned char* const background_sprite_data_start(background_sprite_data.data());
        const std::size_t background_sprite_data_size(static_cast<std::size_t>(background_sprite_data.size()));
        background_texture.loadFromMemory(background_sprite_data_start, background_sprite_data_size);
        const sf::Vector2u background_texture_size(background_texture.getSize());
        const bool background_texture_size_valid(background_texture_size.x>0u&&background_texture_size.y>0u);
        if(!background_texture_size_valid)
        {
            return;
        }

        const float background_texture_height(static_cast<float>(background_texture_size.y));
        const float background_texture_width(static_cast<float>(background_texture_size.x));
        const float desktop_height(static_cast<float>(sf::VideoMode::getDesktopMode().height));
        const float desktop_width(static_cast<float>(sf::VideoMode::getDesktopMode().width));
        const float background_sprite_height_scale(desktop_height/background_texture_height);
        const float background_sprite_width_scale(desktop_width/background_texture_width);

        drawable_adapter_scene_node<sf::Sprite>& character_select_sprite_scene_node(*character_select_background_scene_node);
        character_select_sprite_scene_node->setTexture(background_texture);
        character_select_sprite_scene_node->scale(background_sprite_width_scale, background_sprite_height_scale);
        character_select_sprite_scene_node.insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            [this](const float incoming_frame_delta_time, sf::Sprite& incoming_sprite)
            {
                const bool non_process_frame_state(this_current_state==state::pause||this_current_state!=state::transition);
                if(non_process_frame_state)
                {
                    return;
                }

                const std::array<bool, 3u>::const_iterator character_select_scene_transitions_finish(character_select_scene_transitions.cend());
                const std::array<bool, 3u>::const_iterator character_select_scene_transitions_start(character_select_scene_transitions.cbegin());
                const std::array<bool, 3u>::const_iterator character_select_scene_transitions_iterator(std::find(character_select_scene_transitions_start, character_select_scene_transitions_finish, false));
                const bool character_select_scene_transitions_ended(character_select_scene_transitions_iterator==character_select_scene_transitions_finish);
                if(character_select_scene_transitions_ended)
                {
                    actions.push(action::character_select_scene_transitions_ended);

                    return;
                }

                const bool character_background_sprite_reddened(character_select_scene_transitions[2u]);
                if(character_background_sprite_reddened)
                {
                    return;
                }

                std::chrono::time_point<std::chrono::system_clock>& character_background_sprite_finish_point(character_background_sprite_time_points.second);
                std::chrono::time_point<std::chrono::system_clock>& character_background_sprite_start_point(character_background_sprite_time_points.first);
                character_background_sprite_finish_point=std::chrono::system_clock::now();
                character_background_sprite_duration=character_background_sprite_finish_point-character_background_sprite_start_point;
                const float character_background_sprite_duration_count(character_background_sprite_duration.count());
                const bool character_background_sprite_reddening(character_background_sprite_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*51.0f));
                if(character_background_sprite_reddening)
                {
                    sf::Color sprite_color(incoming_sprite.getColor());
                    sf::Uint8& sprite_color_blue(sprite_color.b);
                    sf::Uint8& sprite_color_green(sprite_color.g);
                    sprite_color_blue-=5u;
                    sprite_color_green-=5u;
                    const bool character_background_sprite_reddened(sprite_color_blue==0u);
                    if(character_background_sprite_reddened)
                    {
                        character_select_scene_transitions[2u]=true;
                    }

                    else
                    {
                        incoming_sprite.setColor(sprite_color);
                        character_background_sprite_start_point=std::chrono::system_clock::now();
                    }
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(character_select_background_scene_node));
        insert_child(layer_child_scene_node, layer::background);
    }

    void character_select_scene::set_character_select_description_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type incoming_character_select_description_scene_node_index, const sf::Vector2f& incoming_character_select_description_position, const std::array<sf::String, 5u>& incoming_character_select_descriptions)
    {
        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type character_select_description_scene_nodes_size(character_select_description_scene_nodes.size());
        const bool character_select_description_scene_node_index_valid(incoming_character_select_description_scene_node_index<character_select_description_scene_nodes_size);
        if(!character_select_description_scene_node_index_valid)
        {
            return;
        }

        const unsigned int character_select_text_character_size(sf::VideoMode::getDesktopMode().width/40u);
        std::unique_ptr<drawable_adapter_scene_node<sf::Text>>& character_select_description_scene_node_pointer(character_select_description_scene_nodes[incoming_character_select_description_scene_node_index]);
        if(!character_select_description_scene_node_pointer)
        {
            return;
        }

        drawable_adapter_scene_node<sf::Text>& character_select_description_scene_node(*character_select_description_scene_node_pointer);
        character_select_description_scene_node->setCharacterSize(character_select_text_character_size);
        character_select_description_scene_node->setFillColor(sf::Color(159u, 24u, 24u));
        character_select_description_scene_node->setFont(character_select_scene_font);
        character_select_description_scene_node->setOutlineColor(sf::Color(162u, 165u, 101u));
        character_select_description_scene_node->setOutlineThickness(3.0f);
        character_select_description_scene_node->setStyle(sf::Text::Bold|sf::Text::Italic);
        character_select_description_scene_node.insert_functions
        (
            [this](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text)
            {
                const bool pause_state(this_current_state==state::pause&&!static_cast<bool>(character_select_pressed_index));
                if(pause_state)
                {
                    return;
                }

                const std::array<bool, 5u>::const_iterator character_select_sprite_hovers_finish(character_select_sprite_hovers.cend());
                const std::array<bool, 5u>::const_iterator character_select_sprite_hovers_start(character_select_sprite_hovers.cbegin());
                const std::array<bool, 5u>::const_iterator character_select_sprite_hovers_iterator(std::find(character_select_sprite_hovers_start, character_select_sprite_hovers_finish, true));
                const bool character_select_sprite_hovered(character_select_sprite_hovers_iterator!=character_select_sprite_hovers_finish);
                if(!character_select_sprite_hovered&&!static_cast<bool>(character_select_pressed_index))
                {
                    return;
                }

                incoming_render_target.draw(incoming_text, incoming_render_states);
            },
            [this, incoming_character_select_description_position, incoming_character_select_descriptions](const float incoming_frame_delta_time, const action incoming_action, sf::Text& incoming_text)
            {
                const bool non_process_action_state(this_current_state==state::pause||this_current_state==state::transition);
                if(non_process_action_state)
                {
                    return;
                }

                switch(incoming_action)
                {
                    case action::change_character_select_description_fifth:
                    {
                        const std::array<sf::String, 5u>::size_type incoming_character_select_description_index(4u);
                        const std::array<sf::String, 5u>::size_type incoming_character_select_descriptions_size(incoming_character_select_descriptions.size());
                        const bool incoming_character_select_description_index_valid(incoming_character_select_description_index<incoming_character_select_descriptions_size);
                        if(!incoming_character_select_description_index_valid)
                        {
                            return;
                        }

                        const sf::String& incoming_character_select_description(incoming_character_select_descriptions[incoming_character_select_description_index]);
                        set_character_description_text(incoming_character_select_description, incoming_character_select_description_position, incoming_text);

                        break;
                    }

                    case action::change_character_select_description_first:
                    {
                        const std::array<sf::String, 5u>::size_type incoming_character_select_description_index(0u);
                        const std::array<sf::String, 5u>::size_type incoming_character_select_descriptions_size(incoming_character_select_descriptions.size());
                        const bool incoming_character_select_description_index_valid(incoming_character_select_description_index<incoming_character_select_descriptions_size);
                        if(!incoming_character_select_description_index_valid)
                        {
                            return;
                        }

                        const sf::String& incoming_character_select_description(incoming_character_select_descriptions[incoming_character_select_description_index]);
                        set_character_description_text(incoming_character_select_description, incoming_character_select_description_position, incoming_text);

                        break;
                    }

                    case action::change_character_select_description_fourth:
                    {
                        const std::array<sf::String, 5u>::size_type incoming_character_select_description_index(3u);
                        const std::array<sf::String, 5u>::size_type incoming_character_select_descriptions_size(incoming_character_select_descriptions.size());
                        const bool incoming_character_select_description_index_valid(incoming_character_select_description_index<incoming_character_select_descriptions_size);
                        if(!incoming_character_select_description_index_valid)
                        {
                            return;
                        }

                        const sf::String& incoming_character_select_description(incoming_character_select_descriptions[incoming_character_select_description_index]);
                        set_character_description_text(incoming_character_select_description, incoming_character_select_description_position, incoming_text);

                        break;
                    }

                    case action::change_character_select_description_second:
                    {
                        const std::array<sf::String, 5u>::size_type incoming_character_select_description_index(1u);
                        const std::array<sf::String, 5u>::size_type incoming_character_select_descriptions_size(incoming_character_select_descriptions.size());
                        const bool incoming_character_select_description_index_valid(incoming_character_select_description_index<incoming_character_select_descriptions_size);
                        if(!incoming_character_select_description_index_valid)
                        {
                            return;
                        }

                        const sf::String& incoming_character_select_description(incoming_character_select_descriptions[incoming_character_select_description_index]);
                        set_character_description_text(incoming_character_select_description, incoming_character_select_description_position, incoming_text);

                        break;
                    }

                    case action::change_character_select_description_third:
                    {
                        const std::array<sf::String, 5u>::size_type incoming_character_select_description_index(2u);
                        const std::array<sf::String, 5u>::size_type incoming_character_select_descriptions_size(incoming_character_select_descriptions.size());
                        const bool incoming_character_select_description_index_valid(incoming_character_select_description_index<incoming_character_select_descriptions_size);
                        if(!incoming_character_select_description_index_valid)
                        {
                            return;
                        }

                        const sf::String& incoming_character_select_description(incoming_character_select_descriptions[incoming_character_select_description_index]);
                        set_character_description_text(incoming_character_select_description, incoming_character_select_description_position, incoming_text);

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            },
            nullptr,
            nullptr
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(character_select_description_scene_node_pointer));
        insert_child(layer_child_scene_node, layer::non_interactable);
    }

    void character_select_scene::set_character_select_description_scene_nodes()
    {
        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type, std::array<sf::String, 5u>> character_select_descriptions_library
        {
            {0u, {"CRAZY WIZARD", "PSYCHOPATH", "PURPLE ORC", "ROBIN ELF", "THIEF"}},
            {1u, {"\"MWAHAHAHA! ALL IS GOING ACCORDING TO PLAN!\"", "\"WATCH YOUR BACK! I'M COMING FOR YA!\"", "\"WWWWRRRRRAAAAARRRR!\"", "\"NO ONE'S A BETTER ARCHER THAN ME IN THESE WOODS!\"", "\"LET'S SEE WHAT'S WORTH ROBBING TODAY!\""}},
            {2u, {"ATTACK POWER : MEDIUM", "ATTACK POWER : MEDIUM-HIGH", "ATTACK POWER : HIGH", "ATTACK POWER : MEDIUM-LOW", "ATTACK POWER : LOW"}},
            {3u, {"DEFENSE POWER : LOW", "DEFENSE POWER : MEDIUM-HIGH", "DEFENSE POWER : LOW", "DEFENSE POWER : MEDIUM", "DEFENSE POWER : MEDIUM"}}
        };

        std::array<sf::Vector2f, 4u> character_select_description_positions
        {
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height)/2.25f),
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height)/2.0f),
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height)/1.80f),
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height)/1.64f)
        };

        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type character_select_description_positions_size(static_cast<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type>(character_select_description_positions.size()));
        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type character_select_description_scene_nodes_size(character_select_description_scene_nodes.size());
        for(std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type character_select_description_scene_node_index(0u); character_select_description_scene_node_index<character_select_description_scene_nodes_size; ++character_select_description_scene_node_index)
        {
            const bool character_select_description_index_valid(character_select_description_scene_node_index<character_select_description_positions_size);
            if(!character_select_description_index_valid)
            {
                continue;
            }

            const std::array<sf::Vector2f, 4u>::size_type character_select_description_position_index(static_cast<std::array<sf::Vector2f, 4u>::size_type>(character_select_description_scene_node_index));
            const sf::Vector2f& character_select_description_position(character_select_description_positions[character_select_description_position_index]);
            const std::array<sf::String, 5u>& character_select_descriptions(character_select_descriptions_library.at(character_select_description_scene_node_index));
            set_character_select_description_scene_node(character_select_description_position_index, character_select_description_position, character_select_descriptions);
        }
    }

    void character_select_scene::set_character_select_scene_font()
    {
        const boost::optional<const std::vector<unsigned char>&> optional_feudal_wars_font_data(graphics_resource_holder::resources[graphics_resource_holder::identifier::feudal_wars_font]);
        if(!optional_feudal_wars_font_data)
        {
            return;
        }

        const std::vector<unsigned char>& feudal_wars_font_data(*optional_feudal_wars_font_data);
        const unsigned char* const feudal_wars_font_data_start(feudal_wars_font_data.data());
        const std::size_t feudal_wars_font_data_size(static_cast<std::size_t>(feudal_wars_font_data.size()));
        character_select_scene_font.loadFromMemory(feudal_wars_font_data_start, feudal_wars_font_data_size);
    }

    void character_select_scene::set_character_select_sprite_scene_nodes()
    {
        const std::array<bool, 5u>::size_type character_select_sprite_hovers_size(character_select_sprite_hovers.size());
        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type character_select_sprite_scene_nodes_size(character_select_sprite_scene_nodes.size());
        const std::array<sf::Texture, 5u>::size_type character_select_textures_size(character_select_textures.size());
        const bool character_select_sprites_available(character_select_sprite_scene_nodes_size==character_select_textures_size&&character_select_sprite_scene_nodes_size==character_select_sprite_hovers_size);
        if(!character_select_sprites_available)
        {
            return;
        }

        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type, action> character_select_sprite_hover_actions_library
        {
            {0u, action::change_character_select_description_first},
            {1u, action::change_character_select_description_second},
            {2u, action::change_character_select_description_third},
            {3u, action::change_character_select_description_fourth},
            {4u, action::change_character_select_description_fifth}
        };

        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type, action> character_select_sprite_press_actions_library
        {
            {0u, action::first_character_selected},
            {1u, action::second_character_selected},
            {2u, action::third_character_selected},
            {3u, action::fourth_character_selected},
            {4u, action::fifth_character_selected}
        };

        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type, sf::Vector2f> character_select_sprite_position_library
        {
            {0u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width*2u/8u), static_cast<float>(sf::VideoMode::getDesktopMode().height/3u))},
            {1u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width*3u/8u), static_cast<float>(sf::VideoMode::getDesktopMode().height/3u))},
            {2u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height/3u))},
            {3u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width*5u/8u), static_cast<float>(sf::VideoMode::getDesktopMode().height/3u))},
            {4u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width*6u/8u), static_cast<float>(sf::VideoMode::getDesktopMode().height/3u))}
        };

        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type, sf::Vector2f> character_select_sprite_scale_library
        {
            {0u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/300u), static_cast<float>(sf::VideoMode::getDesktopMode().height/300u))},
            {1u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/300u), static_cast<float>(sf::VideoMode::getDesktopMode().height/300u))},
            {2u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/300u), static_cast<float>(sf::VideoMode::getDesktopMode().height/300u))},
            {3u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width)/435.0f, static_cast<float>(sf::VideoMode::getDesktopMode().height)/435.0f)},
            {4u, sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/300u), static_cast<float>(sf::VideoMode::getDesktopMode().height/300u))}
        };

        for(std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type character_select_sprite_scene_node_index(0u); character_select_sprite_scene_node_index<character_select_sprite_scene_nodes_size; ++character_select_sprite_scene_node_index)
        {
            std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>& character_select_sprite_scene_node_pointer(character_select_sprite_scene_nodes[character_select_sprite_scene_node_index]);
            drawable_adapter_scene_node<sf::Sprite>& character_select_sprite_scene_node(*character_select_sprite_scene_node_pointer);
            character_select_sprite_scene_node->setTexture(character_select_textures[character_select_sprite_scene_node_index]);

            const sf::FloatRect character_select_sprite_scene_node_rectangle(character_select_sprite_scene_node->getGlobalBounds());
            const float character_select_sprite_scene_node_origin_horizontal_position(character_select_sprite_scene_node_rectangle.left+character_select_sprite_scene_node_rectangle.width/2.0f);
            const float character_select_sprite_scene_node_origin_vertical_position(character_select_sprite_scene_node_rectangle.top+character_select_sprite_scene_node_rectangle.height/2.0f);
            const sf::Vector2f character_select_sprite_position(character_select_sprite_position_library.at(character_select_sprite_scene_node_index));
            const sf::Vector2f character_select_sprite_scale(character_select_sprite_scale_library.at(character_select_sprite_scene_node_index));
            character_select_sprite_scene_node->setOrigin(character_select_sprite_scene_node_origin_horizontal_position, character_select_sprite_scene_node_origin_vertical_position);
            character_select_sprite_scene_node->scale(character_select_sprite_scale);
            character_select_sprite_scene_node->setPosition(character_select_sprite_position);
            character_select_sprite_scene_node.insert_functions
            (
                [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
                {
                    incoming_render_target.draw(incoming_sprite, incoming_render_states);
                },
                [this, character_select_sprite_scene_node_index](const float incoming_frame_delta_time, const action incoming_action, sf::Sprite& incoming_sprite)
                {
                    const bool non_process_action_state(this_current_state==state::pause||this_current_state==state::transition);
                    if(non_process_action_state)
                    {
                        return;
                    }

                    switch(incoming_action)
                    {
                        case action::character_selected:
                        {
                            const bool not_selected_character(static_cast<bool>(character_select_pressed_index)&&character_select_pressed_index!=character_select_sprite_scene_node_index);
                            if(not_selected_character)
                            {
                                incoming_sprite.setColor(sf::Color::White);
                            }

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }
                },
                [this, character_select_sprite_scene_node_index, character_select_sprite_hover_actions_library, character_select_sprite_press_actions_library](const sf::Event& incoming_event, sf::Sprite& incoming_sprite)
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
                            const bool sprite_pressed(process_sprite_pressed(mouse_button_event, incoming_sprite));
                            if(sprite_pressed)
                            {
                                const bool sprite_previously_pressed(character_select_pressed_index&&character_select_pressed_index==character_select_sprite_scene_node_index);
                                if(sprite_previously_pressed)
                                {
                                    character_select_pressed_index=boost::none;
                                    incoming_sprite.setColor(sf::Color::White);
                                    actions.push(action::character_unselected);
                                }

                                else
                                {
                                    character_select_pressed_index=character_select_sprite_scene_node_index;
                                    incoming_sprite.setColor(sf::Color::Green);
                                    actions.push(action::character_selected);
                                    const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type character_select_pressed_index_value(*character_select_pressed_index);
                                    const action character_select_sprite_action(character_select_sprite_press_actions_library.at(character_select_pressed_index_value));
                                    actions.push(character_select_sprite_action);
                                }
                            }

                            break;
                        }

                        case sf::Event::MouseMoved:
                        {
                            const sf::Event::MouseMoveEvent mouse_move_event(incoming_event.mouseMove);
                            const bool sprite_hovered(process_sprite_hovered(mouse_move_event, incoming_sprite));
                            if(sprite_hovered)
                            {
                                const bool sprite_previously_hovered(character_select_sprite_hovers[character_select_sprite_scene_node_index]);
                                if(!sprite_previously_hovered)
                                {
                                    incoming_sprite.setColor(sf::Color::Green);
                                    const action character_select_sprite_action(character_select_sprite_hover_actions_library.at(character_select_sprite_scene_node_index));
                                    actions.push(character_select_sprite_action);
                                    character_select_sprite_hovers[character_select_sprite_scene_node_index]=true;
                                }
                            }

                            else
                            {
                                const bool sprite_previously_hovered(character_select_sprite_hovers[character_select_sprite_scene_node_index]);
                                const bool sprite_previously_pressed(static_cast<bool>(character_select_pressed_index)&&character_select_pressed_index==character_select_sprite_scene_node_index);
                                if(sprite_previously_hovered)
                                {
                                    const bool any_sprite_pressed(static_cast<bool>(character_select_pressed_index));
                                    if(any_sprite_pressed)
                                    {
                                        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>>, 5u>::size_type character_select_pressed_index_value(*character_select_pressed_index);
                                        const action character_select_sprite_action(character_select_sprite_hover_actions_library.at(character_select_pressed_index_value));
                                        actions.push(character_select_sprite_action);
                                    }

                                    if(sprite_previously_pressed)
                                    {
                                        character_select_sprite_hovers[character_select_sprite_scene_node_index]=false;
                                    }

                                    else
                                    {
                                        character_select_sprite_hovers[character_select_sprite_scene_node_index]=false;
                                        incoming_sprite.setColor(sf::Color::White);
                                    }
                                }
                            }

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }
                },
                [this, character_select_sprite_scene_node_index](const float incoming_frame_delta_time, sf::Sprite& incoming_sprite)
                {
                    const bool non_process_frame_state(this_current_state==state::pause||this_current_state!=state::transition);
                    if(non_process_frame_state)
                    {
                        return;
                    }

                    std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>& character_select_sprite_time_points(character_select_sprite_time_point_pairs[static_cast<std::array<std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>, 5u>::size_type>(character_select_sprite_scene_node_index)]);
                    std::chrono::time_point<std::chrono::system_clock>& character_select_sprite_finish_point(character_select_sprite_time_points.second);
                    std::chrono::time_point<std::chrono::system_clock>& character_select_sprite_start_point(character_select_sprite_time_points.first);
                    character_select_sprite_finish_point=std::chrono::system_clock::now();
                    character_select_sprite_duration=character_select_sprite_finish_point-character_select_sprite_start_point;
                    const float character_select_sprite_duration_count(character_select_sprite_duration.count());
                    const bool character_select_sprite_blackening(character_select_sprite_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*51.0f));
                    const bool character_select_sprite_selected(character_select_pressed_index==character_select_sprite_scene_node_index);
                    const std::array<bool, 5u>::const_iterator character_select_sprite_blackeners_finish(character_select_sprite_blackeners.cend());
                    const std::array<bool, 5u>::const_iterator character_select_sprite_blackeners_start(character_select_sprite_blackeners.cbegin());
                    const std::array<bool, 5u>::const_iterator character_select_sprite_blackeners_iterator(std::find(character_select_sprite_blackeners_start, character_select_sprite_blackeners_finish, false));
                    const bool character_select_sprites_blackened(character_select_sprite_blackeners_iterator==character_select_sprite_blackeners_finish);
                    if(character_select_sprite_selected)
                    {
                        character_select_sprite_blackeners[static_cast<std::array<bool, 5u>::size_type>(character_select_sprite_scene_node_index)]=true;
                        incoming_sprite.setColor(sf::Color::White);
                        if(character_select_sprites_blackened)
                        {
                            incoming_sprite.setRotation(0.0f);
                            character_select_scene_transitions[0u]=true;
                        }

                        else
                        {
                            incoming_sprite.rotate(540.0f*incoming_frame_delta_time);
                        }
                    }

                    else if(character_select_sprite_blackening)
                    {
                        if(character_select_sprites_blackened)
                        {
                            character_select_scene_transitions[0u]=true;

                            return;
                        }

                        const bool sprite_color_previously_blackened(character_select_sprite_blackeners[static_cast<std::array<bool, 5u>::size_type>(character_select_sprite_scene_node_index)]);
                        if(sprite_color_previously_blackened)
                        {
                            return;
                        }

                        sf::Color sprite_color(incoming_sprite.getColor());
                        sf::Uint8& sprite_color_blue(sprite_color.b);
                        sf::Uint8& sprite_color_green(sprite_color.g);
                        sf::Uint8& sprite_color_red(sprite_color.r);
                        sprite_color_blue-=5u;
                        sprite_color_green-=5u;
                        sprite_color_red-=5u;
                        incoming_sprite.setColor(sprite_color);
                        character_select_sprite_start_point=std::chrono::system_clock::now();

                        const bool sprite_color_blackened(sprite_color_blue==0u);
                        character_select_sprite_blackeners[static_cast<std::array<bool, 5u>::size_type>(character_select_sprite_scene_node_index)]=sprite_color_blackened;
                    }
                }
            );

            std::unique_ptr<scene_node> layer_child_scene_node(std::move(character_select_sprite_scene_node_pointer));
            insert_child(layer_child_scene_node, layer::user_interface);
        }
    }

    void character_select_scene::set_character_select_text()
    {
        const float character_select_text_horizontal_position(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u));
        const float character_select_text_vertical_position(static_cast<float>(sf::VideoMode::getDesktopMode().height/8u));
        const unsigned int character_select_text_character_size(sf::VideoMode::getDesktopMode().width/10u);

        drawable_adapter_scene_node<sf::Text>& character_select_text(*character_select_text_scene_node);
        character_select_text->setCharacterSize(character_select_text_character_size);
        character_select_text->setFillColor(sf::Color(159u, 24u, 24u));
        character_select_text->setFont(character_select_scene_font);
        character_select_text->setOutlineColor(sf::Color(162u, 165u, 101u));
        character_select_text->setOutlineThickness(3.0f);
        character_select_text->setString("CHARACTER SELECT");
        character_select_text->setStyle(sf::Text::Bold|sf::Text::Italic);

        const sf::FloatRect character_select_text_rectangle(character_select_text->getLocalBounds());
        const float character_select_text_origin_horizontal_position(character_select_text_rectangle.left+character_select_text_rectangle.width/2.0f);
        const float character_select_text_origin_vertical_position(character_select_text_rectangle.top+character_select_text_rectangle.height/2.0f);
        character_select_text->setOrigin(character_select_text_origin_horizontal_position, character_select_text_origin_vertical_position);
        character_select_text->setPosition(character_select_text_horizontal_position, character_select_text_vertical_position);
        character_select_text.insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Text& incoming_text)
            {
                incoming_render_target.draw(incoming_text, incoming_render_states);
            },
            [this](const float incoming_frame_delta_time, const action incoming_action, sf::Text& incoming_text)
            {
                const bool non_process_action_state(this_current_state==state::pause||this_current_state!=state::transition);
                if(non_process_action_state)
                {
                    return;
                }

                switch(incoming_action)
                {
                    case action::change_character_select_text:
                    {
                        const sf::String character_select_transition_string(get_character_select_transition_string());
                        const sf::Vector2f character_select_text_position(static_cast<float>(sf::VideoMode::getDesktopMode().width/2u), static_cast<float>(sf::VideoMode::getDesktopMode().height/8u));
                        set_character_description_text(character_select_transition_string, character_select_text_position, incoming_text);

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            },
            nullptr,
            [this](const float incoming_frame_delta_time, sf::Text& incoming_text)
            {
                const bool non_process_frame_state(this_current_state==state::pause||this_current_state!=state::transition);
                if(non_process_frame_state)
                {
                    return;
                }

                const bool character_select_text_outline_switches_finished(character_select_text_outline_switch_count==51u);
                if(character_select_text_outline_switches_finished)
                {
                    incoming_text.setOutlineColor(sf::Color(162u, 165u, 101u));
                    character_select_scene_transitions[1u]=true;

                    return;
                }

                std::chrono::time_point<std::chrono::system_clock>& character_select_text_finish_point(character_select_text_time_points.second);
                std::chrono::time_point<std::chrono::system_clock>& character_select_text_start_point(character_select_text_time_points.first);
                character_select_text_finish_point=std::chrono::system_clock::now();
                character_select_text_duration=character_select_text_finish_point-character_select_text_start_point;
                const float character_select_text_duration_count(character_select_text_duration.count());
                const bool do_character_select_text_outline_switch(character_select_text_duration_count>=incoming_frame_delta_time/(incoming_frame_delta_time*30.0f));
                if(do_character_select_text_outline_switch)
                {
                    const bool change_character_select_text_outline_color(character_select_text_outline_switch_count%2u!=0u);
                    if(change_character_select_text_outline_color)
                    {
                        incoming_text.setOutlineColor(sf::Color::Blue);
                    }

                    else
                    {
                        incoming_text.setOutlineColor(sf::Color(162u, 165u, 101u));
                    }

                    ++character_select_text_outline_switch_count;
                    character_select_text_start_point=std::chrono::system_clock::now();
                }
            }
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(character_select_text_scene_node));
        insert_child(layer_child_scene_node, layer::non_interactable);
    }

    void character_select_scene::set_character_select_texture(const std::array<sf::Texture, 5u>::size_type incoming_character_select_texture_index, const std::pair<std::size_t, std::size_t>& incoming_frame_dimensions, const graphics_resource_holder::identifier incoming_identifier)
    {
        const std::array<sf::Texture, 5u>::size_type character_select_textures_size(character_select_textures.size());
        const bool character_select_texture_index_valid(incoming_character_select_texture_index<character_select_textures_size);
        if(!character_select_texture_index_valid)
        {
            return;
        }

        const boost::optional<const std::vector<unsigned char>&> optional_character_select_data(graphics_resource_holder::resources[incoming_identifier]);
        if(!optional_character_select_data)
        {
            return;
        }

        const std::vector<unsigned char>& character_select_data(*optional_character_select_data);
        const texture_sheet character_select_texture_sheet(incoming_frame_dimensions, character_select_data);
        const std::size_t first_character_select_texture_index(0u);
        const std::size_t character_select_texture_frame_count(character_select_texture_sheet.get_frame_count());
        const bool first_character_select_texture_index_valid(first_character_select_texture_index<character_select_texture_frame_count);
        if(!first_character_select_texture_index_valid)
        {
            return;
        }

        const boost::optional<const sf::Texture&> optional_sheet_character_select_texture(character_select_texture_sheet[first_character_select_texture_index]);
        if(!optional_sheet_character_select_texture)
        {
            return;
        }

        const sf::Texture& sheet_character_select_texture(*optional_sheet_character_select_texture);
        sf::Texture& character_select_texture(character_select_textures[incoming_character_select_texture_index]);
        character_select_texture=sheet_character_select_texture;
    }

    void character_select_scene::set_character_select_textures()
    {
        const std::unordered_map<std::array<sf::Texture, 5u>::size_type, std::tuple<std::size_t, std::size_t, graphics_resource_holder::identifier>> character_select_textures_information_library
        {
            {0u, std::make_tuple(32u, 32u, graphics_resource_holder::identifier::crazy_wizard_warrior)},
            {1u, std::make_tuple(32u, 32u, graphics_resource_holder::identifier::psychopath_warrior)},
            {2u, std::make_tuple(32u, 32u, graphics_resource_holder::identifier::purple_orc_warrior)},
            {3u, std::make_tuple(56u, 56u, graphics_resource_holder::identifier::robin_archer_warrior)},
            {4u, std::make_tuple(32u, 32u, graphics_resource_holder::identifier::thief_warrior)}
        };

        const std::unordered_map<std::array<sf::Texture, 5u>::size_type, std::tuple<std::size_t, std::size_t, graphics_resource_holder::identifier>>::const_iterator character_select_textures_information_library_finish(character_select_textures_information_library.cend());
        const std::unordered_map<std::array<sf::Texture, 5u>::size_type, std::tuple<std::size_t, std::size_t, graphics_resource_holder::identifier>>::const_iterator character_select_textures_information_library_start(character_select_textures_information_library.cbegin());
        std::for_each
        (
            character_select_textures_information_library_start,
            character_select_textures_information_library_finish,
            [this](const std::unordered_map<std::array<sf::Texture, 5u>::size_type, std::tuple<std::size_t, std::size_t, graphics_resource_holder::identifier>>::value_type& incoming_character_select_texture_information)
            {
                const std::array<sf::Texture, 5u>::size_type character_select_texture_index(incoming_character_select_texture_information.first);
                const std::tuple<std::size_t, std::size_t, graphics_resource_holder::identifier>& character_select_texture_information(incoming_character_select_texture_information.second);
                const std::pair<std::size_t, std::size_t> frame_dimensions(std::get<0u>(character_select_texture_information), std::get<1u>(character_select_texture_information));
                const graphics_resource_holder::identifier character_select_texture_identifier(std::get<2u>(character_select_texture_information));
                set_character_select_texture(character_select_texture_index, frame_dimensions, character_select_texture_identifier);
            }
        );
    }

    void character_select_scene::set_first_interactable_text_scene_node()
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
                            actions.push(action::change_character_select_text);
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
            nullptr
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(interactable_text_scene_node));
        insert_child(layer_child_scene_node, layer::user_interface);
    }

    void character_select_scene::set_interactable_text_scene_nodes()
    {
        set_first_interactable_text_scene_node();
        set_second_interactable_text_scene_node();
        set_third_interactable_text_scene_node();
    }

    void character_select_scene::set_second_interactable_text_scene_node()
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
                            actions.push(action::change_character_select_text);
                            const bool character_selected(static_cast<bool>(character_select_pressed_index));
                            if(!character_selected)
                            {
                                actions.push(action::select_random_character);
                            }

                            const bool multiplayer(options_system.multiplayer_mode());
                            if(!multiplayer)
                            {
                                actions.push(action::select_random_stage);
                            }
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
            nullptr
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(interactable_text_scene_node));
        insert_child(layer_child_scene_node, layer::user_interface);
    }

    void character_select_scene::set_third_interactable_text_scene_node()
    {
        const boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> optional_interactable_text_scene_node(get_interactable_text_scene_node(2u));
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
                            button_pressed_index=2u;
                            actions.push(action::play_button_press_sound);
                            actions.push(action::change_character_select_text);
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
            nullptr
        );

        std::unique_ptr<scene_node> layer_child_scene_node(std::move(interactable_text_scene_node));
        insert_child(layer_child_scene_node, layer::user_interface);
    }

    boost::optional<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>&> character_select_scene::get_interactable_text_scene_node(const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 3u>::size_type incoming_interactable_text_index)
    {
        const std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 3u>::size_type interactable_text_scene_nodes_size(interactable_text_scene_nodes.size());
        const bool interactable_text_index_valid(incoming_interactable_text_index<interactable_text_scene_nodes_size);
        if(!interactable_text_index_valid)
        {
            return boost::none;
        }

        const bool multiplayer(options_system.multiplayer_mode());
        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 3u>::size_type, sf::Vector2f> interactable_text_scene_node_position_library
        {
            {0u, sf::Vector2f(multiplayer?static_cast<float>(sf::VideoMode::getDesktopMode().width)*1.25f/5.0f:static_cast<float>(sf::VideoMode::getDesktopMode().width*2u/8u), static_cast<float>(sf::VideoMode::getDesktopMode().height*7u/8u))},
            {1u, sf::Vector2f(multiplayer?static_cast<float>(sf::VideoMode::getDesktopMode().width)*2.75f/5.0f:static_cast<float>(sf::VideoMode::getDesktopMode().width)*4.5f/8.0f, static_cast<float>(sf::VideoMode::getDesktopMode().height*7u/8u))},
            {2u, sf::Vector2f(multiplayer?static_cast<float>(sf::VideoMode::getDesktopMode().width)*3.95f/5.0f:static_cast<float>(sf::VideoMode::getDesktopMode().width)*6.50f/8.0f, static_cast<float>(sf::VideoMode::getDesktopMode().height*7u/8u))},
        };

        const sf::String second_interactive_text_string(multiplayer?"STAGE SELECT":"BRAWL");
        const std::unordered_map<std::array<std::unique_ptr<drawable_adapter_scene_node<sf::Text>>, 4u>::size_type, sf::String> interactable_text_scene_node_string_library
        {
            {0u, "MAIN MENU"},
            {1u, second_interactive_text_string},
            {2u, "QUIT"},
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
        interactable_text_scene_node->setFont(character_select_scene_font);
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

    sf::String character_select_scene::get_character_select_transition_string() const
    {
        const bool character_selected(static_cast<bool>(character_select_pressed_index));
        if(!character_selected)
        {
            return "RANDOM!";
        }

        const std::size_t character_select_index(static_cast<std::size_t>(*character_select_pressed_index));
        switch(character_select_index)
        {
            case 0u:
            {
                return "CRAZY WIZARD!";
            }

            case 1u:
            {
                return "PSYCHOPATH!";
            }

            case 2u:
            {
                return "PURPLE ORC!";
            }

            case 3u:
            {
                return "ROBIN ELF!";
            }

            case 4u:
            {
                return "THIEF!";
            }

            default:
            {
                break;
            }
        }

        return "UNKOWN!";
    }

    interactable::action character_select_scene::get_character_select_transition_action() const
    {
        const std::size_t button_pressed_index_value(button_pressed_index.value());
        switch(button_pressed_index_value)
        {
            case 0u:
            {
                return action::transition_start_scene;
            }

            case 1u:
            {
                const bool multiplayer(options_system.multiplayer_mode());
                return multiplayer?action::transition_stage_select_scene:action::transition_game_scene;
            }

            case 2u:
            {
                return action::quit_application;
            }

            default:
            {
                break;
            }
        }

        return action::quit_application;
    }

    character_select_scene::character_select_scene(const sf::RenderWindow& incoming_render_window, const settings_system& incoming_settings_system):
    character_select_scene_transitions
    {
        false,
        false,
        false
    },
    character_select_sprite_blackeners
    {
        false,
        false,
        false,
        false,
        false
    },
    character_select_sprite_hovers
    {
        false,
        false,
        false,
        false,
        false
    },
    character_select_sprite_time_point_pairs
    {
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
        std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now(), std::chrono::system_clock::now())
    },
    character_select_sprite_scene_nodes
    {
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Sprite>>()
    },
    character_select_description_scene_nodes
    {
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>()
    },
    interactable_text_scene_nodes
    {
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>(),
        std::make_unique<drawable_adapter_scene_node<sf::Text>>()
    },
    character_background_sprite_time_points(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
    character_select_text_time_points(std::chrono::system_clock::now(), std::chrono::system_clock::now()),
    character_select_background_scene_node(std::make_unique<drawable_adapter_scene_node<sf::Sprite>>()),
    character_select_text_scene_node(std::make_unique<drawable_adapter_scene_node<sf::Text>>()),
    character_select_text_outline_switch_count(0u),
    render_window(incoming_render_window),
    options_system(incoming_settings_system)
    {
        set_character_select_background_sprite_scene_node();
        set_character_select_textures();
        set_character_select_sprite_scene_nodes();
        set_character_select_scene_font();
        set_character_select_description_scene_nodes();
        set_character_select_text();
        set_interactable_text_scene_nodes();
        this_current_state=state::active;
    }
}
