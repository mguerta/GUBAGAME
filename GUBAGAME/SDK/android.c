#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

 int ffps=0,fvps=0,rffps,rfvps;
 long timeFPS=0;
int VVIDEO_HEIGHT = VIDEO_HEIGHT;

void setActualView(int w,int h)
{
	VVIDEO_WIDTH=w;
	VVIDEO_HEIGHT=h;
}
	mouse.y=yTouch;
  yp=yTouch;
    else           JoyState=BTN_DOWN;       // tombado
    
    if((yp-ypi)<-100) JoyState=BTN_RIGHT;
    if((yp-ypi)>100) JoyState=BTN_LEFT; 
/*
  // Can't write more than the free space
  J=GetFreeAudio();
  if(J<Length) Length=J;

  // Copy data
  for(J=0;J<Length;J++)
  {
    *SndWPtr++=Data[J];
    if(SndWPtr-SndBuffer>=SND_BUFSIZE) SndWPtr=SndBuffer;
  }

  // Enqueue buffers ready to play
  PlayBuffers(); */

	sizeActualSndBuffer=0;

	for(J=0;J<Length;J++) SndBuffer[J]=Data[J];

	sizeActualSndBuffer=Length;

  // Done
  return(Length);

  struct zip_file* arq;
  struct zip_stat st;

  
  long size=-1;
  char *b;
  char nameF[8192];
    


    strcpy(nameF,"res/drawable-hdpi");
    strcat(nameF,&nameFile[getIndexLastBar(nameFile)]);  //eliminar ./res/ (6)  // 18 para 3D
	
     LOGE("Name file %s",nameF);
//  arq=fopen(nameFile,"rb");
 // arq=fopen(nameF,"rb");
	 arq = zip_fopen(APKArchive, nameF, 0);
  if(arq)
  {

  	  zip_stat(APKArchive, nameF, 0, &st);
	  LOGE("Achou %s - %d",nameF,st.size);

	  size = st.size;

   // zip_fread(arq, void *, size_t);

	b=mymalloc(size);
	if(b)
	{
	
		size=zip_fread(arq, b, size);

		LOGE("Leu bytes %d",size);
	}
	else
		LOGE("Memoria insuficiente %d",size);
	*buf=b; 
	zip_fclose(arq);
  }
  else
	 LOGE("Nao Achou %s",nameF);

  return size ;
}

    nowTime= tv.tv_sec*1000. + tv.tv_usec/1000.;
#define PULAFRAME 0
	{
		ffps++;
		//if((drawr & PULAFRAME)==PULAFRAME) 
		if(drawr)
			fvps++;
	}
	else
	{
				
		rffps=ffps;
		rfvps=fvps;
		ffps=0;
		fvps=0;
		timeFPS=currentTime;

	}

	*FPSG=rfvps;
	*FPS=rffps;


	return 1;
	*FPS=rffps;
	//return ((drawr & PULAFRAME)==PULAFRAME);
{int n;

	n=strlen(name);

	while(n>0)
	{
		if(name[n]=='/') break;
		n--;
	}

	return n++; 

}
int m_w=1;

int myrand(void)
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;


}
// Sound interface
//---------------------------------------------------------

int SRenderAudio(void)
{
//	return RenderAudio();
	return 0;

}

int SsetMusic(char *nameFile,int flgRepeat)
{
	setListSound(&nameFile[getIndexLastBar(nameFile)+1]);
//	return setMusic(nameFile,flgRepeat);
	return 0;
}

int SsetSoundChannel(char *nameFile,int channel,int flgRepeat)
{
//	return setSoundChannel(nameFile,channel,flgRepeat);
	return 0;
}

int SfreeAudio(void)
{
//	return freeAudio();
	return 0;
}

int SstartSound(tsound *snd)
{
//	return startSound(snd);
	return 0;
}

int SstartSoundBuffer(sample *buf,long size)
{
//	return startSoundBuffer(buf,size);
	return 0;
}
{
	startIDSound(id,flgRepeat);
}