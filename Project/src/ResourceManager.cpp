#include "ResourceManager.h"
#include "LogMessages.h"


ResourceManager::ResourceManager()
{

}
ResourceManager::~ResourceManager()
{
    Release();
}

//Load a texture from a file and associate it with the key id
AppStatus ResourceManager::LoadTexture(ResourceType id, const std::string& file_path)
{
    //Load the texture
    Texture2D texture = ::LoadTexture(file_path.c_str());
    if (texture.id == 0)
    {
        //Error loading texture
        LOG("Failed to load texture ", file_path);
        UnloadTexture(texture);
        return AppStatus::ERROR;
    }

    //Insert the loaded texture into the map with the specified key
    textures[id] = texture;
    return AppStatus::OK;
}

//Release the texture associated with the key id
void ResourceManager::ReleaseTexture(ResourceType id)
{
    //Find the texture associated with the key
    auto it = textures.find(id);

    //If found, unload the texture and remove it from the map
    if (it != textures.end())
    {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

AppStatus ResourceManager::LoadSound(ResourceType id, const std::string& file_path)
{
    // Load the sound
    Sound sound = ::LoadSound(file_path.c_str());
    if (sound.frameCount == 0)
    {
        // Error loading sound
        LOG("Failed to load sound ", file_path);
        UnloadSound(sound); // Liberar la memoria asignada para el sonido
        return AppStatus::ERROR;
    }

    // Insertar el sonido cargado en el mapa con la clave especificada
    sounds[id] = sound;
    return AppStatus::OK;
}
void ResourceManager::ReleaseSound(ResourceType id)
{
    //Find the sound associated with the key
    auto it = sounds.find(id);

    //If found, unload the sound and remove it from the map
    if (it != sounds.end())
    {
        UnloadSound(it->second);
        sounds.erase(it);
    }
}

AppStatus ResourceManager::LoadMusic(ResourceType id, const std::string& file_path)
{
    //Load the music
    Music music = ::LoadMusicStream(file_path.c_str());
    if (music.frameCount == 0)
    {
        //Error loading music
        LOG("Failed to load music ", file_path);
        UnloadMusicStream(music);
        return AppStatus::ERROR;
    }

    //Insert the loaded music into the map with the specified key
    musics[id] = music;
    return AppStatus::OK;
}

void ResourceManager::ReleaseMusic(ResourceType id)
{
    //Find the music associated with the key
    auto it = musics.find(id);

    //If found, unload the music and remove it from the map
    if (it != musics.end())
    {
        UnloadMusicStream(it->second);
        musics.erase(it);
    }
}

//Get a texture by key
const Texture2D* ResourceManager::GetTexture(ResourceType id) const
{
    //Find the texture associated with the key and return it
    auto it = textures.find(id);
    if (it != textures.end())   return &(it->second);

    //Return nullptr if key is not found
    return nullptr;
}

const Sound* ResourceManager::GetSound(ResourceType id) const
{
    //Find the texture associated with the key and return it
    auto it = sounds.find(id);
    if (it != sounds.end())   return &(it->second);

    //Return nullptr if key is not found
    return nullptr;
}


const Music* ResourceManager::GetMusic(ResourceType id) const
{
    //Find the texture associated with the key and return it
    auto it = musics.find(id);
    if (it != musics.end())   return &(it->second);

    //Return nullptr if key is not found
    return nullptr;
}


void ResourceManager::Release()
{
    for (auto& pair : textures)
    {
        UnloadTexture(pair.second);
    }
    textures.clear();
    for (auto& pair : sounds)
    {
        UnloadSound(pair.second);
    }
    sounds.clear();
    for (auto& pair : musics)
    {
        UnloadMusicStream(pair.second);
    }
    musics.clear();
}