#ifndef ACTION_H
#define ACTION_H

#include <string>


class Action {
public:
    Action();

    Action(std::string name, std::string type);

    [[nodiscard]] const std::string &name() const;

    [[nodiscard]] const std::string &type() const;

private:
    std::string m_name;
    std::string m_type;
};

#endif //ACTION_H
