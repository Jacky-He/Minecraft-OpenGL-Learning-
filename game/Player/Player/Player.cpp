#include "Player.hpp"
#include "Constants.hpp"
#include "Input.hpp"

float Player::s_Height = 1.8f;
float Player::s_Width = 0.6f;
float Player::s_EyeLevel = 1.62f;

std::unique_ptr<VertexArray> Player::s_VAO = nullptr;
std::unique_ptr<VertexBuffer> Player::s_VBO = nullptr;
std::unique_ptr<Renderer> Player::s_Renderer = nullptr;
std::unique_ptr<Shader> Player::s_Shader = nullptr;
std::unique_ptr<IndexBuffer> Player::s_IBO = nullptr;

Player::Player(const std::string& name, glm::vec3 position):m_Name(), m_Position(position), m_MovementState(MovementState::WALKING), m_GravityDownVelocity(glm::zero<glm::vec3>()), m_JumpReset(false), m_SpacePressed(false), m_Inventory(10)
{
    using namespace std::placeholders;
    std::function<void(GLFWwindow*, int, int, int, int)> f1 = std::bind(&Player::KeyboardInput, this, _1, _2, _3, _4, _5);
    m_KeyboardInputListener = Input::addKeyboardInputListener(this, f1);
    std::function<void(GLFWwindow*, int, int, int)> f2 = std::bind(&Player::MouseButtonInput, this, _1, _2, _3, _4);
    m_MouseButtonInputListener = Input::addMouseButtonListener(this, f2);
    
    m_FirstPersonCamera = new Camera(position + glm::vec3(0, s_EyeLevel - s_Height/2.0f, 0));
    SetState(m_MovementState);
    m_Map = new Map();
}

Player::~Player()
{
    delete m_FirstPersonCamera;
    m_KeyboardInputListener.Remove();
    m_MouseButtonInputListener.Remove();
    m_ActiveDirections.clear();
}

void Player::Init()
{
    unsigned int ref [36] =
    {
        0, 1, 2,
        2, 3, 0,
        
        4, 5, 6,
        6, 7, 4,
        
        0, 1, 5,
        5, 4, 0,
        
        3, 2, 6,
        6, 7, 3,
        
        1, 2, 6,
        6, 5, 1,
        
        3, 0, 4,
        4, 7, 3
    };
    
    unsigned int indices [432];
    for (int e = 0; e < 12; e++) for (int i = 0; i < 36; i++) indices[e*36 + i] = ref[i] + e*8;
    s_VAO = std::make_unique<VertexArray>();
    s_VBO = std::make_unique<VertexBuffer>(nullptr, sizeof(float)*384, GL_DYNAMIC_DRAW);
    s_IBO = std::make_unique<IndexBuffer>(indices, 432, GL_DYNAMIC_DRAW);
    s_Shader = std::make_unique<Shader> ("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/shaders/line.shader");
    VertexBufferLayout layout;
    layout.Push<float>(4);
    s_VAO -> AddBuffer(*s_VBO, layout);
    s_Renderer = std::make_unique<Renderer>();
}

void Player::Start()
{
    m_FirstPersonCamera -> Start();
}

void Player::Update()
{
    m_DeltaDistance = m_Speed*Constants::deltaTime;
    
    float f1 = m_GravityDownVelocity.y - Constants::deltaTime*Constants::gravity;
    if (m_GravityOn) m_GravityDownVelocity.y = std::min (f1, Constants::terminalVelocity);
    else m_GravityDownVelocity.y = 0;
    CheckJump();
    Move(GetMovementDirection(), m_DeltaDistance*m_Moving, m_GravityDownVelocity*(float)Constants::deltaTime);
    m_FirstPersonCamera -> Update();
    
    std::pair <glm::vec3, std::pair <BlockType, Direction>> target = GetRayCastTarget();
    m_CurrTarget = target;
    if (target.second.first != BlockType::EMPTY) HighLightHitElement(target.first);
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
            m_GravityOn = true;
            SetSpeed(4.317f);
            break;
        case MovementState::SPRINTING:
            m_GravityOn = true;
            SetSpeed(5.612f);
            break;
        case MovementState::SNEAKING:
            m_GravityOn = true;
            SetSpeed(1.3f);
            break;
        case MovementState::FLYING:
            m_GravityOn = false;
            SetSpeed(11.0f);
            break;
        case MovementState::SPRINTFLYING:
            m_GravityOn = false;
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

//MARK::Collision Detection

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
        BlockType type = m_Map -> GetBlockTypeAtLocation((int)std::round(temp.x), (int)std::round(temp.y), (int)std::round(temp.z));
        if (Util::isCollidable (type))
        {
            res.push_back(glm::vec3 (temp.x - 0.5, temp.y - 0.5, temp.z - 0.5));
        }
    }
    return res;
}

