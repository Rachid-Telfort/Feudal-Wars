#ifndef MULTIPLAYER_JOIN_SCENE_H_INCLUDED
#define MULTIPLAYER_JOIN_SCENE_H_INCLUDED

#include<scene.h>

namespace feudal_wars
{
    class multiplayer_join_scene:public scene
    {
        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);

        public:
            multiplayer_join_scene()=default;
            multiplayer_join_scene(const multiplayer_join_scene&)=delete;
            multiplayer_join_scene(multiplayer_join_scene&&)=delete;

            multiplayer_join_scene& operator=(const multiplayer_join_scene&)=delete;
            multiplayer_join_scene& operator=(multiplayer_join_scene&&)=delete;
    };
}

#endif
