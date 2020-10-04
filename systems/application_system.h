#ifndef APPLICATION_SYSTEM_H_INCLUDED
#define APPLICATION_SYSTEM_H_INCLUDED

#include<chrono>
#include<boost/optional/optional.hpp>
#include<SFML/Graphics/RenderWindow.hpp>
#include<SFML/Window/Event.hpp>
#include<audio_system.h>
#include<graphics_system.h>
#include<interactable.h>
#include<settings_system.h>

namespace feudal_wars
{
    class application_system:interactable
    {
        const std::chrono::duration<float> frames_per_second_duration;
        std::chrono::duration<float> accumulate_duration;
        std::chrono::duration<float> frame_duration;
        std::chrono::time_point<std::chrono::system_clock> frame_finish_point;
        std::chrono::time_point<std::chrono::system_clock> frame_start_point;
        sf::RenderWindow render_window;
        sf::Event event;
        audio_system auditory_system;
        graphics_system visual_system;
        settings_system options_system;

        void process_action(const float, const action);
        void process_actions();
        void process_application();
        void process_event(const sf::Event&);
        void process_events();
        void process_frame(const float);
        void process_frames();
        void push_next_action(interactable&);

        public:
            application_system();
            application_system(const application_system&)=delete;
            application_system(application_system&&)=delete;

            application_system& operator=(const application_system&)=delete;
            application_system& operator=(application_system&&)=delete;

            void start();
    };
}

#endif
