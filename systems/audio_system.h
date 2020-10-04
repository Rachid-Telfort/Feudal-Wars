#ifndef AUDIO_SYSTEM_H_INCLUDED
#define AUDIO_SYSTEM_H_INCLUDED

#include<algorithm>
#include<cstddef>
#include<forward_list>
#include<map>
#include<memory>
#include<vector>
#include<boost/optional/optional.hpp>
#include<SFML/Audio/Music.hpp>
#include<SFML/Audio/Sound.hpp>
#include<SFML/Audio/SoundBuffer.hpp>
#include<SFML/Audio/SoundSource.hpp>
#include<audio_resource_holder.h>
#include<interactable.h>

namespace feudal_wars
{
    class audio_system:public interactable
    {
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::Music>> music_library;
        const std::map<audio_resource_holder::identifier, std::shared_ptr<sf::SoundBuffer>> sound_buffer_library;
        const std::size_t maximum_sound_capacity;
        std::forward_list<sf::Sound> sound_library;
        std::shared_ptr<sf::Music> current_music_entry;
        std::size_t sound_capacity;
        std::vector<std::forward_list<sf::Sound>::iterator> active_sound_iterators;

        void cache_active_sounds();
        void load_music_library();
        void load_sound_buffer_library();
        void play_music(const audio_resource_holder::identifier);
        void play_sound(const audio_resource_holder::identifier);
        void process_key_event(const sf::Event::KeyEvent&);
        void process_music(const std::function<void(sf::Music&)>&);
        void process_sounds(const std::function<void(sf::Sound&)>&);
        void resume_sounds();
        void set_music_volume(const float);
        std::forward_list<sf::Sound>::iterator insert_sound(const audio_resource_holder::identifier);
        std::forward_list<sf::Sound>::iterator load_sound(const audio_resource_holder::identifier);
        std::forward_list<sf::Sound>::iterator reset_sound(const audio_resource_holder::identifier);

        public:
            audio_system();
            audio_system(const audio_system&)=delete;
            audio_system(audio_system&&)=delete;

            audio_system& operator=(const audio_system&)=delete;
            audio_system& operator=(audio_system&&)=delete;

            void process_action(const float, const action);
            void process_event(const sf::Event&);
            void process_frame(const float);
            void start();
    };
}

#endif
