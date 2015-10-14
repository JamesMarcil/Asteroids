#ifndef IGAMESTATE_H
#define IGAMESTATE_H

class IGameState
{
private:
    template <typename T>
    friend class StateMachine;

    virtual void Enter( void ) = 0;
    virtual void Update( float deltaTime, float totalTime ) = 0;
    virtual void Render( float deltaTime, float totalTime ) = 0;
    virtual void Exit( void ) = 0;
public:
    virtual ~IGameState() {}
};

#endif
