#include<audio_resource_holder.h>

const extern unsigned char binary_button_press_sound_wav_end[];
const extern unsigned char binary_button_press_sound_wav_start[];

const extern unsigned char binary_character_select_scene_music_wav_end[];
const extern unsigned char binary_character_select_scene_music_wav_start[];

const extern unsigned char binary_first_battle_stage_music_wav_end[];
const extern unsigned char binary_first_battle_stage_music_wav_start[];

const extern unsigned char binary_Feudal_Wars___8_Bit_Theme___You_BIT_ter_Watch_Out____wav_end[];
const extern unsigned char binary_Feudal_Wars___8_Bit_Theme___You_BIT_ter_Watch_Out____wav_start[];

namespace feudal_wars
{
    namespace audio_resource_holder
    {
        const resource_holder<identifier, std::vector<unsigned char>> resources
        (
            {
                {identifier::button_press_sound, std::vector<unsigned char>(binary_button_press_sound_wav_start, binary_button_press_sound_wav_end)},
                {identifier::character_select_music, std::vector<unsigned char>(binary_character_select_scene_music_wav_start, binary_character_select_scene_music_wav_end)},
                {identifier::first_battle_stage_music, std::vector<unsigned char>(binary_first_battle_stage_music_wav_start, binary_first_battle_stage_music_wav_end)},
                {identifier::start_scene_music, std::vector<unsigned char>(binary_Feudal_Wars___8_Bit_Theme___You_BIT_ter_Watch_Out____wav_start, binary_Feudal_Wars___8_Bit_Theme___You_BIT_ter_Watch_Out____wav_end)}
            }
        );
    }
}
