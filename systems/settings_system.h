#ifndef SETTINGS_SYSTEM_H_INCLUDED
#define SETTINGS_SYSTEM_H_INCLUDED

#include<array>
#include<cstddef>
#include<map>
#include<random>
#include<boost/optional/optional.hpp>
#include<SFML/Window/Keyboard.hpp>
#include<artificial_intelligence_input_source.h>
#include<battle_character.h>
#include<battle_stage.h>
#include<first_battle_character.h>
#include<first_battle_stage.h>
#include<input_source.h>
#include<interactable.h>
#include<keyboard_input_source.h>

namespace feudal_wars
{
    class settings_system:public interactable
    {
        bool multiplayer;
        bool input_preference;
        std::default_random_engine select_index_engine;
        std::map<sf::Keyboard::Key, action> character_keyboard_action_map;
        std::uniform_int_distribution<std::size_t> character_select_index_distribution;
        std::uniform_int_distribution<std::size_t> stage_select_index_distribution;
        boost::optional<std::size_t> character_select_index;
        boost::optional<std::size_t> stage_select_index;
        artificial_intelligence_input_source::aggressiveness_level this_aggressiveness_level;

        std::unique_ptr<battle_character> generate_battle_character() const;

        public:
            settings_system();
            settings_system(const settings_system&)=delete;
            settings_system(settings_system&&)=delete;

            settings_system& operator=(const settings_system&)=delete;
            settings_system& operator=(settings_system&&)=delete;

            bool multiplayer_mode() const;
            void process_action(const float, const action);
            void process_event(const sf::Event&);
            void process_frame(const float);

            std::unique_ptr<battle_character> selected_battle_character() const;
            std::unique_ptr<battle_stage> selected_battle_stage() const;
            std::unique_ptr<input_source> selected_input_source() const;
            std::array<std::unique_ptr<input_source>, 3u> generated_input_sources() const;
            std::array<std::unique_ptr<battle_character>, 3u> generated_battle_characters() const;
    };
}

#endif
