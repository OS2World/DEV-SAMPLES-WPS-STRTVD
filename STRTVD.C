/*
 *Very simple straight forward utility for using WinCreateObject to create
 *Dos program icons in the NOWHERE folder for launching a DOS session with
 *specific settings.  Could also be used to create WINOS2 objects so that
 *seamless windows could be achieved.
 *
 *Property Bryan Walker DBA WalkerWerks
 *Placed in the public domain.
 */
#define INCL_BASE
#define INCL_PM
#define INCL_WINWORKPLACE
#define INCL_WINMESSAGEMGR
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>


SHORT Help(VOID)
{
   printf("Strt_Vdm parameters:\n") ;
   printf("/Iimagefile\n") ;
   printf("/Ttitle\n") ;
   printf("/Sadditional Settings\n") ;
   printf("/W w for windowed or f for fullscreen\n") ;
   printf("\nAdditional Settings and window setting are optional,\n") ;
   printf("imagefile and title are required\n") ;
   return 0 ;
}
main(int argc, char *argv[], char *envp[])
{
HAB hab ;
HMQ hmq ;
CHAR title[80], image[256], settings[1024], addsetup[512], wintxt[30] ;
SHORT x, setup = FALSE, Window = FALSE ;

printf("\nStart VDM DOS Session Command Line Utility\n") ;
printf("Brought to you by WalkerWerks (901) 683-3770\n") ;
if(argc < 3)
    return Help() ;

for(x = 1 ; x < argc ; x++)
   {
    if(*(argv[x] + 1)=='H' || *(argv[x] + 1) == 'h')
        return Help() ;
    if(*(argv[x] + 1)=='I' || *(argv[x] + 1) == 'i')
        {
        if( *(argv[x]+2) == '*')
            image[0] = 0 ;
        else
            sprintf(image, "SET DOS_STARTUP_DRIVE=%s", argv[x] + 2) ;
        }
    else if(*(argv[x] + 1)=='T' || *(argv[x] + 1)=='t')
        sprintf(title, "%s", argv[x] + 2) ;
    else if(*(argv[x] + 1)=='S' || *(argv[x] + 1)=='s')
        {
        setup = TRUE ;
        sprintf(addsetup, "%s", argv[x] + 2 );
        }
    else if( *(argv[x] + 1) == 'W' || *(argv[x] + 1) == 'w')
        {
        Window = TRUE ;
        if(*(argv[x]+2) == 'w' || *(argv[x]+2) == 'W')
            sprintf(wintxt, "WINDOWEDVDM") ;
        else
            sprintf(wintxt, "VDM") ;
        }
    else
        return Help() ;
    }

if(setup && argc <= 3)
    return Help() ;

if( Window == FALSE )
    sprintf(wintxt, "WINDOWEDVDM") ;

sprintf(settings, "EXENAME=*;PROGTYPE=%s;%s;OPEN=DEFAULT;", wintxt, image) ;
if(setup)
    strcat(settings, addsetup) ;

/*Initialize our connection to PM*/
hab = WinInitialize(0);
hmq = WinCreateMsgQueue(hab, 0) ;

/*Create an object in the hidden folder that is opened by default.  Replacing
  the previous object*/

if(WinCreateObject("WPProgram", title,
   settings,
   "<WP_NOWHERE>",CO_REPLACEIFEXISTS ) == NULLHANDLE)
    printf("Error Creating the Dos VDM Object\n") ;


/*Terminate PM connection*/
WinDestroyMsgQueue(hmq) ;
WinTerminate(hab) ;

return 0 ;
}

