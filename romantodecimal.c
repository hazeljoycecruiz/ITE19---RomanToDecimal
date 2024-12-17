#include <stdio.h>  // Include the standard input/output library for I/O operations
#include <string.h>  // Include the string library for string manipulation functions

// Function prototypes
int romanToDecimal(const char *roman);  // Prototype for function that converts Roman numeral to decimal
void decimalToWords(int num, char *words);  // Prototype for function that converts decimal to words
void processExpression(char *expression, FILE *outputFile);  // Prototype for processing an expression
int stringLength(const char *str);  // Prototype for function that calculates string length
int isAlpha(char c);  // Prototype for function that checks if a character is alphabetic
int isSpace(char c);  // Prototype for function that checks if a character is whitespace
int isValidRoman(const char *roman);  // Prototype for function that checks if a string is a valid Roman numeral
int isValidOperator(char c);  // Prototype for function that checks if a character is a valid operator (+, -, *, =)
void stringCopy(char *dest, const char *src);  // Prototype for function that copies a string
void stringConcat(char *dest, const char *src);  // Prototype for function that concatenates two strings

// Custom string length function
int stringLength(const char *str) {
    int len = 0;  // Initialize length to 0
    while (str[len] != '\0') len++;  // Count characters until null terminator
    return len;  // Return the length of the string
}

// Custom isAlpha function to check if a character is alphabetic
int isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');  // Return true if the character is between A-Z or a-z
}

// Custom isSpace function to check if a character is a space, tab, or newline
int isSpace(char c) {
    return (c == ' ' || c == '\t' || c == '\n');  // Return true if the character is a space, tab, or newline
}

// Function to validate a Roman numeral string
int isValidRoman(const char *roman) {
    for (int i = 0; roman[i] != '\0'; i++) {  // Loop through each character in the Roman string
        char c = roman[i];  // Store the current character
        if (c != 'I' && c != 'V' && c != 'X' && c != 'L' &&  // Check if the character is not a valid Roman numeral
            c != 'C' && c != 'D' && c != 'M') {
            return 0;  // Return 0 if invalid character is found
        }
    }
    return 1;  // Return 1 if all characters are valid Roman numerals
}

// Function to validate the operator (plus, minus, multiply, equals)
int isValidOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '=';  // Return true if the character is one of the valid operators
}

// Custom string copy function
void stringCopy(char *dest, const char *src) {
    while (*src) {  // Loop through each character in the source string
        *dest++ = *src++;  // Copy each character from src to dest, increment both pointers
    }
    *dest = '\0';  // Null-terminate the destination string
}

// Custom string concatenation function
void stringConcat(char *dest, const char *src) {
    while (*dest) dest++;  // Move to the end of the destination string
    while (*src) {  // Loop through each character in the source string
        *dest++ = *src++;  // Append each character from src to dest, increment both pointers
    }
    *dest = '\0';  // Null-terminate the destination string
}

int main() {
    FILE *inputFile = fopen("Input.txt", "r");  // Open input file for reading
    FILE *outputFile = fopen("Output.txt", "w");  // Open output file for writing

    if (inputFile == NULL || outputFile == NULL) {  // Check if either file failed to open
        printf("Error: Unable to open file.\n");  // Print error message
        return 1;  // Exit program with an error code
    }

    char line[256];  // Buffer to hold a line from the input file
    char ch;  // Variable to store each character read from the input file
    int index = 0;  // Index for the current position in the line buffer

    // Manual line-by-line file reading
    while ((ch = fgetc(inputFile)) != EOF) {  // Read each character from the input file until EOF
        if (ch == '\n') {  // If the character is a newline
            line[index] = '\0';  // End the current line (null-terminate it)
            processExpression(line, outputFile);  // Process the current line
            index = 0;  // Reset index for the next line
        } else {
            line[index++] = ch;  // Add character to the current line
        }
    }

    if (index > 0) {  // If the last line didn't end with a newline
        line[index] = '\0';  // Null-terminate the last line
        processExpression(line, outputFile);  // Process the last line
    }

    fclose(inputFile);  // Close the input file
    fclose(outputFile);  // Close the output file

    printf("Processing complete. Check Output.txt for results.\n");  // Print completion message
    return 0;  // Return 0 to indicate successful execution
}

