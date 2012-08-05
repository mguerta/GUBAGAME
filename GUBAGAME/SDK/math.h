
#define PI 3.141592654 		/* constante Pi */

#define ANGLE_0   0
#define ANGLE_1   10
#define ANGLE_10  100
#define ANGLE_30  300
#define ANGLE_60  600
#define ANGLE_22_5  225
#define ANGLE_45  450
#define ANGLE_90  900
#define ANGLE_180 1800
#define ANGLE_270 2700
#define ANGLE_360 3600		/* Precisao decimos (0.1 graus) */

#define K4D_FIX  65536.0
#define P4D_FIX  16

#define K3D_FIX  65536
#define P3D_FIX  16

#define KDD_FIX  2048.0
#define PDD_FIX  11

#define KDDT_FIX  2048.0
#define PDDT_FIX  11

#define KDL_FIX	256.0
#define PDL_FIX 8

#define KDPJ_FIX 32768.0
#define PDPJ_FIX 15
#define MAXPJ 32767

#define AG1 300
#define AG2 450

typedef struct _point3D
{
	int x,y,z;

} point3D;

typedef struct _point2D
{
	int x,y;

} point2D;


short int get_vetor_arctan_table(int fx,int fy);
float fsen(int a);
float fcos(int a);
int isen(int a);
int icos(int a);
void projOrtho3D(int x,int y,int z,int *xp,int *yp);
void initProjOrtho3D(void);
int projZOrtho3D(int z);
