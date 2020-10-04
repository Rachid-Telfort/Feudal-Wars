#ifndef INTERACTABLE_H_INCLUDED
#define INTERACTABLE_H_INCLUDED

#include<queue>
#include<boost/optional/optional.hpp>
#include<SFML/Window/Event.hpp>

namespace feudal_wars
{
    struct interactable
    {
        enum class action:unsigned char
        {
            change_character_select_description_fifth,
            change_character_select_description_first,
            change_character_select_description_fourth,
            change_character_select_description_second,
            change_character_select_description_third,
            change_character_select_text,
            character_attack,
            character_defense,
            character_jump,
            character_select_scene_transitions_ended,
            character_selected,
            character_unselected,
            fifth_character_selected,
            first_character_selected,
            fourth_character_selected,
            move_character_left,
            move_character_right,
            play_button_press_sound,
            play_character_select_music,
            play_first_battle_stage_music,
            play_start_music,
            quit_application,
            resume_audio,
            second_character_selected,
            select_random_character,
            select_random_stage,
            third_character_selected,
            transition_character_select_scene,
            transition_game_scene,
            transition_multiplayer_join_scene,
            transition_settings_scene,
            transition_stage_select_scene,
            transition_start_scene
        };

        protected:
            std::queue<action> actions;

        public:
            virtual ~interactable()=default;

            virtual void process_action(const float, const action)=0;
            virtual void process_event(const sf::Event&)=0;
            virtual void process_frame(const float)=0;

            interactable()=default;
            interactable(const interactable&)=delete;
            interactable(interactable&&)=delete;

            interactable& operator=(const interactable&)=delete;
            interactable& operator=(interactable&&)=delete;

            boost::optional<action> get_next_action();
    };
}

#endif
