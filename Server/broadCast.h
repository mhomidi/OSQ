#include "server.h"

char q[5][BUF_SIZE];

void broadCast();
int checkSeconds();
char** createQuestion();
void sendQuestions(int index, char **quest);