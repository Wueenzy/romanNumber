#include <ctype.h>
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

#define MAX_LEN 100

char* roman_sort = "MDCLXVI";

char* roman_group_first[3] = {"D", "L", "V"};
char* roman_ungroup_first[3] = {"CCCCC", "XXXXX", "IIIII"};

char* roman_group_second_diff[6] = {"M", "C", "X"};
char* roman_ungroup_second_diff[6] = {"CCCCCCCCCC", "XXXXXXXXXX", "IIIIIIIIII"};

char* roman_group_first_diff[6] = {"D", "M", "L", "C", "V", "X"};
char* roman_ungroup_first_diff[6] = {"CCCCC", "DD", "XXXXX", "LL", "IIIII", "VV"};

char *roman_group_second[6] = {"CD", "CM", "XL", "XC", "IV", "IX"};
char *roman_ungroup_second[6] = {"CCCC", "CCCCCCCCC", "XXXX", "XXXXXXXXX", "IIII", "IIIIIIIII"};

char *roman_ungroup_fix[3] = {"DCD", "LXL", "VIV"};
char *roman_group_fix[3] = {"CM", "XC", "IX"};

char* replaceWord(const char* s, const char* oldW, const char* newW)
{
    char* result;
    int i, cont = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cont++;

            i += oldWlen - 1;
        }
    }

    result = (char*)malloc(i + cont * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

void sortRoman(char *roman) {
    int len = strlen(roman);
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (strchr(roman_sort, roman[j]) > strchr(roman_sort, roman[j + 1])) {
                char temp = roman[j];
                roman[j] = roman[j + 1];
                roman[j + 1] = temp;
            }
        }
    }
}

char* ungroup(const char* s) {
    char* result = (char*)malloc(strlen(s) + 1);
    strcpy(result, s);
    for(int i = 0; i < 6; i++) {
        result = replaceWord(result, roman_group_second[i], roman_ungroup_second[i]);
    }
    for(int i = 0; i < 3; i++) {
        result = replaceWord(result, roman_group_first[i], roman_ungroup_first[i]);
    }
    return result;
}

char* group(const char* s) {
    char* result = (char*)malloc(strlen(s) + 1);
    strcpy(result, s);
    for(int i = 5; i >= 0; i--) {
        while(strcmp(result, replaceWord(result, roman_ungroup_first_diff[i], roman_group_first_diff[i])) != 0) {
            result = replaceWord(result, roman_ungroup_first_diff[i], roman_group_first_diff[i]);
        }
    }
    for(int i = 5; i >= 0; i--) {
        while(strcmp(result, replaceWord(result, roman_ungroup_second[i], roman_group_second[i])) != 0) {
            result = replaceWord(result, roman_ungroup_second[i], roman_group_second[i]);
        }
    }
    for(int i = 2; i >= 0; i--) {
        while(strcmp(result, replaceWord(result, roman_ungroup_fix[i], roman_group_fix[i])) != 0) {
            result = replaceWord(result, roman_ungroup_fix[i], roman_group_fix[i]);
        }
    }
    return result;
}

void plus(char *result, char *roman1, char *roman2) {
    while(strcmp(roman1, ungroup(roman1)) != 0) {
        roman1 = ungroup(roman1);
    }
    while(strcmp(roman2, ungroup(roman2)) != 0) {
        roman2 = ungroup(roman2);
    }
    strcpy(result, roman1);
    strcat(result, roman2);
    sortRoman(result);
}

char* sub(char *result, const char *roman1, const char *roman2) {
    while(strcmp(roman1, ungroup(roman1)) != 0) {
        roman1 = ungroup(roman1);
    }
    while(strcmp(roman2, ungroup(roman2)) != 0) {
        roman2 = ungroup(roman2);
    }
    strcpy(result, roman1);
    int size = strlen(roman2);
    for (int i = 0; i < size; i++) {
        char *pos = strchr(result, roman2[i]);
        if (pos != NULL) {
            memmove(pos, pos + 1, strlen(pos));
        }else {
            char number = roman2[i];
            while(1) {
                char* pos2 = strchr(roman_sort, number);
                pos = strchr(result, *(pos2 - 1));
                if(pos == NULL) {
                    number = *(pos2 - 1);
                    continue;
                }
                switch (*(pos2 - 1)) {
                    case 'M':
                        result = replaceWord(result, "M", "CCCCCCCCCC");
                        break;
                    case 'C':
                        result = replaceWord(result, "C", "XXXXXXXXXX");
                        break;
                    case 'X':
                        result = replaceWord(result, "X", "IIIIIIIIII");
                        break;
                }
                i--;
                break;
            }
        }
    }
    return result;
}

int convertRomanNumber(char roman) {
    switch (roman) {
        case 'I':
            return 1;
        case 'X':
            return 10;
        case 'C':
            return 100;
        case 'M':
            return 1000;
    }
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

int convertRomanNumbers(char* roman) {
    while(strcmp(roman, ungroup(roman)) != 0) {
        roman = ungroup(roman);
    }
    int len = strlen(roman);
    int result = 0;
    for (int i = 0; i < len; i++) {
        result += convertRomanNumber(roman[i]);
    }
    return result;
}

char* convertNumbers(int number){
    char* romanNumber = (char*)malloc(256 * sizeof(char));
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

int main(int argc, char** argv) {
    if(argc >= 3){
        if(strcmp(argv[1], "-n") == 0){
            int number;
            sscanf(argv[2], "%d", &number);
            if(number >= 4000000){
                printf("Enter a valid Roman Numeral or Integer from 1 to 3,999,999");
                return 0;
            }
            char* romanNumber = convertNumbers(number);
            printf("%s", romanNumber);
        }else if(strcmp(argv[1], "-rp") == 0){
            char* romanNumber1 = argv[2];
            char* romanNumber2 = argv[3];
            if(isRomanNumber(romanNumber1) == 0 || isRomanNumber(romanNumber2) == 0){
                printf("It's not a roman number!");
                return 0;
            }
            char* result = (char*)malloc(256 * sizeof(char));
            plus(result, romanNumber1, romanNumber2);

            while(strcmp(result, group(result)) != 0) {
                result = group(result);
            }

            printf("%s", result);
        }else if(strcmp(argv[1], "-rs") == 0){
            char* romanNumber1 = argv[2];
            char* romanNumber2 = argv[3];
            if(isRomanNumber(romanNumber1) == 0 || isRomanNumber(romanNumber2) == 0){
                printf("It's not a roman number!");
                return 0;
            }
            char* result = (char*)malloc(256 * sizeof(char));
            result = sub(result, romanNumber1, romanNumber2);

            while(strcmp(result, group(result)) != 0) {
                result = group(result);
            }

            printf("%s", result);
        }else if(strcmp(argv[1], "-r") == 0){
            char* romanNumber = argv[2];
            int number = 0;

            number = convertRomanNumbers(romanNumber);

            if(isRomanNumber(romanNumber) == 0 || number == 0){
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
