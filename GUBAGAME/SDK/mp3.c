/*=============================================================================================
  M	: Modulo MP3 PLAYER do MEGA DRIVE 87 JOGOS GUITAR IDOL Titan
  Autor : Heriberto
  Inicio: 29/04/2010

  Mais adequado para impementar um PLAYER alimentado por memória de massa.Em nosso caso
  cartão SD.

 =============================================================================================*/


/*--------------------------------------------------------------------------
 D: Includes
--------------------------------------------------------------------------*/
/*#include "accfast.h"
#include "target.h"
#include "timer.h"
#include "malloc.h"
#include "mbox.h"
#include "uart.h"
#include "irq.h"
#include "stl.h"

#include "API.h"
#include "TFS.h"
*/
//#include "bmp.h"
//#include "debug.h"

//#include "vdp.h"

//#include "..\lib\zlib\ZLib.h"
#include <stdio.h>          // Standard filesystem
#include "so.h"
#include "mad.h"

#include	"sound.h"

#include	"mp3.h"

#define SIZEPICA 1000000

/*--------------------------------------------------------------------------
 F: Funcoes definidas
--------------------------------------------------------------------------*/
static int decode(unsigned char const *, unsigned long);
static void mp3_player_idle(void);
void picaWriteAudio(sample *buf,int size);

/*--------------------------------------------------------------------------
 D: Estruturas
 --------------------------------------------------------------------------*/

struct buffer {
  unsigned char const *start;
  unsigned long length;
};




/*--------------------------------------------------------------------------
 D: Dados definidos
 --------------------------------------------------------------------------*/

volatile int Music_Status;

volatile struct MUSIC_DEF *music_ptr,*music_running_ptr;

unsigned int rate_corrector;
int Fade_idx;

unsigned int bug1_fix;

FILE *fmp3;
char  bufMp3[100000];

//int Fade_idx;

#define	FADE_STEPS	52
const signed char Fade_step[FADE_STEPS]={
127,127,114,103,94,
85,77,69,63,57,
51,46,42,38,34,
31,28,25,23,20,
18,17,15,14,12,
11,10,9,8,7,
6,6,5,5,4,
4,3,3,3,2,
2,2,2,1,1,
1,1,1,1,1,
0,0,

};


const int RateCorrector12[100]={
  
0,0,0,0,0,0,0,1,0,0,  
0,0,0,0,0,1,0,0,0,0, 
0,0,1,0,0,0,0,0,0,0, 
1,0,0,0,0,0,0,0,1,0, 
0,0,0,0,0,0,1,0,0,0,   

0,0,0,1,0,0,0,0,0,0,  
0,1,0,0,0,0,0,0,0,1, 
0,0,0,0,0,0,0,1,0,0, 
0,0,0,0,0,1,0,0,0,0, 
0,0,1,0,0,0,0,0,0,1,   
  
};


const int RateCorrector04[100]={
  
0,0,0,0,0,0,0,0,0,0,  
0,0,0,0,0,0,0,0,0,0, 
0,0,0,0,0,1,0,0,0,0, 
0,0,0,0,0,0,0,0,0,0, 
0,0,0,0,0,0,0,0,0,0,   

0,1,0,0,0,0,0,0,0,0,  
0,0,0,0,0,0,0,0,0,0, 
0,0,0,0,0,0,0,1,0,0, 
0,0,0,0,0,0,0,0,0,0, 
0,0,0,0,0,0,0,0,0,1,   
  
};


const int RateCorrector22050[100]={
  
1,1,1,1,1,1,1,1,1,0,  
1,1,1,1,1,1,1,1,1,0, 
1,1,1,1,1,1,1,1,1,0, 
1,1,1,1,1,1,1,1,1,0, 
1,1,1,1,1,1,1,1,1,0,   

1,1,1,1,1,1,1,1,1,0,  
1,1,1,1,1,1,1,1,1,0, 
1,1,1,1,1,1,1,1,1,0, 
1,1,1,1,1,1,1,1,1,0, 
1,1,1,1,1,1,1,1,1,0,   
  
};

struct MUSIC_DEF musicMenu[]={
0,1,4,
0,0,0xFFFF
};


int startMusicMenu(char *fileName,int flgLoop)
{
	musicMenu[0].loop=flgLoop;
	//if(musicMenu[0].stream!=0) myfree(musicMenu[0].stream);
	//musicMenu[0].stream=0;
	musicMenu[0].size=0;

	fmp3=fopen(fileName,"rb");
	musicMenu[0].stream=&bufMp3[0];
	musicMenu[0].size=fread(bufMp3,1,0x8000,fmp3);
	//musicMenu[0].size=readFile(fileName, (char **)&musicMenu[0].stream)/sizeof(sample); 

	mp3_start_song(&musicMenu[0]);

	return musicMenu[0].size;

}

