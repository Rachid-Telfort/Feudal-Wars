#ifndef KEYBOARD_INPUT_SOURCE_H_INCLUDED
#define KEYBOARD_INPUT_SOURCE_H_INCLUDED

#include<algorithm>
#include<map>
#include<SFML/Window/Keyboard.hpp>
#include<input_source.h>

namespace feudal_wars
{
    class keyboard_input_source:public input_source
    {
        std::map<sf::Keyboard::Key, action> keyboard_action_map;

        public:
            keyboard_input_source(const std::map<sf::Keyboard::Key, action>&);
            keyboard_input_source(const keyboard_input_source&)=delete;
            keyboard_input_source(keyboard_input_source&&)=delete;

            keyboard_input_source& operator=(const keyboard_input_source&)=delete;
            keyboard_input_source& operator=(keyboard_input_source&&)=delete;

            void process_action(const float, const action);
            void process_event(const sf::Event&);
            void process_frame(const float);
    };
}

#endif
