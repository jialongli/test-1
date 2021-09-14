#include "cnn.h"

CNN::CNN(void)
{
	ConstructNN();
}

CNN::~CNN(void)
{
	DeleteNN();
}

////////////////////////////////////
void CNN::ConstructNN()
////////////////////////////////////
{
	int i;
	m_nLayer = 5;//��CNN����5�㣬���������Ϣ��֪��5�㼴Ϊ�����

	m_Layer = new Layer[m_nLayer];//����m_nLayer��������Ŀռ�

    //�����ɵĸ�����г�ʼ��
	m_Layer[0].pLayerPrev = NULL;
	for(i=1; i<m_nLayer; i++) m_Layer[i].pLayerPrev = &m_Layer[i-1];

	m_Layer[0].Construct	(	INPUT_LAYER,		1,		29,		0,	0	);//�����
	m_Layer[1].Construct	(	CONVOLUTIONAL,		6,		13,		5,	2	);//�����
	m_Layer[2].Construct	(	CONVOLUTIONAL,		50,		5,		5,	2	);//�����
	m_Layer[3].Construct	(	FULLY_CONNECTED,	100,	1,		5,	1	);//ȫ���Ӳ�
	m_Layer[4].Construct	(	FULLY_CONNECTED,	10,		1,		1,	1	);//ȫ���Ӳ�
}

///////////////////////////////
void CNN::DeleteNN()
///////////////////////////////
{
	for(int i=0; i<m_nLayer; i++) m_Layer[i].Delete();
}

//////////////////////////////////////////////
void CNN::LoadWeightsRandom()
/////////////////////////////////////////////
//�����װ��Ȩֵ����ЩȨֵ����N(0,0.05^2)�ķֲ�
{
	int i, j, k, m;

	srand((unsigned)time(0));

	for ( i=1; i<m_nLayer; i++ )
	{
		for( j=0; j<m_Layer[i].m_nFeatureMap; j++ )
		{
			m_Layer[i].m_FeatureMap[j].bias = 0.05 * RANDOM_PLUS_MINUS_ONE;//ƫ��bias��ʼ��

			for(k=0; k<m_Layer[i].pLayerPrev->m_nFeatureMap; k++)
				for(m=0; m < m_Layer[i].m_KernelSize * m_Layer[i].m_KernelSize; m++)
					m_Layer[i].m_FeatureMap[j].kernel[k][m] = 0.05 * RANDOM_PLUS_MINUS_ONE;//kernel��ʼ��
		}
	}
}

//////////////////////////////////////////////
void CNN::LoadWeights(char *FileName)
/////////////////////////////////////////////
//��FileName��ָ�ļ���װ��Ȩֵ���͸ó����б���Ȩֵ�ĸ�ʽ��һ����
{
	int i, j, k, m;

	FILE *fp;
	if((fp = fopen(FileName, "r")) == NULL) return;

	for ( i=1; i<m_nLayer; i++ )
	{
		for( j=0; j<m_Layer[i].m_nFeatureMap; j++ )
		{
			fscanf(fp, "%lg ", &m_Layer[i].m_FeatureMap[j].bias);

			for(k=0; k<m_Layer[i].pLayerPrev->m_nFeatureMap; k++)
				for(m=0; m < m_Layer[i].m_KernelSize * m_Layer[i].m_KernelSize; m++)
					fscanf(fp, "%lg ", &m_Layer[i].m_FeatureMap[j].kernel[k][m]);
		}
	}
	fclose(fp);
}

//////////////////////////////////////////////
void CNN::SaveWeights(char *FileName)
/////////////////////////////////////////////
//����Ȩֵ��FileName��ָ���ļ���
{
	int i, j, k, m;

	FILE *fp;
	if((fp = fopen(FileName, "w")) == NULL) return;

	for ( i=1; i<m_nLayer; i++ )
	{
		for( j=0; j<m_Layer[i].m_nFeatureMap; j++ )
		{
			fprintf(fp, "%lg ", m_Layer[i].m_FeatureMap[j].bias);

			for(k=0; k<m_Layer[i].pLayerPrev->m_nFeatureMap; k++)
				for(m=0; m < m_Layer[i].m_KernelSize * m_Layer[i].m_KernelSize; m++)
				{
					fprintf(fp, "%lg ", m_Layer[i].m_FeatureMap[j].kernel[k][m]);
				}
		}
	}

	fclose(fp);
}

