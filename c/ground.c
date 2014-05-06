#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#define MAX 32
int teamCount;  //����ƶq�A�����O�G������
int levelCount; //���d�ƶq�A������ ����H�ƪ��@�b�٦h�A�_�h�|������S�����d�i�H��
int maxTeam;	   //�N��Ҧ�����ƶq�A���ȥ� (����ƶq + �ťն���) �Ҳզ��A �Y ���d�ƶq�� (����ƶq/2) �٭n�h�A�N��P�ɶ����@�����d�O�S���H����A�o�ǪŪ�����N�Ϋ᭱��bit�Өϥ�
int playTimes;  //�C�Ӷ���n���X�����d(���y�ܻ��N�O�C�����d�̦h�J��X�Ӷ���)�A ���ȥ����n  �p�󵥩� �Ҧ�����(maxTeam)���@�b�A
/*
	�ثe�������u��䴩�̦h32������A�]���ϥ�bit�B�⪺���G�A�ϥ�int�����x�s�e���A�C��bit�N��@�Ӷ���

*/
int map[MAX][MAX]; //�ΨӦs�񵲪G���A���]�^�X�Oteam 3 vs. team6, �����ȴN�O 01001000
int colTeam[MAX];  //�P�Ӯɶ��A�C�Ӷ���u��X�{�b�@�����d�A�Τ@�Ӿ�ưO����e�ɶ����Ƕ���w�g���L�F�A�̵M�ϥ�bit
int rowTeam[MAX];  //�C�Ӷ����ۦP���d�u�઱�@���A �Τ@�Ӿ�ưO���C�����d�ثe�����Ƕ���w�g���L�F�A�̵M�ϥ�bit
				   //i.e  ����ƶq:8 ���d�ƶq:5 �h�C�Ӯɶ����A�|���|�չ�ԡA���O���������d�A�]�����������d�|�S������A�]���`�@�|�Ψ�10��bit, �e��8��bit�N����1~8 ,�᭱���bit�N��Ū�����
int used[MAX][MAX];//�ΨӰO�����Ƕ��� �w�g����L�F




//  rowTeam: �O���̬Y�@�����d�ثe�w�g�C��������
//  colTeam: �O���̷�e�^�X�w�g���t�令�\������
//  ���P��Ӷ���s��(n1,n2)�A�P�_�o��Ӷ���O�_������M
//  �Y�O�����M�A�N�[�J�� rowTeam �� colTeam���O����
int checkRowCol(int *rowTeam,int *colTeam,int n1, int n2){
	int temp;
	temp = 1 << n1;
	if( 0 != (*rowTeam & temp) )
		return 0;
	temp = 1 << n2;
	if( 0 != (*rowTeam & temp) )
		return 0;
	temp = 1 << n1;
	if( 0 != (*colTeam & temp) )
		return 0;
	temp = 1 << n2;
	if( 0 != (*colTeam & temp) )
		return 0;

	*rowTeam = *rowTeam | (1 << n1) | (1 << n2);
	*colTeam = *colTeam | (1 << n1) | (1 << n2);
	return 1;
}

// �C�L���G�A�� 100100 ��Ʀ� (3,6) ��Ӷ����M
// �p�G���Ȥ񶤥�ƶq�٤j�A�N�N��O�Ŷ����M�A�]����X�ťէY�i

void printAns(int ans){
	int count = 0;
	printf("|(");
	while(ans>0){
		if( (ans & 1)){
			if( count < teamCount)
				printf("%2d ",count);
			else
				printf("   ");
		}
		count ++;
		ans >>=1;
	}
	printf(")|");
}

//��X��Ӱ}�C���G�A���C�@����쳣�I�s printAns��Ʀr�ন����

void showAns(){
	int i,j;
	for( j=0 ; j <playTimes; j++){
			printf("----------");
	}
	printf("\n");
	for( i = 0; i < levelCount; i++){
		for( j =0 ; j < playTimes; j++){
			printAns(map[i][j]);
		}
		printf("\n");
		for( j=0 ; j <playTimes; j++){
			printf("----------");
		}
		printf("\n");
	}
}


// �p��@��32-bit���Ʀr���X��bit�O1
int numberOfOne(int x){
	int count = 0;
	while(x>0){
		x = (x & (x-1));
		count++;
	}
	return count;
}

//���w�@��input, �q��input���G�i���F���h�H���D�X�@��bit�O0����m
//i.e ���pinput�O 1101001 ���O�s����m��(1,2,4),�ҥH��function�|�H���^�� 1,2,4�䤤�@��
//�Q��maxTeam�ӱ���d��O���b����bit���A�]��maxTeam�N��Ҧ�����ƶq(��ڶ���+�ťն���)
int generateInt(int input){
	int temp;
	while(1){
		temp = rand()% maxTeam;
		if(  !((input >> temp) & 1))
			return temp;
	}
}

