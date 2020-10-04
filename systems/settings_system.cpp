#include<settings_system.h>

namespace feudal_wars
{
    std::unique_ptr<battle_character> settings_system::generate_battle_character() const
    {
        const std::size_t generated_character_index(const_cast<settings_system&>(*this).character_select_index_distribution(const_cast<settings_system&>(*this).select_index_engine));
        std::unique_ptr<battle_character> generated_battle_character;

        switch(generated_character_index)
        {
            case 0u:
            {
                generated_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 1u:
            {
                generated_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 2u:
            {
                generated_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 3u:
            {
                generated_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 4u:
            {
                generated_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            default:
            {
                break;
            }
        }

        return generated_battle_character;
    }

    settings_system::settings_system():
    multiplayer(false),
    input_preference(false),
    select_index_engine(std::random_device()()),
    character_keyboard_action_map
    {
        {sf::Keyboard::A, action::move_character_left},
        {sf::Keyboard::D, action::move_character_right},
        {sf::Keyboard::Enter, action::character_attack},
        {sf::Keyboard::Backslash, action::character_defense},
        {sf::Keyboard::W, action::character_jump}
    },
    character_select_index_distribution(0u, 4u),
    stage_select_index_distribution(0u, 4u),
    this_aggressiveness_level(artificial_intelligence_input_source::aggressiveness_level::low){}

    bool settings_system::multiplayer_mode() const
    {
        return multiplayer;
    }

    void settings_system::process_action(const float incoming_frame_delta_time, const action incoming_action)
    {
        switch(incoming_action)
        {
            case action::character_unselected:
            {
                character_select_index=boost::none;

                break;
            }

            case action::fifth_character_selected:
            {
                character_select_index=4u;

                break;
            }

            case action::first_character_selected:
            {
                character_select_index=0u;

                break;
            }

            case action::fourth_character_selected:
            {
                character_select_index=3u;

                break;
            }

            case action::second_character_selected:
            {
                character_select_index=1u;

                break;
            }

            case action::third_character_selected:
            {
                character_select_index=2u;

                break;
            }

            case action::select_random_character:
            {
                character_select_index=character_select_index_distribution(select_index_engine);

                break;
            }

            case action::select_random_stage:
            {
                stage_select_index=stage_select_index_distribution(select_index_engine);

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void settings_system::process_event(const sf::Event& incoming_event){}

    void settings_system::process_frame(const float incoming_frame_delta_time){}

    std::unique_ptr<battle_character> settings_system::selected_battle_character() const
    {
        const bool battle_character_selected(static_cast<bool>(character_select_index));
        if(!battle_character_selected)
        {
            return nullptr;
        }

        const std::size_t selected_character_index(*character_select_index);
        std::unique_ptr<battle_character> selected_battle_character;

        switch(selected_character_index)
        {
            case 0u:
            {
                selected_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 1u:
            {
                selected_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 2u:
            {
                selected_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 3u:
            {
                selected_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            case 4u:
            {
                selected_battle_character=std::make_unique<first_battle_character>();

                break;
            }

            default:
            {
                break;
            }
        }

        return selected_battle_character;
    }

    std::unique_ptr<battle_stage> settings_system::selected_battle_stage() const
    {
        const bool battle_stage_selected(static_cast<bool>(stage_select_index));
        if(!battle_stage_selected)
        {
            return nullptr;
        }

        const std::size_t selected_stage_index(*stage_select_index);
        std::unique_ptr<battle_stage> selected_battle_stage;

        switch(selected_stage_index)
        {
            case 0u:
            {
                selected_battle_stage=std::make_unique<first_battle_stage>();

                break;
            }

            case 1u:
            {
                selected_battle_stage=std::make_unique<first_battle_stage>();

                break;
            }

            case 2u:
            {
                selected_battle_stage=std::make_unique<first_battle_stage>();

                break;
            }

            case 3u:
            {
                selected_battle_stage=std::make_unique<first_battle_stage>();

                break;
            }

            case 4u:
            {
                selected_battle_stage=std::make_unique<first_battle_stage>();

                break;
            }

            default:
            {
                break;
            }
        }

        return selected_battle_stage;
    }

    std::unique_ptr<input_source> settings_system::selected_input_source() const
    {
        std::unique_ptr<input_source> selected_input_source;
        if(!input_preference)
        {
            selected_input_source=std::make_unique<keyboard_input_source>(character_keyboard_action_map);
        }

        else
        {
            selected_input_source=std::make_unique<keyboard_input_source>(character_keyboard_action_map);
        }

        return selected_input_source;
    }

    std::array<std::unique_ptr<input_source>, 3u> settings_system::generated_input_sources() const
    {
        std::array<std::unique_ptr<input_source>, 3u> generated_input_sources
        {
            std::make_unique<artificial_intelligence_input_source>(this_aggressiveness_level),
            std::make_unique<artificial_intelligence_input_source>(this_aggressiveness_level),
            std::make_unique<artificial_intelligence_input_source>(this_aggressiveness_level)
        };

        return generated_input_sources;
    }

    std::array<std::unique_ptr<battle_character>, 3u> settings_system::generated_battle_characters() const
    {
        std::array<std::unique_ptr<battle_character>, 3u> generated_battle_characters
        {
            generate_battle_character(),
            generate_battle_character(),
            generate_battle_character()
        };

        return generated_battle_characters;
    }
}