// Function to process a single expression
void processExpression(char *expression, FILE *outputFile) {
    char roman1[50], roman2[50], operator;  // Buffers for two Roman numerals and the operator
    int i = 0, j = 0;  // Indices for parsing the expression

    // Parse the first Roman numeral
    while (isAlpha(expression[i])) {  // Loop through the string and copy alphabetic characters
        roman1[j++] = expression[i++];  // Store characters in roman1 and increment both indices
    }
    roman1[j] = '\0';  // Null-terminate the first Roman numeral

    // Skip spaces and get the operator if any
    while (isSpace(expression[i])) i++;  // Skip spaces
    operator = expression[i++];  // Assign the operator and move to the next character

    // Parse the second Roman numeral if operator exists
    char result[50] = "";  // Initialize an empty result string for storing the final output
    if (isValidOperator(operator)) {  // If a valid operator is found
        j = 0;  // Reset index for the second Roman numeral
        while (isSpace(expression[i])) i++;  // Skip any spaces before the second numeral
        while (isAlpha(expression[i])) {  // Parse the second Roman numeral
            roman2[j++] = expression[i++];  // Store characters in roman2 and increment both indices
        }
        roman2[j] = '\0';  // Null-terminate the second Roman numeral

        // Validate Roman numerals
        if (!isValidRoman(roman1) || !isValidRoman(roman2)) {  // Check if both Roman numerals are valid
            fprintf(outputFile, "Invalid Roman numeral(s). Skipping line.\n");  // Print error if invalid
            return;  // Exit the function if the Roman numerals are invalid
        }

        // Convert Roman numerals to decimal
        int num1 = romanToDecimal(roman1);  // Convert the first Roman numeral to decimal
        int num2 = romanToDecimal(roman2);  // Convert the second Roman numeral to decimal
        int resultValue = 0;  // Variable to store the result of the operation

        // Perform the operation based on the operator
        switch (operator) {
            case '+': resultValue = num1 + num2; break;  // Addition
            case '-': resultValue = num1 - num2; break;  // Subtraction
            case '*': resultValue = num1 * num2; break;  // Multiplication
            case '=': resultValue = num1; break;  // If '=' operator, just assign the first number
            default: return;  // Should not reach here if the operator is valid
        }

        // Convert result to words
        decimalToWords(resultValue, result);  // Convert the decimal result to words
        fputs(result, outputFile);  // Write the result words to the output file
        fputs("\n", outputFile);  // Add a newline after the result
    } else {
        // If no operator, just convert the first Roman numeral
        if (!isValidRoman(roman1)) {  // If the first Roman numeral is invalid
            fprintf(outputFile, "Invalid Roman numeral(s). Skipping line.\n");  // Print error
            return;  // Exit the function if the Roman numeral is invalid
        }
        int num = romanToDecimal(roman1);  // Convert the first Roman numeral to decimal
        decimalToWords(num, result);  // Convert the decimal number to words
        fputs(result, outputFile);  // Write the result words to the output file
        fputs("\n", outputFile);  // Add a newline after the result
    }
}