void Player::ProcessCollision (glm::vec3 dir, float distance, glm::vec3 gravitydisplacement)
{
    glm::vec3 displacement = dir*distance + gravitydisplacement;
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
            bool collideX = pos.x + 1.0f >= playerPos.x && playerPos.x + s_Width >= pos.x;
            bool collideY = pos.y + 1.0f >= playerPos.y && playerPos.y + s_Height >= pos.y;
            bool collideZ = pos.z + 1.0f >= playerPos.z && playerPos.z + s_Width >= pos.z;
            if (collideX && collideY && collideZ) {collision = true; break;}
        }
        if (!collision)
        {
            if ((targets[i] - GetPosition()).y == 0.0f) {m_GravityDownVelocity.y = 0.0f; m_JumpReset = true;}
            SetPosition(targets[i]);
            break;
        }
    }
}

//MARK: Movements
void Player::Move (glm::vec3 dir, float distance, glm::vec3 gravitydisplacement)
{
    ProcessCollision(dir, distance, gravitydisplacement);
}

void Player::KeyboardInput (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool active = true;
    if (action == GLFW_RELEASE) active = false;
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP) UpdateMovementInfo(Direction::FORWARD, active);
    if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) UpdateMovementInfo(Direction::BACKWARD, active);
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) UpdateMovementInfo(Direction::LEFT, active);
    if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) UpdateMovementInfo(Direction::RIGHT, active);
    if (key == GLFW_KEY_SPACE)
    {
        if (!m_SpacePressed && action == GLFW_PRESS)
        {
            m_DoublePressTimer.Reset();
            m_SpacePressed = true;
        }
        else if (action == GLFW_PRESS)
        {
            if (m_DoublePressTimer.Report(false) <= 0.5)
            {
                if (m_MovementState == MovementState::FLYING) SetState(MovementState::WALKING);
                else if (m_MovementState == MovementState::WALKING) SetState(MovementState::FLYING);
            }
            m_SpacePressed = false;
        }
        UpdateMovementInfo(Direction::UP, active);
    }
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
        else if (each == Direction::UP)
        {
            if (m_GravityOn) continue;
            curr += glm::normalize(m_Updir);
        }
        else if (each == Direction::DOWN)
        {
            if (m_GravityOn) continue;
            curr -= glm::normalize(m_Updir);
        }
    }
    if (abs(glm::length(curr)) < Constants::EPSILONf) return glm::zero<glm::vec3>();
    return glm::normalize(curr);
}

void Player::CheckJump()
{
    if (!m_GravityOn) return;
    if (m_ActiveDirections.find(Direction::UP) != m_ActiveDirections.end() && m_JumpReset)
    {
        m_GravityDownVelocity.y = Constants::jumpVelocity;
        m_JumpReset = false;
    }
}

