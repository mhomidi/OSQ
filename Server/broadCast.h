#include "server.h"

char q[5][BUF_SIZE];

void broadCast();
int checkSeconds();
void createQuestion();
void sendQuestions(int index);
void processReply(char buff[], int client_index);