#include "Sprites.hpp"

#include "GameObject.hpp"
#include "Io.hpp"
#include "Compression.hpp"


Sprite::Sprite(const std::string& name)
    : Size(0, 0)
    , m_name(name)
{
    init();
}

void Sprite::init()
{
    std::vector<uint8_t> rawData;
    const std::string filepath = SI::FILEPATHS::PATH_SPRITES + m_name;

    bool readSuccess = io::readBinaryFile(filepath, rawData);
    if (!readSuccess) {
        Logger::Critical("[Sprite]: Error reading file: %s", filepath.c_str());
        throw "TODO: Something clever, fallback to default sprite instead of throw";
    }

    deserialize(rawData);
}

void Sprite::serialize(void)
{
    std::vector<uint8_t> compressed;
    const std::string filepath = SI::FILEPATHS::PATH_SPRITES + m_name;

    Compression::CompressSpriteData(m_data, (uint16_t) m_width, (uint16_t) m_height, (uint16_t) m_count, compressed);
    io::writeBinaryFile(filepath, compressed);
}

void Sprite::deserialize(const std::vector<uint8_t>& spriteDataBitmap)
{
    std::vector<uint8_t> decompressed;
    Compression::DecompressSpriteData(spriteDataBitmap, decompressed);
    if (decompressed.size() < 7) {
        // First 6 bytes is the values of width, height, m_count as uint16_t
        Logger::Critical("[Sprite]: Error decompressing sprite %s data", m_name.c_str());
        throw "TODO: Something clever, fallback to default sprite instead of throw";
    }

    auto constructUint16_tFromUint8_t = [&decompressed](size_t i) -> uint16_t {
        return (uint16_t) decompressed[i] | (uint16_t) decompressed[i+1] << 8;
    };

    m_width   = constructUint16_tFromUint8_t(0);
    m_height  = constructUint16_tFromUint8_t(2);
    m_count = constructUint16_tFromUint8_t(4);

    if (m_count * GetTotalSize() != decompressed.size() - 6) {
        Logger::Critical("[Sprite]: Error decompressing sprite %s data: size missmatch", m_name.c_str());
        throw "TODO: Something clever, fallback to default sprite instead of throw";
    }

    NEWoper(m_data, new uint8_t[m_count * GetTotalSize()]);
    for (size_t i = 0; i < m_count * GetTotalSize(); ++i) {
        m_data[i] = decompressed[6+i];
    }
}

Sprite::~Sprite(void)
{
    delete[] m_data;
}

const uint8_t* Sprite::getSpritePtr(size_t offset) const
{
    // This method should only be used on the textSpritsheet.
    // TODO: Create own class for Spritesheets.
    return m_data + offset;
}

const uint8_t* Sprite::getNumberSpritePtr(int32_t number) const
{
    // This method should only be used on the textSpritsheet.
    // TODO: Create own class for Spritesheets. For now we just assert
    //       that the size of the Sprite is the one that the Spritsheet has.
    assertpair(GetTotalSize() == 35, GetTotalSize(), 35);
    assert(0 <= number &&  number < 10);
    return getSpritePtr(16 * GetTotalSize() + number * GetTotalSize());
}

bool Sprite::operator[](size_t i) const
{
    return m_data[i] != 0;
}

Sprites& Sprites::GetInstance(void)
{
    static Sprites instance;
    return instance;
}

const std::vector<Sprite *> * Sprites::GetSpriteAlive(const GameObjectType objectType) const
{
    switch (objectType)
        {
            case GameObjectType::ALIEN_A:       return &m_sprites_aliens[0];
            case GameObjectType::ALIEN_B:       return &m_sprites_aliens[1];
            case GameObjectType::ALIEN_C:       return &m_sprites_aliens[2];
            case GameObjectType::BULLET_ALIEN:  return &m_sprites_alien_bullets;
            case GameObjectType::ALIEN_DEAD:    return &m_sprites_alien_deaths;
            case GameObjectType::PLAYER:        return &m_sprites_player;
            case GameObjectType::BULLET_PLAYER: return &m_sprites_player_bullets;
            default:
                Logger::Debug("Requested spriteType not found: %d", objectType);
                assert(false && "Invalid sprite requested!");
                throw "[ERROR]: Invalid sprite requested!"; // for release-modes
                break;
        }
    return nullptr;
}

const std::vector<Sprite *> * Sprites::GetSpriteDead(const GameObjectType objectType) const
{
    switch (objectType)
    {
        case GameObjectType::ALIEN_A: return &m_sprites_alien_deaths;
        case GameObjectType::ALIEN_B: return &m_sprites_alien_deaths;
        case GameObjectType::ALIEN_C: return &m_sprites_alien_deaths;
        default:                      return nullptr;
    }
}

const std::vector<Sprite *> & Sprites::GetSprite(const GameObjectType objectType) const
{
    switch (objectType)
    {
    case GameObjectType::ALIEN_A:       return m_sprites_aliens[0];
    case GameObjectType::ALIEN_B:       return m_sprites_aliens[1];
    case GameObjectType::ALIEN_C:       return m_sprites_aliens[2];
    case GameObjectType::BULLET_ALIEN:  return m_sprites_alien_bullets;
    case GameObjectType::ALIEN_DEAD:    return m_sprites_alien_deaths;
    case GameObjectType::PLAYER:        return m_sprites_player;
    case GameObjectType::BULLET_PLAYER: return m_sprites_player_bullets;
    default: // Should never branch here. => Bug
        Logger::Debug("Requested spriteType not found: %d", objectType);
        assert(false && "Invalid sprite requested!");
        throw "[ERROR]: Invalid sprite requested!"; // for release-modes
        break;
    }
}

Sprites::Sprites(void)
    : m_sprites_player({ new Sprite("player") })
    , m_sprites_player_bullets({ new Sprite("player_bullet") })
    , m_sprites_alien_deaths({ new Sprite("alien_death") })
    , m_sprites_alien_bullets({ new Sprite("alien_bullet0"), new Sprite("alien_bullet1") })
    , m_sprites_aliens({
        {new Sprite("alien0"), new Sprite("alien1")},
        {new Sprite("alien2"), new Sprite("alien3")},
        {new Sprite("alien4"), new Sprite("alien5")}
    })
    , text_spritesheet("spritesheet_text")
{
    //
}

Sprites::~Sprites(void)
{
    for (const std::vector<Sprite *> & alien : m_sprites_aliens) {
        for (Sprite * ptr : alien) {
            delete ptr;
        }
    }
    for (Sprite * ptr : m_sprites_alien_bullets)
        delete ptr;
    for (Sprite * ptr : m_sprites_alien_deaths)
        delete ptr;
    for (Sprite * ptr : m_sprites_player_bullets)
        delete ptr;
    for (Sprite * ptr : m_sprites_player)
        delete ptr;
}
