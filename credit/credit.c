#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long cardNum;
    do
    {
        cardNum = get_long("Enter card number: ");
    }
    while (!cardNum);

    int count = 0;
    long numberDigs = cardNum;
    do
    {
        numberDigs = numberDigs / 10;
        ++count;
    }
    while (numberDigs != 0);

    int sumOnce = 0;
    long sumTwice = 0;
    int flag = 1;
    int multi = 10;
    int temp;
    long tempCardNum = cardNum;

    for (int i = 0; i < count; i++)
    {
        temp = tempCardNum % multi;
        tempCardNum = tempCardNum / 10;

        if (flag == 1)
        {
            sumOnce = sumOnce + temp;
            flag = 0;
        }
        else
        {
            sumTwice = sumTwice * 10;
            sumTwice = sumTwice + (temp * 2);
            flag = 1;
        }
    }

    int countTwice = 0;
    long tempTwice = sumTwice;

    do
    {
        tempTwice = tempTwice / 10;
        ++countTwice;
    }
    while (tempTwice != 0);

    long tempTwice2 = sumTwice;
    int finSumTwice = 0;
    int temp2;

    for (int j = 0; j < countTwice; j++)
    {
        temp2 = tempTwice2 % multi;
        tempTwice2 = tempTwice2 / 10;
        finSumTwice = finSumTwice + temp2;
    }

    int check = finSumTwice + sumOnce;

    if (check % 10 == 0)
    {

        if (temp == 4 && (count == 13 || count == 16))
        {
            printf("VISA\n");
        }
        else
        {
            long tempCardNum2 = cardNum;
            long lastNums = 0;
            long tempDivided;

            for (int i = 0; i < count; i++)
            {
                tempDivided = tempCardNum2 % 10;
                tempCardNum2 = tempCardNum2 / 10;

                if (i > (count - 3))
                {
                    lastNums = lastNums * 10;
                    lastNums = lastNums + tempDivided;
                }
            }

            long lastNumsCopy = lastNums;
            long tempReplaced1 = lastNumsCopy % 10;
            lastNumsCopy = lastNumsCopy / 10;
            long tempReplaced2 = lastNumsCopy % 10;

            long last2Nums = (tempReplaced1 * 10) + tempReplaced2;

            if (count == 15 && (last2Nums == 34 || last2Nums == 37))
            {
                printf("AMEX\n");
            }
            else if (count == 16 && (last2Nums == 51 || last2Nums == 52 || last2Nums == 53 || last2Nums == 54 || last2Nums == 55))
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }
}