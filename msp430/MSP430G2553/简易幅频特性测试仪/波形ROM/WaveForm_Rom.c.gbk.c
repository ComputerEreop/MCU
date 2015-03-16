/**********************************************
 *
 * 		Author	:		Shawn Guo
 *      Email   :       iguoxiaopeng@gmail.com
 * 		Date	:		2013/4/21
 *      Last    :       2013/7/17
 * 		Notes	:       �������������� for DDS
 * 		Tool    :       TLA5617A or TLA5618
 *
 **********************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const double DAC_REF = 1.49;    // ����DAC�ο���ѹ�� DAC�������ѹΪ0~2*DAC_REF;
const int VOLTAGE_REF = 150;    //DAC12 �ο���ѹ��100��
const double DAC_V = 0.5;       //����DAC������η�ֵ��ѹ
int N = 4096;                    // ���ROM��ĳ��ȣ�Ĭ��4096������
int STEP = 0;           // ������

unsigned int DAC_V2Code_5617(double v); // �ض���DACת������������Ĭ��ΪTLV5617A
unsigned int DAC_V2Code_5618(double v);

unsigned int DAC12_V2Int_121(unsigned long int v100);   //100���ĵ�ѹֵ
unsigned int DAC12_V2Int_123(unsigned long int v100);
unsigned int DAC12_V2Int_81(unsigned long int v100);
unsigned int DAC12_V2Int_83(unsigned long int v100);

int main()
{
    FILE *fp = NULL;
    unsigned int i, k = 0;
    unsigned short y = 0;
    unsigned char h8 = 0, l8 = 0;
    unsigned long int dac12_temp = 0;

    printf("��������������\n");
    printf("1.���Ҳ�\n");
    printf("2.���Ҳ�\n");
    printf("3.TLV5617A����Bͨ�����Ҳ�\n");
    printf("4.��ѡ��3�����ݲ�ֳ�8λ\n");

    printf("5.�����ź�(0~1V) for TLV5618\n");
    printf("6.�����ź�(��1V��0) �������ź�(No.5)���� for TLV5618\n");
    printf("7.���ò������������ź����� for TLV5618\n");
    printf("8.���ò�����������+��������for TLV5618\n");
    printf("9.�����ź�(0~2V) for MSP430X2XX dac12 \n");


    printf("�������Ӧ����ţ�");
    scanf("%d", &k);
    if(k > 9 || k < 1)
    {
        printf("Error! ����������\n");
        exit(0);
    }
    switch(k)
    {
    case 1:
        if((fp = fopen("sin_ROM.txt","w+")))
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "DEPTH = %d\n", N);
        fprintf(fp, "CONTENT BEGIN:\n");
        for(i = 1; i <= N; i++)
        {
            y = DAC_V2Code_5617(DAC_V * (sin(i * 2 * 3.1415926 / N) + 1));
            if(i % 10 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d ,",y);
        }
        break;
    case 2:
        if((fp = fopen("cos_ROM.txt","w+")) == NULL)
        {
            printf("Error! Cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "DEPTH = %d\n", N);
        fprintf(fp, "CONTENT BEGIN:\n");
        for(i = 1; i <= N; i++)
        {
            y = DAC_V2Code_5617(DAC_V * (cos(i * 2 * 3.1415926 / N) + 1));
            if(i % 10 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d ,",y);
        }
        break;
    case 3:
        if((fp = fopen("TLV5617_fast_sin_ROM.txt","w+")) == NULL)
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "WIDTH = 16\n");
        fprintf(fp, "DEPTH = %d \n", N);
        fprintf(fp, "CONTENT BEGIN:\n");
        for(i = 1; i <= N; i++)
        {
            y = DAC_V2Code_5617(DAC_V * (sin(i * 2 * 3.1415926 / N) + 1));
            y |= 0x4000;
            if(i % 10 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d ,",y);
        }
        break;
    case 4:
        if((fp = fopen("TLV5617_fast_sin_ROM_2.txt","w+")) == NULL)
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "WIDTH = 8\n");
        fprintf(fp, "DEPTH = %d \n", N);
        fprintf(fp, "CONTENT BEGIN:\n����Bͨ��\n");
        for(i = 1; i <= N; i++)
        {
            y = DAC_V2Code_5617(DAC_V * (sin(i * 2 * 3.1415926 / N) + 1));
            y |= 0x4000;
            l8 = y & (0x00ff);
            h8 = (char)(y >> 8);
            if(i % 10 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d , %6d ,",h8, l8);
        }
        break;
    case 5:       // TLV5618
    {
        if((fp = fopen("TLV5618_1V��������.txt","w+")) == NULL)
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "WIDTH = 8\n");
        fprintf(fp, "DEPTH = %d \n", N);
        fprintf(fp, "CONTENT BEGIN:\n����Bͨ��\n");

        for(i = 1; i <= N; i++)
        {
            y = DAC_V2Code_5618(0.5 * (sin(i * 2 * 3.1415926 / N) + 1));
            y |= 0x4000;
            l8 = y & (0x00ff);
            h8 = (char)(y >> 8);
            if(i % 10 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d , %6d ,",h8, l8);
        }
        break;
    }
    case 6:
    {
        if((fp = fopen("TLV5618_����+��������.txt","w+")) == NULL)
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "WIDTH = 8\n");
        fprintf(fp, "DEPTH = %d \n", N);
        fprintf(fp, "CONTENT BEGIN:\n����Bͨ��\n");

        for(i = 1; i < N; i++)
        {
            y = DAC_V2Code_5618(0.5 * (sin(i * 2 * 3.1415926 / N) + 1) + 1) ;
            y |= 0x4000;
            l8 = y & (0x00ff);
            h8 = (char)(y >> 8);
            if(i % 10 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d , %6d ,",h8, l8);
        }
        break;
    }
    case 7:
    {
        int t = 0;
        if((fp = fopen("TLV5618_��������������.txt","w+")) == NULL)
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "WIDTH = 8\n");
        fprintf(fp, "DEPTH = %d \n", N);
        printf("�����벽����:\t");
        scanf("%d",&STEP);
        fprintf(fp, "STEP = %d \n", STEP);
        fprintf(fp, "CONTENT BEGIN:\n����Bͨ��\n");

        for(i = 1; i < N; i += STEP)
        {
            y = DAC_V2Code_5618(0.5 * (sin(i * 2 * 3.1415926 / N) + 1));
            y |= 0x4000;
            l8 = y & (0x00ff);
            h8 = (char)(y >> 8);
            if(++t % 5 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d , %6d ,",h8, l8);
        }
        break;
    }
    case 8:
    {
        int t = 0;

        if((fp = fopen("TLV5618_����������+��������.txt","w+")) == NULL)
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "WIDTH = 8\n");
        fprintf(fp, "DEPTH = %d \n", N);
        printf("�����벽����:\t");
        scanf("%d",&STEP);
        fprintf(fp, "STEP = %d \n", STEP);
        fprintf(fp, "CONTENT BEGIN:\n����Bͨ��\n");

        for(i = 1; i < N; i += STEP)
        {
            y = DAC_V2Code_5618(0.5 * (sin(i * 2 * 3.1415926 / N) + 1) + 1);
            y |= 0x4000;
            l8 = y & (0x00ff);
            h8 = (char)(y >> 8);
            if(++t % 5 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d , %6d ,",h8, l8);
        }
        break;
    }
    case 9:
    {
        if((fp = fopen("�������� for DAC12.txt","w+")) == NULL)
        {
            printf("cannot open this file.\n");
            exit(0);
        }
        fprintf(fp, "WIDTH = 12\n");
        fprintf(fp, "DEPTH = %d \n", N);
        fprintf(fp, "CONTENT BEGIN:\n�������� for DAC12\n");

        for(i = 0; i < N; i++)
        {
            dac12_temp = DAC12_V2Int_121(DAC_V * (sin(i * 2 * 3.1415926 / N) + 1) * 100);
            if(i % 10 == 0)
                fprintf(fp, "\n" );
            fprintf(fp,"%6d ,", dac12_temp);
        }
        break;
    }

    }
    fclose(fp);
    fp = NULL;

    return 0;
}

unsigned int DAC_V2Code_5617(double v)
{
    unsigned int dat = 0;

    if(v >= 2 * DAC_REF || v < 0) return 0;

    dat = (int)(v * 512 / DAC_REF);       // v = 2 * ref * (code / 2^10); 10λDA,���ݷ���16�ֽ��е�2��11λ,�ճ���0�͵�1λ
    dat <<= 2;

    return dat;
}

unsigned int DAC_V2Code_5618(double v)
{
    unsigned int dat = 0;

    if(v >= 2 * DAC_REF || v < 0) return 0;

    dat = (int)(v * 2048 / DAC_REF); // v = 2 * ref * (code / 2^12);

    return dat;
}

unsigned int DAC12_V2Int_121(unsigned long int v100)
{
    return ((v100 << 12) / VOLTAGE_REF);
}

unsigned int DAC12_V2Int_123(unsigned long int v100)
{
    return ((v100 << 12) / (VOLTAGE_REF * 3));
}

unsigned int DAC12_V2Int_81(unsigned long int v100)
{
    return ((v100 << 8) / VOLTAGE_REF);
}

unsigned int DAC12_V2Int_83(unsigned long int v100)
{
    return ((v100 << 12) / (VOLTAGE_REF * 3));
}
