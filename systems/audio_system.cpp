#include<audio_system.h>

namespace feudal_wars
{
    void audio_system::cache_active_sounds()
    {
        for(std::forward_list<sf::Sound>::iterator sound_library_start(sound_library.begin()), sound_library_finish(sound_library.end()); sound_library_start!=sound_library_finish; ++sound_library_start)
        {
            sf::Sound& sound_library_entry(*sound_library_start);
            const sf::SoundSource::Status sound_library_entry_status(sound_library_entry.getStatus());
            const bool sound_active(sound_library_entry_status==sf::SoundSource::Playing);
            if(sound_active)
            {
                active_sound_iterators.push_back(sound_library_start);
                sound_library_entry.pause();
            }
        }
    }

    void audio_system::load_music_library()
    {
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::Music>>::const_iterator music_library_finish(music_library.cend());
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::Music>>::const_iterator music_library_start(music_library.cbegin());
        std::for_each
        (
            music_library_start,
            music_library_finish,
            [](const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::Music>>::value_type& music_library_entry)
            {
                const audio_resource_holder::identifier music_identifier(music_library_entry.first);
                const boost::optional<const std::vector<unsigned char>&> optional_music_entry_data(audio_resource_holder::resources[music_identifier]);
                if(!optional_music_entry_data)
                {
                    return;
                }

                std::shared_ptr<sf::Music> music_entry(music_library_entry.second);
                if(!music_entry)
                {
                    return;
                }

                const std::vector<unsigned char>& music_entry_data(*optional_music_entry_data);
                const unsigned char* const music_entry_data_start(music_entry_data.data());
                const std::size_t music_entry_data_size(static_cast<std::size_t>(music_entry_data.size()));
                music_entry->openFromMemory(music_entry_data_start, music_entry_data_size);
                music_entry->setLoop(true);
            }
        );
    }

    void audio_system::load_sound_buffer_library()
    {
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>>::const_iterator sound_buffer_library_finish(sound_buffer_library.cend());
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>>::const_iterator sound_buffer_library_start(sound_buffer_library.cbegin());
        std::for_each
        (
            sound_buffer_library_start,
            sound_buffer_library_finish,
            [](const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>>::value_type& sound_buffer_library_entry)
            {
                const audio_resource_holder::identifier sound_buffer_identifier(sound_buffer_library_entry.first);
                const boost::optional<const std::vector<unsigned char>&> optional_sound_buffer_entry_data(audio_resource_holder::resources[sound_buffer_identifier]);
                if(!optional_sound_buffer_entry_data)
                {
                    return;
                }

                std::shared_ptr<sf::SoundBuffer> sound_buffer_entry(sound_buffer_library_entry.second);
                if(!sound_buffer_entry)
                {
                    return;
                }

                const std::vector<unsigned char>& sound_buffer_entry_data(*optional_sound_buffer_entry_data);
                const unsigned char* const sound_buffer_entry_data_start(sound_buffer_entry_data.data());
                const std::size_t sound_buffer_entry_data_size(static_cast<std::size_t>(sound_buffer_entry_data.size()));
                sound_buffer_entry->loadFromMemory(sound_buffer_entry_data_start, sound_buffer_entry_data_size);
            }
        );
    }

    void audio_system::play_music(const audio_resource_holder::identifier incoming_identifier)
    {
        if(current_music_entry)
        {
            current_music_entry->stop();
        }

        current_music_entry=music_library.at(incoming_identifier);
        if(current_music_entry)
        {
            current_music_entry->play();
        }
    }

    void audio_system::play_sound(const audio_resource_holder::identifier incoming_identifier)
    {
        const std::forward_list<sf::Sound>::iterator sound_library_finish(sound_library.end());
        const std::forward_list<sf::Sound>::iterator sound_iterator(load_sound(incoming_identifier));
        const bool sound_loaded(sound_iterator!=sound_library_finish);
        if(!sound_loaded)
        {
            return;
        }

        sf::Sound& loaded_sound(*sound_iterator);
        loaded_sound.play();
    }

