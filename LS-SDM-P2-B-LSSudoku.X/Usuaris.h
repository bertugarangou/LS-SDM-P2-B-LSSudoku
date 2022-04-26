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
__bit UcheckExistsGetError(void);
void escriureEEPROM(void);
#endif