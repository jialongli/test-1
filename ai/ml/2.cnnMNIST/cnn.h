#include "global.h"

//typedef unsigned char uchar;
#define INPUT_LAYER		0
#define CONVOLUTIONAL	1
#define FULLY_CONNECTED 2

#define max(a,b) ((a)>(b) ? (a):(b))
#define RANDOM_PLUS_MINUS_ONE	( (double)(2.0 * rand())/RAND_MAX - 1.0 )	//[-1,1]�������
#define SIGMOID(x) (1.7159*tanh(0.66666667*(x)))
#define DSIGMOID(x) (0.66666667/1.7159*(1.7159+(x))*(1.7159-(x)))


class Layer;

class FeatureMap
{
public:
	double bias, dErr_wrtB;//ƫ�ã�������ƫ�õ�ƫ����
	double *value, *dError;//ÿ����Ԫ������������������ƫ����
	double **kernel, **dErr_wrtW;//Ȩֵ��������Ȩֵ��ƫ����

	int m_nFeatureMapPrev;//ǰһ��featureMap�Ĵ�С

	Layer *pLayer;//ָ��featureMap���ڲ�

	void Construct( );
	void Delete();

	void Clear();
	void ClearDError();
	void ClearDErrWrtW();

	double Convolute(double *input, int size, int r0, int c0, double *weight, int kernel_size);
	void Calculate(double *valueFeatureMapPrev, int idxFeatureMapPrev );
	void BackPropagate(double *valueFeatureMapPrev, int idxFeatureMapPrev, double *dErrorFeatureMapPrev);
};

class Layer
{
public:
	int m_type;//��ʶ�ò�����ͣ�������㡢����㻹��ȫ���Ӳ�
	int m_SamplingFactor;//�ò���kernel�����ƶ��Ĳ��������Ǻ�m_type���Ӧ��,�����ȡ0�������ȡ2��ȫ���Ӳ�ȡ1

	Layer *pLayerPrev;//ָ��ǰ���ǰһ��

	int m_nFeatureMap;//��ǰ��featureMap�ĸ���
	int m_FeatureSize;//��ǰ��featureMap�Ĵ�С
	int m_KernelSize;//��ǰ��kernel�Ĵ�С

	FeatureMap* m_FeatureMap;//ָ��ǰ���ָ��

	void ClearAll()
	{
		for(int i=0; i<m_nFeatureMap; i++)
		{
			m_FeatureMap[i].Clear();
			m_FeatureMap[i].ClearDError();
			m_FeatureMap[i].ClearDErrWrtW();
		}
	}

	void Calculate();
	void BackPropagate(double etaLearningRate);

	void Construct(int type, int nFeatureMap, int FeatureSize, int KernelSize, int SamplingFactor);
	void Delete();
};

class CNN
{
public:
	CNN(void);
	~CNN(void);

	Layer *m_Layer;//ָ��ǰ�������Layer��ָ��
	int m_nLayer;//����������������

	void ConstructNN();
	void DeleteNN();

	void LoadWeights(char *FileName);
	void LoadWeightsRandom();
	void SaveWeights(char *FileName);
	int Calculate(double *input);
	void BackPropagate(double *desiredOutput, double eta);
};
