#ifndef GAME_SCENE_H_INCLUDED
#define GAME_SCENE_H_INCLUDED

#include<algorithm>
#include<array>
#include<memory>
#include<utility>
#include<SFML/Graphics/RenderWindow.hpp>
#include<boost/optional/optional.hpp>
#include<battle_character.h>
#include<battle_stage.h>
#include<scene.h>
#include<settings_system.h>

namespace feudal_wars
{
    class game_scene:public scene
    {
        std::array<std::unique_ptr<battle_character>, 4u> chosen_battle_characters;
        std::unique_ptr<battle_stage> chosen_battle_stage;

        const settings_system& options_system;
        const sf::RenderWindow& render_window;

        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);
        void set_battle_character(std::unique_ptr<battle_character>&, std::unique_ptr<input_source>&, const std::array<std::unique_ptr<battle_character>, 4u>::size_type);
        void set_battle_characters();
        void set_battle_characters_multiplayer();
        void set_battle_characters_single_player();
        void set_battle_stage();

        public:
            game_scene(const sf::RenderWindow&, const settings_system&);
            game_scene(const game_scene&)=delete;
            game_scene(game_scene&&)=delete;

            game_scene& operator=(const game_scene&)=delete;
            game_scene& operator=(game_scene&&)=delete;
    };
}

#endif