    void audio_system::process_key_event(const sf::Event::KeyEvent& incoming_key_event)
    {
        const sf::Keyboard::Key key(incoming_key_event.code);
        switch(key)
        {
            case sf::Keyboard::Escape:
            {
                process_music
                (
                    [](sf::Music& incoming_music)
                    {
                        incoming_music.stop();
                    }
                );

                process_sounds
                (
                    [](sf::Sound& incoming_sound)
                    {
                        incoming_sound.stop();
                    }
                );

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void audio_system::process_music(const std::function<void(sf::Music&)>& incoming_process_music_function)
    {
        if(!current_music_entry)
        {
            return;
        }

        if(!incoming_process_music_function)
        {
            return;
        }

        sf::Music& current_music(*current_music_entry);
        incoming_process_music_function(current_music);
    }

    void audio_system::process_sounds(const std::function<void(sf::Sound&)>& incoming_process_sound_function)
    {
        if(!incoming_process_sound_function)
        {
            return;
        }

        const std::forward_list<sf::Sound>::iterator sound_library_finish(sound_library.end());
        const std::forward_list<sf::Sound>::iterator sound_library_start(sound_library.begin());
        std::for_each(sound_library_start, sound_library_finish, incoming_process_sound_function);
    }

    void audio_system::resume_sounds()
    {
        const std::vector<std::forward_list<sf::Sound>::iterator>::iterator active_sound_iterators_finish(active_sound_iterators.end());
        const std::vector<std::forward_list<sf::Sound>::iterator>::iterator active_sound_iterators_start(active_sound_iterators.begin());
        std::for_each
        (
            active_sound_iterators_start,
            active_sound_iterators_finish,
            [](std::forward_list<sf::Sound>::iterator& active_sound_iterator)
            {
                sf::Sound& active_sound(*active_sound_iterator);
                active_sound.play();
            }
        );

        active_sound_iterators.clear();
    }

    void audio_system::set_music_volume(const float incoming_music_volume)
    {
        const bool incoming_music_volume_valid(incoming_music_volume>=0.0f&&incoming_music_volume<=100.0f);
        if(!incoming_music_volume_valid)
        {
            return;
        }

        if(!current_music_entry)
        {
            return;
        }

        current_music_entry->setVolume(incoming_music_volume);
    }

    std::forward_list<sf::Sound>::iterator audio_system::insert_sound(const audio_resource_holder::identifier incoming_identifier)
    {
        const std::forward_list<sf::Sound>::iterator sound_library_finish(sound_library.end());
        const bool maximum_sound_capacity_reached(sound_capacity==maximum_sound_capacity);
        if(maximum_sound_capacity_reached)
        {
            return sound_library_finish;
        }

        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>>::const_iterator sound_buffer_library_entry(sound_buffer_library.find(incoming_identifier));
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>>::const_iterator sound_buffer_library_finish(sound_buffer_library.cend());
        const bool sound_buffer_library_entry_found(sound_buffer_library_entry!=sound_buffer_library_finish);
        if(!sound_buffer_library_entry_found)
        {
            return sound_library_finish;
        }

        const std::shared_ptr<sf::SoundBuffer>& sound_buffer_entry_pointer(sound_buffer_library_entry->second);
        if(!sound_buffer_entry_pointer)
        {
            return sound_library_finish;
        }

        const sf::SoundBuffer& sound_buffer_entry(*sound_buffer_entry_pointer);
        sound_library.emplace_front(sound_buffer_entry);
        ++sound_capacity;
        const std::forward_list<sf::Sound>::iterator sound_library_iterator(sound_library.begin());

        return sound_library_iterator;
    }

    std::forward_list<sf::Sound>::iterator audio_system::load_sound(const audio_resource_holder::identifier incoming_identifier)
    {
        const std::forward_list<sf::Sound>::iterator sound_library_finish(sound_library.end());
        const std::forward_list<sf::Sound>::iterator sound_library_insert_iterator(insert_sound(incoming_identifier));
        const bool sound_inserted(sound_library_insert_iterator!=sound_library_finish);
        if(sound_inserted)
        {
            return sound_library_insert_iterator;
        }

        const std::forward_list<sf::Sound>::iterator sound_library_reset_iterator(reset_sound(incoming_identifier));

        return sound_library_reset_iterator;
    }

    std::forward_list<sf::Sound>::iterator audio_system::reset_sound(const audio_resource_holder::identifier incoming_identifier)
    {
        const std::forward_list<sf::Sound>::iterator sound_library_finish(sound_library.end());
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>>::const_iterator sound_buffer_library_entry(sound_buffer_library.find(incoming_identifier));
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>>::const_iterator sound_buffer_library_finish(sound_buffer_library.cend());
        const bool sound_buffer_library_entry_found(sound_buffer_library_entry!=sound_buffer_library_finish);
        if(!sound_buffer_library_entry_found)
        {
            return sound_library_finish;
        }

        const std::forward_list<sf::Sound>::iterator sound_library_start(sound_library.begin());
        const std::forward_list<sf::Sound>::iterator sound_library_iterator
        (
            std::find_if
            (
                sound_library_start,
                sound_library_finish,
                [](const sf::Sound& incoming_sound)
                {
                    const sf::SoundSource::Status sound_status(incoming_sound.getStatus());
                    const bool sound_stopped(sound_status==sf::SoundSource::Stopped);

                    return sound_stopped;
                }
            )
        );

        const bool stopped_sound_found(sound_library_iterator!=sound_library_finish);
        if(!stopped_sound_found)
        {
            return sound_library_finish;
        }

        const std::shared_ptr<sf::SoundBuffer>& sound_buffer_entry_pointer(sound_buffer_library_entry->second);
        if(!sound_buffer_entry_pointer)
        {
            return sound_library_finish;
        }

        sf::Sound& stopped_sound(*sound_library_iterator);
        const sf::SoundBuffer& sound_buffer_entry(*sound_buffer_entry_pointer);
        stopped_sound.setBuffer(sound_buffer_entry);

        return sound_library_iterator;
    }

    audio_system::audio_system():
    music_library
    {
        {audio_resource_holder::identifier::character_select_music, std::make_shared<sf::Music>()},
        {audio_resource_holder::identifier::first_battle_stage_music, std::make_shared<sf::Music>()},
        {audio_resource_holder::identifier::start_scene_music, std::make_shared<sf::Music>()}
    },
    sound_buffer_library
    {
        {audio_resource_holder::identifier::button_press_sound, std::make_shared<sf::SoundBuffer>()}
    },
    maximum_sound_capacity(30u),
    current_music_entry(music_library.at(audio_resource_holder::identifier::start_scene_music)),
    sound_capacity(0u){}

    void audio_system::process_action(const float incoming_frame_delta_time, const action incoming_action)
    {
        switch(incoming_action)
        {
            case action::play_button_press_sound:
            {
                set_music_volume(25.0f);
                play_sound(audio_resource_holder::identifier::button_press_sound);

                break;
            }

            case action::play_character_select_music:
            {
                play_music(audio_resource_holder::identifier::character_select_music);

                break;
            }

            case action::play_first_battle_stage_music:
            {
                play_music(audio_resource_holder::identifier::first_battle_stage_music);

                break;
            }

            case action::play_start_music:
            {
                play_music(audio_resource_holder::identifier::start_scene_music);

                break;
            }

            case action::quit_application:
            {
                process_music
                (
                    [](sf::Music& incoming_music)
                    {
                        incoming_music.stop();
                    }
                );

                process_sounds
                (
                    [](sf::Sound& incoming_sound)
                    {
                        incoming_sound.stop();
                    }
                );

                break;
            }

            case action::resume_audio:
            {
                process_music
                (
                    [](sf::Music& incoming_music)
                    {
                        incoming_music.play();
                    }
                );

                resume_sounds();

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void audio_system::process_event(const sf::Event& incoming_event)
    {
        const sf::Event::EventType event_type(incoming_event.type);
        switch(event_type)
        {
            case sf::Event::Closed:
            {
                process_music
                (
                    [](sf::Music& incoming_music)
                    {
                        incoming_music.stop();
                    }
                );

                process_sounds
                (
                    [](sf::Sound& incoming_sound)
                    {
                        incoming_sound.stop();
                    }
                );

                break;
            }

            case sf::Event::KeyPressed:
            {
                const sf::Event::KeyEvent key_event(incoming_event.key);
                process_key_event(key_event);

                break;
            }

            case sf::Event::LostFocus:
            {
                process_music
                (
                    [](sf::Music& incoming_music)
                    {
                        incoming_music.pause();
                    }
                );

                cache_active_sounds();

                break;
            }

            default:
            {
                break;
            }
        }
    }

    void audio_system::process_frame(const float incoming_frame_delta_time)
    {
        const std::forward_list<sf::Sound>::const_iterator sound_library_finish(sound_library.cend());
        const std::forward_list<sf::Sound>::const_iterator sound_library_start(sound_library.cend());
        const std::forward_list<sf::Sound>::const_iterator sound_library_iterator
        (
            std::find_if
            (
                sound_library_start,
                sound_library_finish,
                [](const sf::Sound& incoming_sound)
                {
                    const sf::SoundSource::Status incoming_sound_status(incoming_sound.getStatus());
                    const bool sound_playing(incoming_sound_status==sf::SoundSource::Playing);

                    return sound_playing;
                }
            )
        );

        const bool sounds_inactive(sound_library_iterator==sound_library_finish&&current_music_entry);
        if(sounds_inactive)
        {
            current_music_entry->setVolume(100.0f);
        }
    }

    void audio_system::start()
    {
        load_music_library();
        load_sound_buffer_library();
        if(current_music_entry)
        {
            current_music_entry->play();
        }
    }
}
