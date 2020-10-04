#include<first_battle_character.h>

namespace feudal_wars
{
    bool first_battle_character::create_battle_character_scene_node(const scene::state& incoming_scene_state, const std::unique_ptr<battle_stage>& incoming_battle_stage, const std::size_t incoming_battle_character_index)
    {
        const std::vector<sf::Vector2f>& battle_character_starting_positions(incoming_battle_stage->get_starting_positions());
        const std::vector<sf::Vector2f>::size_type battle_character_starting_positions_size(battle_character_starting_positions.size());
        const std::vector<sf::Vector2f>::size_type battle_character_starting_postion_index(static_cast<std::vector<sf::Vector2f>::size_type>(incoming_battle_character_index));
        const bool battle_character_starting_position_valid(battle_character_starting_postion_index<battle_character_starting_positions_size);
        if(!battle_character_starting_position_valid)
        {
            return false;
        }

        const sf::Vector2f& chosen_battle_character_starting_postion(battle_character_starting_positions[battle_character_starting_postion_index]);
        battle_character_scene_node=std::make_unique<drawable_adapter_scene_node<sf::Sprite>>();
        const bool character_facing_set(set_character_facing(incoming_battle_character_index));
        if(!character_facing_set)
        {
            return false;
        }

        drawable_adapter_scene_node<sf::Sprite>& battle_character_sprite_scene_node(*battle_character_scene_node);
        battle_character_sprite_scene_node->setScale(sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/300u), static_cast<float>(sf::VideoMode::getDesktopMode().height/300u)));
        const sf::FloatRect incoming_sprite_rectangle(battle_character_sprite_scene_node->getLocalBounds());
        const float incoming_sprite_origin_horizontal_position(incoming_sprite_rectangle.left+incoming_sprite_rectangle.width/2.0f);
        const float incoming_sprite_origin_vertical_position(incoming_sprite_rectangle.top+incoming_sprite_rectangle.height/2.0f);
        battle_character_sprite_scene_node->setOrigin(incoming_sprite_origin_horizontal_position, incoming_sprite_origin_vertical_position);
        const sf::Vector2f battle_character_sprite_scene_node_origin(battle_character_sprite_scene_node->getOrigin());
        battle_character_sprite_scene_node->setPosition(chosen_battle_character_starting_postion+battle_character_sprite_scene_node_origin*2.0f);
        battle_character_sprite_scene_node.insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            [this, &incoming_scene_state, &incoming_battle_stage](const float incoming_frame_delta_time, sf::Sprite& incoming_sprite)
            {
                const bool non_process_frame_state(incoming_scene_state==scene::state::pause||incoming_scene_state==scene::state::transition);
                if(non_process_frame_state)
                {
                    return;
                }

                if(jumping)
                {
                    jump_character(incoming_frame_delta_time, incoming_battle_stage, incoming_sprite);
                }

                else if(!grounded)
                {
                    ground_character(incoming_frame_delta_time, incoming_battle_stage, incoming_sprite);
                }

                const bool power_state(this_animation_state==animation_state::attack||this_animation_state==animation_state::defense);
                if(power_state)
                {
                    const std::pair<std::size_t, std::size_t>& this_animation_state_boundaries(animation_state_boundaries.at(this_animation_state));
                    const std::size_t last_animation_index(this_animation_state_boundaries.second);
                    const bool last_animation_index_reached(animation_index==last_animation_index);
                    if(last_animation_index_reached)
                    {
                        set_animation_state(animation_state::idle, incoming_sprite);
                    }

                    else
                    {
                        cycle_this_animation(incoming_sprite);
                    }

                    return;
                }

                battle_character_input_source->process_frame(incoming_frame_delta_time);
                boost::optional<interactable::action> optional_character_input_source_action(battle_character_input_source->get_next_action());
                const bool character_input_source_action_valid(static_cast<bool>(optional_character_input_source_action));
                if(!character_input_source_action_valid)
                {
                    const bool character_idle(this_animation_state==animation_state::idle);
                    if(!character_idle)
                    {
                        set_animation_state(animation_state::idle, incoming_sprite);
                    }

                    else
                    {
                        cycle_this_animation(incoming_sprite);
                    }

                    return;
                }

                const interactable::action character_input_source_action(*optional_character_input_source_action);
                switch(character_input_source_action)
                {
                    case interactable::action::character_attack:
                    {
                        set_animation_state(animation_state::attack, incoming_sprite);

                        break;
                    }

                    case interactable::action::character_defense:
                    {
                        set_animation_state(animation_state::defense, incoming_sprite);

                        break;
                    }

                    case interactable::action::character_jump:
                    {
                        if(!jumping&&grounded)
                        {
                            grounded=false;
                            jumping=true;
                        }

                        break;
                    }

                    case interactable::action::move_character_left:
                    {
                        battle_character_face_right=false;
                        const bool character_moving(this_animation_state==animation_state::move);
                        if(!character_moving)
                        {
                            set_animation_state(animation_state::move, incoming_sprite);
                        }

                        else
                        {
                            cycle_this_animation(incoming_sprite);
                        }

                        move_character(incoming_frame_delta_time, -1000.0f, incoming_battle_stage, incoming_sprite);

                        break;
                    }

                    case interactable::action::move_character_right:
                    {
                        battle_character_face_right=true;
                        const bool character_moving(this_animation_state==animation_state::move);
                        if(!character_moving)
                        {
                            set_animation_state(animation_state::move, incoming_sprite);
                        }

                        else
                        {
                            cycle_this_animation(incoming_sprite);
                        }

                        move_character(incoming_frame_delta_time, 1000.0f, incoming_battle_stage, incoming_sprite);

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            }
        );

        return true;
    }

    bool first_battle_character::create_texture_sheets()
    {
        const boost::optional<const std::vector<unsigned char>&> optional_first_battle_character_data(graphics_resource_holder::resources[graphics_resource_holder::identifier::crazy_wizard_warrior]);
        if(!optional_first_battle_character_data)
        {
            return false;
        }

        const std::vector<unsigned char>& first_battle_character_data(*optional_first_battle_character_data);
        const unsigned char* const first_battle_character_data_start(first_battle_character_data.data());
        const std::size_t first_battle_character_data_size(static_cast<std::size_t>(first_battle_character_data.size()));

        sf::Image first_battle_character_image;
        first_battle_character_image.loadFromMemory(first_battle_character_data_start, first_battle_character_data_size);
        const texture_sheet first_character_right_texture_sheet(frame_dimensions, first_battle_character_image);
        texture_sheets.push_back(first_character_right_texture_sheet);

        first_battle_character_image.flipHorizontally();
        const texture_sheet first_character_left_texture_sheet(frame_dimensions, first_battle_character_image);
        texture_sheets.push_back(first_character_left_texture_sheet);

        return true;
    }

    bool first_battle_character::set_character_facing(const std::size_t incoming_battle_character_index)
    {
        const bool battle_character_scene_node_valid(static_cast<bool>(battle_character_scene_node));
        if(!battle_character_scene_node_valid)
        {
            return false;
        }

        drawable_adapter_scene_node<sf::Sprite>& battle_character_sprite_scene_node(*battle_character_scene_node);
        battle_character_face_right=incoming_battle_character_index%2u==0u;
        if(battle_character_face_right)
        {
            const texture_sheet& right_texture_sheet(texture_sheets[0u]);
            const boost::optional<const sf::Texture&> optional_first_right_texture(right_texture_sheet[animation_index]);
            const bool optional_right_texture_sheet_valid(static_cast<bool>(optional_first_right_texture));
            if(!optional_right_texture_sheet_valid)
            {
                return false;
            }

            const sf::Texture& first_right_texture(*optional_first_right_texture);
            battle_character_sprite_scene_node->setTexture(first_right_texture);
        }

        else
        {
            const texture_sheet& left_texture_sheet(texture_sheets[1u]);
            const boost::optional<const sf::Texture&> optional_first_left_texture(left_texture_sheet[animation_index]);
            const bool optional_left_texture_sheet_valid(static_cast<bool>(optional_first_left_texture));
            if(!optional_left_texture_sheet_valid)
            {
                return false;
            }

            const sf::Texture& first_left_texture(*optional_first_left_texture);
            battle_character_sprite_scene_node->setTexture(first_left_texture);
        }

        return true;
    }

    void first_battle_character::cycle_this_animation(sf::Sprite& incoming_sprite)
    {
        const std::pair<std::size_t, std::size_t>& this_animation_state_boundaries(animation_state_boundaries.at(this_animation_state));
        const std::size_t last_animation_index(this_animation_state_boundaries.second);
        const bool last_animation_index_reached(animation_index==last_animation_index);
        if(last_animation_index_reached)
        {
            const std::size_t first_animation_index(this_animation_state_boundaries.first);
            animation_index=first_animation_index;
        }

        else
        {
            ++animation_index;
        }

        if(battle_character_face_right)
        {
            const texture_sheet& right_texture_sheet(texture_sheets[0u]);
            const boost::optional<const sf::Texture&> optional_first_right_texture(right_texture_sheet[animation_index]);
            const bool optional_right_texture_sheet_valid(static_cast<bool>(optional_first_right_texture));
            if(!optional_right_texture_sheet_valid)
            {
                return;
            }

            const sf::Texture& first_right_texture(*optional_first_right_texture);
            incoming_sprite.setTexture(first_right_texture);
        }

        else
        {
            const texture_sheet& left_texture_sheet(texture_sheets[1u]);
            const boost::optional<const sf::Texture&> optional_first_left_texture(left_texture_sheet[animation_index]);
            const bool optional_left_texture_sheet_valid(static_cast<bool>(optional_first_left_texture));
            if(!optional_left_texture_sheet_valid)
            {
                return;
            }

            const sf::Texture& first_left_texture(*optional_first_left_texture);
            incoming_sprite.setTexture(first_left_texture);
        }
    }

    void first_battle_character::ground_character(const float incoming_frame_delta_time, const std::unique_ptr<battle_stage>& incoming_battle_stage, sf::Sprite& incoming_sprite)
    {
        cycle_this_animation(incoming_sprite);

        const sf::Vector2f incoming_battle_stage_vertical_boundaries(incoming_battle_stage->get_vertical_boundaries());
        const sf::Vector2f incoming_sprite_origin(incoming_sprite.getOrigin());
        const sf::Vector2f incoming_sprite_position(incoming_sprite.getPosition());
        const float incoming_battle_stage_vertical_boundary(incoming_battle_stage_vertical_boundaries.y);
        const float incoming_sprite_vertical_origin(incoming_sprite_origin.y);
        const float incoming_sprite_vertical_position(incoming_sprite_position.y);

        if(incoming_sprite_vertical_position<incoming_battle_stage_vertical_boundary+incoming_sprite_vertical_origin*2.0f)
        {
            incoming_sprite.move(0.0f, 1000.0f*incoming_frame_delta_time);
        }

        else if(incoming_sprite_vertical_position>incoming_battle_stage_vertical_boundary+incoming_sprite_vertical_origin*2.0f)
        {
            const float incoming_sprite_horizontal_position(incoming_sprite_position.x);
            incoming_sprite.setPosition(incoming_sprite_horizontal_position, incoming_battle_stage_vertical_boundary+incoming_sprite_vertical_origin*2.0f);
            grounded=true;
            jumping=false;
        }
    }

    void first_battle_character::jump_character(const float incoming_frame_delta_time, const std::unique_ptr<battle_stage>& incoming_battle_stage, sf::Sprite& incoming_sprite)
    {
        cycle_this_animation(incoming_sprite);

        const sf::Vector2f incoming_battle_stage_vertical_boundaries(incoming_battle_stage->get_vertical_boundaries());
        const sf::Vector2f incoming_sprite_origin(incoming_sprite.getOrigin());
        const sf::Vector2f incoming_sprite_position(incoming_sprite.getPosition());
        const float incoming_battle_stage_vertical_boundary(incoming_battle_stage_vertical_boundaries.x);
        const float incoming_sprite_vertical_origin(incoming_sprite_origin.y);
        const float incoming_sprite_vertical_position(incoming_sprite_position.y);
        const bool jumping_peak_reached(incoming_sprite_vertical_position<=incoming_battle_stage_vertical_boundary+incoming_sprite_vertical_origin*2.0f);
        if(!jumping_peak_reached)
        {
            incoming_sprite.move(0.0f, -1000.0f*incoming_frame_delta_time);
        }

        else
        {
            const float incoming_sprite_horizontal_position(incoming_sprite_position.x);
            incoming_sprite.setPosition(incoming_sprite_horizontal_position, incoming_battle_stage_vertical_boundary+incoming_sprite_vertical_origin*2.0f);
            grounded=false;
            jumping=false;
        }
    }

    void first_battle_character::move_character(const float incoming_frame_delta_time, const float incoming_movement_speed, const std::unique_ptr<battle_stage>& incoming_battle_stage, sf::Sprite& incoming_sprite)
    {
        const sf::Vector2f incoming_battle_stage_horizontal_boundaries(incoming_battle_stage->get_horizontal_boundaries());
        const sf::Vector2f incoming_sprite_origin(incoming_sprite.getOrigin());
        const float incoming_battle_stage_left_horizontal_boundary(incoming_battle_stage_horizontal_boundaries.x);
        const float incoming_battle_stage_right_horizontal_boundary(incoming_battle_stage_horizontal_boundaries.y);
        const float incoming_sprite_vertical_origin(incoming_sprite_origin.y);

        incoming_sprite.move(incoming_movement_speed*incoming_frame_delta_time, 0.0f);

        const sf::Vector2f incoming_sprite_position(incoming_sprite.getPosition());
        const float incoming_sprite_horizontal_position(incoming_sprite_position.x);
        if(incoming_sprite_horizontal_position<incoming_battle_stage_left_horizontal_boundary+incoming_sprite_vertical_origin*2.0f)
        {
            const float incoming_sprite_vertical_position(incoming_sprite_position.y);
            incoming_sprite.setPosition(incoming_battle_stage_left_horizontal_boundary+incoming_sprite_vertical_origin*2.0f, incoming_sprite_vertical_position);
        }

        else if(incoming_sprite_horizontal_position>incoming_battle_stage_right_horizontal_boundary-incoming_sprite_vertical_origin*2.0f)
        {
            const float incoming_sprite_vertical_position(incoming_sprite_position.y);
            incoming_sprite.setPosition(incoming_battle_stage_right_horizontal_boundary-incoming_sprite_vertical_origin*2.0f, incoming_sprite_vertical_position);
        }
    }

    void first_battle_character::set_animation_state(const animation_state incoming_animation_state, sf::Sprite& incoming_sprite)
    {
        this_animation_state=incoming_animation_state;
        const std::pair<std::size_t, std::size_t>& this_animation_state_boundaries(animation_state_boundaries.at(this_animation_state));
        const std::size_t first_animation_index(this_animation_state_boundaries.first);
        animation_index=first_animation_index;

        if(battle_character_face_right)
        {
            const texture_sheet& right_texture_sheet(texture_sheets[0u]);
            const boost::optional<const sf::Texture&> optional_first_right_texture(right_texture_sheet[animation_index]);
            const bool optional_right_texture_sheet_valid(static_cast<bool>(optional_first_right_texture));
            if(!optional_right_texture_sheet_valid)
            {
                return;
            }

            const sf::Texture& first_right_texture(*optional_first_right_texture);
            incoming_sprite.setTexture(first_right_texture);
        }

        else
        {
            const texture_sheet& left_texture_sheet(texture_sheets[1u]);
            const boost::optional<const sf::Texture&> optional_first_left_texture(left_texture_sheet[animation_index]);
            const bool optional_left_texture_sheet_valid(static_cast<bool>(optional_first_left_texture));
            if(!optional_left_texture_sheet_valid)
            {
                return;
            }

            const sf::Texture& first_left_texture(*optional_first_left_texture);
            incoming_sprite.setTexture(first_left_texture);
        }
    }

    first_battle_character::first_battle_character():
    battle_character
    (
        std::map<animation_state, std::pair<std::size_t, std::size_t>>
        {
            {animation_state::attack, {30u, 39u}},
            {animation_state::death, {40u, 49u}},
            {animation_state::defense, {10u, 19u}},
            {animation_state::idle, {0u, 9u}},
            {animation_state::move, {20u, 29u}},
        },
        std::make_pair(32u, 32u),
        std::make_pair(power_level::medium, power_level::low)
    ){}

    bool first_battle_character::create_this(const scene::state& incoming_scene_state, const std::unique_ptr<battle_stage>& incoming_battle_stage, const std::size_t incoming_battle_character_index)
    {
        const bool texture_sheets_created(create_texture_sheets());
        if(!texture_sheets_created)
        {
            return false;
        }

        const bool battle_character_scene_node_created(create_battle_character_scene_node(incoming_scene_state, incoming_battle_stage, incoming_battle_character_index));
        if(!battle_character_scene_node_created)
        {
            return false;
        }

        return true;
    }
}
