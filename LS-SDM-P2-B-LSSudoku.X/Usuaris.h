#ifndef UUsuaris_
#define UUsuaris_

char UgetNumUsuaris(void);
void Uinit(void);
void UenviaChar(char c, char pos);
void UenviaPas(char c, char pos);
void UmotorUsers(void);
__bit UcheckExistsNotFinished(void);
void UcheckExists(void);
signed char UcheckExistsGetError(void);
void URegister(void);
char* UgetUserName(char quin);
void calculateShowUsers(void);
char* getArrayShowUsers(char quin);
__bit UshowUsersCalculat(void);

unsigned char UgetScore(char quin);
signed char UgetTop5(char quin);
void UnewScore(char scoretmp);
#endif