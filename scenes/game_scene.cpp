#include<game_scene.h>

namespace feudal_wars
{
    void game_scene::process_action_this(const float incoming_frame_delta_time, const action incoming_action){}

    void game_scene::process_event_this(const sf::Event& incoming_event)
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

    void game_scene::set_battle_character(std::unique_ptr<battle_character>& incoming_battle_character, std::unique_ptr<input_source>& incoming_input_source, const std::array<std::unique_ptr<battle_character>, 4u>::size_type incoming_chosen_battle_character_index)
    {
        const std::array<std::unique_ptr<battle_character>, 4u>::size_type chosen_battle_characters_size(chosen_battle_characters.size());
        const bool incoming_battle_character_index_valid(incoming_chosen_battle_character_index<chosen_battle_characters_size);
        if(!incoming_battle_character_index_valid)
        {
            return;
        }

        chosen_battle_characters[incoming_chosen_battle_character_index]=std::move(incoming_battle_character);
        std::unique_ptr<battle_character>& chosen_battle_character(chosen_battle_characters[incoming_chosen_battle_character_index]);
        const bool chosen_battle_character_valid(static_cast<bool>(chosen_battle_character));
        if(!chosen_battle_character_valid)
        {
            return;
        }

        const std::size_t battle_character_index(static_cast<std::size_t>(incoming_chosen_battle_character_index));
        const bool battle_character_created(chosen_battle_character->create(this_current_state, incoming_input_source, chosen_battle_stage, battle_character_index));
        if(!battle_character_created)
        {
            return;
        }

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>> chosen_battle_character_scene_node(std::move(chosen_battle_character->get_battle_character_scene_node()));
        std::unique_ptr<scene_node> layer_child_scene_node(std::move(chosen_battle_character_scene_node));
        insert_child(layer_child_scene_node, layer::interactable);
    }

    void game_scene::set_battle_characters()
    {
        const bool multiplayer(options_system.multiplayer_mode());
        if(multiplayer)
        {
            set_battle_characters_multiplayer();
        }

        else
        {
            set_battle_characters_single_player();
        }
    }

    void game_scene::set_battle_characters_multiplayer()
    {

    }

    void game_scene::set_battle_characters_single_player()
    {
        std::unique_ptr<battle_character> chosen_battle_character(std::move(options_system.selected_battle_character()));
        std::unique_ptr<input_source> chosen_input_source(std::move(options_system.selected_input_source()));
        set_battle_character(chosen_battle_character, chosen_input_source, 0u);

        std::array<std::unique_ptr<battle_character>, 3u> generated_battle_characters(options_system.generated_battle_characters());
        std::array<std::unique_ptr<input_source>, 3u> generated_input_sources(options_system.generated_input_sources());
        set_battle_character(generated_battle_characters[0], generated_input_sources[0], 1u);
        set_battle_character(generated_battle_characters[1], generated_input_sources[1], 2u);
        set_battle_character(generated_battle_characters[2], generated_input_sources[2], 3u);
    }

    void game_scene::set_battle_stage()
    {
        chosen_battle_stage=std::move(options_system.selected_battle_stage());
        const bool chosen_battle_stage_valid(static_cast<bool>(chosen_battle_stage));
        if(!chosen_battle_stage_valid)
        {
            return;
        }

        chosen_battle_stage->create(this_current_state);
        std::multimap<scene::layer, std::unique_ptr<scene_node>> layered_stage_scene_nodes(chosen_battle_stage->get_layered_stage_scene_nodes());
        const std::multimap<scene::layer, std::unique_ptr<scene_node>>::iterator layered_stage_scene_nodes_finish(layered_stage_scene_nodes.end());
        const std::multimap<scene::layer, std::unique_ptr<scene_node>>::iterator layered_stage_scene_nodes_start(layered_stage_scene_nodes.begin());
        std::for_each
        (
            layered_stage_scene_nodes_start,
            layered_stage_scene_nodes_finish,
            [this](std::multimap<scene::layer, std::unique_ptr<scene_node>>::value_type& layered_stage_scene_nodes_entry)
            {
                const scene::layer incoming_scene_layer(layered_stage_scene_nodes_entry.first);
                std::unique_ptr<scene_node> layer_child_scene_node(std::move(layered_stage_scene_nodes_entry.second));
                insert_child(layer_child_scene_node, incoming_scene_layer);
            }
        );

        const action battle_stage_music_action(chosen_battle_stage->get_music_action());
        actions.push(battle_stage_music_action);
    }

    game_scene::game_scene(const sf::RenderWindow& incoming_render_window, const settings_system& incoming_options_system):options_system(incoming_options_system), render_window(incoming_render_window)
    {
        set_battle_stage();
        set_battle_characters();
    }
}
