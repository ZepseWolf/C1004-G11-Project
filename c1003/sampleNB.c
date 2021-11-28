#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define trainingsize 10 /* 90% of the data used for training*/
#define datacount 11   /* 90% of the data used for training,10% used for testing*/
#define featuresize 3
#define PI 3.142

int main()
{
    FILE *file_ptr;
    int i,ii,count;
    double input[datacount][featuresize+1];
    double traininginput[trainingsize][featuresize]; 
    double trainingoutput[trainingsize][1]; 
    double testinginput[datacount-trainingsize][featuresize]; 
    double testingoutput[datacount-trainingsize][1];
    double P_DefaultedBorrow[trainingsize][2]; /**Training set:probability of defaulted borrower, firstelement =No, second=Yes*/
    double P_DefaultedBorrowtotal[2]; /*Training set: probability of defaulted borrower, firstelement =No, second=Yes*/
    double P_HomeOwner_Default_No[2];/*Training set: conditional probability of Home owner under defaulted borrow=No*/
    double P_HomeOwner_Default_Yes[2];/*Training set:conditional probability of Home owner under defaulted borrow=Yes*/ 
    double P_test_HomeOwner_Default_No;/*Testing set: conditional probability of Home owner under defaulted borrow=No*/
    double P_test_HomeOwner_Default_Yes;/*Testing set: conditional probability of Home owner under defaulted borrow=Yes*/
    double P_Maritial_Default_No[3];/*Training set: conditional probability of Maritial Status under defaulted borrow=No*/
    double P_Maritial_Default_Yes[3];/*Training set: conditional probability of Maritial Status under defaulted borrow=Yes*/
    double P_test_Maritial_Default_No;/*Testing set: conditional probability of Maritial Status under defaulted borrow=No*/
    double P_test_Maritial_Default_Yes;/*Testing set: conditional probability of Maritial Status under defaulted borrow=Yes*/
    double mean_annualincome_Default_No;/*Training set: mean of annual income for Gaussian PDF under defaulted borrow=No*/
    double mean_annualincome_Default_Yes;/*Training set: mean of annual income for Gaussian PDF under defaulted borrow=Yes*/
    double var_annualincome_Default_No;/*Training set: variance of annual income for Gaussian PDF under defaulted borrow=No*/
    double var_annualincome_Default_Yes;/*Training set: variance of annual income for Gaussian PDF under defaulted borrow=Yes*/
    double P_annualincome_Default_No;/*Training set: conditional probability of Annual Income under defaulted borrow=No*/
    double P_annualincome_Default_Yes;/*Training set: conditional probability of Annual Income under defaulted borrow=Yes*/
    long double P_test_annualincome_Default_No;/*Testing set: conditional probability of Annual Income under defaulted borrow=No*/
    long double P_test_annualincome_Default_Yes;/*Testing set: conditional probability of Annual Income under defaulted borrow=Yes*/
    double z;
    long double postp_Default_No_train[trainingsize][1]; /*posterior probability of defaulted borrow=No*/
    long double postp_Default_Yes_train[trainingsize][1];/*posterior probability of defaulted borrow=Yes*/
    long double postp_Default_No_test[datacount-trainingsize][1]; /*posterior probability of defaulted borrow=No*/
    long double postp_Default_Yes_test[datacount-trainingsize][1];/*posterior probability of defaulted borrow=Yes*/
    long double diff;
    double Default_Yes_test[datacount-trainingsize][1];
    double errortest[datacount-trainingsize][1];
    double Default_Yes_train[trainingsize][1];
    double errortrain[trainingsize][1];
    double errorper;
  
    /****************************************************************/
    /*Read data from input file                                     */
    /* Feature 1 :Home Owner    - Yes=1, No=0                       */
    /* Feature 2 :Martial Status- Single=-1, Married=0, Divorced=1  */
    /* Feature 3 :Annual Income - Gaussian distributed              */
    /* Outcome   :Defaulted Owner-Yes=1, No=0                       */
    /****************************************************************/
    file_ptr=fopen("sampleNB.txt","r");
    if (file_ptr==NULL)
    {
        printf("File could not be opened \n");
        exit(1);
    }
    for(i=0;i<=datacount-1;i++)
    {
       fscanf(file_ptr,"%lf, %lf, %lf ,%lf", &input[i][0],&input[i][1],&input[i][2],&input[i][3]);
    }
    fclose(file_ptr);
    /*****************************************/
    /*Assigned training set and testing set  */
    /******************************************/
    /*1)Assigned training input set from input*/
    for (i=0;i<=trainingsize-1;i++)
    {
        for (ii=0;ii<=(featuresize-1);ii++)
        {
            traininginput[i][ii]=input[i][ii];
        }
    }
     /*2)Assigned testing input set from input*/
    for (i=0;i<=(datacount-trainingsize-1);i++)
    {
        for (ii=0;ii<=featuresize-1;ii++)
        {
            testinginput[i][ii]=input[trainingsize+i][ii];
        }
    }
    /*3)Assigned training output set from input*/
    for (i=0;i<=trainingsize-1;i++)
    {
            trainingoutput[i][0]=input[i][featuresize];
    }
     /*4)Assigned tested output set from input*/
    for (i=0;i<=(datacount-trainingsize-1);i++)
    {
            testingoutput[i][0]=input[trainingsize+i][featuresize];
    }
/**************************************************/
/*Find the prior probability of default borrower No or Yes */
/**************************************************/
    for (i=0;i<=trainingsize-1;i++)
    {
           if (trainingoutput[i][0]==1)
           {
                P_DefaultedBorrow[i][1]=1; /*defaulter=Yes=1*/
                P_DefaultedBorrow[i][0]=0; 
           }
           else
           {
               P_DefaultedBorrow[i][0]=1; /*defaulter=No=0*/
              P_DefaultedBorrow[i][1]=0;
           }        
    }
    P_DefaultedBorrowtotal[0]=0;
    P_DefaultedBorrowtotal[1]=0;
    for (i=0;i<=trainingsize-1;i++)
    {
        if (P_DefaultedBorrow[i][0]==1)
        {
             P_DefaultedBorrowtotal[0]+=1; /*defaulter=No=0 */
        }
        else
        {
            P_DefaultedBorrowtotal[1]+=1; /*defaulter=Yes=1 */
        }
    }
/******************************************************/
/*Find the conditional probability of Homeowner under defaulter=No=0*/
/*******************************************************/
    P_HomeOwner_Default_No[0]=0; /*Homer owner=No,Defaulter=No*/
    P_HomeOwner_Default_No[1]=0; /*Homer owner=Yes,Defaulter=No*/
    for (i=0;i<=trainingsize-1;i++)
    {
        if((trainingoutput[i][0]==0)&&(traininginput[i][0]==0))/*Homer owner=No,Defaulter=No*/
        {
            P_HomeOwner_Default_No[0]+=1; //count of feautre 1 when it 0 /total of class 0 
        }
        if((trainingoutput[i][0]==0)&&(traininginput[i][0]==1))/*Homer owner=Yes,Defaulter=No*/
        {
           P_HomeOwner_Default_No[1]+=1;//count of feautre 1 when it 1/total of class 0
        }
    }
    P_HomeOwner_Default_No[0]/= P_DefaultedBorrowtotal[0]; /*conditional probability of Homer owner= No under Defaulter=No */
    P_HomeOwner_Default_No[1]/= P_DefaultedBorrowtotal[0]; /*conditional probability of Homer owner= Yes under Defaulter=No */
    /******************************************************/
    /*Find the conditional probability of Homeowner under defaulter=Yes=1*/
    /*******************************************************/
    P_HomeOwner_Default_Yes[0]=0; /*Homer owner=No,Defaulter=Yes*/
    P_HomeOwner_Default_Yes[1]=0; /*Homer owner=Yes,Defaulter=Yes*/
    for (i=0;i<=trainingsize-1;i++)
    {
        /*diff=fabs(traininginput[i][0]-(-1));*/
        if((trainingoutput[i][0]==1)&&(traininginput[i][0]==0))/*Homer owner=No,Defaulter=Yes*/
        {
            P_HomeOwner_Default_Yes[0]+=1;//count of feautre 1 of 0 /total of conditon 1
        }
        /*diff=fabs(traininginput[i][0]-(-0.33));*/
        if((trainingoutput[i][0]==1)&&(traininginput[i][0]==1))/*Homer owner=Yes,Defaulter=Yes*/
        {
            P_HomeOwner_Default_Yes[1]+=1;//count of feautre 1 of 1 /total of conditon 1
        }
    }
    P_HomeOwner_Default_Yes[0]/= P_DefaultedBorrowtotal[1]; /*conditional probability of Homer owner= No under Defaulter=Yes */
    P_HomeOwner_Default_Yes[1]/= P_DefaultedBorrowtotal[1]; /*conditional probability of Homer owner= Yes under Defaulter=Yes */
    
    /******************************************************/
    /*Find the conditional probability of Maritial Status under defaulter=No=0*/
    /*******************************************************/
    P_Maritial_Default_No[0]=0;/*under defaulter=No, single;*/
    P_Maritial_Default_No[1]=0;/*under defaulter=No, married;*/
    P_Maritial_Default_No[2]=0;/*under defaulter=No, divorced;*/
    count=0;
    for (i=0;i<=trainingsize-1;i++)
    {
        if((trainingoutput[i][0]==0)&&(traininginput[i][1]==-1))/*under defaulter=No, single;*/
        {
            P_Maritial_Default_No[0]+=1;
        }
        if((trainingoutput[i][0]==0)&&(traininginput[i][1]==0))/*under defaulter=No, married;*/
        {
            P_Maritial_Default_No[1]+=1;
        }
        if((trainingoutput[i][0]==0)&&(traininginput[i][1]==1))/*under defaulter=No, divorced;*/
        {
           P_Maritial_Default_No[2]+=1;
        }
    }
    P_Maritial_Default_No[0]/= P_DefaultedBorrowtotal[0];
    P_Maritial_Default_No[1]/= P_DefaultedBorrowtotal[0];
    P_Maritial_Default_No[2]/= P_DefaultedBorrowtotal[0];
    /******************************************************/
    /*Find the conditional probability of Maritial Status under defaulter=Yes=1*/
    /*******************************************************/
    P_Maritial_Default_Yes[0]=0;/*under defaulter=Yes, single*/
    P_Maritial_Default_Yes[1]=0;/*under defaulter=Yes, married*/
    P_Maritial_Default_Yes[2]=0;/*under defaulter=No, divorced*/
    count=0;
    for (i=0;i<=trainingsize-1;i++)
    {
        if((trainingoutput[i][0]==1)&&(traininginput[i][1]==-1))/*never, abnormal*/
        {
            P_Maritial_Default_Yes[0]+=1;
        }
        if((trainingoutput[i][0]==1)&&(traininginput[i][1]==0))/*occasional, abnormal*/
        {
            P_Maritial_Default_Yes[1]+=1;
        }
        if((trainingoutput[i][0]==1)&&(traininginput[i][1]==1))/*daily, abnormal*/
        {
           P_Maritial_Default_Yes[2]+=1;
        }
    }
    P_Maritial_Default_Yes[0]/= P_DefaultedBorrowtotal[1];
    P_Maritial_Default_Yes[1]/= P_DefaultedBorrowtotal[1];
    P_Maritial_Default_Yes[2]/= P_DefaultedBorrowtotal[1];
    /******************************************************/
    /*find the conditional probability (normal gaussian)parameter of annual income under defaulter=0 */
    /*******************************************************/
    mean_annualincome_Default_No=100000;/*mean=100k*/
    var_annualincome_Default_No=2975000; /*variance=2975*/
    /******************************************************/
    /*find the conditional probability (normal gaussian)parameter of annual income under defaulter=1 */
    /*******************************************************/
    mean_annualincome_Default_Yes=90000;/*mean=90k*/
    var_annualincome_Default_Yes=25000;/*variance=25*/
/******************************************************/
/*convert prior probability to probability */
/*******************************************************/  
    P_DefaultedBorrowtotal[0]/=trainingsize; /*defaulter=No*/ 
    P_DefaultedBorrowtotal[1]/=trainingsize; /*defaulter=Yes*/

/******************************************************/
/*Predict the testing set result under defaulter=No*/
/*******************************************************/
    for (i=0;i<=(datacount-trainingsize-1);i++)
    {
        /********************************************************/
        /*to get conditional probability for Homeowner under defaulter=No and Yes*/
        /********************************************************/
        P_test_HomeOwner_Default_No=0;
        P_test_HomeOwner_Default_Yes=0;
        if (testinginput[i][0]==0)
        {
             P_test_HomeOwner_Default_No=P_HomeOwner_Default_No[0]; //
             P_test_HomeOwner_Default_Yes=P_HomeOwner_Default_Yes[0];
        }
        if (testinginput[i][0]==1)
        {
            P_test_HomeOwner_Default_No=P_HomeOwner_Default_No[1];
            P_test_HomeOwner_Default_Yes=P_HomeOwner_Default_Yes[1];
        }
       
        /********************************************************/
        /*to get conditional probability for maritial status under defaulter=No and Yes*/
        /********************************************************/
        P_test_Maritial_Default_No=0;
        P_test_Maritial_Default_Yes=0;
        if (testinginput[i][1]==-1)
        {
            P_test_Maritial_Default_No=P_Maritial_Default_No[0];
            P_test_Maritial_Default_Yes=P_Maritial_Default_Yes[0];
        }
        if (testinginput[i][1]==0)
        {
            P_test_Maritial_Default_No=P_Maritial_Default_No[1];
            P_test_Maritial_Default_Yes=P_Maritial_Default_Yes[1];
        }
        if (testinginput[i][1]==1)
        {
            P_test_Maritial_Default_No=P_Maritial_Default_No[2];
            P_test_Maritial_Default_Yes=P_Maritial_Default_Yes[2];
        }
        /********************************************************/
        /*to get conditional probability for annual income under defaulter=No and Yes**/
        /********************************************************/
        z=((testinginput[i][2])*1e3-mean_annualincome_Default_No)/sqrt(var_annualincome_Default_No);
        P_test_annualincome_Default_No=1/sqrt(2*PI*var_annualincome_Default_No)*expl(-0.5*pow(z,2));
        z=((testinginput[i][2])*1e3-mean_annualincome_Default_Yes)/sqrt(var_annualincome_Default_Yes);
        P_test_annualincome_Default_Yes=1/sqrt(2*PI*var_annualincome_Default_Yes)*expl(-0.5*pow(z,2));
        postp_Default_No_test[i][0]=P_test_HomeOwner_Default_No*P_test_Maritial_Default_No*P_test_annualincome_Default_No*P_DefaultedBorrowtotal[0];
        postp_Default_Yes_test[i][0]=P_test_HomeOwner_Default_Yes*P_test_Maritial_Default_Yes*P_test_annualincome_Default_Yes*P_DefaultedBorrowtotal[1];
        diff= postp_Default_No_test[i][0]-postp_Default_Yes_test[i][0];
        if (diff>=0)
        {
            Default_Yes_test[i][0]=0;
        }
        else
        {
            Default_Yes_test[i][0]=1;
        }
        errortest[i][0]=0;
        if (Default_Yes_test[i][0]!=testingoutput[i][0])
        {
            errortest[i][0]=1;
        }

    }
errorper=0;
for (i=0;i<=(datacount-trainingsize-1);i++)
{
    errorper+=errortest[i][0];
}
errorper/=(datacount-trainingsize);
printf("\ntesting error =%lf",errorper);
/******************************************************/
/*Predict the training accuracy set result under normal*/
/*******************************************************/
   for (i=0;i<=trainingsize-1;i++)
    {
        /********************************************************/
        /*to get conditional probability for Homeowner under defaulter=No and Yes*/
        /********************************************************/
        P_test_HomeOwner_Default_No=0;
        P_test_HomeOwner_Default_Yes=0;
        if (traininginput[i][0]==0)
        {
             P_test_HomeOwner_Default_No=P_HomeOwner_Default_No[0];
             P_test_HomeOwner_Default_Yes=P_HomeOwner_Default_Yes[0];
        }
        if (traininginput[i][0]==1)
        {
            P_test_HomeOwner_Default_No=P_HomeOwner_Default_No[1];
            P_test_HomeOwner_Default_Yes=P_HomeOwner_Default_Yes[1];
        }
       
        /********************************************************/
        /*to get conditional probability for maritial status under defaulter=No and Yes*/
        /********************************************************/
        P_test_Maritial_Default_No=0;
        P_test_Maritial_Default_Yes=0;
        if (traininginput[i][1]==-1)
        {
            P_test_Maritial_Default_No=P_Maritial_Default_No[0];
            P_test_Maritial_Default_Yes=P_Maritial_Default_Yes[0];
        }
        if (traininginput[i][1]==0)
        {
            P_test_Maritial_Default_No=P_Maritial_Default_No[1];
            P_test_Maritial_Default_Yes=P_Maritial_Default_Yes[1];
        }
        if (traininginput[i][1]==1)
        {
            P_test_Maritial_Default_No=P_Maritial_Default_No[2];
            P_test_Maritial_Default_Yes=P_Maritial_Default_Yes[2];
        }
        /********************************************************/
        /*to get conditional probability for annual income under defaulter=No and Yes**/
        /********************************************************/
        z=((traininginput[i][2])*1e3-mean_annualincome_Default_No)/sqrt(var_annualincome_Default_No);
        P_test_annualincome_Default_No=exp(-0.5*pow(z,2));
        P_test_annualincome_Default_No=1/sqrt(2*PI*var_annualincome_Default_No)*P_test_annualincome_Default_No;
        z=((traininginput[i][2])*1e3-mean_annualincome_Default_Yes)/sqrt(var_annualincome_Default_Yes);
        P_test_annualincome_Default_Yes=exp(-0.5*pow(z,2));
        P_test_annualincome_Default_Yes=1/sqrt(2*PI*var_annualincome_Default_Yes)*P_test_annualincome_Default_Yes;
        postp_Default_No_train[i][0]=P_test_HomeOwner_Default_No*P_test_Maritial_Default_No*P_test_annualincome_Default_No*P_DefaultedBorrowtotal[0];
        postp_Default_Yes_train[i][0]=P_test_HomeOwner_Default_Yes*P_test_Maritial_Default_Yes*P_test_annualincome_Default_Yes*P_DefaultedBorrowtotal[1];
        diff= postp_Default_No_train[i][0]-postp_Default_Yes_train[i][0];
        if (diff>=0)
        {
            Default_Yes_train[i][0]=0;
        }
        else
        {
            Default_Yes_train[i][0]=1;
        }
        errortrain[i][0]=0;
        if (Default_Yes_train[i][0]!=trainingoutput[i][0])
        {
            errortrain[i][0]=1;
        }

    }
errorper=0;
for (i=0;i<=(trainingsize-1);i++)
{
    errorper+=errortrain[i][0];
}
errorper/=(trainingsize);
printf("\ntraining error =%lf",errorper);
return (0);
}