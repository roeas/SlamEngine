#pragma once

namespace sl
{

class Event;

class Layer
{
public:
    virtual ~Layer() = default;

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnEvent(Event &event) = 0;
    
    virtual void BeginFrame() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnRender() = 0;
    virtual void EndFrame() = 0;
};

} // namespace sl
