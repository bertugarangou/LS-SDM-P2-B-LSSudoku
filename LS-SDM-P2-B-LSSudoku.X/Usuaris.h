#ifndef UUsuaris_
#define UUsuaris_

char UgetNumUsuaris(void);
void Uinit(void);
void UcreateUser(void);
void UenviaChar(char c, char pos);
void UenviaPas(char c, char pos);
void UmotorUsers(void);
__bit UcheckExistsNotFinished(void);
void UcheckExists(void);
signed char UcheckExistsGetError(void);
void escriureEEPROM(void);
__bit URegisterEnded(void);
void URegister(void);
char* UgetUserName(char quin);
void UchangeScore(char quin, char score);
void initPuntuacions(void);
void calculateShowUsers(void);
char* getArrayShowUsers(char quin);
void escriure2usuarisStruct(void);
__bit UshowUsersCalculat(void);

unsigned char UgetScore(char quin);
signed char UgetTop5(char quin);
__bit UHaAcabatCalcTop5(void);
void UnewScore(char score);
#endif