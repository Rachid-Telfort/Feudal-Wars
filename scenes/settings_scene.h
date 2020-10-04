#ifndef SETTINGS_SCENE_H_INCLUDED
#define SETTINGS_SCENE_H_INCLUDED

#include<scene.h>

namespace feudal_wars
{
    class settings_scene:public scene
    {
        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);

        public:
            settings_scene()=default;
            settings_scene(const settings_scene&)=delete;
            settings_scene(settings_scene&&)=delete;

            settings_scene& operator=(const settings_scene&)=delete;
            settings_scene& operator=(settings_scene&&)=delete;
    };
}

#endif
