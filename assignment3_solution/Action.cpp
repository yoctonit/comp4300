#include "Action.h"

#include <utility>


Action::Action() = default;

Action::Action(std::string name, std::string type)
        : m_name(std::move(name)), m_type(std::move(type)) {
}

const std::string &Action::name() const {
    return m_name;
}

const std::string &Action::type() const {
    return m_type;
}
