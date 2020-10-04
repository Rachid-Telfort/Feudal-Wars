#ifndef INPUT_SOURCE_H_INCLUDED
#define INPUT_SOURCE_H_INCLUDED

#include<SFML/Window/Event.hpp>
#include<interactable.h>

namespace feudal_wars
{
    struct input_source:interactable
    {
        input_source()=default;
        input_source(const input_source&)=delete;
        input_source(input_source&&)=delete;

        input_source& operator=(const input_source&)=delete;
        input_source& operator=(input_source&&)=delete;

        virtual ~input_source()=default;

        virtual void process_action(const float, const action)=0;
        virtual void process_event(const sf::Event&)=0;
        virtual void process_frame(const float)=0;
    };
}

#endif
