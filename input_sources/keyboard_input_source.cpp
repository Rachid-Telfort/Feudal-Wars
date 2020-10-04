#include<keyboard_input_source.h>

namespace feudal_wars
{
    keyboard_input_source::keyboard_input_source(const std::map<sf::Keyboard::Key, action>& incoming_keyboard_action_map):keyboard_action_map(incoming_keyboard_action_map){}

    void keyboard_input_source::process_action(const float incoming_frame_delta_time, const action incoming_action){}

    void keyboard_input_source::process_event(const sf::Event& incoming_event){}

    void keyboard_input_source::process_frame(const float incoming_frame_delta_time)
    {
        const std::map<sf::Keyboard::Key, interactable::action>::const_iterator keyboard_action_map_finish(keyboard_action_map.cend());
        const std::map<sf::Keyboard::Key, interactable::action>::const_iterator keyboard_action_map_start(keyboard_action_map.cbegin());
        std::for_each
        (
            keyboard_action_map_start,
            keyboard_action_map_finish,
            [this](const std::map<sf::Keyboard::Key, action>::value_type& incoming_keyboard_action_entry)
            {
                const sf::Keyboard::Key incoming_keyboard_key(incoming_keyboard_action_entry.first);
                const bool incoming_keyboard_key_pressed(sf::Keyboard::isKeyPressed(incoming_keyboard_key));
                if(incoming_keyboard_key_pressed)
                {
                    const action incoming_keyboard_action(incoming_keyboard_action_entry.second);
                    actions.push(incoming_keyboard_action);
                }
            }
        );
    }
}