// row,col �N���e�n���ժ��Ʀr�Ҧ�b����m
// row: �ĴX�����d
// col: �ĴX�Ӧ^�X
//
void groundGame(int row,int col){
	int temp1,temp2;
	int team1,team2;
	int count = 0;
	int try[MAX][MAX]; //�O����e�^�X���A�w�g���չL���ǲզX
	int i,j,rowOrCol,oneCount;

	//�p�Gcol���ƶq�w�g�W�L�W�w�F�A�N�N��]�X���G�F�A�⵲�G�C�L�X�Өõ���process
	if( (0 == row) &&  (playTimes == col) ){
		showAns();
		exit(0);
	}
	//��l�� 
	for(i=0;i<MAX;i++)
		for(j=0;j<MAX;j++)
			try[i][j]=0;

	//rowTeam��colTeam���O�N�� �����d�w�g�C���L������ �H�� �Ӯɬq�w�g�C���L������
	//�ҥH�ڭ̭n�D�諸���������b��rowTeam��colTeam�X�{�L�A�]���ڭ̥����ӼƦr���X
	rowOrCol = (rowTeam[row] | colTeam[col]) & (( 1<<maxTeam)-1);
	//���U�ӷQ�n�p��o�Ӧ^�X���h���`�i��ʻݭn����
	//���O���`�@���h�֭�1
	oneCount = numberOfOne(rowOrCol);
	//1�N��w�g���L�F�A�ҥH�ڭ̥ά۴�覡�o�즳�h�֭�0�A�N���h�֭Ӷ���i�H����
	count = (maxTeam-oneCount); 
	//��n�Ӷ���A�|�� n * (n-1)�`�i��ʡA �o��ư� �⦸���D��P�Ӷ���i���
	count = count *  (count-1);
	//�p�G�S������i��A�N�������}�o�^�X
	if( 0 == count  ) {
		return ;
	}
	while(1){
		//�έ��X�ño�쪺�Ȩ��H�����ͤ@��bit�O0���s������
		team1 = generateInt(rowOrCol);
		team2 = generateInt(rowOrCol);
		//�p�G�w�g�S������i��A�N�������}�o�^�X
		if(count <=0)
			return ;
		//�]���Y�O�����ӬD�X�Ӫ��s����teamCount�٭n�j�A�N�N���ܪ��O�ťն���,
		//�]���ťն���u���ťն���M�A�ҥH�Y�O�t�~�@�ӼƦr���O�ťն���A���N�O���@�U�o�ӲզX�ϥιL�A�åB���s�D�Ʀr
		if( (team1 >= teamCount) ^ (team2 >= teamCount)){
			if(!try[team1][team2]){
				try[team1][team2]  = try[team2][team1] =  1; // 6 v 7  === 7 v 6, �ҥHcount�@�����G
				count-=2;
			}
			continue;
		}

		//���\���D�沈���n�����U�C����
		//1. team1 != team2
		if(team1 == team2 )
			continue ;
		//2. team1 �� team2 �O�Ӧ^�X�Ĥ@������
		//
		//�Y�O�Ĥ@�����աA�N�O���@�U���չL�A�åB�� count(�٦��h�ֺإi���)���ȵ���G�A�]��1v2 == 2v1
		if( try[team1][team2] || try[team2][team1] )
			continue;
		else{
			try[team1][team2]  = try[team2][team1] =  1;
			count -=2;
		}
		//3. team1 �� team2 �H�e�S����M�L
		//�u�����O�ťն���~�ݭn"�p��"�H�e���S����M�L
		if( (team1< teamCount) &&  used[team1][team2] | used[team2][team1])
			continue;
		//4. team1 �� team2 �b�Ӧ^�X�٨S�����L colTeam 
		//5. team1 �� team2 �b�����d�٨S�����L rowTeam
		//
		//���O�����Ӫ�row��col���ȡA���j ������n�_��
		temp1 = rowTeam[row];
		temp2 = colTeam[col];
		if( 0 == checkRowCol(&rowTeam[row],&colTeam[col],team1,team2))
			continue;
		//�q�L���O�A�ϥγo�@�չ�M
		//�O���o��մ��gPK�L
		used[team1][team2] = used[team2][team1] = 1;
		//�O���o��զbcol�^�X��row���M�A���G�n��
		map[row][col] = (1<<team1) | (1<<team2);
		//�p�G�w�g�O�Ӧ^�X���̫�@�����d�A�N���U�@�Ӧ^�X�h����
		if( (levelCount-1) == row)
			groundGame(0,col+1);
		else
			groundGame(row+1,col);

		//���j������_��
		used[team1][team2] = used[team2][team1] = 0;
		rowTeam[row] = temp1;
		colTeam[col] = temp2;
	}

}




int main(int argv,char** argc){
	int i,j;
	srand(time(0));
	if( 4 != argv){
		fprintf(stderr,"Usage: teamCount levelCount, playTimes\n");
		exit(1);
	}
	teamCount = strtol(argc[1],NULL,10);
	levelCount = strtol(argc[2],NULL,10);
	playTimes = strtol(argc[3],NULL,10);
	maxTeam = levelCount * 2; //maxTeam���ƶq�N�O���d�ƶq���⭿�A �p�G�o�ӭȤ񶤥�ƶq�j�A���᭱��bit�N�O�ťն���A�p�G���ȵ��󶤥�ƶq�A�N��S���ťն���
	if( (teamCount&1) || ( (maxTeam/2) < playTimes) ||  (teamCount > maxTeam) ){
		fprintf(stderr,"No solution!!\n");
		exit(1);
	}

	//��l�Ƭ����}�C
	for( i = 0; i <levelCount ;i++){
		memset(map[i],0,playTimes);
	}
	for( i = 0; i < MAX;i++){
		colTeam[i]=0;
		rowTeam[i]=0;
	}
	//�qrow:0 col:0 �}�l�]
	groundGame(0,0);

}
