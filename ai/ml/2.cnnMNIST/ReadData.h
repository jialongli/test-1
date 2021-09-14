UCHR ** readImage(UINT type)
//����MNIST�е���������
//type������ʾ��ѵ���������ǲ�������,1��ʾѵ����0��ʾ����
{
	UCHR **myimage;//imageΪ���е�����
	char *pathImage;//����·��
	UINT imageNumber;//��������
    FILE *fp;//�ļ�ָ��

	//Ҫ�����ļ�·���������ĸ���
	if(type==1)//��ʾ��Ҫ����ѵ������
	{
		pathImage="./MNIST/train-images-idx3-ubyte";

		imageNumber=g_cCountTrainingSample;
	}
	else if(type==0)//��ʾ��Ҫ������������
	{
		pathImage="./MNIST/t10k-images-idx3-ubyte";
		imageNumber=g_cCountTestingSample;
	}
	//����Image����ŵ�image[][]��
	fp=fopen(pathImage,"rb");
	if(fp==NULL) printf("Cannot open the image file!");

	myimage=(UCHR **)malloc(sizeof(UCHR *)*imageNumber);
    fseek(fp,16,SEEK_SET);//�����ļ�ͷ16���ֽ�
	for(UINT i=0;i<imageNumber;i++)
	{
		myimage[i]=(UCHR *)malloc(sizeof(UCHR)*g_cImageSize*g_cImageSize);
		fread(myimage[i],1,g_cImageSize*g_cImageSize,fp);//��������
	}
	fclose(fp);

	return myimage;
}
UCHR * readLabel(UINT type)
////����MNIST�е����б�ǩ
//type������ʾ��ѵ���������ǲ�������,1��ʾѵ����0��ʾ����
{
	UCHR *mylabel;//labelΪ���еı�ǩ
	char *pathLabel;//��ǩ·��
	UINT imageNumber;//��������
    FILE *fp;//�ļ�ָ��

	//Ҫ�����ļ�·���������ĸ���
	if(type==1)//��ʾ��Ҫ����ѵ������
	{
		pathLabel="./MNIST/train-labels-idx1-ubyte";
		imageNumber=g_cCountTrainingSample;
	}
	else if(type==0)//��ʾ��Ҫ������������
	{
		pathLabel="./MNIST/t10k-labels-idx1-ubyte";
		imageNumber=g_cCountTestingSample;
	}
	//�����ǩ������label[]��
	fp=fopen(pathLabel,"rb");
	if(fp==NULL)
	{
		printf("Cannot open the label file!");
	}

    mylabel=(UCHR *)malloc(sizeof(UCHR)*imageNumber);
    fseek(fp,8,SEEK_SET);//�����ļ�ͷ8���ֽ�
	fread(mylabel,1,imageNumber,fp);//�����ǩ

	fclose(fp);

	return mylabel;
}
