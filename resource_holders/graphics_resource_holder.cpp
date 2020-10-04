#include<graphics_resource_holder.h>

const extern unsigned char binary_character_select_scene_default_png_end[];
const extern unsigned char binary_character_select_scene_default_png_start[];

const extern unsigned char binary_crazy_wizard_warrior_png_end[];
const extern unsigned char binary_crazy_wizard_warrior_png_start[];

const extern unsigned char binary_feudal_wars_font_ttf_end[];
const extern unsigned char binary_feudal_wars_font_ttf_start[];

const extern unsigned char binary_feudal_wars_window_icons_png_end[];
const extern unsigned char binary_feudal_wars_window_icons_png_start[];

const extern unsigned char binary_first_battle_stage_background_png_end[];
const extern unsigned char binary_first_battle_stage_background_png_start[];

const extern unsigned char binary_psychopath_warrior_png_end[];
const extern unsigned char binary_psychopath_warrior_png_start[];

const extern unsigned char binary_purple_orc_warrior_png_end[];
const extern unsigned char binary_purple_orc_warrior_png_start[];

const extern unsigned char binary_robin_archer_warrior_png_end[];
const extern unsigned char binary_robin_archer_warrior_png_start[];

const extern unsigned char binary_start_scene_fifth_layer_png_end[];
const extern unsigned char binary_start_scene_fifth_layer_png_start[];

const extern unsigned char binary_start_scene_first_layer_png_end[];
const extern unsigned char binary_start_scene_first_layer_png_start[];

const extern unsigned char binary_start_scene_fourth_layer_png_end[];
const extern unsigned char binary_start_scene_fourth_layer_png_start[];

const extern unsigned char binary_start_scene_second_layer_png_end[];
const extern unsigned char binary_start_scene_second_layer_png_start[];

const extern unsigned char binary_start_scene_third_layer_png_end[];
const extern unsigned char binary_start_scene_third_layer_png_start[];

const extern unsigned char binary_thief_warrior_png_end[];
const extern unsigned char binary_thief_warrior_png_start[];

namespace feudal_wars
{
    namespace graphics_resource_holder
    {
        const resource_holder<identifier, std::vector<unsigned char>> resources
        (
            {
                {identifier::character_select_scene_default, std::vector<unsigned char>(binary_character_select_scene_default_png_start, binary_character_select_scene_default_png_end)},
                {identifier::crazy_wizard_warrior, std::vector<unsigned char>(binary_crazy_wizard_warrior_png_start, binary_crazy_wizard_warrior_png_end)},
                {identifier::feudal_wars_font, std::vector<unsigned char>(binary_feudal_wars_font_ttf_start, binary_feudal_wars_font_ttf_end)},
                {identifier::feudal_wars_window_icons, std::vector<unsigned char>(binary_feudal_wars_window_icons_png_start, binary_feudal_wars_window_icons_png_end)},
                {identifier::psychopath_warrior, std::vector<unsigned char>(binary_psychopath_warrior_png_start, binary_psychopath_warrior_png_end)},
                {identifier::purple_orc_warrior, std::vector<unsigned char>(binary_purple_orc_warrior_png_start, binary_purple_orc_warrior_png_end)},
                {identifier::robin_archer_warrior, std::vector<unsigned char>(binary_robin_archer_warrior_png_start, binary_robin_archer_warrior_png_end)},
                {identifier::first_battle_stage_background, std::vector<unsigned char>(binary_first_battle_stage_background_png_start, binary_first_battle_stage_background_png_end)},
                {identifier::start_scene_fifth_layer, std::vector<unsigned char>(binary_start_scene_fifth_layer_png_start, binary_start_scene_fifth_layer_png_end)},
                {identifier::start_scene_first_layer, std::vector<unsigned char>(binary_start_scene_first_layer_png_start, binary_start_scene_first_layer_png_end)},
                {identifier::start_scene_fourth_layer, std::vector<unsigned char>(binary_start_scene_fourth_layer_png_start, binary_start_scene_fourth_layer_png_end)},
                {identifier::start_scene_second_layer, std::vector<unsigned char>(binary_start_scene_second_layer_png_start, binary_start_scene_second_layer_png_end)},
                {identifier::start_scene_third_layer, std::vector<unsigned char>(binary_start_scene_third_layer_png_start, binary_start_scene_third_layer_png_end)},
                {identifier::thief_warrior, std::vector<unsigned char>(binary_thief_warrior_png_start, binary_thief_warrior_png_end)}
            }
        );
    }
}
