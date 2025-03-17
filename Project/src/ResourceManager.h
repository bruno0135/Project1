#pragma once
#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class ResourceType {
    IMG_PLAYER,
    IMG_GHOSTS,
    IMG_MAP,
    IMG_MAP_WHITE,
    IMG_ITEMS,
    IMG_PUNTUATIONS,
    IMG_LETTERS,
    IMG_MENU,
    IMG_MENU_ABOVE,
    IMG_MENU_ANIMATIONS,
    IMG_MENU_TITLE,
    SOUND_MUNCH_1,
    SOUND_MUNCH_2,
    MUSIC_START,
    SOUND_RETREATING,
    SOUND_POWER_PELLET,
    SOUND_SIREN_1,
    SOUND_SIREN_2,
    SOUND_SIREN_3,
    SOUND_SIREN_4,
    SOUND_SIREN_5,
    SOUND_EAT_GHOST,
    SOUND_DEAD,
    SOUND_EAT_FRUIT
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload sound
    AppStatus LoadSound(ResourceType id, const std::string& file_path);
    void ReleaseSound(ResourceType id);

    AppStatus LoadMusic(ResourceType id, const std::string& file_path);
    void ReleaseMusic(ResourceType id);

    //Load and unload texture
    AppStatus LoadTexture(ResourceType id, const std::string& file_path);
    void ReleaseTexture(ResourceType id);

    //Get resources by key
    const Texture2D* GetTexture(ResourceType id) const;
    const Sound* GetSound(ResourceType id) const;
    const Music* GetMusic(ResourceType id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<ResourceType, Texture2D> textures;
    std::unordered_map<ResourceType, Sound> sounds;
    std::unordered_map<ResourceType, Music> musics;
};