/*--------------------------------------------------------------------------
 F: Rotina: permite que DP dispare uma musica 
--------------------------------------------------------------------------*/
void mp3_start_song(struct MUSIC_DEF *song_ptr)
{
 
	
  /* Dispara Musica no NP */
  //mbox_cmd_write(DP_RISC, MAIL_NP_WAIT);
  //while (mbox_cmd_read(DP_RISC) != MAIL_ACK);  /* DP_RISC is receiver */
  //CacheFlush();
  music_ptr=song_ptr;
  music_running_ptr=music_ptr;
  Music_Status=MUSIC_RUN;
  bug1_fix=0;
  //mbox_cmd_write(DP_RISC, MAIL_NP_RUN);
  //while (mbox_cmd_read(DP_RISC) != MAIL_ACK);  /* DP_RISC is receiver */



}


/*--------------------------------------------------------------------------
 F: Rotina: permite que DP pare a musica
--------------------------------------------------------------------------*/
void mp3_fadeout_song(void)
{
	
  
  //mbox_cmd_write(DP_RISC, MAIL_NP_WAIT);
  //while (mbox_cmd_read(DP_RISC) != MAIL_ACK);  /* DP_RISC is receiver */
  //CacheFlush();
  Fade_idx=FADE_STEPS-1;
  //mbox_cmd_write(DP_RISC, MAIL_NP_RUN);
  //while (mbox_cmd_read(DP_RISC) != MAIL_ACK);  /* DP_RISC is receiver */



}

/*--------------------------------------------------------------------------
 F: Rotina: NP em idle state, permite que DP atualize variaveis de controle 
--------------------------------------------------------------------------*/
static void mp3_player_idle(void)
{
unsigned char cmd;

//  mbox_cmd_write(NP_RISC, MAIL_ACK);  /* NP_RISC is sender */

//  while(1)
//  {

	//Leitura do mail-box
//	cmd = mbox_cmd_read(NP_RISC);  /* NP_RISC is receiver */
//	if(cmd)
//		if(cmd == MAIL_NP_RUN)
//		{
//			mbox_cmd_write(NP_RISC, MAIL_ACK);  /* NP_RISC is sender */
//			CacheFlush();
//			return;
//		}

//  }


}


/*--------------------------------------------------------------------------
 F: Rotina principal para chamada do decoder
--------------------------------------------------------------------------*/
long idxPica=0;
sample pica[SIZEPICA];

void mp3_play( void )
{
int i;
//unsigned char cmd; 

//signed char zero=0;


 // Fade_idx=0;
 // Music_Status=MUSIC_STOP;

 // while(1)
 /* {
	
	if (Music_Status == MUSIC_RUN)
	{

		switch(music_running_ptr->loop)
		{

		case 0:
			Music_Status=MUSIC_STOP;

			break;

		case 0xFFFF:
			music_running_ptr=music_ptr;
			//printf("NP RUNNIG: %x\n",music_running_ptr);
			break;

		default:
			if(music_running_ptr->size>0)
			{
				for(i=0;i<music_running_ptr->loop;i++){
				bug1_fix=0;
					decode(music_running_ptr->stream, music_running_ptr->size);
				}
			}
			music_running_ptr->size=0;
			music_running_ptr++;
			if(music_running_ptr->loop==0xFFFF) music_running_ptr=music_ptr;
			//printf("NP RUNNIG: %x\n",music_running_ptr);
			break;

		}
	}

	if (Music_Status == MUSIC_STOP)
	{
	

		while(GetFreeAudio()==0);
		WriteAudio(&zero,1);
		//printf("NPSTOPED: %x\n",music_running_ptr);
	}


	//Leitura do mail-box
//	cmd = mbox_cmd_read(NP_RISC);  // NP_RISC is receiver 
//	if(cmd)
//		if(cmd == MAIL_NP_WAIT)
//			mp3_player_idle();

  }
*/

	if (Music_Status == MUSIC_RUN)
	{
		if(music_running_ptr->size>0)
		{
			bug1_fix=0;
			decode(music_running_ptr->stream, music_running_ptr->size);

			//music_running_ptr->size=0;
			musicMenu[0].size=fread(bufMp3,1,0x8000,fmp3);
			if(musicMenu[0].size==0) 
			{
				fclose(fmp3);
				fmp3=0;
			}
		}
	}

  	startSoundBuffer(pica,idxPica);

}

/*--------------------------------------------------------------------------
 F: Rotina input callback
--------------------------------------------------------------------------*/

static
enum mad_flow input(void *data,
		    struct mad_stream *stream)
{
  struct buffer *buffer; 

  buffer = (struct buffer*) data;

  if (!buffer->length)
    return MAD_FLOW_STOP;


  mad_stream_buffer(stream, buffer->start, buffer->length);

