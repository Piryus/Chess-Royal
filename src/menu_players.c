#include "defs.h"

void select_players(SDL_Renderer *renderer, int isAIGame, Game *partie)
{
    Square square[8][8];
    SDL_Event event;
    int stop = 0;
    SDL_Rect newPlayerButton = {(WINDOW_WIDTH - BUTTON_WIDTH) / 2, 20, BUTTON_WIDTH, BUTTON_HEIGHT};
    int nbPlayers = isAIGame + 1;
    SDL_RenderClear(renderer);
    render_menu_background(renderer, square);
    render_playersList(renderer);
    render_button(renderer, "Nouveau joueur", newPlayerButton);
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            stop = 1;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(isCursorOnButton(renderer, newPlayerButton))
            {
                render_newPlayerMenu(renderer, &stop);
                if(!stop)
                {
                    SDL_RenderClear(renderer);
                    render_menu_background(renderer, square);
                    render_playersList(renderer);
                    render_button(renderer, "Nouveau joueur", newPlayerButton);
                }
            }
        }
    }
}

void render_playersList(SDL_Renderer *renderer)
{
    SDL_Rect playerButton = {(WINDOW_WIDTH - BUTTON_WIDTH) / 2, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    Joueur players[nbJoueur()];
    listerJoueur(players);
    for(int i = 0; i < nbJoueur(); i++)
    {
        playerButton.y = 150 + (BUTTON_HEIGHT + 15) * i;
        render_button(renderer, players[i].Nom, playerButton);
        printf("%d", i);
    }
}

void render_newPlayerMenu(SDL_Renderer *renderer, int *stop_up)
{
    SDL_Event event;
    int stop = 0;
    Square square[8][8];
    char name[40] = {NULL};
    SDL_Rect cancelButton = {WINDOW_WIDTH / 2 - 500, 700, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect createPlayerButton = {WINDOW_WIDTH / 2 + (500 - BUTTON_WIDTH), 700, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect textArea = {WINDOW_WIDTH / 2 - 450, (WINDOW_HEIGHT - 100) / 2, 900, 100};
    SDL_RenderClear(renderer);
    render_menu_background(renderer, square);
    render_button(renderer, "Annuler", cancelButton);
    render_button(renderer, "Créer", createPlayerButton);
    renderFillRect(renderer, textArea, 255, 255, 255, 255);
    SDL_StartTextInput();
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            printf("%s", name);
            if(isCursorOnButton(renderer, cancelButton))
            {
                stop = 1;
                *stop_up = 0;
            }
            else if(isCursorOnButton(renderer, createPlayerButton))
            {
                nouveauJoueur(name);
                stop = 1;
                *stop_up = 1;
            }
            break;
        case SDL_TEXTINPUT:
            strcat(name, event.text.text);
            renderTextInRect(renderer,name,textArea);
            break;
        }
    }
}
