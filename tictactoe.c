#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#if defined (__linux) || defined (__APPLE__)

static void LimpaTela()
{
    fputs("\x1b[2J",stdout);
}

#else

#include <windows.h>


static void LimpaTela()
{
    HANDLE hConsole;
    COORD coord = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConsoleSize, dwWriten;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConsoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConsoleSize, coord, &dwWriten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConsoleSize, coord, &dwWriten);
    SetConsoleCursorPosition(hConsole, coord);
 
}


#endif

__attribute__((noinline)) static void DesenhaTabuleiro(char *tabuleiro)
{
    
    LimpaTela();
    
    printf
    (
    "__________________\n\n"
    "  %c |  %c | %c \n"
    "____|____|____\n"
    "    |    |    \n"
    "  %c |  %c | %c \n"
    "____|____|____\n"
    "    |    |    \n"
    "  %c |  %c | %c \n"
    "_________________\n\n",
    *(tabuleiro),
    *(tabuleiro + 1),
    *(tabuleiro + 2),
    *(tabuleiro + 3),
    *(tabuleiro + 4),
    *(tabuleiro + 5),
    *(tabuleiro + 6),
    *(tabuleiro + 7),
    *(tabuleiro + 8)
    );
  
}

__attribute__((noinline)) static void MovimentoComputador(char *tabuleiro, char simbolo)
{

    int i = 0, j = 0;
    char simbolos[3] = "OX",ch;// vai servir para definir o modo como o computador ira encarar a varredura
    static char cantos[]  = {0, 2, 8, 6}; // para que o computador possa marcar um dos cantos "aleatoriamente
    static char casas[] =
                        {
                        0,1,2,2,1,0,0,2,1,3,4,5,5,4,3,3,5,4,
                        6,7,8,8,7,6,6,8,7,0,3,6,6,3,0,0,6,3,
                        1,4,7,7,4,1,1,7,4,2,5,8,8,5,2,2,8,5,
                        0,4,8,8,4,0,0,8,4,2,4,6,6,4,2,2,6,4
                        };
  
  
    if (simbolo == 'X')
    {
        simbolos[0] = 'X';
        simbolos[1] = 'O';
    }

    for(j = 0; j < 2; j++) // o jota serve como um chaveador do modo de varredura
    {
        
        ch = simbolos[j];// o computador faz a primeira varredura ofensivamente

        for(i = 0 ; i < sizeof(casas); i+=3 )

            if( ( *(tabuleiro+casas[i]) == ch ) && ( *(tabuleiro+casas[i+1]) == ch))
                if( *(tabuleiro+casas[i+2]) == ' ')
                {
                    *(tabuleiro+casas[i+2]) = simbolo;
                    return;
                    
                    
                } // todos aqueles loops e ifs foram substituidos por apenas esse

    
    //*********************************************************************
    } // fim do loop de chaveamento


    if ( *(tabuleiro + 4) == ' ') // caso nao tenha feito nenhuma jogada de bloqueio, a prioridade eh marcar o centro
    {
        *(tabuleiro + 4) = simbolo;
        return;
    }
    
    if ( *(tabuleiro + 4) == simbolo) // caso o centro esteja marcado, marca na primeira vasia na horizontal ou vertical
        for ( i = 1; i < 9; i += 2 )
            if ( *(tabuleiro + i) == ' ')
            {
                *(tabuleiro + i) = simbolo;
                return;
            }
            
    i = cantos[rand()%4];
    j = 0;

    while( (*(tabuleiro+i) == ' ') && (j++ < 4))
        i = cantos[rand()%4];
    
    *(tabuleiro + i) = simbolo;
    
}

__attribute__((noinline)) static void MovimentoJogador(char *tabuleiro, char simbolo)
{
    int linha = 0, col = 0;
    
    do
    {
        printf("Seu simbolo eh %c \n\nDigite a linha e coluna que deseja preencher\n", simbolo);

        scanf("%d %d", &linha,&col);
    }
    while ( (linha > 3 || linha < 1 || col > 3 || col < 1) || ( *(tabuleiro + (linha - 1) * 3 + col - 1) != ' ' ) );
    
    *(tabuleiro + (linha - 1) * 3 + col - 1) = simbolo;
}

__attribute__((noinline)) static char Check(char *tabuleiro)
{
   
   int i;
    
    for (i = 0 ; i < 3 ; i++) // verifica linhas horizontais
        if ( ( *(tabuleiro + i * 3) != ' ') &&  *(tabuleiro + i * 3) == ( *(tabuleiro + i * 3 + 1) & *(tabuleiro + i * 3 + 2) ))
            return *(tabuleiro + i * 3 );

    for (i = 0 ; i < 3; i++) // verifica linhas verticais
        if ( (*(tabuleiro + i) != ' ') &&  *(tabuleiro + i) == ( *(tabuleiro + 3 + i) & *(tabuleiro + 6 + i) ))
            return *(tabuleiro + i);

    if ( *(tabuleiro) == ( *(tabuleiro + 4) & *(tabuleiro + 8) )) // diagonal direita
        return *(tabuleiro);

    if ( *(tabuleiro + 2) == ( *(tabuleiro + 4) & *(tabuleiro + 6) )) //diagonal esquerda
        return *(tabuleiro + 2);

    for (i = 0 ; i < 9; i++)
    {
        if (i == 8)
            return 'E';

        if ( *(tabuleiro + i) == ' ')
            break;
    }

    return ' ';
}

int main(void)
{

    char simbolo_jogador;
    char simbolo_computador;
    char vencedor ;
    char tabuleiro[3][3];

  /* FRED: Ok... só uma dica: gettimeofday() vai te dar uma semente
           ainda mais "aleatória" para o LCG. */
  /*BRUNO: A qual biblioteca essa função, gettimeofday, pertence ?*/
    srand((unsigned)time(NULL));

    while (1)
    {
        memset(tabuleiro, ' ', sizeof(tabuleiro));
        puts("Qual simbolo voce escolhe X ou O? S para sair");
        simbolo_jogador = toupper(getchar());

        if (simbolo_jogador == 'S')
            break;

        if (simbolo_jogador == 'X')
        {
            simbolo_computador = 'O';
            
            DesenhaTabuleiro((char *)tabuleiro);
            MovimentoJogador((char *)tabuleiro, simbolo_jogador);
            DesenhaTabuleiro((char *)tabuleiro);
        }
        else
        {
            simbolo_jogador = 'O';
            simbolo_computador = 'X';
        }

        while (1)
        {
          
            MovimentoComputador((char *)tabuleiro, simbolo_computador);
            DesenhaTabuleiro((char *)tabuleiro);
          
            vencedor = Check((char *)tabuleiro);

            if (vencedor == simbolo_computador)
            {
                puts("Que pena :< tente outra vez!\n");
                getchar();
                break;
            }
            else if (vencedor == simbolo_jogador)
            {
                puts("Parabens voce venceu :)\n");
                getchar();
                break;
            }
            else if (vencedor == 'E')
            {
                puts("Empate\n");
                getchar();
                break;
            }

            MovimentoJogador((char *)tabuleiro, simbolo_jogador);
            DesenhaTabuleiro((char *)tabuleiro);

        }
    }

    return 0;
}