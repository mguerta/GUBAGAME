#include "so.h"
#include "sound.h"

/*============================================================
 F: Data
 =============================================================*/


tsound listSounds[NUMMAXSOUNDS];


/*============================================================
 F: Set music 
 =============================================================*/

int setMusic(char *nameFile,int flgRepeat)
{int i,ret=0;

	for(i=0;i<NUMMAXSOUNDS;i++)
	{
		if(listSounds[i].buf==0)
		{
			
			listSounds[i].statusRepeat=flgRepeat;
			listSounds[i].size=readFile(nameFile, (char **)&listSounds[i].buf);
			listSounds[i].idxTotal=listSounds[i].size;
			listSounds[i].size/=sizeof(sample);
			listSounds[i].cache=1;
			ret=listSounds[i].size;

			listSounds[i].idx=listSounds[i].size;//HEADERWAVFILE; //apenas carrega e nao inicia o som.
			break;
		}
	}

	return ret;

}
/*============================================================
 F: StartID music 
 =============================================================*/

void startIDMusic(int id,int flgRepeat)
{
//	if(listSounds[id].idx==listSounds[id].size)
	{
		listSounds[id].idx=HEADERWAVFILE;
		listSounds[id].statusRepeat=flgRepeat;
	}
}

/*============================================================
 F: Set music into struct
 =============================================================*/

int startSound(tsound *snd)
{int i,ret=0;

	for(i=0;i<NUMMAXSOUNDS;i++)
	{
		if(listSounds[i].buf==0)
		{
			
			listSounds[i].fileName=snd->fileName;
			listSounds[i].statusRepeat=snd->statusRepeat;
			listSounds[i].cache=snd->cache;
			listSounds[i].idxTotal=0;
			if(listSounds[i].cache)
				listSounds[i].size=readFile(snd->fileName, (char **)&listSounds[i].buf);
			else
				listSounds[i].size=readFileChunk(listSounds[i].fileName, (char **)&listSounds[i].buf,listSounds[i].idxTotal,8192);

			listSounds[i].idxTotal=listSounds[i].size;

			listSounds[i].size/=sizeof(sample);
			ret=listSounds[i].size;

			listSounds[i].idx=listSounds[i].size;//HEADERWAVFILE;
			break;
		}
	}

	return ret;

}

/*============================================================
 F: Set music into buffer
 =============================================================*/

int startSoundBuffer(sample *buf,long size)
{int i,ret=0;

	for(i=0;i<NUMMAXSOUNDS;i++)
	{
		if(listSounds[i].buf==0)
		{
			listSounds[i].idx=0;
			listSounds[i].fileName=0;
			listSounds[i].statusRepeat=1;
			listSounds[i].cache=1;
			listSounds[i].idxTotal=0;
			listSounds[i].buf=buf;
			listSounds[i].size=size;
			
			listSounds[i].idxTotal=listSounds[i].size*sizeof(sample);

			ret=listSounds[i].size;
			break;
		}
	}

	return ret;

}


/*============================================================
 F: Set effect 
 =============================================================*/

int setSoundChannel(char *nameFile,int channel,int flgRepeat)
{int i,ret=0;

	i=channel;
	if(listSounds[i].buf==0)
	{
		listSounds[i].idx=HEADERWAVFILE;
		listSounds[i].statusRepeat=flgRepeat;
		listSounds[i].size=readFile(nameFile, (char **)&listSounds[i].buf);

		listSounds[i].cache=1;
		listSounds[i].idxTotal=listSounds[i].size;
		listSounds[i].size/=sizeof(sample);
		ret=listSounds[i].size;
	}

	return ret;

}
/*============================================================
 F: Renderize Audio
 =============================================================*/

int RenderAudio(void)
{int J,i,k,idx,s,p=0;
 sample bufSnd[MAXCACHESOUND];

 int soma=0;

	J  = GetFreeAudio();
	J--;


	if(J)
	{

		for(k=0;k<J;k++) bufSnd[k]=0;
		
		for(i=0;i<NUMMAXSOUNDS;i++)
		{
			if(listSounds[i].buf!=0)
			{
				idx=listSounds[i].idx;

				if(idx+J>listSounds[i].size) s=listSounds[i].size-idx;
				else s=J;

				if(s>MAXCACHESOUND) 
					s=MAXCACHESOUND;

				for(k=0;k<s;k++)
				{
					if(p>0)
					{
						bufSnd[k]+=listSounds[i].buf[idx+k];
						bufSnd[k]/=2;
					}
					else
						bufSnd[k]=listSounds[i].buf[idx+k];

					
					soma+=bufSnd[k];
				}
				listSounds[i].idx+=s;
				p++;


				if(s==0)
				{
					if(listSounds[i].statusRepeat)
					{
			
						if(!listSounds[i].cache)
						{
							listSounds[i].idx=0;
							listSounds[i].size=readFileChunk(listSounds[i].fileName, (char **)&listSounds[i].buf,listSounds[i].idxTotal,8192);
							listSounds[i].idxTotal+=listSounds[i].size;
							if(listSounds[i].size==-1)
							{
								listSounds[i].idx=HEADERWAVFILE;
								listSounds[i].idxTotal=0;
								listSounds[i].size=readFileChunk(listSounds[i].fileName, (char **)&listSounds[i].buf,listSounds[i].idxTotal,8192);
							}
						}
						else
							listSounds[i].idx=HEADERWAVFILE;							

					}
					else
					{
						if(!listSounds[i].cache)
						{
							listSounds[i].idx=0;
							listSounds[i].size=readFileChunk(listSounds[i].fileName, (char **)&listSounds[i].buf,listSounds[i].idxTotal,8192);
							listSounds[i].idxTotal+=listSounds[i].size;
							if(listSounds[i].size==-1)
							{
								if(listSounds[i].buf) myfree(listSounds[i].buf);
								listSounds[i].buf=0;
								listSounds[i].idx=0;
							}
						}
						else
						{
						//	if(listSounds[i].buf) myfree(listSounds[i].buf);
						//	listSounds[i].buf=0;
						//	listSounds[i].idx=0;
						}
					}
				} 

			}
		}
	
		if(J) 
			WriteAudio((sample *)&bufSnd[0],J);
	}


  return(soma);
}

/*============================================================
 F: free Audio
 =============================================================*/

int freeAudio(void)
{int i;

	for(i=0;i<NUMMAXSOUNDS;i++)
	{
		if(listSounds[i].buf!=0)
		{
			myfree(listSounds[i].buf);
			listSounds[i].buf=0;
		}
	}

	return 0;

}