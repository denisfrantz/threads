#include <stdio.h>
#include <windows.h>

int a1, a2, s1, s2;
int recurso[3] = {0,0,0};
int t1_finish = 0, t2_finish = 0;

void WINAPI ThreadUm (void) {
	recurso[a1] = 1;
    printf("T1 aloca... R%d\n", a1);
    Sleep(2000);
    printf("T1 solicita R%d\n", s1);
    
    while(1){
		if (recurso[s1] == 0) {
            printf("T1 aloca.. R%d\n", s1);
            recurso[s1] = 1;
            break;
        }
        Sleep(2000);
    }
    
    Sleep(500);
    printf("T1 libera R%d e R%d\n", s1, a1);
    recurso[s1] = recurso[a1] = 0;
    t1_finish = 1;
}

void WINAPI ThreadDois (void) {
    int r1 = 1, r2 = 0;
    Sleep(1000);
    
    if (recurso[a2] == 0) {
		printf("T2 aloca... R%d\n", a2);
        recurso[a2] = 1;
    }
    else {
		printf("T2 solicita R%d\n", a2);
        r1 = 0;
    }
    
    Sleep(1500);
    printf("T2 solicita R%d\n", s2);
    
    while (1) {
		if (!r1 && recurso[a2] == 0) {
			printf("T2 aloca... R%dan", a2);
            recurso[a2] = 1;
            r1 = 1;
        }
        if (!r2 && recurso[s2] == 0) {
			printf("T2 aloca... R%d\n", s2);
            recurso[s2] = 1;
        }
		if (r1 && r2)
			break;
        Sleep(1500);
    }
     
    Sleep(200);
    printf("T2 libera R%d e R%d\n", s2, a2);
    recurso[s2] = recurso[a2] = 0;
    t2_finish = 1;
}

void especifica_alocacoes (void) {
    srand(time(NULL));
    printf("Recursos disponiveis para alocacao: R0, R1 e eR2\n");
    a1 = rand() % 3;
    a2 = rand() % 3;
     
    do {
		s1 = rand() % 3;
    } while (a1 == s1);
     
    do {
		s2 = rand() % 3;
    } while (a2 == s2);
}

int main (void) {
    especifica_alocacoes();
    unsigned long iID;
    HANDLE hThreadUm, hThreadDois;
    
    hThreadUm = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadUm, NULL, 0, &iID);
    hThreadDois = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadDois, NULL, 0, &iID);
    
    Sleep(3000);
    
    while (!t1_finish && !t2_finish){;}
    
    TerminateThread(hThreadUm, 0);
    TerminateThread(hThreadDois, 0);
    system("pause");
    
    return 0;
}
