#include "defs.h"

void scores_menu(SDL_Renderer *renderer)
{
    Square square[8][8];
    SDL_Event event;
    int stop=0;
    int adder=0;
    SDL_Rect goBackButton= {WINDOW_WIDTH-(BUTTON_WIDTH-150)-10, 20,BUTTON_WIDTH-150,BUTTON_HEIGHT};
    SDL_RenderClear(renderer);
    render_menu_background(renderer,square);
    goBackButton.y=20+adder;
    render_button(renderer,"Retour", goBackButton);
    renderScoreMenuHeader(renderer,adder);
    renderPlayersTiles(renderer,adder);
    renderPlayerInfos(renderer,adder);
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if(isCursorOnButton(goBackButton))
            {
                stop=1;
            }
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_DOWN:
                adder=adder-100;
                SDL_RenderClear(renderer);
                render_menu_background(renderer,square);
                goBackButton.y=20+adder;
                render_button(renderer,"Retour", goBackButton);
                renderScoreMenuHeader(renderer,adder);
                renderPlayersTiles(renderer,adder);
                renderPlayerInfos(renderer,adder);
                break;
            case SDLK_UP:
                if(adder<=-100)
                {
                    adder=adder+100;
                    SDL_RenderClear(renderer);
                    render_menu_background(renderer,square);
                    goBackButton.y=20+adder;
                    render_button(renderer,"Retour", goBackButton);
                    renderScoreMenuHeader(renderer,adder);
                    renderPlayersTiles(renderer,adder);
                    renderPlayerInfos(renderer,adder);
                }
                break;
            }
            break;
        }
    }
}

void renderScoreMenuHeader(SDL_Renderer *renderer, int adder)
{
    SDL_Texture *header;
    SDL_Rect font_rect;
    char text[5][100] = {"Nom", "Victoires", "Défaites", "Egalités", "Total"};
    TTF_Font *font_OpenSans = NULL;
    font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 30);
    for(int i = 0; i <= 4; i++)
    {
        header = loadFont_Blended(renderer, font_OpenSans, text[i], 39, 174, 96);
        SDL_QueryTexture(header, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(header, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * (i + 1), (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2+adder);
        SDL_RenderPresent(renderer);
    }
}

void renderPlayersTiles(SDL_Renderer *renderer, int adder)
{
    SDL_Rect tile = {(WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2,
                     (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 50+adder,
                     WINDOW_WIDTH / 1.1 +adder,
                     100
                    };
    int size = nbJoueur();
    printf("%d", size);
    SDL_SetRenderDrawColor(renderer, 230, 126, 34, 255);
    for(int i = 0; i < size; i++)
    {
        SDL_RenderFillRect(renderer, &tile);
        tile.y = tile.y + tile.h + 10;
    }
    SDL_RenderPresent(renderer);
}

void renderPlayerInfos(SDL_Renderer *renderer, int adder)
{
    int size = nbJoueur();
    Joueur players[size];
    char name[size][20];
    char wins[size][20];
    char losses[size][20];
    char ties[size][20];
    char total[size][20];
    SDL_Rect font_rect;
    SDL_Texture *text;
    TTF_Font *font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 30);
    listerJoueur(players);
    for(int i = 0; i <= size; i++)
    {
        strcpy(name[i],players[i].Nom);
        sprintf(wins[i], "%d", players[i].nbWin);
        sprintf(losses[i], "%d", players[i].nbLose);
        sprintf(ties[i], "%d", players[i].nbEgal);
        sprintf(total[i], "%d", players[i].nbGame);
    }
    for(int j = 0; j < size; j++)
    {
        text = loadFont_Blended(renderer, font_OpenSans, name[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30 +adder);
        text = loadFont_Blended(renderer, font_OpenSans, wins[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 2, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30 +adder);
        text = loadFont_Blended(renderer, font_OpenSans, losses[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 3, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30 +adder);
        text = loadFont_Blended(renderer, font_OpenSans, ties[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 4, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30+adder);
        text = loadFont_Blended(renderer, font_OpenSans, total[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 5, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30+adder);
    }
    SDL_RenderPresent(renderer);
}
