#include "Sprite.h"

Sprite::Sprite(const Texture2D* texture)
{
    img = texture;
    current_anim = -1;
    current_frame = 0;
    current_delay = 0;
    mode = AnimMode::AUTOMATIC;
}
Sprite::~Sprite()
{
    Release();
}
void Sprite::SetNumberAnimations(int num)
{
    animations.clear();
    animations.resize(num);
}
void Sprite::SetAnimationDelay(int id, int delay)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].delay = delay;
    }
}
void Sprite::AddKeyFrame(int id, const Rectangle& rect)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].frames.push_back(rect);
    }
}
void Sprite::SetAnimation(int id, bool l)
{
    if (id >= 0 && id < animations.size())
    {
        current_anim = id;
        current_frame = 0;
        current_delay = animations[current_anim].delay;
        animations[id].loop = l;
    }
}
int Sprite::GetAnimation()
{
    return current_anim;
}
void Sprite::SetCurrentDelay(int i)
{
    current_delay = i;
}
void Sprite::SetManualMode()
{
    mode = AnimMode::MANUAL;
}
void Sprite::SetAutomaticMode()
{
    mode = AnimMode::AUTOMATIC;
}
void Sprite::Update()
{
    // Ambos modos de animaci�n (autom�tico y manual) se realizan con retraso de animaci�n
    if (current_delay > 0)
    {
        current_delay--;
        if (current_delay == 0)
        {
            // Solo el modo de animaci�n autom�tico avanza al siguiente fotograma
            if (mode == AnimMode::AUTOMATIC)
            {
                current_frame++;

                // Verificar si loop es false y si ya estamos en el �ltimo fotograma
                if (!animations[current_anim].loop && current_frame >= animations[current_anim].frames.size())
                {
                    // No avanzar al siguiente fotograma y mantener el fotograma actual
                    current_frame = animations[current_anim].frames.size() - 1;
                }
                else
                {
                    // Si loop es true o a�n no hemos alcanzado el �ltimo fotograma, continuar como de costumbre
                    current_frame %= animations[current_anim].frames.size();
                }

                current_delay = animations[current_anim].delay;
            }
        }
    }
}
void Sprite::NextFrame()
{
    //Next frame is only available in manual animation mode
    if (mode == AnimMode::MANUAL)
    {
        current_delay--;
        if (current_delay <= 0)
        {
            current_frame++;
            current_frame %= animations[current_anim].frames.size();
            current_delay = animations[current_anim].delay;
        }
    }
}
void Sprite::PrevFrame()
{
    //Previous frame is only available in manual animation mode
    if (mode == AnimMode::MANUAL)
    {
        current_delay--;
        if (current_delay <= 0)
        {
            current_frame--;
            current_frame %= animations[current_anim].frames.size();
            current_delay = animations[current_anim].delay;
        }
    }
}
void Sprite::Draw(int x, int y) const
{
    DrawTint(x, y, WHITE);
}
void Sprite::DrawTint(int x, int y, const Color& col) const
{
    if (current_anim >= 0 && current_anim < animations.size())
    {
        Rectangle rect = animations[current_anim].frames[current_frame];
        DrawTextureRec(*img, rect, { (float)x, (float)y }, col);
    }
}
void Sprite::Release()
{
    //Release each animation's frames
    for (auto& animation : animations)
    {
        animation.frames.clear();
    }
    //Clear the animations vector
    animations.clear();
}