// Function to convert Roman numeral to decimal
int romanToDecimal(const char *roman) {
    int value = 0, prev = 0;  // Initialize value to 0 and prev to 0 for comparison

    for (int i = stringLength(roman) - 1; i >= 0; i--) {  // Loop from the end of the Roman numeral string
        int current = 0;  // Variable to store the decimal value of the current Roman numeral
        switch (roman[i]) {  // Check the current Roman numeral character
            case 'I': current = 1; break;  // If 'I', assign 1
            case 'V': current = 5; break;  // If 'V', assign 5
            case 'X': current = 10; break;  // If 'X', assign 10
            case 'L': current = 50; break;  // If 'L', assign 50
            case 'C': current = 100; break;  // If 'C', assign 100
            case 'D': current = 500; break;  // If 'D', assign 500
            case 'M': current = 1000; break;  // If 'M', assign 1000
        }

        if (current < prev) {  // Check if the current value is less than the previous one (subtraction rule)
            value -= current;  // Subtract current value from the total value
        } else {
            value += current;  // Add current value to the total value
        }
        prev = current;  // Update prev to the current value for the next iteration
    }

    return value;  // Return the calculated decimal value
}

// Function to convert a number to words
void decimalToWords(int num, char *words) {
    static const char *ones[] = {"", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE"};  // Array for ones place
    static const char *teens[] = {"TEN", "ELEVEN", "TWELVE", "THIRTEEN", "FOURTEEN", "FIFTEEN", "SIXTEEN", "SEVENTEEN", "EIGHTEEN", "NINETEEN"};  // Array for teen numbers
    static const char *tens[] = {"", "", "TWENTY", "THIRTY", "FORTY", "FIFTY", "SIXTY", "SEVENTY", "EIGHTY", "NINETY"};  // Array for tens place
    static const char *thousands[] = {"", "THOUSAND", "MILLION"};  // Array for larger place values like thousand, million

    int isNegative = 0;  // Flag to check if the number is negative
    if (num < 0) {  // If the number is negative
        isNegative = 1;  // Set isNegative flag to 1
        num = -num;  // Make the number positive for processing
    }

    if (num == 0) {  // If the number is 0
        stringCopy(words, "ZERO");  // Set the words to "ZERO"
        return;  // Return since there's no further conversion needed
    }

    char temp[256] = "";  // Temporary string to store the words during conversion
    int part, place = 0;  // Variables for storing part of the number and place (thousands, millions, etc.)
}

    while (num > 0) {  // Loop until the number is greater than 0
    part = num % 1000;  // Extract the last three digits (part) of the number
    if (part > 0) {  // If the extracted part is greater than 0, process it
        char chunk[100] = "";  // Initialize a temporary string to store the current part's words
        if (part / 100 > 0) {  // If there are hundreds in the part
            stringConcat(chunk, ones[part / 100]);  // Add the word for the hundreds place
            stringConcat(chunk, " HUNDRED ");  // Add the word "HUNDRED"
        }
        if (part % 100 >= 10 && part % 100 < 20) {  // If the part is between 10 and 19 (teens)
            stringConcat(chunk, teens[part % 100 - 10]);  // Add the appropriate teen word
        } else {  // If the part is not between 10 and 19, process tens and ones
            if (part % 100 / 10 > 0) {  // If there are tens in the part
                stringConcat(chunk, tens[part % 100 / 10]);  // Add the word for the tens place
                stringConcat(chunk, " ");  // Add a space after the tens place
            }
            stringConcat(chunk, ones[part % 10]);  // Add the word for the ones place
        }
        if (place > 0) {  // If we're processing a place larger than units (thousand, million, etc.)
            stringConcat(chunk, " ");  // Add a space before the place word
            stringConcat(chunk, thousands[place]);  // Add the appropriate place (thousand, million, etc.)
        }
        stringConcat(chunk, " ");  // Add a space after the place word
        stringConcat(chunk, temp);  // Prepend the current chunk to the temporary result string
        stringCopy(temp, chunk);  // Copy the current chunk into the temp string for the next iteration
    }
    num /= 1000;  // Divide the number by 1000 to process the next group of three digits
    place++;  // Increment the place value (thousands, millions, etc.)
}

if (isNegative) {  // If the number was negative
    stringConcat(words, "NEGATIVE ");  // Add the word "NEGATIVE" to the result
}
stringConcat(words, temp);  // Append the final result (in words) to the output string
