#include "defs.h"
int init(SDL_Window **g_Window,SDL_Renderer **renderer)
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
    if(0!=SDL_CreateWindowAndRenderer(WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN,g_Window,renderer))
    {
        printf("Erreur SDL_CreateWindowAndRenderer : %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowTitle(*g_Window,"Chess Royal Alpha");
    icon=IMG_Load("sprites/ico.png");
    SDL_SetWindowIcon(*g_Window,icon);
    return 0;
}

SDL_Texture *loadIMG(char path[], SDL_Renderer *renderer)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *texture=NULL;
    surface = IMG_Load(path);
    if(surface==NULL)
    {
        printf("Erreur IMG_Load : %s\n", IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    if(texture==NULL)
    {
        printf("Erreur SDL_CreateTextureFromSurface : %s\n",SDL_GetError());
        return NULL;
    }
    return texture;
}

SDL_Texture *loadFont_Blended(SDL_Renderer *renderer, TTF_Font *police, char texte[], int red, int green, int blue)
{
    SDL_Surface *surface=NULL;
    SDL_Texture *texture=NULL;
    SDL_Color couleur={red, green, blue, 255};
    surface = TTF_RenderText_Blended(police, texte, couleur);
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    if(texture==NULL)
    {
        printf("Erreur SDL_CreateTextureFromSurface : %s\n",SDL_GetError());
        return NULL;
    }
    return texture;
}

int RendTex(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    if(0!=SDL_QueryTexture(texture,NULL,NULL,&rect.w,&rect.h))
    {
        printf("Erreur SDL_QueryTexture : %s\n",SDL_GetError());
        return -1;
    }
    if(0!=SDL_RenderCopy(renderer,texture,NULL,&rect))
    {
        printf("Erreur SDL_RenderCopy : %s\n",SDL_GetError());
        return -1;
    }
    return 0;
}

void render_button(SDL_Renderer *renderer, char text[], SDL_Rect button)
{
    SDL_Rect button_outline = {0, 0, 0, 0};
    int outline = 1;
    SDL_Texture *button_text = NULL;
    SDL_Rect font_rect;
    TTF_Font *font_OpenSans = NULL;
    SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);//Couleur des boutons (intérieur)
    SDL_RenderFillRect(renderer, &button);
    SDL_SetRenderDrawColor(renderer, 41, 128, 185, 255);//Couleur des boutons (contour)
    while(outline < BUTTON_OUTLINE)
    {
        button_outline.w = button.w + 2 * outline;
        button_outline.h = button.h + 2 * outline;
        button_outline.x = button.x - outline;
        button_outline.y = button.y - outline;
        SDL_RenderDrawRect(renderer, &button_outline);
        outline++;
    }
    SDL_RenderPresent(renderer);
    font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 40);
    button_text = loadFont_Blended(renderer, font_OpenSans, text, 236, 240, 241);
    SDL_QueryTexture(button_text, NULL, NULL, &font_rect.w, &font_rect.h);
    RendTex(button_text, renderer, button.x + (button.w-font_rect.w) / 2, button.y + (button.h-font_rect.h) / 2);
    SDL_RenderPresent(renderer);
}
