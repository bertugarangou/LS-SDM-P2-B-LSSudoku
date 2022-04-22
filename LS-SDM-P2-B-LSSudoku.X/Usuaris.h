#ifndef UUsuaris_
#define UUsuaris_

char UgetNumUsuaris(void);
void Uinit(void);
void UcreateUser(void);
void UsetData(char user[], char pass[]);
void UmotorUsers(void);
__bit UcheckExistsNotFinished(void);
void UcheckExists(void);
__bit UcheckExistsGetError(void);
#endif