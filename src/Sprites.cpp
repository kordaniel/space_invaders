#include "Sprites.h"

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
        io::print_to_stdout_varargs("[Sprite]: Error reading file: ", filepath);
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
        io::print_to_stdout_varargs("[Sprite]: Error decompressing sprite data.");
        throw "TODO: Something clever, fallback to default sprite instead of throw";
    }

    auto constructUint16_tFromUint8_t = [&decompressed](size_t i) -> uint16_t {
        return (uint16_t) decompressed[i] | (uint16_t) decompressed[i+1] << 8;
    };

    m_width   = constructUint16_tFromUint8_t(0);
    m_height  = constructUint16_tFromUint8_t(2);
    m_count = constructUint16_tFromUint8_t(4);

    if (m_count * GetTotalSize() != decompressed.size() - 6) {
        io::print_to_stdout_varargs("[Sprite]: Error decompressing sprite data.");
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

const Sprite& Sprites::getSprite(const SpaceobjectType& spriteType,
                                 SpaceobjectTypeSpriteSelector& spriteSelector) const
{
    switch (spriteType)
    {
        case ALIEN_DEAD:
            return alien_death_sprite;
        case ALIEN_A:
            return alien_sprites[spriteSelector.getCurrentSpriteIdx(spriteType)];
            break;
        case ALIEN_B:
            return alien_sprites[spriteSelector.getCurrentSpriteIdx(spriteType)];
            break;
        case ALIEN_C:
            return alien_sprites[spriteSelector.getCurrentSpriteIdx(spriteType)];
            break;
        case PLAYER:
            return player_sprite;
            break;
        case BULLET_ALIEN:
            return alien_bullet_sprites[spriteSelector.getCurrentSpriteIdx()];
            break;
        case BULLET_PLAYER:
            return player_bullet_sprite;
            break;
        default:
            throw "[ERROR]: Sprite not Found";
            //return alien_death_sprite;
            break;
    }
}

Sprites::Sprites(void)
    : player_sprite("player")
    , player_bullet_sprite("player_bullet")
    , alien_death_sprite("alien_death")
    , alien_sprites{{
        {"alien0"},
        {"alien1"},
        {"alien2"},
        {"alien3"},
        {"alien4"},
        {"alien5"},
      }}
    , alien_bullet_sprites{{
        {"alien_bullet0"},
        {"alien_bullet1"}
    }}
    , text_spritesheet("spritesheet_text")
{
    //
}
