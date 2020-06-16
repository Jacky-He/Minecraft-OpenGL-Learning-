#pragma once
#include "Util.hpp"
#include "Camera.hpp"
#include "Listener.hpp"
#include "Timer.hpp"
#include "Map.hpp"

enum class MovementState
{
    WALKING, SPRINTING, SNEAKING, FLYING, SPRINTFLYING
};

class Player
{
private:
    static float s_Height;
    static float s_Width;
    static float s_Depth;
    static float s_EyeLevel;
    
    Map* m_Map;
    float m_Speed;
    std::string m_Name;
    glm::vec3 m_Position;
    bool m_Moving; //whether the camera is moving or not
    float m_DeltaDistance;
    bool m_JumpReset;
    std::set <Direction> m_ActiveDirections;
    Listener <GLFWwindow*, int, int, int, int> m_KeyboardInputListener;
    MovementState m_MovementState;
    
    void ProcessCollision (glm::vec3 dir, float distance, glm::vec3 gravitydisplacement);
    glm::vec3 GetBoxCoords(glm::vec3 pos);
    std::vector<glm::vec3> GetSurroundingBoxCoords(glm::vec3 origin);
    
    void Move(glm::vec3 dir, float distance, glm::vec3 gravitydisplacement);
    void KeyboardInput (GLFWwindow* window, int key, int scancode, int action, int mods);
    void UpdateMovementInfo(Direction dir, bool active);
    glm::vec3 GetMovementDirection ();
    void CheckJump();
    bool m_SpacePressed;
    Timer m_DoublePressTimer;
    
    //gravity
    bool m_GravityOn;
    glm::vec3 m_GravityDownVelocity;
    
public:
    Camera* m_FirstPersonCamera;

    Player(const std::string& name, glm::vec3 position);
    ~Player();
    void SetState(MovementState state);
    void SetSpeed(float speed);
    void SetPosition(glm::vec3 position);
    glm::vec3 GetPosition ();
    void Update();
    void Start();
};
