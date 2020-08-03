

/***********Header Files*******************/

extern int is_higher_ver(void);
extern int Apply_Patch(float );
extern int Download_ConfigFile(int);
extern int Download_PatchFiles( int ,float *, int );
extern int Verify_PatchVersion( float , float  );
extern int Verify_CurrentVersion(float *);
//extern int Check_Versions(int *,float **,float *);
extern int  get_ftp_details (char *,char *,char *,char *,char *,char *,char *,char *,char *,char *);

