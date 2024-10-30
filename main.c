#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Kural 1: Bir sayı en fazla 3 kere tekrar edebilir ve 3 kere tekrar eden her sayı toplama işlemine dahil edilir.(Yanlış: IIIII = 5, Doğru: V = 5)
 * Kural 2: Her ondalıklı sayıda en fazla bir kez çıkartma işlemi yapılabilir.(Yanlış: IIX = 8, IXIXIX = 27, Doğru: VIII = 8)
 * Kural 3: Sayılar soldan sağa doğru küçülüyorsa(ya da aynı sayı ise) toplama işlemine tabii tutulur onun dışındaki her işlem ya yanlış ya da çıkartma kabul edilir.(Örnek: XXV = 25, LXXVIII = 78)
 * Kural 4: Her roma rakamının yalnız 1 çözümü olabilir.(Yani 18 sayısını roma rakamına çevirirken nasıl ki XVIII yazıyorsak XVIII haricinde bir çözüm olmamalı bu yüzden IXIX != 18)
 * Kural 5: Sadece on sayısının üssü olan rakamlar ile çıkartma yapılabilir.(I,X,C; Yanlış: VX = 5, Doğru XL = 40)
 * Kural 6: Sayı en fazla 3.999.999 en az ise 1 olabilir.
 * Kural 7: M = 1000, I# = 1000 karışıklığı önlemek adına sadece 1000 kullanılan yerlerde M yi kullanılacak (Örnek: V#MMM = 8000)
 */

typedef struct Node {
    int data;
    char opp;
    struct Node *next;
} Node;

Node* deleteNode(Node* head, int delNum){
    Node* temp = head;
    Node* prev = NULL;

    if(temp == NULL){
        return head;
    }

    if (delNum == temp->data) {
        head = temp->next;
        free(temp);
        return head;
    }

    for (int i = 1; temp != NULL && temp->data != delNum; i++) {
        prev = temp;
        temp = temp->next;
    }

    if(temp != NULL && prev != NULL){
        prev->next = temp->next;
        free(temp);
    }

    return head;
}

void printList(Node* head)
{
    while (head != NULL) {
        if(head->opp != '='){
            printf("%d %c ", head->data, head->opp);
        }else{
            printf("%d", head->data);
        }
        head = head->next;
    }
    printf("\n");
}

int getSize(const char* text){
    int result = 0;

    while(text[result] != '\0'){
        result++;
    }

    return result;
}

int convertRomanNumber(char romanNumber){
    int result = 0;
    switch (romanNumber) {
        case 'I':
            result = 1;
            break;
        case 'V':
            result = 5;
            break;
        case 'X':
            result = 10;
            break;
        case 'L':
            result = 50;
            break;
        case 'C':
            result = 100;
            break;
        case 'D':
            result = 500;
            break;
        case 'M':
            result = 1000;
            break;
        default:
            break;
    }

    return result;
}

int convertNumber(int number){
    char result;
    switch (number) {
        case 1:
            result = 'I';
            break;
        case 5:
            result = 'V';
            break;
        case 10:
            result = 'X';
            break;
        case 50:
            result = 'L';
            break;
        case 100:
            result = 'C';
            break;
        case 500:
            result = 'D';
            break;
        case 1000:
            result = 'M';
            break;
        default:
            result = ' ';
            break;
    }

    return result;
}

int convertRomanNumbers(char* romanNumbers){
    int maxLong = getSize(romanNumbers);
    int result = 0;
    char beforeIndex = ' ';
    for(int i = 0; i < maxLong ; i++){
        if(romanNumbers[i] == '#'){
            result *= 1000;
            beforeIndex = ' ';
            continue;
        }else if(convertRomanNumber(beforeIndex) < convertRomanNumber(romanNumbers[i]) && beforeIndex != ' '){
            result -= 2 * convertRomanNumber(beforeIndex);
        }
        result += convertRomanNumber(romanNumbers[i]);
        beforeIndex = romanNumbers[i];
    }

    return result;
}

char* convertNumbers(int number){
    char* romanNumber = "";
    romanNumber[0] = '\0';
    int numberDivision = number;
    int numberLeft = 0;
    int bigNumber = 0;
    int k = 1000;

    if(numberDivision/k >= 4){
        numberDivision = number/1000;
        numberLeft = number%1000;
        bigNumber = 1;
    }

    while(1){
        if(numberDivision/k >= 1){
            sprintf(romanNumber, "%s%c", romanNumber, convertNumber(k));
            numberDivision -= 1*k;
        }else if(numberDivision/(k/10) >= 9){
            sprintf(romanNumber, "%s%c%c", romanNumber, convertNumber(k/10), convertNumber(k));
            numberDivision -= 9*(k/10);
            continue;
        }else if(numberDivision/(k/10) >= 5){
            sprintf(romanNumber, "%s%c", romanNumber,convertNumber(k/2));
            numberDivision -= 5*(k/10);
            continue;
        }else if(numberDivision/(k/10) >= 4){
            sprintf(romanNumber, "%s%c%c", romanNumber, convertNumber(k/10), convertNumber(k/2));
            numberDivision -= 4*(k/10);
            continue;
        }else{
            k = k/10;
        }

        if(numberDivision <= 0 && bigNumber == 0){
            break;
        }else if(numberDivision <= 3 && bigNumber == 1) {
            sprintf(romanNumber, "%s%c", romanNumber, '#');
            numberDivision = numberDivision*1000 + numberLeft;
            k = 1000;
            bigNumber = 0;
        }
    }
    return romanNumber;
}

