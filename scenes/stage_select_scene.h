#ifndef STAGE_SELECT_SCENE_H_INCLUDED
#define STAGE_SELECT_SCENE_H_INCLUDED

#include<scene.h>

namespace feudal_wars
{
    class stage_select_scene:public scene
    {
        void process_action_this(const float, const action);
        void process_event_this(const sf::Event&);

        public:
            stage_select_scene()=default;
            stage_select_scene(const stage_select_scene&)=delete;
            stage_select_scene(stage_select_scene&&)=delete;

            stage_select_scene& operator=(const stage_select_scene&)=delete;
            stage_select_scene& operator=(stage_select_scene&&)=delete;
    };
}

#endif
