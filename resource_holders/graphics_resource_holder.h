#ifndef GRAPHICS_RESOURCE_HOLDER_H_INCLUDED
#define GRAPHICS_RESOURCE_HOLDER_H_INCLUDED

#include<vector>
#include<resource_holder.h>

namespace feudal_wars
{
    namespace graphics_resource_holder
    {
        enum class identifier:unsigned char
        {
            character_select_scene_default,
            crazy_wizard_warrior,
            feudal_wars_font,
            feudal_wars_window_icons,
            first_battle_stage_background,
            psychopath_warrior,
            purple_orc_warrior,
            robin_archer_warrior,
            start_scene_fifth_layer,
            start_scene_first_layer,
            start_scene_fourth_layer,
            start_scene_second_layer,
            start_scene_third_layer,
            thief_warrior
        };

        const extern resource_holder<identifier, std::vector<unsigned char>> resources;
    }
}

#endif
