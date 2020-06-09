#include "Player.hpp"
#include "Map.hpp"
#include "Constants.hpp"
#include "Input.hpp"

float Player::s_Height = 1.8f;
float Player::s_Width = 0.6f;
float Player::s_EyeLevel = 1.62f;

Player::Player(const std::string& name, glm::vec3 position):m_Name(), m_Position(position), m_MovementState(MovementState::WALKING)
{
    using namespace std::placeholders;
    std::function<void(GLFWwindow*, int, int, int, int)> f = std::bind(&Player::KeyboardInput, this, _1, _2, _3, _4, _5);
    m_KeyboardInputListener = Input::addKeyboardInputListener(this, f);
    
    m_FirstPersonCamera = new Camera(position + glm::vec3(0, s_EyeLevel - s_Height/2.0f, 0));
    SetState(m_MovementState);
}

Player::~Player()
{
    delete m_FirstPersonCamera;
    m_KeyboardInputListener.Remove();
    m_ActiveDirections.clear();
}

void Player::Start()
{
    m_FirstPersonCamera -> Start();
}

void Player::Update()
{
    m_DeltaDistance = m_Speed*Constants::deltaTime;
    if (m_Moving) Move(GetMovementDirection(), m_DeltaDistance);
    
    m_FirstPersonCamera -> Update();
}

void Player::SetSpeed(float speed)
{
    m_Speed = speed;
}

void Player::SetState(MovementState state)
{
    m_MovementState = state;
    switch (state)
    {
        case MovementState::WALKING:
            SetSpeed(4.317f);
            break;
        case MovementState::SPRINTING:
            SetSpeed(5.612f);
            break;
        case MovementState::SNEAKING:
            SetSpeed(1.3f);
            break;
        case MovementState::FLYING:
            SetSpeed(11.0f);
            break;
        case MovementState::SPRINTFLYING:
            SetSpeed(22.0);
            break;
        default: SetSpeed(0);
    }
}

void Player::SetPosition(glm::vec3 position)
{
    m_Position = position;
    m_FirstPersonCamera -> SetPosition(m_Position + glm::vec3(0, s_EyeLevel - s_Height/2.0f, 0));
}

glm::vec3 Player::GetPosition()
{
    return m_Position;
}

glm::vec3 Player::GetBoxCoords(glm::vec3 pos)
{
    return glm::vec3 (pos.x - s_Width/2, pos.y - s_Height/2, pos.z - s_Width/2);
}

std::vector<glm::vec3> Player::GetSurroundingBoxCoords(glm::vec3 origin)
{
    std::vector <glm::vec3> res;
    glm::vec3 currCube = glm::vec3 (std::round(origin.x), std::round(origin.y), std::round(origin.z));
    for (int i = 0; i < 18; i++)
    {
        glm::vec3 temp = currCube + Util::s_DirectionsUnitVectors [i];
        BlockType type = Map::CurrMap -> GetBlockTypeAtLocation((int)std::round(temp.x), (int)std::round(temp.y), (int)std::round(temp.z));
        if (type != BlockType::EMPTY)
        {
            res.push_back(glm::vec3 (temp.x - 0.5, temp.y - 0.5, temp.z - 0.5));
        }
    }
    return res;
}

void Player::ProcessCollision (glm::vec3 dir, float distance)
{
    glm::vec3 displacement = dir*distance;
    std::vector<glm::vec3> targets;
    
    int arr [7] = {7, 6, 5, 3, 4, 2, 1}; //in order of importance
    
    for (int i = 0; i < 7; i++)
    {
        glm::vec3 projected = glm::vec3 ((arr[i]&1)*displacement.x, ((arr[i]>>1)&1)*displacement.y, ((arr[i]>>2)&1)*displacement.z);
        targets.push_back(GetPosition() + projected);
    }
    
    for (int i = 0; i < (int)targets.size(); i++)
    {
        std::vector<glm::vec3> surroundPos = GetSurroundingBoxCoords(targets[i]);
        glm::vec3 playerPos = GetBoxCoords(targets[i]);
        bool collision = false;
        for (int j = 0; j < (int)surroundPos.size(); j++)
        {
            glm::vec3 pos = surroundPos[j];
            bool collideX = pos.x + 1.0f > playerPos.x && playerPos.x + s_Width > pos.x;
            bool collideY = pos.y + 1.0f > playerPos.y && playerPos.y + s_Height > pos.y;
            bool collideZ = pos.z + 1.0f > playerPos.z && playerPos.z + s_Width > pos.z;
            if (collideX && collideY && collideZ) {collision = true; break;}
        }
        if (!collision)
        {
            SetPosition(targets[i]);
            break;
        }
    }
}

//Movements
void Player::Move (glm::vec3 dir, float distance)
{
    ProcessCollision(dir, distance);
}

void Player::KeyboardInput (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool active = true;
    if (action == GLFW_RELEASE) active = false;
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP) UpdateMovementInfo(Direction::FORWARD, active);
    if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) UpdateMovementInfo(Direction::BACKWARD, active);
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) UpdateMovementInfo(Direction::LEFT, active);
    if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) UpdateMovementInfo(Direction::RIGHT, active);
    if (key == GLFW_KEY_SPACE) UpdateMovementInfo(Direction::UP, active);
    if (key == GLFW_KEY_LEFT_SHIFT) UpdateMovementInfo(Direction::DOWN, active);
}

void Player::UpdateMovementInfo(Direction dir, bool active)
{
    if (active) m_ActiveDirections.insert(dir);
    else m_ActiveDirections.erase(dir);
    if (m_ActiveDirections.empty()) m_Moving = false;
    else m_Moving = true;
}

glm::vec3 Player::GetMovementDirection ()
{
    glm::vec3 curr = glm::vec3(0, 0, 0);
    glm::vec3 m_Direction = m_FirstPersonCamera -> GetDirection();
    glm::vec3 m_Updir = glm::vec3 (0, 1, 0);
    for (Direction each: m_ActiveDirections)
    {
        if (each == Direction::FORWARD) curr += glm::normalize(glm::vec3(m_Direction.x, 0.0f, m_Direction.z));
        else if (each == Direction::BACKWARD) curr -= glm::normalize(glm::vec3(m_Direction.x, 0.0f, m_Direction.z));
        else if (each == Direction::LEFT) curr += glm::normalize(glm::cross(m_Updir, m_Direction));
        else if (each == Direction::RIGHT) curr -= glm::normalize(glm::cross(m_Updir, m_Direction));
        else if (each == Direction::UP) curr += glm::normalize(m_Updir);
        else if (each == Direction::DOWN) curr -= glm::normalize(m_Updir);
    }
    if (abs(glm::length(curr)) < Constants::EPSILONf) return glm::zero<glm::vec3>();
    return glm::normalize(curr);
}