  buffer->length = 0;

  return MAD_FLOW_CONTINUE;
}



/*--------------------------------------------------------------------------
 F: Rotina de arredondamento
--------------------------------------------------------------------------*/
static
signed int scale(mad_fixed_t sample)
{
 
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}



/*--------------------------------------------------------------------------
 F: Rotina output callback
--------------------------------------------------------------------------*/


static
enum mad_flow output(void *data,
		     struct mad_header const *header,
		     struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;
  unsigned int iiscfg;

  unsigned char cmd;
  int i;

  signed int sample,sample_r,sample_l;
 // signed char sampleb;
  signed short sampleb;

  signed char zero=0;



  /* pcm->samplerate contains the sampling frequency */
 
 // printf("sample rate = %d\n",pcm->samplerate);
//  iiscfg= (((((22050*5)/pcm->samplerate)&0x3F)<<10)|0x201);
//  I2SCFG   = 0;
//  I2SCFG   = (short) iiscfg;
  

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];


  
  while (nsamples--) {


	/* output sample(s) in 16-bit signed little-endian PCM */



	sample_l = scale(*left_ch++);
	sample_r = scale(*right_ch++);

	if(nchannels>1)
		sample= (sample_l + sample_r)/2;
	else
		sample= sample_l;


  
//	sampleb=(signed char)((sample >> 9) & 0xff); 
//	sampleb=(signed short)((sample >> 1) & 0xffff);
	sampleb=sample;
	

	if (Fade_idx){
		
		if(sampleb>=0){
			sampleb-=Fade_step[Fade_idx];
			if (sampleb<0) sampleb=0;
		}
		else{
			sampleb+=Fade_step[Fade_idx];
			if (sampleb>0) sampleb=0;
		}


		if (--Fade_idx == 0){
			Music_Status=MUSIC_STOP;
		}

	}

//	picaWriteAudio(&sampleb,1);

	if(bug1_fix==10)
	{

	if (++rate_corrector>=100) rate_corrector = 0; 

//	while(GetFreeAudio()==0);

		
    if (pcm->samplerate==22050){
      
        if (RateCorrector22050[rate_corrector])picaWriteAudio(&sampleb,1);

    } 
    else picaWriteAudio(&sampleb,1);
		
 	
    if (pcm->samplerate==44100){
      
		    if (RateCorrector12[rate_corrector]) picaWriteAudio(&sampleb,1);
		
    }
    
    if ( (pcm->samplerate==32000) || (pcm->samplerate==48000) || (pcm->samplerate==24000)  ){

        if (RateCorrector04[rate_corrector]) picaWriteAudio(&sampleb,1);
  
    }
      
	
		
	}


  }



  if (bug1_fix<10) bug1_fix++;
 
  //Leitura do mail-box
//  cmd = mbox_cmd_read(NP_RISC);  /* NP_RISC is receiver */
//  if(cmd)
//  		if(cmd == MAIL_NP_WAIT)
//  			mp3_player_idle();


  if(Music_Status==MUSIC_STOP)
	return MAD_FLOW_STOP;
  
/*
  while(Music_Status==MUSIC_PAUSE)
  {
		while(GetFreeAudio()==0);
		WriteAudio(&zero,1);

		//Leitura do mail-box
//		cmd = mbox_cmd_read(NP_RISC);  // NP_RISC is receiver 
//		if(cmd)
//  			if(cmd == MAIL_NP_WAIT)
//  				mp3_player_idle();
  }
*/
   
  return MAD_FLOW_CONTINUE;
}

void picaWriteAudio(sample *buf,int size)
{int i;
	for(i=0;i<size;i++)
	{
		if(idxPica<SIZEPICA)	
			pica[idxPica++]=*buf;
		else
			break;
	}
}

/*--------------------------------------------------------------------------
 F: Rotina error callback
--------------------------------------------------------------------------*/
static
enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{
  struct buffer *buffer;


  buffer = (struct buffer*) data;
/*
  fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - buffer->start);
*/
//  printf("decoding error 0x%04x (%s) at byte offset %u\n",
//	  stream->error, mad_stream_errorstr(stream),
//	  stream->this_frame - buffer->start);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}



/*--------------------------------------------------------------------------
 F: Rotina decoder
--------------------------------------------------------------------------*/
static
int decode(unsigned char const *start, unsigned long length)
{
  struct buffer buffer;
  struct mad_decoder decoder;
  int result;

  /* initialize our private message structure */

  buffer.start  = start;
  buffer.length = length;

  /* configure input, output, and error functions */


  mad_decoder_init(&decoder, &buffer,
		   input, 0 /* header */, 0 /* filter */, output,
		   error, 0 /* message */);

  /* start decoding */

  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */

  mad_decoder_finish(&decoder);

  return result;
}
