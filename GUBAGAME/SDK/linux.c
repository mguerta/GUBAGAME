#include <directfb.h>
int VVIDEO_HEIGHT = VIDEO_HEIGHT;

void setActualView(int w,int h)
{
	VVIDEO_WIDTH=w;
	VVIDEO_HEIGHT=h;
}
{int n;

/*	n=strlen(name);

	while(n>0)
	{
		if(name[n]=='/') break;
		n--;
	}

	return n++; */
	return 0;
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
	return RenderAudio();

}

int SsetMusic(char *nameFile,int flgRepeat)
{
	return setMusic(nameFile,flgRepeat);
}

int SsetSoundChannel(char *nameFile,int channel,int flgRepeat)
{
	return setSoundChannel(nameFile,channel,flgRepeat);
}

int SfreeAudio(void)
{
	return freeAudio();
}

int SstartSound(tsound *snd)
{
	return startSound(snd);
}

int SstartSoundBuffer(sample *buf,long size)
{
	return startSoundBuffer(buf,size);
}

void SstartIDMusic(int id,int flgRepeat)
{
	startIDMusic(id,flgRepeat);
}