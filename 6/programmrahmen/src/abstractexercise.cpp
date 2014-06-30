
#include "abstractexercise.h"

AbstractExercise::AbstractExercise()
:   m_initialized(false)
{

}

void AbstractExercise::setActive(const bool active)
{
    if(!active)
        return;

    if(!m_initialized)
        m_initialized = initialize();

    if(!m_initialized)
        return;

    active ? enable() : disable();
    m_active = active;
}

const bool AbstractExercise::isActive() const
{
    return m_active;
}

void AbstractExercise::enable()
{
}

void AbstractExercise::disable()
{
}
