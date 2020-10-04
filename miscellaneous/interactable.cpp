#include<interactable.h>

namespace feudal_wars
{
    boost::optional<interactable::action> interactable::get_next_action()
    {
        const bool actions_available(!actions.empty());
        if(!actions_available)
        {
            return boost::none;
        }

        const action next_action(actions.front());
        const boost::optional<action> optional_next_action(next_action);
        actions.pop();

        return optional_next_action;
    }
}