//////////////////////////////////////////////////////////////////////////
int CNN::Calculate(double *input)
//////////////////////////////////////////////////////////////////////////
//ǰ�򴫲�����Ҫ�������룻�ú�������input��������µ�Ԥ�����
{
	int i, j;
    double output[g_cOutputSize];//�������������
	//��input��ֵ��layer 0
	for(i=0; i<m_Layer[0].m_nFeatureMap; i++)
		for(j=0; j < m_Layer[0].m_FeatureSize * m_Layer[0].m_FeatureSize; j++)
							m_Layer[0].m_FeatureMap[0].value[j] = input[j];

	//ǰ�򴫲�(forward propagation),�ӵڶ���(Layer[1])��ʼ������ÿ����ÿ����Ԫ�����
	for(i=1; i<m_nLayer; i++)
	{
		//ÿ����Ԫ���������˵ÿ��feature maps��ǰ�򴫲�֮ǰ��Ӧ��ʼ��Ϊ0
		for(j=0; j<m_Layer[i].m_nFeatureMap; j++)
					m_Layer[i].m_FeatureMap[j].Clear();

		//���ò�������ʼǰ�򴫲�
		m_Layer[i].Calculate();
	}

	//ǰ�򴫲�����ʱ��������㸳ֵ��output
	for(i=0; i<m_Layer[m_nLayer-1].m_nFeatureMap; i++) //��һ�����Բ�Ҫ,����ֱ�ӱȽ�������еĸ����,����ֻ��Ϊ�����ӿɶ���
		output[i] = m_Layer[m_nLayer-1].m_FeatureMap[i].value[0];

	//�ҵ����������±꣬��������ΪԤ������
	j = 0;
	for(i=1; i<m_Layer[m_nLayer-1].m_nFeatureMap; i++)
		if(output[i] > output[j]) j = i;

	return j;//�������������һ����Ԫ����ΪԤ������
}

