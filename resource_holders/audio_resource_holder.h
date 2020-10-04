#ifndef AUDIO_RESOURCE_HOLDER_H_INCLUDED
#define AUDIO_RESOURCE_HOLDER_H_INCLUDED

#include<vector>
#include<resource_holder.h>

namespace feudal_wars
{
    namespace audio_resource_holder
    {
        enum class identifier:unsigned char
        {
            button_press_sound,
            character_select_music,
            first_battle_stage_music,
            start_scene_music
        };

        const extern resource_holder<identifier, std::vector<unsigned char>> resources;
    }
}

#endif
