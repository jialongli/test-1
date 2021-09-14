#include "cnn.cpp"
#include "ReadData.h"

#define TRAIN 1//������ʶ��ѵ���������ǲ�������
#define TEST 0

class CNN  m_cnn;//����CNN��һ��ʵ��m_cnn

double LearningRate=0.001;//ѧϰЧ�ʳ�ʼ��
UCHR **image=NULL,*label=NULL;//��������ǩ��ȫ�ֱ���
UCHR **myimage=NULL,*mylabel=NULL;//��������ǩ��ȫ�ֱ���

void  startTraining();
void  startTest();

int main()
{

	srand((unsigned)time(NULL));
	image=readImage(TRAIN);//����ѵ������
	label=readLabel(TRAIN);//����ѵ����ǩ
    printf("read input data\n");

	myimage=readImage(TEST);//�����������
	mylabel=readLabel(TEST);//������Ա�ǩ
    printf("read label data\n");

    printf("before training \n");
	startTraining();//��ʼѵ��
    printf("after training \n");
	free(image);free(label);//�ͷſռ�
	free(myimage);free(mylabel);//�ͷſռ�

	//image=readImage(TEST);//�����������
	//label=readLabel(TEST);//������Ա�ǩ
	//startTest();//��ʼ����
	//free(image);free(label);//�ͷſռ�

	return 0;
}


///////////////////////////////////////////////////
int  Calculate(int index)
////////////////////////////////////////////////////
{
	int cCount = g_cImageSize*g_cImageSize;
	int ii, jj;
    double inputVector[(g_cImageSize+1)*(g_cImageSize+1)];//������������չ��29*29�����任��[-1��1]
	//copy gray scale image to a double input vector in -1 to 1 range
	//-1 is white, 1 is black
	for ( ii=0; ii<g_cVectorSize * g_cVectorSize; ++ii ) inputVector[ii] = -1.0;
	for ( ii=0; ii<g_cImageSize; ++ii )
	{
		for ( jj=0; jj<g_cImageSize; ++jj )
		{
			int idxVector = 1 + jj + g_cVectorSize * (1 + ii);//����Ϊ29*29����±�
			int idxImage = jj + g_cImageSize * ii;//28*28ʱ���±�

			inputVector[ idxVector ] = (double)(2/255.0)*(int)(image[index][ idxImage ]) - 1.0;
		}
	}
	//call forward propagation function of CNN
	return m_cnn.Calculate(inputVector);
}

void random(int *p,int count)
{
	for(int i=0;i<count;i++)
	{
	    int n=rand()%count;
		int temp;
		temp=p[i];p[i]=p[n];p[n]=temp;
	}

}
//////////////////////////////////////////////////////
void  startTraining()
//////////////////////////////////////////////////////
{
	int errorCount;//��¼ѵ���Ĵ�������

	int N=84;//ѵ������

	//clock_t start,finish;

	double desiredOutputVector[g_cOutputSize];//ʵ�����
	int m_iOutput;//��¼ÿ������ǰ�򴫲����Ԥ���ֵ

	char outFileName[]="weights.txt";//����Ȩֵ���ı�

	int index[g_cCountTrainingSample];//�����������ѵ��������ʼ��
	for(int n=0;n<g_cCountTrainingSample;n++)  index[n]=n;//����ֵ

	m_cnn.LoadWeightsRandom();//[-0.05,0.05]�ڵ������
	//m_cnn.LoadWeights(outFileName);

    for(int k=1;k<=N;k++)
	{
		random(index,g_cCountTrainingSample);
		errorCount = 0;//ÿ��ѵ��֮ǰ��ʼ��Ϊ0
		for(int i=0; i<g_cCountTrainingSample; i++)
		{
			//ǰ�򴫲�
			m_iOutput=Calculate(index[i]);//��i������ǰ����㣬���ر���������Ԥ��ֵ
			if(m_iOutput != (int)label[index[i]]) errorCount++;//��Ԥ��ֵ��ʵ��ֵ����ȣ���ѵ������������1
			//���򴫲�
			for(int j=0; j<g_cOutputSize; j++) desiredOutputVector[j] = -1;
			desiredOutputVector[(int)label[index[i]]] = 1;

			m_cnn.BackPropagate( desiredOutputVector, LearningRate );

			if(0==i%1000) printf("%d,%d\n",i,errorCount);
		}

		printf("�� %d ��ѵ���Ĵ�����Ϊ��%d,��ȷ��Ϊ: %f%%\n",k,errorCount,(1-(errorCount/60000.0))*100);

		FILE *fp;
		fp=fopen("error.txt","a");
		fprintf(fp,"�� %d ��ѵ���Ĵ�����Ϊ��%d,��ȷ��Ϊ: %f%%\n",k,errorCount,(1-(errorCount/60000.0))*100);
		fclose(fp);
		//����ѧϰЧ��
		if(k%6==0) LearningRate *= 0.794;
		//����Ȩֵ
		if(outFileName != NULL) m_cnn.SaveWeights(outFileName);//����Ȩֵ��outFileName�ļ�
		//ÿ��ѵ����������һ��
		startTest();//��ʼ����

	}


}

void startTest()
{
	int errCount=0;//��¼�����ܵĴ������
	char outFileName[]="weights.txt";//����Ȩֵ���ı�
	int err;//��¼ÿ�������Ƿ������ȷ

	m_cnn.LoadWeights(outFileName);//�����Ѿ�ѵ���õ�Ȩֵ

	for(int i=0;i<g_cCountTestingSample;i++)//��ʼ����
	{
		err=Calculate(i);
		if(err != (int)label[i]) errCount++;

		if(0==i%100) printf("%d,%d\n",i,errCount);

	}
	printf("���˲��ԵĴ�����Ϊ��%d,��ȷ��Ϊ: %f%%\n",errCount,(1-(errCount/10000.0))*100);
	FILE *fp;
	fp=fopen("error.txt","a");
	fprintf(fp,"���˲��ԵĴ�����Ϊ��%d,��ȷ��Ϊ: %f%%\n",errCount,(1-(errCount/10000.0))*100);
	fclose(fp);

}