std::pair <glm::vec3, std::pair <BlockType, Direction>> Player::GetRayCastTarget()
{
    glm::vec3 dir = glm::normalize(m_FirstPersonCamera -> GetDirection());
    glm::vec3 pos = m_FirstPersonCamera -> GetPosition();
    glm::vec3 prev = glm::zero<glm::vec3>();
    for (int i = 0; i <= 450; i++)
    {
        double dis = 0.01*i;
        glm::vec3 curr = pos + float(dis)*dir;
        curr.x = round(curr.x); curr.y = round(curr.y); curr.z = round(curr.z);
        if (i == 0 || curr != prev)
        {
            BlockType type = m_Map -> GetBlockTypeAtLocation(curr.x, curr.y, curr.z);
            if (Util::isBreakable(type))
            {
                Direction res = Direction::CENTER;
                float mindis = std::numeric_limits<float>::infinity();
                for (int i = 0; i < 6; i++)
                {
                    glm::vec3 duv = Util::s_DirectionsUnitVectors[i];
                    float temp = glm::dot (glm::vec3 (abs(duv.x), abs(duv.y), abs(duv.z)), curr);
                    float temp2 = glm::dot(duv, glm::vec3 (0.5f, 0.5f, 0.5f));
                    glm::vec4 plane = glm::vec4(abs(duv.x), abs(duv.y), abs(duv.z), -(temp + temp2));
                    float temp3 = glm::dot(plane, glm::vec4 (dir, 0));
                    if (temp3 == 0.0f) break;
                    float t = -glm::dot(plane, glm::vec4 (pos, 1))/temp3;
                    glm::vec3 inter = pos + t*dir;
                    float dis = glm::distance(inter, pos);
                    if (inter.x >= curr.x - 0.501f && inter.x <= curr.x + 0.501f && inter.y >= curr.y - 0.501f && inter.y <= curr.y + 0.501f && inter.z >= curr.z - 0.501f && inter.z <= curr.z + 0.501f)
                    {
                        if (dis < mindis)
                        {
                            mindis = dis;
                            res = static_cast<Direction>(i);
                        }
                    }
                }
                return {curr, {type, res}};
            }
        }
        prev = curr;
    }
    return {glm::zero<glm::vec3>(), {BlockType::EMPTY, Direction::CENTER}};
}

void Player::HighLightHitElement(glm::vec3 pos)
{
    float data [384];
    for (int offset = 0; offset < 384; offset += 4)
    {
        glm::vec4 tpos = glm::vec4(Constants::cubeOutLineModel[offset], Constants::cubeOutLineModel[offset + 1], Constants::cubeOutLineModel[offset + 2], Constants::cubeOutLineModel[offset + 3]);
        tpos += glm::vec4(pos, 0);
        data[offset] = tpos.x;
        data[offset + 1] = tpos.y;
        data[offset + 2] = tpos.z;
        data[offset + 3] = tpos.w;
    }
    s_Shader -> SetUniformMat4f("u_MVP", m_FirstPersonCamera -> GetPVMatrix());
    s_VBO -> SetData(data, sizeof(data));
    s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader);
}

void Player::MouseButtonInput (GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) BreakBlock (m_CurrTarget);
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) PlaceBlock (m_CurrTarget);
}

void Player::BreakBlock (std::pair <glm::vec3, std::pair <BlockType, Direction>> target)
{
    if (target.second.first == BlockType::EMPTY) return;
    m_Inventory.AddItem(ItemType::Block, static_cast<int>(target.second.first), 1);
    Map::s_Mutex.lock();
    m_Map -> SetBlockTypeAtLocation((int)round(target.first.x), (int)round(target.first.y), (int)round(target.first.z), BlockType::EMPTY);
    Map::s_Mutex.unlock();
    m_SceneRenderer -> UpdateBlockAtLocation (target.first, BlockType::EMPTY);
}

void Player::PlaceBlock (std::pair <glm::vec3, std::pair <BlockType, Direction>> target)
{
    if (target.second.first == BlockType::EMPTY) return;
    //get selected block in inventory
    Item* item = m_Inventory.GetCurrItem();
    if (item == nullptr) return;
    ItemType type = item -> GetType(); int enumidx = item -> GetEnumIdx();
    if (type != ItemType::Block) return;
    m_Inventory.RemoveItem(type, enumidx, 1);
    //now item might be null, so avoid using item
    int diridx = static_cast<int>(target.second.second);
    glm::vec3 tarpos = target.first + Util::s_DirectionsUnitVectors[diridx];
    Map::s_Mutex.lock();
    m_Map -> SetBlockTypeAtLocation((int)round(tarpos.x), (int)round(tarpos.y), (int)round(tarpos.z), static_cast<BlockType>(enumidx));
    Map::s_Mutex.unlock();
    m_SceneRenderer -> UpdateBlockAtLocation(tarpos, static_cast<BlockType>(enumidx));
}
