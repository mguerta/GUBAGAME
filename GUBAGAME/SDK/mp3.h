/*==========================================================================
  M	: Modulo MP3 do MEGA DRIVE 70 JOGOS Titan
  Autor : Heriberto
  Rev   : 0.0
  Inicio: 10/04/2006

 
 ==========================================================================*/

/*--------------------------------------------------------------------------
 D: Defines
 --------------------------------------------------------------------------*/

#define		MUSIC_STOP	0
#define		MUSIC_RUN	1
#define		MUSIC_PAUSE	2

// rdazcal: sending defines to MBOX.H to use them elsewhere
//#define		MAIL_NP_WAIT 0xAA
//#define		MAIL_NP_RUN  0x55
// rdazcal

/*--------------------------------------------------------------------------
 D: Estruturas
 --------------------------------------------------------------------------*/
struct MUSIC_DEF{
  unsigned char *stream;
  int size;
  unsigned short loop;

};



/*--------------------------------------------------------------------------
 F: Rotinas definidas
--------------------------------------------------------------------------*/

void mp3_play( void );
void mp3_start_song(struct MUSIC_DEF *song_ptr);
void mp3_fadeout_song(void);


/*--------------------------------------------------------------------------
 : Controle por variaveis globais. Nada pode ser mais rapido
--------------------------------------------------------------------------*/
extern volatile int Music_Status;

extern volatile struct MUSIC_DEF *music_ptr,*music_running_ptr;

extern unsigned int bug1_fix;