int isRomanNumber(char* romanNumbers){

    if(strcmp(convertNumbers(convertRomanNumbers(romanNumbers)),romanNumbers) == 0){
        return 1;
    }

    return 0;
}

int isMath(char* romanNumber){
    int maxLong = getSize(romanNumber);
    int count = 0;
    for(int i = 0; i < maxLong; i++){
        if(romanNumber[i] == '*' || romanNumber[i] == '/' || romanNumber[i] == '+' || romanNumber[i] == '-'){
            count++;
        }
    }

    return count;
}

int calculation(int first, int second, char operation){
    int result = 0;

    switch (operation) {
        case '*':
            result = first*second;
            break;
        case '/':
            result = first/second;
            break;
        case '+':
            result = first+second;
            break;
        case '-':
            result = first-second;
            break;
        default:
            return 0;
    }

    return result;
}

Node* convertCalculation(char* romanNumber) {
    Node* intArray = (Node*)malloc(sizeof(Node));
    Node* result = intArray;
    Node* zero = (Node*)malloc(sizeof(Node));
    zero->data = 0;
    char* split = (char*)malloc(getSize(romanNumber) * sizeof(char));
    split[0] = '\0';
    int maxLong = getSize(romanNumber);

    for (int i = 0; i < maxLong; i++) {
        if(romanNumber[i] == ' '){
            continue;
        }
        if (romanNumber[i] == '*' || romanNumber[i] == '/' || romanNumber[i] == '+' || romanNumber[i] == '-') {
            intArray->data = convertRomanNumbers(split);
            if(strcmp(convertNumbers(intArray->data), split) != 0){
                return zero;
            }
            intArray->opp = romanNumber[i];
            intArray->next = (Node*) malloc(sizeof(Node));
            intArray = intArray->next;
            intArray->next = NULL;
            split[0] = '\0';
        } else {
            char buffer[2] = {romanNumber[i], '\0'};
            sprintf(split, "%s%s", split, buffer);
        }
    }
    intArray->data = convertRomanNumbers(split);
    if(strcmp(convertNumbers(intArray->data), split) != 0){
        return zero;
    }
    intArray->opp = '=';

    free(split);

    return result;
}

int makeCalculation(char* romanNumber){
    Node* intArray = convertCalculation(romanNumber);
    if(intArray->data == 0){
        return 0;
    }
    Node* dataTemp;
    int intTemp1;
    int intTemp2;
    char opp;
    char* opp1 = "*+";
    char* opp2 = "/-";
    //printList(intArray);
    for(int i = 0; i < 2;i++){
        dataTemp = intArray;
        intTemp1 = dataTemp->data;
        intTemp2 = 0;
        opp = dataTemp->opp;
        while(1){
            dataTemp = dataTemp->next;
            if(dataTemp != NULL){
                intTemp2 = dataTemp->data;
            }
            if(opp == opp1[i] || opp == opp2[i]){
                intArray = deleteNode(intArray, intTemp1);
                if(dataTemp != NULL){
                    dataTemp->data = calculation(intTemp1, intTemp2, opp);
                }
                //printList(intArray);
            }
            if(dataTemp != NULL){
                opp = dataTemp->opp;
                intTemp1 = dataTemp->data;
            }
            if(opp == '='){
                break;
            }
        }
    }

    return intArray->data;
}

int main(int argc, char** argv) {
    if(argc == 3){
        if(strcmp(argv[1], "-n") == 0){
            int number;
            sscanf(argv[2], "%d", &number);
            if(number >= 4000000){
                printf("Enter a valid Roman Numeral or Integer from 1 to 3,999,999");
                return 0;
            }
            char* romanNumber = convertNumbers(number);
            printf("%s", romanNumber);
        }else if(strcmp(argv[1], "-r") == 0){
            char* romanNumber = argv[2];
            int number = 0;
            if(isMath(romanNumber)){
                number = makeCalculation(romanNumber);
            }else{
                number = convertRomanNumbers(romanNumber);
                if(isRomanNumber(romanNumber) == 0){
                    printf("It's not a roman number!");
                    return 0;
                }
            }

            if(number == 0){
                printf("It's not a roman number!");
                return 0;
            }

            printf("%d", number);
        }
    }else{
        printf("Usage: romanNumerals.exe -r/-n {inputValue}\n    -r -> Roman Number to Number\n    -n -> Number to Roman Number");
    }
    return 0;
}
