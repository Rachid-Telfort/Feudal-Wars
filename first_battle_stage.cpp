#include<first_battle_stage.h>

namespace feudal_wars
{
    void first_battle_stage::set_background_layer_sprite_scene_node()
    {
        const boost::optional<const std::vector<unsigned char>&> optional_background_sprite_data(graphics_resource_holder::resources[graphics_resource_holder::identifier::first_battle_stage_background]);
        if(!optional_background_sprite_data)
        {
            return;
        }

        const std::vector<unsigned char>& background_sprite_data(*optional_background_sprite_data);
        const unsigned char* const background_sprite_data_start(background_sprite_data.data());
        const std::size_t background_sprite_data_size(static_cast<std::size_t>(background_sprite_data.size()));
        background_texture.loadFromMemory(background_sprite_data_start, background_sprite_data_size);
        const sf::Vector2u background_texture_size(background_texture.getSize());
        const bool background_texture_size_valid(background_texture_size.x>0u&&background_texture_size.y>0u);
        if(!background_texture_size_valid)
        {
            return;
        }

        const float background_texture_height(static_cast<float>(background_texture_size.y));
        const float background_texture_width(static_cast<float>(background_texture_size.x));
        const float desktop_height(static_cast<float>(sf::VideoMode::getDesktopMode().height));
        const float desktop_width(static_cast<float>(sf::VideoMode::getDesktopMode().width));
        const float background_sprite_height_scale(desktop_height/background_texture_height);
        const float background_sprite_width_scale(desktop_width/background_texture_width);

        std::unique_ptr<drawable_adapter_scene_node<sf::Sprite>> background_layer_sprite_scene_node_pointer(std::make_unique<drawable_adapter_scene_node<sf::Sprite>>());
        drawable_adapter_scene_node<sf::Sprite>& background_layer_sprite_scene_node(*background_layer_sprite_scene_node_pointer);
        background_layer_sprite_scene_node->setTexture(background_texture);
        background_layer_sprite_scene_node->scale(background_sprite_width_scale, background_sprite_height_scale);
        background_layer_sprite_scene_node.insert_functions
        (
            [](sf::RenderTarget& incoming_render_target, sf::RenderStates incoming_render_states, const sf::Sprite& incoming_sprite)
            {
                incoming_render_target.draw(incoming_sprite, incoming_render_states);
            },
            nullptr,
            nullptr,
            nullptr
        );

        std::multimap<scene::layer, std::unique_ptr<scene_node>>::value_type layered_stage_scene_nodes_entry(scene::layer::background, std::move(background_layer_sprite_scene_node_pointer));
        layered_stage_scene_nodes.insert(std::move(layered_stage_scene_nodes_entry));
    }

    first_battle_stage::first_battle_stage():
    battle_stage
    (
        std::vector<sf::Vector2f>
        {
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/32u), static_cast<float>(sf::VideoMode::getDesktopMode().height)*2.75f/4.0f),
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width*7u/8u), static_cast<float>(sf::VideoMode::getDesktopMode().height)*2.75f/4.0f),
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width/8u), static_cast<float>(sf::VideoMode::getDesktopMode().height)*2.75f/4.0f),
            sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width*7u/9u), static_cast<float>(sf::VideoMode::getDesktopMode().height)*2.75f/4.0f)
        },
        sf::Vector2f(0.0f, static_cast<float>(sf::VideoMode::getDesktopMode().width)),
        sf::Vector2f(0.0f, static_cast<float>(sf::VideoMode::getDesktopMode().height)*2.75f/4.0f),
        interactable::action::play_first_battle_stage_music
    ){}

    void first_battle_stage::create(const scene::state& incoming_scene_state)
    {
        set_background_layer_sprite_scene_node();
    }
}