///////////////////////////////////////////////////////////
void CNN::BackPropagate(double *desiredOutput, double eta)
///////////////////////////////////////////////////////////
//���򴫲���������Ҫ���������������ѧϰЧ��
{
	int i;
	//��������������������ƫ�����������������X��ƫ����
	for(i=0; i<m_Layer[m_nLayer-1].m_nFeatureMap; i++)
	{
		m_Layer[m_nLayer-1].m_FeatureMap[i].dError[0] =
		    m_Layer[m_nLayer-1].m_FeatureMap[i].value[0] - desiredOutput[i];
	}

	//������㿪ʼ��������ƫ�������򴫲���ֱ����������һ��(ǰ�򴫲�ʱ�ĵ�һ�����ز�)Ϊֹ
	for(i=m_nLayer-1; i>0; i--)
	{
		m_Layer[i].BackPropagate(eta);
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Layer::Construct(int type, int nFeatureMap, int FeatureSize, int KernelSize, int SamplingFactor)
/////////////////////////////////////////////////////////////////////////////////////////////////////
{
	m_type = type;//�ò������
	m_nFeatureMap = nFeatureMap;//�ò㺬��featureMap�ĸ���
	m_FeatureSize = FeatureSize;//�ò�feature�Ĵ�С
	m_KernelSize = KernelSize;//�ò������ڵĴ�С
	m_SamplingFactor = SamplingFactor;//�ò���о��ʱ�������ƶ��Ĳ�����ȫ���ӿɿ��ɲ���Ϊ1�ľ��

	m_FeatureMap = new FeatureMap[ m_nFeatureMap ];//����m_nFeatureMap��featureMap����Ŀռ�

	for(int j=0; j<m_nFeatureMap; j++)//����ЩfeatureMap���г�ʼ��
	{
		m_FeatureMap[j].pLayer = this;
		m_FeatureMap[j].Construct(  );
	}
}

/////////////////////////
void Layer::Delete()
/////////////////////////
{
	for(int j=0; j<m_nFeatureMap; j++) m_FeatureMap[j].Delete();
}

///////////////////////////////////////////////////
void Layer::Calculate()
///////////////////////////////////////////////////
//Layer��ǰ�򴫲���ÿ����Ԫ���������ָ�ʽ��Y->sigmoid->X
{
	for(int i=0; i<m_nFeatureMap; i++)//ѭ����ǰ���ÿ��featureMap
	{
	    for(int k=0; k < m_FeatureSize * m_FeatureSize; k++)//�Ȱѵ�i��featureMap��bias���ϣ��Ժ�Ĳ���ֻ����о�����㼴��
		{
				m_FeatureMap[i].value[k] = m_FeatureMap[i].bias;
		}
		int j;
		//����featureMap���о�����㣬ֻ�ǰ�����򵥵���ӣ��õ�Y
		for( j=0; j<pLayerPrev->m_nFeatureMap; j++)//��ǰһ���ÿ��featureMap��������㵱ǰ��ĵ�i��featureMap
		{
			m_FeatureMap[i].Calculate(
							pLayerPrev->m_FeatureMap[j].value,		//input feature map
							j										//index of input feature map
										);
		}

		//��Y����SIGMOD������ѹ���õ��ò�����X
		for(j=0; j < m_FeatureSize * m_FeatureSize; j++)
		{
			m_FeatureMap[i].value[j] = SIGMOID(m_FeatureMap[i].value[j]);
		}
	}
}

///////////////////////////////////////////////////////////////
void Layer::BackPropagate(double etaLearningRate)
//////////////////////////////////////////////////////////////
//Layer���з��򴫲�
{
	int i,j;
	for(i=0; i<m_nFeatureMap; i++)
	{
		for(j=0; j < m_FeatureSize * m_FeatureSize; j++)
		{
			double temp = DSIGMOID(m_FeatureMap[i].value[j]);
			m_FeatureMap[i].dError[j] = temp * m_FeatureMap[i].dError[j];//��ʼ����ǹ���X��ƫ����������temp����
			                                                             //����Y��ƫ����
		}
	}

	for(i=0; i<m_nFeatureMap; i++)  m_FeatureMap[i].ClearDErrWrtW();//������Ȩֵ��ƫ������ʼ��Ϊ0

	for(i=0; i<pLayerPrev->m_nFeatureMap; i++)//�ò��ǰһ��(ǰ�򴫲�����)�������X��ƫ������0
		pLayerPrev->m_FeatureMap[i].ClearDError();

	//���򴫲������򴫲���ǰһ��
	for(i=0; i<m_nFeatureMap; i++)//�Ե�ǰ���ÿ��featureMap���д���
	{
		for( j=0; j<pLayerPrev->m_nFeatureMap; j++)//���㵱ǰfeatureMap��ǰһ���featureMap��Ӱ�죬
		{                                          //ѭ����ǰ���ÿ��kernel���൱��ͬʱѭ��ǰһ���ÿ��featureMap
			m_FeatureMap[i].BackPropagate(
				pLayerPrev->m_FeatureMap[j].value,	//ǰһ���feature map
				j,									//ǰһ���j��feature map
				pLayerPrev->m_FeatureMap[j].dError	//ǰһ��������X��ƫ����
				);
		}

		for(int j=0; j<m_FeatureSize * m_FeatureSize; j++)//������bias��ƫ����
			m_FeatureMap[i].dErr_wrtB += m_FeatureMap[i].dError[j];
	}

	//����Ȩֵ
	for(i=0; i<m_nFeatureMap; i++)
	{
		m_FeatureMap[i].bias -= etaLearningRate * m_FeatureMap[i].dErr_wrtB;//����bias

		for(int j=0; j<pLayerPrev->m_nFeatureMap; j++)//����kernel
		{
			for(int k=0; k < m_KernelSize * m_KernelSize; k++)
				m_FeatureMap[i].kernel[j][k] -= etaLearningRate * m_FeatureMap[i].dErr_wrtW[j][k];
		}
	}

}


//////////////////////////////////////////////////////////////////////////////////////////
void FeatureMap::Construct( )
//////////////////////////////////////////////////////////////////////////////////////////
{
	int i;
	//ǰһ��featureMap�ĸ���
	if(pLayer->m_type == INPUT_LAYER) m_nFeatureMapPrev = 0;
	else m_nFeatureMapPrev = pLayer->pLayerPrev->m_nFeatureMap;

	int FeatureSize = pLayer->m_FeatureSize;//featureMap�Ĵ�С
	int KernelSize  = pLayer->m_KernelSize;//������ڵĴ�С

	value = new double [ FeatureSize * FeatureSize ];//��Ԫ�����X
	dError = new double [ FeatureSize * FeatureSize ];//��������Ԫ���X��ƫ����

	//����Ȩֵ
	kernel = new double* [ m_nFeatureMapPrev ];
	for(i=0; i<m_nFeatureMapPrev; i++)
	{
		kernel[i] = new double [KernelSize * KernelSize];

		bias = 0.05 * RANDOM_PLUS_MINUS_ONE;//bias��ʼ��
		for(int j=0; j < KernelSize * KernelSize; j++) kernel[i][j] = 0.05 * RANDOM_PLUS_MINUS_ONE;//kernel��ʼ��
	}
	//������Ȩֵ��ƫ����
	dErr_wrtW = new double* [ m_nFeatureMapPrev ];
	for(i=0; i<m_nFeatureMapPrev; i++)
		dErr_wrtW[i] = new double [KernelSize * KernelSize];

	//������bias��ƫ����dErr_wrtB�����ʼ��
}

///////////////////////////
void FeatureMap::Delete()
///////////////////////////
{
	delete[] value;
	delete[] dError;

	for(int i=0; i<m_nFeatureMapPrev; i++)
	{
		delete[] kernel[i];
		delete[] dErr_wrtW[i];
	}
}

////////////////////////////
void FeatureMap::Clear()
/////////////////////////////
{
	for(int i=0; i < pLayer->m_FeatureSize * pLayer->m_FeatureSize; i++) value[i] = 0.0;
}

////////////////////////////////
void FeatureMap::ClearDError()
/////////////////////////////////
{
	for(int i=0; i < pLayer->m_FeatureSize * pLayer->m_FeatureSize; i++) dError[i] = 0.0;
}

////////////////////////////////
void FeatureMap::ClearDErrWrtW()
////////////////////////////////
{
	dErr_wrtB = 0;
	for(int i=0; i < m_nFeatureMapPrev; i++)
		for(int j=0; j < pLayer->m_KernelSize * pLayer->m_KernelSize; j++) dErr_wrtW[i][j] = 0.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void FeatureMap::Calculate(double *valueFeatureMapPrev, int idxFeatureMapPrev )
//////////////////////////////////////////////////////////////////////////////////////////////////////
//featureMapǰ�򴫲�

//	valueFeatureMapPrev:ָ��ǰһ���һ��featureMap��ָ��
//	idxFeatureMapPrev :	��ʶvalueFeatureMapPrev��ǰһ��ĵڼ���featureMap
{
	int isize = pLayer->pLayerPrev->m_FeatureSize; //ǰһ��featureMap�Ĵ�С
	int ksize = pLayer->m_KernelSize;//��ǰ��kernel���ڵĴ�С
	int step_size = pLayer->m_SamplingFactor;//����Ĳ�����ȫ�����൱�ڲ���Ϊ1�ľ��

	int k = 0;

	for(int row0 = 0; row0 <= isize - ksize; row0 += step_size)
		for(int col0 = 0; col0 <= isize - ksize; col0 += step_size)
			value[k++] += Convolute(valueFeatureMapPrev, isize, row0, col0, kernel[idxFeatureMapPrev], ksize);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
double FeatureMap::Convolute(double *input, int size, int r0, int c0, double *weight, int kernel_size)
//////////////////////////////////////////////////////////////////////////////////////////////////////
{
	int i, j, k = 0;
	double summ = 0;

	for(i = r0; i < r0 + kernel_size; i++)
		for(j = c0; j < c0 + kernel_size; j++)
			summ += input[i * size + j] * weight[k++];

	return summ;
}

/////////////////////////////////////////////////////////////////////////////////////
void FeatureMap::BackPropagate(double *valueFeatureMapPrev, int idxFeatureMapPrev,
							   double *dErrorFeatureMapPrev)
/////////////////////////////////////////////////////////////////////////////////////
//featureMap���򴫲�

//	valueFeatureMapPrev:ǰһ��(ǰ�򴫲�����)featureMap�����
//	idxFeatureMapPrev :��ʶvalueFeatureMapPrev�������ǰһ��ĵڼ���featureMap
//  dErrorFeatureMapPrev:ǰһ��featureMap�У����������X��ƫ����

{
	int isize = pLayer->pLayerPrev->m_FeatureSize;	//ǰһ��featureMap�Ĵ�С
	int ksize = pLayer->m_KernelSize;				//��ǰfeatureMap��kernel�Ĵ�С
	int step_size = pLayer->m_SamplingFactor;		//��ǰ���������ƶ��Ĳ���

	int row0, col0, k;

	k = 0;
	for(row0 = 0; row0 <= isize - ksize; row0 += step_size)//ѭ���õ�����ǰһ��featureMap�ھ������ʱ��kernel����
	{                                                      //ÿ���ƶ�����ʼλ��
		for(col0 = 0; col0 <= isize - ksize; col0 += step_size)
		{
			for(int i=0; i<ksize; i++)
			{
				for(int j=0; j<ksize; j++)//ǰһ��featureMap��ÿ����ʼλ�õ�һ��ksize*ksize�����Ӧ��ǰ���һ����Ԫ
				{
					//���ڵõ�ǰһ��������X��ƫ����
					double temp = kernel[idxFeatureMapPrev][i * ksize + j];
					dErrorFeatureMapPrev[(row0 + i) * isize + (j + col0)] += dError[k] * temp;

                    //���ڵõ���ǰ��������kernel��ƫ����
					temp = valueFeatureMapPrev[(row0 + i) * isize + (j + col0)];
					dErr_wrtW[idxFeatureMapPrev][i * ksize + j] += dError[k] * temp;
				}//j
			}//i
			k++;//������ʶ��ǰ���dError_wrt_Y��ÿ��dError_wrt_Y��Ӧǰһ���һ��ksize*ksize����
		}//col0
	}//row0

}//end function
