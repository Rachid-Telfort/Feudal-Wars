#include<application_system.h>

namespace feudal_wars
{
    application_system::application_system():frames_per_second_duration(1.0f/60.0f), frame_finish_point(std::chrono::system_clock::now()), frame_start_point(std::chrono::system_clock::now()), visual_system(render_window, options_system){}

    void application_system::process_action(const float incoming_frame_delta_time, const action incoming_action)
    {
        auditory_system.process_action(incoming_frame_delta_time, incoming_action);
        visual_system.process_action(incoming_frame_delta_time, incoming_action);
        options_system.process_action(incoming_frame_delta_time, incoming_action);
    }

    void application_system::process_actions()
    {
        const float frame_delta_time(frames_per_second_duration.count());
        const boost::optional<action> optional_action(get_next_action());
        if(optional_action)
        {
            const action next_action(*optional_action);
            process_action(frame_delta_time, next_action);
        }

        push_next_action(auditory_system);
        push_next_action(visual_system);
        push_next_action(options_system);
    }

    void application_system::process_application()
    {
        while(render_window.isOpen())
        {
            frame_finish_point=std::chrono::system_clock::now();
            frame_duration=frame_finish_point-frame_start_point;
            accumulate_duration+=frame_duration;
            while(accumulate_duration>=frames_per_second_duration)
            {
                accumulate_duration-=frames_per_second_duration;
                process_actions();
                process_events();
                process_frames();
            }

            frame_start_point=std::chrono::system_clock::now();
            visual_system.render();
        }
    }

    void application_system::process_event(const sf::Event& incoming_event)
    {
        auditory_system.process_event(incoming_event);
        visual_system.process_event(incoming_event);
        options_system.process_event(incoming_event);
    }

    void application_system::process_events()
    {
        while(render_window.pollEvent(event))
        {
            process_event(event);
        }
    }

    void application_system::process_frame(const float incoming_frame_delta_time)
    {
        auditory_system.process_frame(incoming_frame_delta_time);
        visual_system.process_frame(incoming_frame_delta_time);
        options_system.process_frame(incoming_frame_delta_time);
    }

    void application_system::process_frames()
    {
        const float frame_delta_time(frames_per_second_duration.count());
        process_frame(frame_delta_time);
    }

    void application_system::push_next_action(interactable& incoming_interactable)
    {
        const boost::optional<action> optional_action(incoming_interactable.get_next_action());
        if(optional_action)
        {
            const action next_action(*optional_action);
            actions.push(next_action);
        }
    }

    void application_system::start()
    {
        auditory_system.start();
        visual_system.start();
        process_application();
    }
}
