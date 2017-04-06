#include "defs.h"
SDL_Texture *loadIMG(char path[], SDL_Renderer *g_Renderer);
int init(SDL_Window **g_Window,SDL_Renderer **g_Renderer)
{
    SDL_Surface *icon=NULL;
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return -1;
    }
    if(0 != TTF_Init())
    {
        printf("Erreur TTF_Init : %s\n", SDL_GetError());
        return -1;
    }
    if(0!=SDL_CreateWindowAndRenderer(WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN,g_Window,g_Renderer))
    {
        printf("Erreur SDL_CreateWindowAndRenderer : %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowTitle(*g_Window,"Chess Royal Alpha");
    icon=IMG_Load("sprites/ico.png");
    SDL_SetWindowIcon(*g_Window,icon);
    return 0;
}

SDL_Texture *loadIMG(char path[], SDL_Renderer *g_Renderer)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *texture=NULL;
    surface = IMG_Load(path);
    if(surface==NULL)
    {
        printf("Erreur IMG_Load : %s\n", IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(g_Renderer,surface);
    SDL_FreeSurface(surface);
    if(texture==NULL)
    {
        printf("Erreur SDL_CreateTextureFromSurface : %s\n",SDL_GetError());
        return NULL;
    }
    return texture;
}

SDL_Texture *loadFont_Blended(SDL_Renderer *g_Renderer, TTF_Font *police, char texte[], int red, int green, int blue)
{
    SDL_Surface *surface=NULL;
    SDL_Texture *texture=NULL;
    SDL_Color couleur={red, green, blue, 255};
    surface = TTF_RenderText_Blended(police, texte, couleur);
    texture = SDL_CreateTextureFromSurface(g_Renderer,surface);
    SDL_FreeSurface(surface);
    if(texture==NULL)
    {
        printf("Erreur SDL_CreateTextureFromSurface : %s\n",SDL_GetError());
        return NULL;
    }
    return texture;
}

int RendTex(SDL_Texture *texture, SDL_Renderer *g_Renderer, int x, int y)
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    if(0!=SDL_QueryTexture(texture,NULL,NULL,&rect.w,&rect.h))
    {
        printf("Erreur SDL_QueryTexture : %s\n",SDL_GetError());
        return -1;
    }
    if(0!=SDL_RenderCopy(g_Renderer,texture,NULL,&rect))
    {
        printf("Erreur SDL_RenderCopy : %s\n",SDL_GetError());
        return -1;
    }
    return 0;
}
