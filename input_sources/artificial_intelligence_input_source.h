#ifndef ARTIFICIAL_INTELLIGENCE_INPUT_SOURCE_H_INCLUDED
#define ARTIFICIAL_INTELLIGENCE_INPUT_SOURCE_H_INCLUDED

#include<input_source.h>

namespace feudal_wars
{
    struct artificial_intelligence_input_source:public input_source
    {
        enum class aggressiveness_level:unsigned char
        {
            high,
            low,
            medium
        };

        artificial_intelligence_input_source(const aggressiveness_level);
        artificial_intelligence_input_source(const artificial_intelligence_input_source&)=delete;
        artificial_intelligence_input_source(artificial_intelligence_input_source&&)=delete;

        artificial_intelligence_input_source& operator=(const artificial_intelligence_input_source&)=delete;
        artificial_intelligence_input_source& operator=(artificial_intelligence_input_source&&)=delete;

        void process_action(const float, const action);
        void process_event(const sf::Event&);
        void process_frame(const float);
    };
}

#endif
